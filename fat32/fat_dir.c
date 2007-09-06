#include "fat32/fat_dir.h"
#include "message.h"
#include "compatibility.h"

fat_dir *
fat_dir_new (fat_instance * ins, fat_cluster_chain * clusterChain)
{
  fat_dir *dir = (fat_dir *) calloc (1, sizeof (fat_dir));
  if (!dir)
    {
      MESSAGE_ERROR ("malloc failed");
      return NULL;
    }
  dir->file.ins = ins;
  dir->file.chain = clusterChain;
  dir->de_len = bpb_cluster_size (ins->bpb) / sizeof (dir_entry);
  dir->de_pos = -1;
  assert (bpb_cluster_size (ins->bpb) % sizeof (dir_entry) == 0);
  dir->dir_entry = (dir_entry *) calloc (dir->de_len, sizeof (dir_entry));
  return dir;
}

fat_dir *
fat_dir_open (fat_instance * ins, const char *name)
{
  fat_cluster_chain *clusterChain;
  if (!(clusterChain = fat_cluster_chain_get_by_name (ins, name)))
    return NULL;
  return fat_dir_new (ins, clusterChain);
}

int
fat_dir_read (fat_dir * dir, fat_dir_content * content)
{
  if (dir->de_pos == -1)
    {
      int res;
      if ((res = fat_file_read (&(dir->file), dir->dir_entry)) <= 0)
	return res;
      dir->de_pos = 0;
    }
  dir->de_pos =
    fat_dir_content_get (content, dir->dir_entry, dir->de_pos, dir->de_len);
  if (dir->de_pos == -1 && dir->file.is_reached_to_tail)
    return -1;
  return 1;
}

bool
fat_dir_find (fat_dir * dir, const char *name, fat_dir_content * content)
{
  while ((fat_dir_read (dir, content)) == 1)
    {
      char shortName[13], longName[256];

      fat_dir_content_combine_short_name (content, shortName);
      if (!strncmp (shortName, name, 12))
	return true;
      utf8_to_ascii (longName, content->long_name, 255);
      if (!strncmp (longName, name, 255))
	return true;
    }
  return false;
}

void
fat_dir_close (fat_dir * dir)
{
  fat_cluster_chain *chain = LIST_GET (list_head (&(dir->file.chain->list)), list,
				    fat_cluster_chain);
  assert (chain);
  fat_cluster_chain_delete (chain);
  free (dir->dir_entry);
  free (dir);
}
