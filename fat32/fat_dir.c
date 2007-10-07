#include "fat32/fat_dir.h"
#include "message.h"
#include <string.h>
#include <assert.h>

int fat_dir_read (fat_file * file, fat_dir_entry * dir)
{
	MESSAGE_DEBUG("file:%p\n", file);

	retry:
	if ((fat_file_read(file, &(dir->dir_entry), sizeof(dir_entry)))
			!= sizeof(dir_entry))
	{
		MESSAGE_DEBUG("read failed\n");
		return -1;
	}
	if (DIR_ENTRY_IS_END(&(dir->dir_entry)))
	{
		MESSAGE_DEBUG("dir is end\n");
		return -1;
	}else if (DIR_ENTRY_IS_DELETED(&(dir->dir_entry)) || DIR_ENTRY_IS_LDIR_ENTRY(&(dir->dir_entry)))
	{
		MESSAGE_DEBUG("retrying\n");
		goto retry;
	}else{
		dir_entry_combine_short_name(&(dir->dir_entry), dir->short_name);
		
	}
	return 0;
}

int fat_dir_find (fat_file * file, const char *name, fat_dir_entry *dir)
{
	MESSAGE_DEBUG("file:%p name:%s\n", file, name);
	
	while (!fat_dir_read (file, dir))
	{
		if (!strncmp (dir->short_name, name, 12))
		{
			MESSAGE_DEBUG("%s matched with %p\n", name, dir);
			return 0;
		}
		else
			MESSAGE_DEBUG("%s doesn't match with %s\n", name, dir->short_name);
	}
	return -1;
}
