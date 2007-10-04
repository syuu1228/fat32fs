#ifndef FAT_DIR_H_
#define FAT_DIR_H_
#include "fat32/fat_file.h"
#include "fat32/dir_entry.h"

dir_entry *fat_dir_read (fat_file * file);
dir_entry *fat_dir_find (fat_file * file, const char *name);
#endif /*FAT_DIR_H_ */
