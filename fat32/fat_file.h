#ifndef FAT_FILE_H_
#define FAT_FILE_H_
#include "fat32/fat_type.h"
#include "fat32/fat_instance.h"
#include "fat32/fat_cluster_chain.h"

typedef struct
{
  fat_instance * ins;
  fat_cluster_chain *chain;
  off_t offset;
  bool is_reached_to_tail;
} fat_file;

fat_file *fat_file_new (fat_instance * ins, fat_cluster_chain * cluster_chain);
fat_file *fat_file_open (fat_instance * ins, const char *name);
ssize_t fat_file_read (fat_file * file, void *buffer, size_t count);
bool fat_file_cluster_seek (fat_file * file, cluster_t pos);
off_t fat_file_lseek (fat_file *file, off_t offset);
int fat_file_close (fat_file * file);
#endif /*FAT_FILE_H_ */
