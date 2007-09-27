
#define FUSE_USE_VERSION 26

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#define _GNU_SOURCE

#include <fuse.h>
#include <ulockmgr.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include "vfs.h"

static struct fuse_operations fuse_oper = {
	.getattr	= fuse_getattr,
	.opendir	= fuse_opendir,
	.readdir	= fuse_readdir,
	.releasedir = fuse_releasedir,
	.open		= fuse_open,
	.read		= fuse_read,
	.release	= fuse_release
};

static int fuse_getattr(const char *path, struct stat *stbuf)
{
    int res;

    res = vfs_getattr(path, stbuf);
    if (res == -1)
        return -errno;

    return 0;
}

static int fuse_opendir(const char *path, struct fuse_file_info *fi)
{
	int fd;
    if ((fd = vfs_opendir(path)) < 0)
        return fd;

    fi->fh = fd;
    return 0;
}

static int fuse_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                       off_t offset, struct fuse_file_info *fi)
{
    dirent *de;

    vfs_seekdir(fi->fh, offset);
    while ((de = readdir(fi->fh)) != NULL) {
        struct stat st;
        memset(&st, 0, sizeof(st));
        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;
        if (filler(buf, de->d_name, &st, vfs_telldir(fi->fh)))
            break;
    }

    return 0;
}

static int fuse_releasedir(const char *path, struct fuse_file_info *fi)
{
    closedir(fi->fh);
    return 0;
}


static int fuse_open(const char *path, struct fuse_file_info *fi)
{
    int fd;

    if((fd = vfs_open(path, fi->flags, 0)) < 0)
        return fd;

    fi->fh = fd;
    return 0;
}

static int fuse_read(const char *path, char *buf, size_t size, off_t offset,
                    struct fuse_file_info *fi)
{
    int res;
    if((res = vfs_lseek(fi->fh, offset, 0)) < 0)
    	return res;
    if((res = vfs_read(fi->fh, buf, size)) < 0)
        return res;

    return res;
}

static int fuse_release(const char *path, struct fuse_file_info *fi)
{
    return vfs_close(fi->fh);
}

extern vfs_operations fat32fs_oper;
extern vfs_config fat32fs_config;

int main(int argc, char *argv[])
{
	vfs_init(&fat32fs_oper, fat32fs_config);
    return fuse_main(argc, argv, &fuse_oper, NULL);
}
