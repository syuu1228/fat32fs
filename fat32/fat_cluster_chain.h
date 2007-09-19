#ifndef FAT_CLUSTER_CHAIN_H_
#define FAT_CLUSTER_CHAIN_H_
#include "fat32/fat_type.h"
#include "fat32/fat_instance.h"
#include "list.h"

typedef struct
{
  cluster_t cluster_no;
  list_node list;
} fat_cluster_chain;

fat_cluster_chain *fat_cluster_chain_get (fat_instance * ins, cluster_t cluster_no);
fat_cluster_chain *fat_cluster_chain_get_by_name (fat_instance * ins,
					   const char *name);
void fat_cluster_chain_delete (fat_cluster_chain * chain);
void fat_cluster_chain_dump (const fat_cluster_chain * const chain);
fat_cluster_chain *fat_cluster_chain_next(fat_cluster_chain *chain);

#endif /*FAT_CLUSTER_CHAIN_H_ */
