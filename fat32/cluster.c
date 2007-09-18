#include "fat32/cluster.h"
#include "message.h"
#include "string.h"
#include <assert.h>

off_t
cluster_offset (fat_instance * ins, const cluster_t cluster_no)
{
  MESSAGE_DEBUG("ins:%p clusterNo:%u\n", ins, cluster_no);
  off_t res = fat_instance_sector_to_offset (ins, ins->bpb->reserved_sectors);
  res += cluster_no * 4;
  MESSAGE_DEBUG("return:%u\n", res);
  return res;
}

dword_t
cluster_read (fat_instance * ins, const cluster_t cluster_no)
{
  dword_t extent;

  MESSAGE_DEBUG("ins:%p clusterNo:%u\n", ins, cluster_no);
  assert (bpb_validate_cluster_no (ins->bpb, cluster_no));
  if ((lseek (ins->disk_id,
	      cluster_offset (ins, cluster_no),
	      SEEK_SET)) != cluster_offset (ins, cluster_no))
    MESSAGE_ERROR_ABORT ("seek failed\n");
      if ((read (ins->disk_id, &extent, sizeof (dword_t))) != sizeof (dword_t))
	MESSAGE_ERROR_ABORT ("read failed\n");
      extent &= 0x0FFFFFFF;
      MESSAGE_DEBUG("return:%u\n", extent);
  return extent;
}

bool
cluster_is_bad (bpb * bpb, const dword_t extent)
{
	MESSAGE_DEBUG("bpb:%p extent:%u\n", bpb, extent);
	bool res;
  if (extent == 0x0ffffff7)
    res = true;
  else
	  res = false;
  MESSAGE_DEBUG("return:%d\n", res);
  return res;
}

bool
cluster_is_end (bpb * bpb, const dword_t extent)
{
	MESSAGE_DEBUG("bpb:%p extent:%u\n", bpb, extent);
	bool res;
  if (extent >= 0x0ffffff8 && extent <= 0x0fffffff)
    res = true;
  else
	  res = false;
  MESSAGE_DEBUG("return:%d\n", res);
  return res;
}

void
cluster_dump (fat_instance * ins, const cluster_t cluster_no)
{
	MESSAGE_DEBUG("ins:%p clusterNo:%u\n", ins, cluster_no);
  dword_t extent = cluster_read (ins, cluster_no);
  printf ("%u->", cluster_no);
  if (cluster_is_bad (ins->bpb, extent))
    {
      printf ("bad\n");
      return;
    }
  else if (cluster_is_end (ins->bpb, extent))
    {
      printf("end\n");
      return;
    }
  else
    {
      printf ("%u\n", extent);
      cluster_dump (ins, extent);
    }
}
