#ifndef FAT_DIR_ENTRY_H_
#define FAT_DIR_ENTRY_H_
#include "fat32/dir_entry.h"
typedef struct
{
	dir_entry dir_entry;
	char short_name[13];
	char long_name[256];
} fat_dir_entry;

#endif /*FAT_DIR_ENTRY_H_*/
