#include "vfs.h"

static vfs_operations *oper;
static void * fd_table[FD_TABLE_SIZE];

int vfs_init(vfs_operations * o, vfs_config * conf)
{
	memset(fd_table, 0, FD_TABLE_SIZE);
	oper = o;
	if(oper->init)
		return oper->init(conf);
	return 0;
}

int vfs_getattr(const char *path, stat *stbuf)
{
	if(oper->getattr)
		return oper->getattr(path, stbuf);
	return -1;
}

int vfs_opendir(const char *name)
{
	if(oper->opendir)
		return oper->opendir(name);
	return NULL;
}

dirent *vfs_readdir(int fd)
{
	if(oper->readdir)
		return oper->readdir(fd);
	return NULL;
}
int vfs_closedir(int fd)
{
	if(oper->closedir)
		return oper->closedir(fd);
	return -1;
}
int vfs_open(const char *pathname, int flags, mode_t mode)
{
	if(oper->open)
		return oper->open(pathname, flags, mode);
	return -1;
}

ssize_t vfs_read(int fd, void *buf, size_t count)
{
	if(oper->read)
		return oper->read(fd, buf, count);
	return -1;
}

off_t vfs_lseek(int fd, off_t offset, int whence)
{
	if(oper->lseek)
		return oper->lseek(fd, offset, whence);
	return 0;
}

int vfs_close(int fd)
{
	if(oper->close)
		return oper->close(fd);
	return -1;
}
