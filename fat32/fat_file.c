#include "fat32/cluster_data.h"
#include "fat32/fat_file.h"
#include "message.h"

fat_file *
fat_file_new (fat_instance * ins, fat_cluster_chain * clusterChain)
{
  fat_file *file = (fat_file *) calloc (1, sizeof (fat_file));
  if (!file)
    {
      MESSAGE_ERROR ("malloc failed");
      return NULL;
    }
  file->ins = ins;
  file->chain = clusterChain;
  file->is_reached_to_tail = false;
  return file;
}

fat_file *
fat_file_open (fat_instance * ins, const char *name)
{
  fat_cluster_chain *clusterChain;
  MESSAGE_DEBUG("ins:%p name:%s\n", ins, name);
  if (!(clusterChain = fat_cluster_chain_get_by_name (ins, name)))
    return NULL;
  return fat_file_new (ins, clusterChain);
}

int
fat_file_read (fat_file * file, void *buffer)
{
  if (file->is_reached_to_tail)
    return -1;
  if (!cluster_data_read (file->ins, file->chain->cluster_no, buffer))
    return -1;
  if (file->chain->list.next)
    {
      file->chain = LIST_GET (file->chain->list.next, list, fat_cluster_chain);
      return list_count (&(file->chain->list));
    }
  else
    {
      file->is_reached_to_tail = true;
      return 0;
    }
}

bool
fat_file_seek (fat_file * file, cluster_t pos)
{
  list_node *hp = list_head (&(file->chain->list));
  int i;
  for (i = 0; i <= pos; i++)
    if (hp->next)
      hp = hp->next;
    else
      {
	MESSAGE_ERROR ("seek failed");
	return false;
      }
  file->chain = LIST_GET (hp, list, fat_cluster_chain);
  assert(file->chain);
  return true;
}

void
fat_file_close (fat_file * file)
{
  fat_cluster_chain *chain = LIST_GET
             (list_head (&(file->chain->list)), list,
              fat_cluster_chain);
  assert(chain);
  fat_cluster_chain_delete (chain);
  free (file);
}
