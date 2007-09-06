#ifndef BPB_H_
#define BPB_H_
#include "fat32/fat_type.h"

typedef struct _bpb
{
	byte_t jmp_ope_code[3];
	byte_t oem_name[8];
	word_t bytes_per_sector;
	byte_t sectors_per_cluster;
	word_t reserved_sectors;
	byte_t number_of_fats;
	word_t root_entries;
	word_t total_sectors16;
	byte_t media_descriptor;
	word_t sectors_per_fat16;
	word_t sectors_per_track;
	word_t heads;
	dword_t hidden_sectors;
	dword_t total_sectors32;
	dword_t sectors_per_fat32;
	word_t ext_flags;
	word_t fs_version;
	dword_t root_dir_cluster;
	word_t fs_info_sector;
	word_t backup_boot_sector;
	byte_t reserved[12];
	byte_t drive_number;
	byte_t reserved1;
	byte_t boot_signature;
	dword_t volume_id;
	byte_t volume_label[11];
	byte_t file_system_type[8];
	byte_t load_program_code[420];
	word_t signature;
} __attribute__ ((packed)) bpb;

bool bpb_validate_cluster_no(bpb * bpb, cluster_t cluster_no);
bool bpb_validate (bpb * bpb);

bool bpb_read (void *ins);
void bpb_dump(bpb * bpb);
dword_t bpb_cluster_size(bpb * bpb);
cluster_t bpb_count_of_clusters(bpb * bpb);
sector_t bpb_cluster_to_sector(bpb * bpb, const cluster_t cluster);
qword_t bpb_sector_to_byte(bpb * bpb, const sector_t sector);
#endif /*BPB_H_ */
