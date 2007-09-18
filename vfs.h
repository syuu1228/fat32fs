#ifndef VFS_H_
#define VFS_H_
#include "vfs_fd.h"

typedef struct
{
	int disk_id, partition_no;
	void *private_config;
} vfs_config;

typedef struct
{
	int (*init)(vfs_config *conf);
	int (*getattr)(const char *path, stat *stbuf);
	int (*opendir)(vfs_fd *vfd, const char *name);
	dirent *(*readdir)(vfs_fd *vfd);
	int (*closedir)(vfs_fd *vfd);
	int (*open)(vfs_fd *vfd, const char *path, int flags, mode_t mode);
	ssize_t (*read)(vfs_fd *vfd, void *buf, size_t count);
	off_t (*lseek)(vfs_fd *vfd, off_t offset, int whence);
	int (*close)(vfs_fd *vfd);
} vfs_operations;

typedef struct 
{
        unsigned long  st_dev;
        unsigned long  st_ino;
        unsigned short st_mode;
        unsigned short st_nlink;
        unsigned short st_uid;
        unsigned short st_gid;
        unsigned long  st_rdev;
        unsigned long  st_size;
        unsigned long  st_blksize;
        unsigned long  st_blocks;
        unsigned long  st_atime;
        unsigned long  st_atime_nsec;
        unsigned long  st_mtime;
        unsigned long  st_mtime_nsec;
        unsigned long  st_ctime;
        unsigned long  st_ctime_nsec;
        unsigned long  __unused4;
        unsigned long  __unused5;
} stat;

typedef struct
{
        long            d_ino;
        off_t			 d_off;
        unsigned short  d_reclen;
        char            d_name[256]; /* We must not include limits.h! */
} dirent;

void vfs_init(vfs_operations * oper, vfs_config * conf);
int vfs_getattr(const char *path, stat *stbuf);
int vfs_opendir(const char *name);
dirent *vfs_readdir(int fd);
int vfs_closedir(int fd);
int vfs_open(const char *path, int flags, mode_t mode);
ssize_t vfs_read(int fd, void *buf, size_t count);
off_t vfs_lseek(int fd, off_t offset, int whence);
int vfs_close(int fd);

#endif /*VFS_H_*/
