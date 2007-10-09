#include "fat32/cluster_data.h"
#include "fat32/fat_file.h"
#include "message.h"
#include "fat32/fat_type.h"
#include <assert.h>
#include <stdlib.h>
#include "fat32/fat_path.h"

fat_file *fat_file_new(fat_instance * ins, fat_cluster_list * cluster)
{
	MESSAGE_DEBUG("ins:%p cluster:%p\n", ins, cluster);
	assert(ins);
	if(!cluster)
		return NULL;
	fat_file *file = (fat_file *) calloc (1, sizeof(fat_file));
	if (!file)
	{
		MESSAGE_ERROR ("malloc failed");
		return NULL;
	}
	file->ins = ins;
	file->cluster = cluster;
	return file;
}

fat_file *fat_file_open (fat_instance * ins, const char *path)
{
	return fat_file_new(ins, fat_path_get_cluster(ins, path));
}

ssize_t fat_file_read(fat_file * file, void *buffer, size_t c)
{
	MESSAGE_DEBUG("file:%p buffer:%p count:%u\n", file, buffer, c);
	size_t count = c;
	off_t offset = file->offset;

	if(IS_END_OF_CLUSTER(fat_cluster_list_read(file->cluster)))
		return 0;
	
	while (c >= bpb_cluster_size(file->ins->bpb))
	{
		ssize_t res = cluster_data_read (file->ins, fat_cluster_list_read(file->cluster), buffer,
				offset, bpb_cluster_size(file->ins->bpb) - offset);
		c -= res;
		if (res != bpb_cluster_size(file->ins->bpb))
		{
			file->offset += res;
			return count - c;
		}
		buffer += res;
		offset = 0;
		if(IS_END_OF_CLUSTER(fat_cluster_list_next(file->cluster)))
			return count - c;
	}
	
	if (!c)
		return count;
	
	ssize_t res = cluster_data_read (file->ins, fat_cluster_list_read(file->cluster), buffer, offset, c);
	file->offset += res;
	if (res != c - offset)
		return res;
	assert(c - res == 0);
	assert(offset + c <= bpb_cluster_size(file->ins->bpb));
	if (offset + c == bpb_cluster_size(file->ins->bpb))
		fat_cluster_list_next(file->cluster);
	return count;
}

off_t fat_file_lseek(fat_file *file, off_t o)
{
	off_t offset = o;
	MESSAGE_DEBUG("file:%p offset:%u\n", file, o);
	if (o >= bpb_cluster_size(file->ins->bpb))
	{
		cluster_t coff = offset / bpb_cluster_size(file->ins->bpb);
		if(fat_cluster_list_seek(file->cluster, coff) != coff)
			return offset - o;
		o -= coff * bpb_cluster_size(file->ins->bpb);
	}
	file->offset = o;
	return offset;
}

off_t fat_file_tell (fat_file *file)
{
	return file->offset + fat_cluster_list_tell(file->cluster) * bpb_cluster_size(file->ins->bpb); 
}

int fat_file_close(fat_file * file)
{
	MESSAGE_DEBUG("file:%p\n", file);
	fat_cluster_list_close(file->cluster);
	free (file);
	return 0;
}
