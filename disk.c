#include "message.h"
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define DISK_FILE_NAME "/home/syuu/workspace/fat32fs/fat32test.img"

int disk_read(int id, void *buf, off_t offset, int count)
{
	int c = count, fd;

	MESSAGE_DEBUG("id:%d buf:%p\n", id, buf); 
	MESSAGE_DEBUG("offset:%u\n", offset);
	MESSAGE_DEBUG("count:%d\n",  count);
	if((fd = open(DISK_FILE_NAME, O_RDONLY)) <=  0)
		MESSAGE_ERROR_ABORT("open failed\n");
	if((lseek(fd, offset, SEEK_SET)) != offset)
		MESSAGE_ERROR_ABORT("lseek failed\n");
	if((read(fd, buf, count)) != count)
		MESSAGE_ERROR_ABORT("read failed\n");
	close(fd);
	return c;
}
