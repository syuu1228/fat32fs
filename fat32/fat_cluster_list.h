#ifndef FAT_CLUSTER_LIST_H_
#define FAT_CLUSTER_LIST_H_
#include "fat32/fat_type.h"
#include "fat32/fat_instance.h"
#include "fat32/cluster.h"
#define DEFAULT_LIST_SIZE 128

typedef struct {
	cluster_t *clusters;
	int offset, length, end;
} fat_cluster_list;

fat_cluster_list *fat_cluster_list_open(fat_instance * ins, cluster_t head, dword_t size);
cluster_t fat_cluster_list_read(fat_cluster_list *list);
int fat_cluster_list_tell(fat_cluster_list *list);
int fat_cluster_list_seek(fat_cluster_list *list, int offset);
int fat_cluster_list_size(fat_cluster_list *list);
void fat_cluster_list_close(fat_cluster_list * list);
void fat_cluster_list_dump(fat_cluster_list * list);
#endif /*FAT_CLUSTER_LIST_H_*/
