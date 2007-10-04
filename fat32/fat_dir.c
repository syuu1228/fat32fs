#include "fat32/fat_dir.h"
#include "message.h"
#include "string.h"
#include <assert.h>

dir_entry *fat_dir_read(fat_file * file)
{
	MESSAGE_DEBUG("file:%p\n", file);
	static dir_entry dir;

	retry:
	if ((fat_file_read(file, &dir, sizeof(dir_entry)))
			!= sizeof(dir_entry))
	{
		MESSAGE_DEBUG("read failed\n");
		return NULL;
	}
	if (DIR_ENTRY_IS_END(&dir))
	{
		MESSAGE_DEBUG("dir is end\n");
		return NULL;
	}
	if (DIR_ENTRY_IS_DELETED(&dir) || DIR_ENTRY_IS_LDIR_ENTRY(&dir))
	{
		MESSAGE_DEBUG("retrying\n");
		goto retry;
	}
	MESSAGE_DEBUG("return:%p\n", &dir);
	return &dir;
}

dir_entry *fat_dir_find(fat_file * file, const char *name)
{
	MESSAGE_DEBUG("file:%p name:%s\n", file, name);
	dir_entry *dir;
	while (dir = fat_dir_read (file))
	{
		char short_name[13];
		dir_entry_combine_short_name (dir, short_name);
		if (!strncmp (short_name, name, 12))
		{
			MESSAGE_DEBUG("%s matched with %p\n", name, dir);
			break;
		}
		else
			MESSAGE_DEBUG("%s doesn't match with %s\n", name, short_name);
	}
	return dir;
}
