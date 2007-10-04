#include "fat32/fat_path.h"
#include "fat32/fat_file.h"
#include "fat32/fat_dir.h"
#include "message.h"

static inline char * pathtok(const char *p)
{
	static int start, end;
	static char *path, buf[256];
	
	if(p)
	{
		start = end = 1;
		path = p;
		return NULL;
	}
	while(path[end] && !(path[end] == '/' && !path[end + 1]))
	{
		memset(buf, 0, 256);
		for(end = start; path[end] && path[end] != '/'; end++)
			;
		strncpy(buf, path + start, end - start);
		start = end + 1;
		return buf;
	}
	return NULL;
}

dir_entry *fat_path_get_dir_entry(fat_instance *ins, const char *path)
{
	fat_cluster_chain *chain;
	char *name;
	dir_entry *dir;
	cluster_t cluster_no;
	
	if(!strcmp("/", path))
		return NULL;
	
	cluster_no = ins->bpb->root_dir_cluster;
	pathtok(path);
	while((name = pathtok(NULL)))
	{
		MESSAGE_DEBUG("name:%s\n", name);
		chain = fat_cluster_chain_get(ins, cluster_no);
		fat_file *file = fat_file_new(ins, chain);
		if(!(dir = fat_dir_find(file, name)))
		{
			fat_file_close(file);
			return NULL;
		}
		cluster_no = dir_entry_cluster(dir);
		fat_file_close(file);
	}
	return dir;
}

fat_cluster_chain *fat_path_get_cluster(fat_instance *ins, const char *path)
{
	if(!strcmp("/", path))
		return ins->bpb->root_dir_cluster;
	dir_entry *dir = fat_path_get_dir_entry(ins, path);
	if(!dir)
		return NULL;
	return fat_cluster_chain_get(ins, dir_entry_cluster(dir));
}
