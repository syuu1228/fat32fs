#include "fat32/dir_entry.h"
#include "fat32/ldir_entry.h"
#include "fat32/cluster_data.h"
#include "message.h"
#include <assert.h>

static inline sector_t cluster_data_head_sector(bpb * bpb)
{
	MESSAGE_DEBUG("bpb:%p\n", bpb);
	dword_t res = bpb->sectors_per_fat32 * bpb->number_of_fats
			+ bpb->reserved_sectors;
	MESSAGE_DEBUG("return:%u\n", res);
	return res;
}

off_t cluster_data_offset(fat_instance * ins, const cluster_t cluster_no)
{
	MESSAGE_DEBUG("ins:%p clusterNo:%u\n", ins, cluster_no);
	off_t res = fat_instance_sector_to_offset (ins,
			cluster_data_head_sector (ins->bpb)+ bpb_cluster_to_sector (
					ins->bpb, cluster_no -2));
	MESSAGE_DEBUG("return:%u\n", res);
	return res;
}

ssize_t
cluster_data_read (fat_instance * ins, const cluster_t cluster_no, void *buffer, off_t o, size_t count)
{
	MESSAGE_DEBUG("ins:%p clusterNo:%u buffer:%p\n", ins, cluster_no, buffer);
	assert(o < bpb_cluster_size(&ins->bpb));
	assert(count <  bpb_cluster_size(&ins->bpb));
	off_t offset = cluster_data_offset (ins, cluster_no) + o;
	if ((lseek (ins->disk_id, offset, SEEK_SET)) != offset)
	{
		MESSAGE_ERROR ("lseek failed\n");
		MESSAGE_DEBUG("return:false\n");
		return -1;
	}
	ssize_t ret = read (ins->disk_id, buffer, count);
	MESSAGE_DEBUG("return:%d\n", ret);
	return ret;
}

void cluster_data_dump(fat_instance * ins, const cluster_t cluster_no)
{
	MESSAGE_DEBUG("ins:%p clusterNo:%u\n", ins, cluster_no);
	byte_t buf[bpb_cluster_size (ins->bpb)];
	cluster_data_read (ins, cluster_no, buf, 0, bpb_cluster_size (ins->bpb));
	printf("data:");
	outns(buf, bpb_cluster_size (ins->bpb));
}

extern void cluster_dump(fat_instance * ins, cluster_t cluster_no);
void cluster_data_dump_dir(fat_instance * ins, const cluster_t cluster_no)
{
	MESSAGE_DEBUG("ins:%p clusterNo:%u\n", ins, cluster_no);
	dir_entry *root_dir;
	int i;
	const int len = bpb_cluster_size (ins->bpb) / sizeof(dir_entry);
	assert (bpb_cluster_size (ins->bpb) % sizeof (dir_entry) == 0);
	root_dir = (dir_entry *) calloc (sizeof(dir_entry), len);
	cluster_data_read (ins, cluster_no, (void *) root_dir, 0, sizeof(dir_entry));
	for (i = 0; i < len; i++)
	{
		printf("[%d]\n", i);
		if (DIR_ENTRY_IS_END (root_dir + i))
		{
			printf ("End\n");
			break;
		}
		else if (DIR_ENTRY_IS_DELETED (root_dir + i))
		{
			printf ("Deleted entry\n");
		}
		else if (DIR_ENTRY_IS_LDIR_ENTRY (root_dir + i))
		{
			ldir_entry_dump ((ldir_entry *)(root_dir + i));
		}
		else
		{
			dir_entry_dump (root_dir + i);
			cluster_t clus = dir_entry_cluster (root_dir + i);
			if (clus)
			{
				printf ("cluster:");
				cluster_dump (ins, clus);
			}
			if (root_dir[i].name[0] != '.'&& root_dir[i].attributes.directory)
				cluster_data_dump_dir (ins, clus);
			else if (!root_dir[i].attributes.directory)
				cluster_data_dump (ins, clus);
		}
	}
}
