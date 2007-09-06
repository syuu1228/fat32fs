#include "fat32/bpb.h"
#include "fat32/fat_instance.h"
#include "message.h"

static inline sector_t bpb_root_dir_sectors(bpb * bpb)
{
	MESSAGE_DEBUG("bpb:%p\n", bpb);
	sector_t
			res =((bpb->root_entries * 32)+(bpb->bytes_per_sector - 1))
					/ bpb->bytes_per_sector;
	MESSAGE_DEBUG("return:%u\n", res);
	return res;
}

static inline sector_t bpb_data_sectors(bpb * bpb)
{
	MESSAGE_DEBUG("bpb:%p\n", bpb);
	sector_t res = bpb->total_sectors32- (bpb->reserved_sectors
			+(bpb->number_of_fats * bpb->sectors_per_fat32))
			+bpb_root_dir_sectors (bpb);
	MESSAGE_DEBUG("return:%u\n", res);
	return res;
}

bool bpb_validate_cluster_no(bpb * bpb, cluster_t clusterNo)
{
	MESSAGE_DEBUG("bpb:%p clusterNo:%u\n", bpb, clusterNo);
	return (bpb_count_of_clusters(bpb) >= clusterNo
			&& clusterNo > 1);
}

bool
bpb_validate (bpb * bpb)
{
	return (!bpb->total_sectors16 && bpb->signature == 0xaa55);
}

bool
bpb_read (void *ins)
{
	MESSAGE_DEBUG("ins:%p\n", ins);
	fat_instance *fIns = (fat_instance *) ins;
	if ((lseek
					(fIns->disk_id,
							fat_instance_head_offset (fIns),
							SEEK_SET)) != fat_instance_head_offset (fIns))
	{
		MESSAGE_ERROR ("lseek failed\n");
		MESSAGE_DEBUG("return:false\n");
		return false;
	}
	fIns->bpb = (bpb *) calloc (1, sizeof (bpb));
	if (!fIns->bpb)
	{
		MESSAGE_ERROR ("malloc failed\n");
		MESSAGE_DEBUG("return:false\n");
		return false;
	}
	if ((read (fIns->disk_id, fIns->bpb, sizeof (bpb))) != sizeof (bpb))
	{
		MESSAGE_ERROR ("read failed\n");
		MESSAGE_DEBUG("return:false\n");
		return false;
	}
	MESSAGE_DEBUG("return:true\n");
	return true;
}

cluster_t bpb_count_of_clusters(bpb * bpb)
{
	MESSAGE_DEBUG("bpb:%p\n", bpb);
	cluster_t res = bpb_data_sectors (bpb) / bpb->sectors_per_cluster;
	MESSAGE_DEBUG("return:%u\n", res);
	return res;
}

dword_t bpb_cluster_size(bpb * bpb)
{
	MESSAGE_DEBUG("bpb:%p\n", bpb);
	dword_t res = bpb->sectors_per_cluster * bpb->bytes_per_sector;
	MESSAGE_DEBUG("return:%u\n", res);
	return res;
}

void bpb_dump(bpb * bpb)
{
	printf ("JmpOpeCode:%p %p %p\n", bpb->jmp_ope_code[0],
			bpb->jmp_ope_code[1], bpb->jmp_ope_code[2]);
	printf ("OEMName:");
	outns ((const char *) bpb->oem_name, 8);
	printf ("BytesPerSector:%u\n", bpb->bytes_per_sector);
	printf ("SectorsPerCluster:%u\n", bpb->sectors_per_cluster);
	printf ("ReservedSectors:%u\n", bpb->reserved_sectors);
	printf ("NumberOfFATs:%u\n", bpb->number_of_fats);
	printf ("RootEntries:%u\n", bpb->root_entries);
	printf ("TotalSectors16:%u\n", bpb->total_sectors16);
	printf ("MediaDescriptor:%u\n", bpb->media_descriptor);
	printf ("SectorsPerFAT16:%u\n", bpb->sectors_per_fat16);
	printf ("SectorsPerTrack:%u\n", bpb->sectors_per_track);
	printf ("Heads:%u\n", bpb->heads);
	printf ("HiddenSectors:%u\n", bpb->hidden_sectors);
	printf ("TotalSectors32:%u\n", bpb->total_sectors32);

	printf ("SectorsPerFAT32:%u\n", bpb->sectors_per_fat32);
	printf ("ExtFlags:%u\n", bpb->ext_flags);
	printf ("FSVersion:%u\n", bpb->fs_version);
	printf ("RootDirCluster:%u\n", bpb->root_dir_cluster);
	printf ("FSInfoSector:%u\n", bpb->fs_info_sector);
	printf ("BackupBootSector:%u\n", bpb->backup_boot_sector);
	printf ("DriveNumber:%u\n", bpb->drive_number);
	printf ("BootSignature:%u\n", bpb->boot_signature);
	printf ("VolumeID:%u\n", bpb->volume_id);
	printf ("VolumeLabel:");
	outns ((const char *) bpb->volume_label, 11);
	printf ("\nFileSystemType:");
	outns ((const char *) bpb->file_system_type, 8);
	printf ("\nSignature:%p\n", bpb->signature);
}

sector_t bpb_cluster_to_sector(bpb * bpb, const cluster_t cluster)
{
	MESSAGE_DEBUG("bpb:%p cluster:%u\n", bpb, cluster);
	sector_t res = cluster * bpb->sectors_per_cluster;
	MESSAGE_DEBUG("return:%u\n", res);
	return res;
}

qword_t bpb_sector_to_byte(bpb * bpb, const sector_t sector)
{
	MESSAGE_DEBUG("bpb:%p sector:%u\n", bpb, sector);
	qword_t res = sector * bpb->bytes_per_sector;
	MESSAGE_DEBUG("return:%u\n", res);
	return res;
}
