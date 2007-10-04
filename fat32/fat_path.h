#ifndef FAT_PATH_H_
#define FAT_PATH_H_
#include "fat32/fat_type.h"
#include "fat32/dir_entry.h"
#include "fat32/fat_cluster_chain.h"

dir_entry *fat_path_get_dir_entry(fat_instance *ins, const char *path);
fat_cluster_chain *fat_path_get_cluster(fat_instance *ins, const char *path);
#endif /*FAT_PATH_H_*/
