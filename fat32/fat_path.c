#include "fat32/fat_path.h"
#include "fat32/fat_file.h"
#include "fat32/fat_dir.h"
#include "message.h"

static char * pathtok(const char *p);

char * pathtok(const char *p)
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

int fat_path_get_entry(fat_instance *ins, const char *path, fat_dir_entry *dir)
{
	char *name;
	cluster_t cluster_no;
	
	cluster_no = ins->bpb->root_dir_cluster;
	if(!strcmp("/", path))
	{
		memset(dir, 0, sizeof(fat_dir_entry));
		dir->dir_entry.attributes.directory = 1;
		dir->dir_entry.cluster_lo = cluster_no;
		dir->short_name[0] = '/';
		return 0;
	}
	pathtok(path);
	while((name = pathtok(NULL)))
	{
		MESSAGE_DEBUG("name:%s\n", name);
		fat_cluster_list* list = fat_cluster_list_open(ins, cluster_no);
		if(!list)
			return -1;
		fat_file *file = fat_file_new(ins, list);
		if(!file)
		{
			fat_cluster_list_close(list);
			return -1;
		}
		if(fat_dir_find(file, name, dir) < 0)
		{
			fat_file_close(file);
			return -1;
		}
		cluster_no = dir_entry_get_cluster(&dir->dir_entry);
		fat_file_close(file);
	}
	return 0;
}

fat_cluster_list *fat_path_get_cluster(fat_instance *ins, const char *path)
{
	if(!strcmp("/", path))
		return fat_cluster_list_open(ins, ins->bpb->root_dir_cluster);
	fat_dir_entry dir;
	if(fat_path_get_entry(ins, path, &dir) < 0)
		return NULL;
	return fat_cluster_list_open(ins, dir_entry_get_cluster(&(dir.dir_entry)));
}
