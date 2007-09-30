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
#include "message.h"


extern vfs_operations fat32fs_oper;
extern vfs_config fat32fs_config;

static void *fuse_init(void)
{
	vfs_init(&fat32fs_oper, &fat32fs_config);
}

static int fuse_getattr(const char *path, stat *stbuf)
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
    {
    	MESSAGE_DEBUG("return:%d\n", fd);
        return fd;
    }
    
    fi->fh = fd;
    return 0;
}

static int fuse_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                       off_t offset, struct fuse_file_info *fi)
{
    dirent *de;

    filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);
    vfs_seekdir(fi->fh, offset);
    while ((de = vfs_readdir(fi->fh)) != NULL) {
        stat st;
        memset(&st, 0, sizeof(st));
        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;
        if (filler(buf, de->d_name, &st, 0))
            break;
    }

    return 0;
}

static int fuse_releasedir(const char *path, struct fuse_file_info *fi)
{
    vfs_closedir(fi->fh);
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

static struct fuse_operations fuse_oper = {
	.init		= fuse_init,
	.getattr	= fuse_getattr,
	.opendir	= fuse_opendir,
	.readdir	= fuse_readdir,
	.releasedir = fuse_releasedir,
	.open		= fuse_open,
	.read		= fuse_read,
	.release	= fuse_release
};

#include "fat32/fat_instance.h"
#include "fat32/fat_dir.h"
#include "fat32/fat_file.h"
void test_high_api (void)
{
	
  fat_instance *ins = fat_instance_new(0, 0);
  printf("bpb_cluster_size:%d\n", bpb_cluster_size(ins->bpb));
  fat_dir *dir = fat_dir_open(ins, "/");
  fat_dir_content content;
  while (fat_dir_read (dir, &content) > 0)
	  fat_dir_content_dump (&content);
  fat_dir_close(dir);
  /*
  dir = fat_dir_open(ins, "/HOGE2");
  while (fat_dir_read (dir, &content) > 0)
	  fat_dir_content_dump (&content);
  fat_dir_close(dir);
  fat_file *file = fat_file_open(ins, "/HOGE");
  char buf[512];
  fat_file_read(file, buf, 512);
  printf("/HOGE:%s\n", buf);
  fat_file_close(file);
  file = fat_file_open(ins, "/HOGE2/HIGE");
  fat_file_read(file, buf, 512);
  printf("/HOGE2/HIGE:%s\n", buf);
  fat_file_close(file);*/
  fat_instance_delete(ins);
  return 0;
}

int main(int argc, char *argv[])
{
	test_high_api();
    //return fuse_main(argc, argv, &fuse_oper, NULL);
}
