#include "mbr.h"
#include "message.h"
#include <stdlib.h>
#include "disk.h"

mbr *mbr_read(const int id)
{
	MESSAGE_DEBUG("id:%d\n", id);
	mbr *m = (mbr *)calloc(1, sizeof(mbr));
	if ((disk_read (id, m, 0, sizeof (mbr))) != sizeof (mbr))
	{
		MESSAGE_ERROR_ABORT ("read failed\n");
		free(m);
		return NULL;
	}
	MESSAGE_DEBUG("return:%p\n", m);
	return m;
}

void mbr_dump(mbr * mbr)
{
	MESSAGE_DEBUG("mbr:%p\n", mbr);
	int i;
	for (i = 0; i < 4; i++)
	{
		printf ("Nr:%2d ", i);
		partition_record_dump(&(mbr->partition_table[i]));
	}
	printf ("Signature:0x%04X\n\n", mbr->signature);
}

void partition_record_dump(partition_record * partition)
{
	MESSAGE_DEBUG("partition:%p\n", partition);
	printf ("Bt:%3u Hd:%4u Sec:%4u Cyl:%4u ID:%2x\n", partition->boot_flag,
			partition->chs_start.head, partition->chs_start.sector,
			partition->chs_start.cylinder, partition->type);
	printf ("Hd:%4u Sec:%4u Cyl:%4u Start:%10u Size:%10u\n",
			partition->chs_end.head, partition->chs_end.sector,
			partition->chs_end.cylinder, partition->lba_start,
			partition->lba_total);
}

off_t mbr_partition_offset(partition_record * partition)
{
	MESSAGE_DEBUG("partition:%p\n", partition);
	off_t res = partition->lba_start * MBR_SECTOR_SIZE;
	MESSAGE_DEBUG("return:%d\n", res);
	return res;
}
