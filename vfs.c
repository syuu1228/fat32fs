#include "vfs.h"
#include "vfs_fd.h"
#include "message.h"

static vfs_operations *oper;

int vfs_init(vfs_operations * o, vfs_config * conf)
{
	vfs_fd_init();
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
	{
		vfs_fd *vfd = vfs_fd_new();
		if(!vfd)
		{
			MESSAGE_DEBUG("return:-1\n");
			return -1;
		}
		int ret = oper->opendir(vfd, name);
		if(ret < 0)
		{
			MESSAGE_DEBUG("return:%d\n", ret);
			return ret;
		}
		vfs_fd_open(vfd);
		return vfd->no;
	}
	return 0;
}

dirent *vfs_readdir(int fd)
{
	if(oper->readdir)
	{
		vfs_fd *vfd = vfs_fd_get(fd);
		if(!vfd)
			return NULL;
		return oper->readdir(vfd);
	}
	return NULL;
}

void vfs_seekdir(int fd, off_t offset)
{
	if(oper->seekdir)
	{
		vfs_fd *vfd = vfs_fd_get(fd);
		if(!vfd)
			return;
		return oper->seekdir(vfd, offset);
	}
	return;
}

off_t vfs_telldir(int fd)
{
	if(oper->telldir)
	{
		vfs_fd *vfd = vfs_fd_get(fd);
		if(!vfd)
			return 0;
		return oper->telldir(vfd);
	}
	return 0;
}

int vfs_closedir(int fd)
{
	if(oper->closedir)
	{
		vfs_fd *vfd = vfs_fd_get(fd);
		if(!vfd)
			return -1;
		int ret = oper->closedir(vfd);
		vfs_fd_close(vfd);
		return ret;
	}
	return -1;
}

int vfs_open(const char *path, int flags, mode_t mode)
{
	if(oper->open)
	{
		vfs_fd *vfd = vfs_fd_new();
		if(!vfd)
			return -1;
		int ret = oper->open(vfd, path, flags, mode);
		if(!ret)
			return -1;
		vfs_fd_open(vfd);
		return vfd->no;
	}
	return -1;
}

ssize_t vfs_read(int fd, void *buf, size_t count)
{
	if(oper->read)
	{
		vfs_fd *vfd = vfs_fd_get(fd);
		if(!vfd)
			return -1;
		return oper->read(vfd, buf, count);
	}
	return -1;
}

off_t vfs_lseek(int fd, off_t offset, int whence)
{
	if(oper->lseek)
	{
		vfs_fd *vfd = vfs_fd_get(fd);
		if(!vfd)
			return -1;
		return oper->lseek(vfd, offset, whence);
	}
	return 0;
}

int vfs_close(int fd)
{
	if(oper->close)
	{
		vfs_fd *vfd = vfs_fd_get(fd);
		if(!vfd)
			return -1;
		int ret = oper->close(vfd);
		vfs_fd_close(vfd);
		return ret;
	}
	return -1;
}
