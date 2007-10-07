#ifndef FAT_DIR_H_
#define FAT_DIR_H_
#include "fat32/fat_file.h"
#include "fat32/fat_dir_entry.h"
int fat_dir_read (fat_file * file, fat_dir_entry * dir);
int fat_dir_find (fat_file * file, const char *name, fat_dir_entry *dir);
#endif /*FAT_DIR_H_ */
