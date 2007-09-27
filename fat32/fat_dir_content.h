#ifndef FAT_DIR_CONTENT_H_
#define FAT_DIR_CONTENT_H_
#include "fat32/dir_entry.h"
#include "fat32/ldir_entry.h"
#include "list.h"

typedef struct
{
  char name[9];
  char extension[4];
  wchar_t long_name[256];
  cluster_t cluster_no;
  dir_attributes attributes;
  byte_t create_time_ms;
  dir_entry_time create_time;
  dir_entry_date create_date;
  dir_entry_date access_date;
  dir_entry_time update_time;
  dir_entry_date update_date;
  dword_t file_size;
} fat_dir_content;

int
fat_dir_content_get (fat_dir_content * content, dir_entry * dir_entry,
		  int pos, const int len);
void fat_dir_content_combine_short_name (fat_dir_content * content, char *buf);
void fat_dir_content_dump (fat_dir_content * content);
#endif /*FAT_DIR_CONTENT_H_ */
