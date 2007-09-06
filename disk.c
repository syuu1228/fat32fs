#include "message.h"
#define DISK_FILE_NAME "fat32test.img"

int disk_read(int id, void *buf, unsigned offset, int count)
{
	int i, c = count, fd;

	MESSAGE_DEBUG("id:%d buf:%x offset:%d count:%d\n", id, buf, offset, count);
	if((fd = open(DISK_FILE_NAME, O_RDONLY)) <  0)
		MESSAGE_ERROR_ABORT("open failed\n");
	if((lseek(fd, offset, SEEK_SET)))
		MESSAGE_ERROR_ABORT("lseek failed\n");
	if((read(fd, buf, count)) != count)
		MESSAGE_ERROR_ABORT("read failed\n");
	return c;
}
