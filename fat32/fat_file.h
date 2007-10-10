#ifndef FAT_FILE_H_
#define FAT_FILE_H_
#include "fat32/fat_type.h"
#include "fat32/fat_instance.h"
#include "fat32/fat_cluster_list.h"

typedef struct {
	fat_instance * ins;
	fat_cluster_list *cluster;
	off_t offset;
} fat_file;

fat_file *fat_file_new(fat_instance * ins, fat_cluster_list *cluster);
fat_file *fat_file_open(fat_instance * ins, const char *path);
ssize_t fat_file_read(fat_file * file, void *buffer, size_t count);
off_t fat_file_seek_set(fat_file *file, off_t offset);
off_t fat_file_seek_cur(fat_file *file, off_t offset);
off_t fat_file_tell(fat_file *file);
int fat_file_close(fat_file * file);
#endif /*FAT_FILE_H_ */
