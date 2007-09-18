#ifndef FAT_INSTANCE_H_
#define FAT_INSTANCE_H_
#include "mbr.h"
#include "fat32/bpb.h"
#include "type.h"

typedef struct _fat_instance
{
  int disk_id;
  partition_record *partition;
  bpb *bpb;
} fat_instance;

fat_instance *fat_instance_new (int disk_id, int partition_no);
void fat_instance_delete (fat_instance * ins);
void fat_instance_dump (fat_instance * ins);
off_t fat_instance_head_offset (fat_instance * ins);
off_t fat_instance_byte_to_offset (fat_instance * ins, qword_t byte);
off_t fat_instance_sector_to_offset (fat_instance * ins, sector_t sector);
off_t fat_instance_cluster_to_offset (fat_instance * ins, cluster_t cluster);
#endif /*FAT_INSTANCE_H_ */
