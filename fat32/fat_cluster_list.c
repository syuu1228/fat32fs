#include "fat32/fat_cluster_list.h"
#include "message.h"
#include <assert.h>

static fat_cluster_list *fat_cluster_list_new(fat_instance * ins, int size);
static int fat_cluster_list_calculate_size_from_cluster_no(fat_instance * ins,
		cluster_t cluster_no);
static int fat_cluster_list_calculate_size_from_fat_dir_entry(
		fat_instance * ins, fat_dir_entry *dir);
static fat_cluster_list *fat_cluster_list_open(fat_instance * ins,
		cluster_t head, int size);

fat_cluster_list *fat_cluster_list_new(fat_instance * ins, int size)
{
	MESSAGE_DEBUG("\n");
	fat_cluster_list *list =(fat_cluster_list *) calloc (1, sizeof(fat_cluster_list));
	if (!list)
	{
		MESSAGE_ERROR ("malloc error\n");
		MESSAGE_DEBUG ("return:NULL\n");
		return NULL;
	}
	list->clusters = calloc(size, sizeof(cluster_t));
	if(!list->clusters)
	{
		free(list);
		MESSAGE_ERROR("malloc error\n");
		return NULL;
	}
	list->length = size;
	MESSAGE_DEBUG("return:%p\n", list);
	return list;
}

int fat_cluster_list_calculate_size_from_cluster_no(fat_instance * ins,
		cluster_t cluster_no)
{
	if (!cluster_no)
		return 0;
	assert(!IS_END_OF_CLUSTER(cluster_no) && !IS_BAD_CLUSTER(cluster_no));
	int i = 1;
	do
	{
		cluster_no = cluster_read(ins, cluster_no);
		assert(!IS_BAD_CLUSTER(cluster_no));
		i++;
	} while (!IS_END_OF_CLUSTER(cluster_no));
	return i;
}

int fat_cluster_list_calculate_size_from_fat_dir_entry(fat_instance * ins,
		fat_dir_entry *dir)
{
	return dir->dir_entry.file_size / bpb_cluster_size(ins->bpb) + 2;
}

fat_cluster_list *fat_cluster_list_open(fat_instance * ins, cluster_t head,
		int size)
{
	MESSAGE_DEBUG("ins:%p head:%u\n", ins, head);
	fat_cluster_list *list = fat_cluster_list_new(ins, size);
	if (!list)
		return list;
	cluster_t cluster_no = head;
	list->clusters[0] = head;
	if (!head)
		return list;
	MESSAGE_DEBUG("list->clusters[%x] = %x\n", 0, head);
	list->offset = 1;
	do
	{
		assert(list->offset < list->length);
		cluster_no = cluster_read(ins, cluster_no);
		assert(!IS_BAD_CLUSTER(cluster_no));
		list->clusters[list->offset++] = cluster_no;
		MESSAGE_DEBUG("list->clusters[%x] = %x\n", list->offset - 1, cluster_no);
#ifdef DEBUG
		fat_cluster_list_dump(list);
#endif
	} while (!IS_END_OF_CLUSTER(cluster_no));
	list->end = list->offset;
	list->offset = 0;
	return list;
}

fat_cluster_list *fat_cluster_list_open_by_cluster_no(fat_instance * ins,
		cluster_t cluster_no)
{
	return fat_cluster_list_open(ins, cluster_no,
			fat_cluster_list_calculate_size_from_cluster_no(ins, cluster_no));
}

fat_cluster_list *fat_cluster_list_open_by_fat_dir_entry(fat_instance * ins,
		fat_dir_entry *dir)
{
	cluster_t cluster_no = dir_entry_get_cluster(&(dir->dir_entry));
	if (dir->dir_entry.attributes.directory)
		return fat_cluster_list_open(
				ins,
				cluster_no,
				fat_cluster_list_calculate_size_from_cluster_no(ins, cluster_no));
	else
		return fat_cluster_list_open(ins, cluster_no,
				fat_cluster_list_calculate_size_from_fat_dir_entry(ins, dir));
}

cluster_t fat_cluster_list_read(fat_cluster_list *list)
{
	if (list->offset >= list->end)
		return END_OF_CLUSTER;
	return list->clusters[list->offset];
}

cluster_t fat_cluster_list_next(fat_cluster_list *list)
{
	cluster_t cluster = fat_cluster_list_read(list);
	if (IS_END_OF_CLUSTER(cluster))
		return END_OF_CLUSTER;
	list->offset++;
	return cluster;
}

int fat_cluster_list_tell(fat_cluster_list *list)
{
	return list->offset;
}

int fat_cluster_list_seek_set(fat_cluster_list *list, int offset)
{
	if (offset >= list->end)
		return -1;
	return list->offset = offset;
}

int fat_cluster_list_seek_cur(fat_cluster_list *list, int offset)
{
	if (list->offset + offset >= list->end)
		return -1;
	return list->offset += offset;
}

int fat_cluster_list_size(fat_cluster_list *list)
{
	return list->end;
}

void fat_cluster_list_close(fat_cluster_list * list)
{
	free(list->clusters);
	free(list);
}

void fat_cluster_list_dump(fat_cluster_list * list)
{
	MESSAGE_DEBUG("list:%p\n", list);
	int i;
	for (i = 0; i < list->offset; i++)
	{
		printf("list->clusters[%u]", i);
		printf(" == %x\n", list->clusters[i]);
		if (IS_END_OF_CLUSTER(list->clusters[i]))
		{
			printf("end\n");
			break;
		}
		else if (IS_BAD_CLUSTER(list->clusters[i]))
		{
			printf("bad\n");
			break;
		}
	}
}