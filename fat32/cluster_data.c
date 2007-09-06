#include "fat32/dir_entry.h"
#include "fat32/ldir_entry.h"
#include "fat32/cluster_data.h"
#include "message.h"

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

bool
cluster_data_read (fat_instance * ins, const cluster_t cluster_no, void *buffer)
{
	MESSAGE_DEBUG("ins:%p clusterNo:%u buffer:%p\n", ins, cluster_no, buffer);
	off_t offset = cluster_data_offset (ins, cluster_no);
	if ((lseek (ins->disk_id, offset, SEEK_SET)) != offset)
	{
		MESSAGE_ERROR ("lseek failed\n");
		MESSAGE_DEBUG("return:false\n");
		return false;
	}
	if ((read (ins->disk_id, buffer, bpb_cluster_size (ins->bpb))) !=
			bpb_cluster_size (ins->bpb))
	{
		MESSAGE_ERROR ("read failed\n");
		MESSAGE_DEBUG("return:false\n");
		return false;
	}
	MESSAGE_DEBUG("return:true\n");
	return true;
}

void cluster_data_dump(fat_instance * ins, const cluster_t cluster_no)
{
	MESSAGE_DEBUG("ins:%p clusterNo:%u\n", ins, cluster_no);
	byte_t buf[bpb_cluster_size (ins->bpb)];
	cluster_data_read (ins, cluster_no, buf);
	printf("data:");
	outns(buf, bpb_cluster_size (ins->bpb));
}

extern void cluster_dump(fat_instance * ins, cluster_t cluster_no);
void cluster_data_dump_dir(fat_instance * ins, const cluster_t cluster_no)
{
	MESSAGE_DEBUG("ins:%p clusterNo:%u\n", ins, cluster_no);
	dir_entry *rootDir;
	int i;
	const int len = bpb_cluster_size (ins->bpb) / sizeof(dir_entry);
	assert (bpb_cluster_size (ins->bpb) % sizeof (dir_entry) == 0);
	rootDir = (dir_entry *) calloc (sizeof(dir_entry), len);
	cluster_data_read (ins, cluster_no, (void *) rootDir);
	for (i = 0; i < len; i++)
	{
		printf("[%d]\n", i);
		if (DIR_ENTRY_IS_END (rootDir + i))
		{
			printf ("End\n");
			break;
		}
		if (DIR_ENTRY_IS_DELETED (rootDir + i))
		{
			printf ("Deleted entry\n");
			continue;
		}
		if (DIR_ENTRY_IS_LDIR_ENTRY (rootDir + i))
		{
			ldir_entry_dump ((ldir_entry *)(rootDir + i));
		}
		else
		{
			dir_entry_dump (rootDir + i);
			cluster_t clus = dir_entry_cluster (rootDir + i);
			if (clus)
			{
				printf ("cluster:");
				cluster_dump (ins, clus);
			}
			if (rootDir[i].name[0] != '.'&& rootDir[i].attributes.directory)
				cluster_data_dump_dir (ins, clus);
			//	  else if (!rootDir[i].attributes.directory)
			//	    cluster_data_dump (ins, clus);
		}
	}
}
