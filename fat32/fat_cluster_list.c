#include "fat32/fat_cluster_list.h"
#include "message.h"
#include <assert.h>

static fat_cluster_list *fat_cluster_list_new(fat_instance * ins, dword_t size);

fat_cluster_list *fat_cluster_list_new(fat_instance * ins, dword_t size)
{
	MESSAGE_DEBUG("\n");
	fat_cluster_list *list =(fat_cluster_list *) calloc (1, sizeof(fat_cluster_list));
	if (!list)
	{
		MESSAGE_ERROR ("malloc error\n");
		MESSAGE_DEBUG ("return:NULL\n");
		return NULL;
	}
	list->clusters = calloc((size / bpb_cluster_size(ins->bpb)) + 2, sizeof(cluster_t));
	if(!list->clusters)
	{
		free(list);
		MESSAGE_ERROR("malloc error\n");
		return NULL;
	}
	list->length = (size / bpb_cluster_size(ins->bpb)) + 2;
	MESSAGE_DEBUG("return:%p\n", list);
	return list;
}

fat_cluster_list *fat_cluster_list_open(fat_instance * ins, cluster_t head, dword_t size)
{
	MESSAGE_DEBUG("ins:%p head:%u\n", ins, head);
	fat_cluster_list *list = fat_cluster_list_new(ins, size);
	if (!list)
		return list;
	cluster_t cluster_no = head;
	list->clusters[0] = head;
	if(!head)
		return list;
	MESSAGE_DEBUG("list->clusters[%x] = %x\n", 0, head);
	list->offset = 1;
	do
	{
/*		if (list->offset == list->length - 1)
		{
			list->clusters = realloc(list->clusters, list->length * 2);
			if (!list->clusters)
			{
				MESSAGE_ERROR("realloc error\n");
				free(list);
				return NULL;
			}
			list->length *= 2;
			MESSAGE_DEBUG("reallocated as %u\n", list->length);
		}*/
		assert(list->offset < list->length);
		cluster_no = cluster_read(ins, cluster_no);
		list->clusters[list->offset++] = cluster_no;
		MESSAGE_DEBUG("list->clusters[%x] = %x\n", list->offset - 1, cluster_no);
		fat_cluster_list_dump(list);
	} while (!IS_END_OF_CLUSTER(cluster_no) && !IS_BAD_CLUSTER(cluster_no));
	list->end = list->offset;
	list->offset = 0;
	return list;
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

int fat_cluster_list_seek(fat_cluster_list *list, int offset)
{
	if (offset >= list->end)
		return -1;
	return list->offset = offset;
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