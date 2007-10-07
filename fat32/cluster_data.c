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
	MESSAGE_DEBUG("ins:%p cluster_no:%u buffer:%p offset:%u\n", ins, cluster_no, buffer, o);
	MESSAGE_DEBUG("count:%u\n", count);
	assert(o + count <=  bpb_cluster_size(ins->bpb));
	off_t offset = cluster_data_offset (ins, cluster_no) + o;
	ssize_t ret;
	if((ret = disk_read (ins->disk_id, buffer, offset, count)) != count)
		MESSAGE_ERROR("read failed\n");
	MESSAGE_DEBUG("return:%d\n", ret);
	return ret;
}
