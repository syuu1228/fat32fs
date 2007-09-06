#ifndef CLUSTER_H_
#define CLUSTER_H_
#include "fat32/fat_instance.h"
#include "fat32/fat_type.h"

off_t
cluster_offset (fat_instance * ins, const cluster_t cluster_no)
;
dword_t cluster_read (fat_instance * ins, const cluster_t cluster_no);
bool cluster_is_bad (bpb * bpb, const dword_t extent);
bool cluster_is_end (bpb * bpb, const dword_t extent);
void cluster_dump (fat_instance * ins, const cluster_t cluster_no);
#endif /*CLUSTER_H_ */
