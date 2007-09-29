#ifndef FAT_DIR_H_
#define FAT_DIR_H_
#include "fat32/fat_file.h"
#include "fat32/fat_dir_content.h"

typedef struct
{
  fat_file file;
  dir_entry *dir_entry;
  int de_pos, de_len;
} fat_dir;

fat_dir *fat_dir_new (fat_instance * ins, fat_cluster_chain * cluster_chain);
fat_dir *fat_dir_open (fat_instance * ins, const char *name);
void fat_dir_seek (fat_dir * dir, off_t offset);
off_t fat_dir_tell (fat_dir * dir);
int fat_dir_read (fat_dir * dir, fat_dir_content * content);
bool fat_dir_find (fat_dir * dir, const char *name, fat_dir_content * content);
int fat_dir_close (fat_dir * dir);
#endif /*FAT_DIR_H_ */
