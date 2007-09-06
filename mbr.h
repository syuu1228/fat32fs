#ifndef MBR_H_
#define MBR_H_
#include "type.h"

#define MBR_SECTOR_SIZE 512

typedef struct
{
  byte_t head;
  byte_t sector;
  byte_t cylinder;
}
chs;

typedef struct
{
  byte_t boot_flag;
  chs chs_start;
  byte_t type;
  chs chs_end;
  dword_t lba_start;
  dword_t lba_total;
}
partition_record;

typedef enum
{
  PT_FAT12 = 0x01,
  PT_FAT16lt32M = 0x04,
  PT_FAT16 = 0x06,
  PT_W95FAT32 = 0x0b,
  PT_W95FAT32LBA = 0x0c,
  PT_W95FAT16LBA = 0x0e
} partition_type;

typedef struct
{
  byte_t loader[446];
  partition_record partition_table[4];
  word_t signature;
}
mbr;

mbr *mbr_read (const int fd);
void mbr_dump (mbr * mbr);
void partition_record_dump (partition_record * partition);
off_t mbr_partition_offset (partition_record * partition);
#endif /*MBR_H_ */
