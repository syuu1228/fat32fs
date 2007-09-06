#ifndef LDIR_ENTRY_H_
#define LDIR_ENTRY_H_
#include "fat32/fat_type.h"

typedef struct
{
  word_t number:6;
  word_t is_end:1;
  word_t deleted:1;
}
__attribute__ ((packed)) lfn_sequence_number;

typedef struct
{
  lfn_sequence_number sequence_number;
  word_t name1[5];
  byte_t attributes;
  byte_t type;
  byte_t check_code;
  word_t name2[6];
  word_t cluster_lo;
  word_t name3[2];
}
__attribute__ ((packed)) ldir_entry;

void ldir_entry_dump (ldir_entry * ldir);
void lfn_sequence_number_dump (lfn_sequence_number * seq); 
#endif /*LDIR_ENTRY_H_ */
