#ifndef DIR_ENTRY_H_
#define DIR_ENTRY_H_
#include "fat32/fat_type.h"

#define DIR_ENTRY_IS_END(dir) \
	((dir)->name[0] == 0x00)

#define DIR_ENTRY_IS_DELETED(dir) \
	((dir)->name[0] == 0x05 ||(dir)->name[0] == 0xe5)

#define DIR_ENTRY_IS_LDIR_ENTRY(dir) \
	((dir)->attributes.read_only && (dir)->attributes.hidden \
	&& (dir)->attributes.system && (dir)->attributes.volume_label)

typedef struct
{
  word_t date:5;
  word_t month:4;
  word_t year:7;
}
__attribute__ ((packed)) dir_entry_date;

typedef struct
{
  word_t two_sec:5;
  word_t minite:6;
  word_t hour:5;
}
__attribute__ ((packed)) dir_entry_time;

typedef struct
{
  word_t read_only:1;
  word_t hidden:1;
  word_t system:1;
  word_t volume_label:1;
  word_t directory:1;
  word_t archive:1;
  word_t reserved:2;
}
__attribute__ ((packed)) dir_attributes;

typedef struct
{
  byte_t name[8];
  byte_t extension[3];
  dir_attributes attributes;
  byte_t nt_reserved;
  byte_t create_time_ms;
  dir_entry_time create_time;
  dir_entry_date create_date;
  dir_entry_date access_date;
  word_t cluster_hi;
  dir_entry_time update_time;
  dir_entry_date update_date;
  word_t cluster_lo;
  dword_t file_size;
}
__attribute__ ((packed)) dir_entry;

void dir_entry_dump (dir_entry * dir);
void dir_attributes_dump (dir_attributes * attributes);
void time_dump (dir_entry_time * time);
void date_dump (dir_entry_date * date);
byte_t dir_entry_short_name_check_code (dir_entry * dir);
cluster_t dir_entry_cluster (dir_entry * dir);

#endif /*DIR_ENTRY_H_ */
