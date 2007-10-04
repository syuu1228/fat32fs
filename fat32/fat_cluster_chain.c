#include "fat32/fat_cluster_chain.h"
#include "fat32/cluster.h"
#include "message.h"
#include <assert.h>
#include <stdlib.h>

static inline fat_cluster_chain *fat_cluster_chain_new(cluster_t cluster_no)
{
	MESSAGE_DEBUG("cluster_no:%u\n", cluster_no);
	fat_cluster_chain *chain =(fat_cluster_chain *) calloc (1,
			sizeof(fat_cluster_chain));
	if (!chain)
	{
		MESSAGE_ERROR ("malloc error\n");
		MESSAGE_DEBUG("return:NULL\n");
		return NULL;
	}
	chain->cluster_no = cluster_no;
	MESSAGE_DEBUG("return:%p\n", chain);
	return chain;
}

fat_cluster_chain *fat_cluster_chain_get(fat_instance * ins,
		cluster_t cluster_no)
{
	MESSAGE_DEBUG("ins:%p cluster_no:%u\n", ins, cluster_no);
	fat_cluster_chain *head, *chain;
	cluster_t head_no = cluster_no;
	if(!cluster_no)
		return NULL;
	do
	{
		chain = fat_cluster_chain_new (cluster_no);
		if (cluster_no == head_no)
			head = chain;
		else
			list_insert_tail(&head->list, &chain->list);
		cluster_no = cluster_read (ins, cluster_no);
	} while (!cluster_is_end (ins->bpb, cluster_no) &&!cluster_is_bad (
			ins->bpb, cluster_no));
#ifdef DEBUG
	fat_cluster_chain_dump (head);
#endif
	MESSAGE_DEBUG("return:%p\n", head);
	return head;
}

void fat_cluster_chain_delete(fat_cluster_chain * chain)
{
	list_node iterator;
	MESSAGE_DEBUG("chain:%p\n", chain);
	list_node_copy (&iterator, &(chain->list));
	while (iterator.next)
	{
		chain = LIST_GET (iterator.next, list, fat_cluster_chain);
		assert (chain);
		list_node_copy (&iterator, &(chain->list));
		free (chain);
	}
}

fat_cluster_chain * fat_cluster_chain_next(fat_cluster_chain *chain)
{
	if (chain && chain->list.next)
		return LIST_GET (chain->list.next, list, fat_cluster_chain);
	return NULL;
}

void fat_cluster_chain_dump(fat_cluster_chain * chain)
{
	MESSAGE_DEBUG("chain:%p\n", chain);
	list_node *lp = (list_node *) & (chain->list);
	LIST_FOR_EACH (lp)
	{
		printf("%p:", lp);
		fat_cluster_chain *chain = LIST_GET (lp, list, fat_cluster_chain);
		assert (chain);
		printf ("%p\n", chain->cluster_no);
	}
}
