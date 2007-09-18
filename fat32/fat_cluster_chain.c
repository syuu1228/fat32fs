#include "fat32/fat_cluster_chain.h"
#include "fat32/cluster.h"
#include "fat32/fat_dir.h"
#include "message.h"
#include <assert.h>

static inline fat_cluster_chain *fat_cluster_chain_new(cluster_t cluster_no,
		fat_cluster_chain * prev)
{
	MESSAGE_DEBUG("clusterNo:%u prev:%p\n", cluster_no, prev);
	fat_cluster_chain *chain =(fat_cluster_chain *) calloc (1,
			sizeof(fat_cluster_chain));
	if (!chain)
	{
		MESSAGE_ERROR ("malloc error\n");
		MESSAGE_DEBUG("return:NULL\n");
		return NULL;
	}
	chain->cluster_no = cluster_no;
	if (prev)
		list_insert_after (&(prev->list), &(chain->list));
	MESSAGE_DEBUG("return:%p\n", chain);
	return chain;
}

static inline bool get_next_content(fat_instance * ins, cluster_t clusterNo,
		const char *name, fat_dir_content * content)
{
	fat_cluster_chain *chain;
	fat_dir *dir;
	bool result;

	MESSAGE_DEBUG("ins:%p clusterNo:%u name:%s content:%p\n", ins, clusterNo, name, content);
	if (!(chain = fat_cluster_chain_get (ins, clusterNo)))
		return false;
	if (!(dir = fat_dir_new (ins, chain)))
	{
		fat_cluster_chain_delete(chain);
		return false;
	}
	result = fat_dir_find (dir, name, content);
	fat_dir_close (dir);
	MESSAGE_DEBUG("result:%d\n", result);
	return result;
}

fat_cluster_chain *fat_cluster_chain_get(fat_instance * ins,
		cluster_t cluster_no)
{
	fat_cluster_chain *head, *chain;
	MESSAGE_DEBUG("ins:%p clusterNo:%u\n", ins, cluster_no);
	head = chain = fat_cluster_chain_new (cluster_no, NULL);
	if (!head)
	{
		MESSAGE_DEBUG("return:NULL\n");
		return NULL;
	}
	while (!cluster_is_end (ins->bpb, cluster_no)&& !cluster_is_bad (ins->bpb,
			cluster_no))
	{
		cluster_no = cluster_read (ins, cluster_no);
		MESSAGE_DEBUG("clusterNo:%u\n", cluster_no);
		chain = fat_cluster_chain_new (cluster_no, chain);
	}
#ifdef DEBUG
	fat_cluster_chain_dump (head);
	list_dump (&(head->list));
#endif
	MESSAGE_DEBUG("return:%p\n", head);
	return head;
}

fat_cluster_chain *fat_cluster_chain_get_by_name(fat_instance * ins,
		const char *name)
{
	fat_dir_content content;
	char buf[256];
	MESSAGE_DEBUG("ins:%p name:%s\n", ins, name);
	if (!strcmp (name, "/"))
		return fat_cluster_chain_get (ins, ins->bpb->root_dir_cluster);
	memset (&content, 0, sizeof (content));
	strncpy (buf, name, 255);
	char *tp = strtok (buf, "/");
	if (!get_next_content (ins, ins->bpb->root_dir_cluster, tp, &content))
	{
		MESSAGE_DEBUG("return:NULL\n");
		return NULL;
	}
	while ((tp = strtok (NULL, "/")))
	if (!get_next_content (ins, content.cluster_no, tp, &content))
	{
		MESSAGE_DEBUG("return:NULL\n");
		return NULL;
	}
	fat_cluster_chain *res = fat_cluster_chain_get (ins, content.cluster_no);
	MESSAGE_DEBUG("return:%p\n", res);
	return res;
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

void fat_cluster_chain_dump(const fat_cluster_chain * const chain)
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
