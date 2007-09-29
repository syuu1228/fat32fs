#ifndef VFS_H_
#define VFS_H_
#include "vfs_fd.h"
#include "dirent.h"

typedef struct
{
	int disk_id, partition_no;
	void *private_config;
} vfs_config;

#define stat struct stat
typedef struct dirent dirent;

typedef struct
{
	int (*init)(vfs_config *conf);
	int (*getattr)(const char *path, stat *stbuf);
	int (*opendir)(vfs_fd *vfd, const char *name);
	dirent *(*readdir)(vfs_fd *vfd);
	void (*seekdir)(vfs_fd *vfd, off_t offset);
	off_t (*telldir)(vfs_fd *vfd);
	int (*closedir)(vfs_fd *vfd);
	int (*open)(vfs_fd *vfd, const char *path, int flags, mode_t mode);
	ssize_t (*read)(vfs_fd *vfd, void *buf, size_t count);
	off_t (*lseek)(vfs_fd *vfd, off_t offset, int whence);
	int (*close)(vfs_fd *vfd);
} vfs_operations;

int vfs_init(vfs_operations * oper, vfs_config * conf);
int vfs_getattr(const char *path,stat *stbuf);
int vfs_opendir(const char *name);
dirent *vfs_readdir(int fd);
void vfs_seekdir(int fd, off_t offset);
off_t vfs_telldir(int fd);
int vfs_closedir(int fd);
int vfs_open(const char *path, int flags, mode_t mode);
ssize_t vfs_read(int fd, void *buf, size_t count);
off_t vfs_lseek(int fd, off_t offset, int whence);
int vfs_close(int fd);

#endif /*VFS_H_*/
