#include "fat32/mbr.h"
#include "fat32/fat.h"
#include "message.h"
#include "compatibility.h"

static mbr *m;

static inline partition_record *find_partition(mbr * mbr)
{
	partition_type valid_pt[] =
	{ PT_W95FAT32, PT_W95FAT32LBA };
	int i, j;

	MESSAGE_DEBUG("mbr:%p\n", mbr);
	for (i = 0; i < 4; i++)
		for (j = 0; j < sizeof (valid_pt); j++)
		if (mbr->partition_table[i].type == valid_pt[j])
		{
			MESSAGE_DEBUG("return:%p\n", mbr->partition_table + i);
			return mbr->partition_table + i;
		}
		MESSAGE_ERROR ("Unable to find FAT partition\n");
		MESSAGE_DEBUG("return:NULL\n");
		return NULL;
	}

fat_instance *fat_mount(const char *block)
{
	int fd;
	partition_record *partition;
	fat_instance *ins;
	MESSAGE_DEBUG("block:%s\n", block);
	if ((fd = open (block, O_RDONLY)) < 0)
	{
		MESSAGE_ERROR ("open failed\n");
		MESSAGE_DEBUG ("return:NULL\n");
		return NULL;
	}
	m = mbr_read (fd)
;	if (!(partition = find_partition (m)))
	{
		MESSAGE_DEBUG("return:NULL\n");
		return NULL;
	}

	if (!(ins = fat_instance_new (fd, partition)))
	{
		MESSAGE_DEBUG("return:NULL\n");
		return NULL;
	}
	MESSAGE_DEBUG("return:%p\n", ins);
	return ins;
}

void fat_unmount(fat_instance * ins)
{
	MESSAGE_DEBUG("ins:%p\n", ins);
	close (ins->disk_id)
;
  		fat_instance_delete (ins)
;
  		free (m)
;}
