#ifndef CLUSTER_DATA_H_
#define CLUSTER_DATA_H_
#include "fat32/fat_instance.h"

off_t cluster_data_offset (fat_instance * ins, const cluster_t cluster_no);
ssize_t cluster_data_read (fat_instance * ins, const cluster_t cluster_no, void *buffer, off_t offset, size_t count);
void cluster_data_dump (fat_instance * ins, const cluster_t cluster_no);
void cluster_data_dump_dir (fat_instance * ins, const cluster_t cluster_no);
#endif /*CLUSTER_DATA_H_ */
