#ifndef FAT_PATH_H_
#define FAT_PATH_H_
#include "fat32/fat_type.h"
#include "fat32/fat_dir_entry.h"
#include "fat32/fat_cluster_list.h"

int fat_path_get_entry(fat_instance *ins, const char *path, fat_dir_entry *dir);
fat_cluster_list *fat_path_get_cluster(fat_instance *ins, const char *path);
#endif /*FAT_PATH_H_*/
