#ifndef FAT_H_
#define FAT_H_
#include "fat32/fat_instance.h"
#include "fat32/fat_file.h"
#include "fat32/fat_dir.h"

fat_instance *fat_mount (const char *block);
void fat_unmount (fat_instance * ins);
#define FAT_CLUSTER_SIZE(ins) bpb_cluster_size(ins->bpb)
#endif /*FAT_H_ */
