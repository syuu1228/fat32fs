#include "fat32/cluster_data.h"
#include "fat32/fat_file.h"
#include "message.h"
#include "fat32/fat_type.h"
#include <assert.h>
#include <stdlib.h>
#include "fat32/fat_path.h"

static inline fat_cluster_chain *fat_file_next_cluster(fat_file * file)
{
	return file->current_cluster = fat_cluster_chain_next(file->current_cluster);
}

static inline cluster_t fat_file_seek_cluster(fat_file *file, cluster_t o)
{
	cluster_t offset = o;
	while (o && fat_file_next_cluster(file))
		o--;
	return offset - o;
}

fat_file *fat_file_new(fat_instance * ins, fat_cluster_chain * cluster_chain)
{
	MESSAGE_DEBUG("ins:%p cluster_chain:%p\n", ins, cluster_chain);
	assert(ins);
	if(!cluster_chain)
		return NULL;
	fat_file *file = (fat_file *) calloc (1, sizeof(fat_file));
	if (!file)
	{
		MESSAGE_ERROR ("malloc failed");
		return NULL;
	}
	file->ins = ins;
	file->head_cluster = file->current_cluster = cluster_chain;
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
	
	if (!file->current_cluster)
		return -1;
	
	while (c >= bpb_cluster_size(file->ins->bpb))
	{
		ssize_t res = cluster_data_read (file->ins, file->current_cluster->cluster_no, buffer,
				offset, bpb_cluster_size(file->ins->bpb) - offset);
		c -= res;
		if (res != bpb_cluster_size(file->ins->bpb))
		{
			file->offset += res;
			return count - c;
		}
		buffer += res;
		offset = 0;
		if(!fat_file_next_cluster(file))
			return count - c;
	}
	
	if (!c)
		return count;
	
	ssize_t res = cluster_data_read (file->ins, file->current_cluster->cluster_no, buffer, offset, c);
	file->offset += res;
	if (res != c - offset)
		return res;
	assert(c - res == 0);
	assert(offset + c <= bpb_cluster_size(file->ins->bpb));
	if (offset + c == bpb_cluster_size(file->ins->bpb))
		fat_file_next_cluster(file);
	return count;
}

off_t fat_file_lseek(fat_file *file, off_t o)
{
	off_t offset = o;
	MESSAGE_DEBUG("file:%p offset:%u\n", file, o);
	if (o >= bpb_cluster_size(file->ins->bpb))
	{
		cluster_t coff = offset / bpb_cluster_size(file->ins->bpb);
		cluster_t res = fat_file_seek_cluster(file, coff);
		o -= res * bpb_cluster_size(file->ins->bpb);
		if(res != coff)
			return offset - o;
	}
	file->offset = o;
	return offset;
}

off_t fat_file_tell (fat_file *file)
{
	int i = 0;
	list_node *lp = &(file->head_cluster->list);
	LIST_FOR_EACH(lp)
		if(lp != &(file->current_cluster->list))
			i++;
	return file->offset + i * bpb_cluster_size(file->ins->bpb); 
}

int fat_file_close(fat_file * file)
{
	MESSAGE_DEBUG("file:%p\n", file);
	assert(file->head_cluster);
	fat_cluster_chain_delete (file->head_cluster);
	free (file);
	return 0;
}
