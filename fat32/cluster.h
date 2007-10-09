#ifndef CLUSTER_H_
#define CLUSTER_H_
#include "fat32/fat_instance.h"
#include "fat32/fat_type.h"

#define IS_BAD_CLUSTER(ext) (ext == 0x0ffffff7)
#define IS_END_OF_CLUSTER(ext) (ext >= 0x0ffffff8 && ext <= 0x0fffffff)
#define END_OF_CLUSTER 0x0fffffff

off_t cluster_offset (fat_instance * ins, const cluster_t cluster_no);
dword_t cluster_read (fat_instance * ins, const cluster_t cluster_no);
void cluster_dump (fat_instance * ins, const cluster_t cluster_no);
#endif /*CLUSTER_H_ */
