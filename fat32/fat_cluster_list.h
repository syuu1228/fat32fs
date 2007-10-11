#ifndef FAT_CLUSTER_LIST_H_
#define FAT_CLUSTER_LIST_H_
#include "fat32/fat_type.h"
#include "fat32/fat_instance.h"
#include "fat32/cluster.h"
#include "fat32/fat_dir_entry.h"

typedef struct
{
	cluster_t *clusters;
	int offset, length, end;
} fat_cluster_list;

fat_cluster_list *fat_cluster_list_open_by_cluster_no(fat_instance * ins,
		cluster_t cluster_no);
fat_cluster_list *fat_cluster_list_open_by_fat_dir_entry(fat_instance * ins,
		fat_dir_entry *dir);
cluster_t fat_cluster_list_read(fat_cluster_list *list);
int fat_cluster_list_tell(fat_cluster_list *list);
int fat_cluster_list_seek_set(fat_cluster_list *list, int offset);
int fat_cluster_list_seek_cur(fat_cluster_list *list, int offset);
int fat_cluster_list_size(fat_cluster_list *list);
void fat_cluster_list_close(fat_cluster_list * list);
void fat_cluster_list_dump(fat_cluster_list * list);
#endif /*FAT_CLUSTER_LIST_H_*/
