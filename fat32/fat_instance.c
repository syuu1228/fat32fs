#include "fat32/fat_instance.h"
#include "mbr.h"
#include "message.h"
#include <stdlib.h>

fat_instance *fat_instance_new(int disk_id, int partition_no)
{
	fat_instance *ins = (fat_instance *) calloc (1, sizeof(fat_instance));
	if (!ins)
	{
		MESSAGE_ERROR ("malloc failed\n");
		return NULL;
	}
	mbr *m = mbr_read(disk_id);
	if(!m)
	{
		MESSAGE_ERROR ("mbr read failed\n");
		free(ins);
		return NULL;
	}
#ifdef DEBUG
	mbr_dump(m);
#endif
	ins->disk_id = disk_id;
	ins->partition = m->partition_table + partition_no;
	if (!bpb_read (ins))
		goto free_and_return;
#ifdef DEBUG
	bpb_dump(ins->bpb);
#endif
	if (!bpb_validate (ins->bpb))
		goto free_and_return;
	if (ins->bpb->bytes_per_sector != MBR_SECTOR_SIZE)
	{
		MESSAGE_ERROR ("Illigal sector size\n");
		goto free_and_return;
	}
	return ins;
	free_and_return: free (ins);
	return NULL;
}

void fat_instance_delete(fat_instance * ins)
{
	free (ins->bpb);
	free (ins);
}

void fat_instance_dump(fat_instance * ins)
{
	bpb_dump (ins->bpb);
	bpb_cluster_size (ins->bpb);
	bpb_count_of_clusters (ins->bpb);
}

off_t fat_instance_head_offset(fat_instance * ins)
{
	MESSAGE_DEBUG("ins:%p\n", ins);
	off_t res = mbr_partition_offset (ins->partition);
	MESSAGE_DEBUG("return:%u\n", res);
	return res;
}

off_t fat_instance_byte_to_offset(fat_instance * ins, qword_t byte)
{
	MESSAGE_DEBUG("ins:%p byte:%u\n", ins, byte);
	off_t res = fat_instance_head_offset (ins) + byte;
	MESSAGE_DEBUG("return:%u\n", res);
	return res;
}

off_t fat_instance_sector_to_offset(fat_instance * ins, sector_t sector)
{
	MESSAGE_DEBUG("ins:%p sector:%u\n", ins, sector);
	off_t res =fat_instance_byte_to_offset (ins, bpb_sector_to_byte (ins->bpb,
			sector));

	MESSAGE_DEBUG("return:%u\n", res);
	return res;
}

off_t fat_instance_cluster_to_offset(fat_instance * ins, cluster_t cluster)
{
	MESSAGE_DEBUG("ins:%p cluster:%u\n", ins, cluster);
	off_t res = fat_instance_sector_to_offset (ins, bpb_cluster_to_sector (
			ins->bpb, cluster));
	MESSAGE_DEBUG("return:%u\n", res);
	return res;
}
