#define DISK_ID 0
#define PARTITION_NO 0
#include "vfs.h"
#include "message.h"
#include "fat32/fat_instance.h"
#include "fat32/fat_dir.h"
#include "fat32/fat_file.h"
#include "fat32/fat_path.h"
#include "string.h"

static fat_instance *ins;

static int fat32fs_init(vfs_config *conf) {
	if (!(ins = fat_instance_new(conf->disk_id, conf->partition_no))) {
		MESSAGE_ERROR("fat32fs initialize failed\n");
		return -1;
	}
	return 0;
}

static int fat32fs_getattr(const char *path, stat *stbuf) {
	MESSAGE_DEBUG("path:%s stbuf:%p\n", path, stbuf);

	fat_dir_entry dir;
	if (fat_path_get_entry(ins, path, &dir) < 0)
		return -1;
	memset(stbuf, 0, sizeof(stat));
	if (dir.dir_entry.attributes.directory) {
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
	} else {
		stbuf->st_mode = S_IFREG | 0644;
		stbuf->st_nlink = 1;
	}
	stbuf->st_size = dir.dir_entry.file_size;
	stbuf->st_ino = dir_entry_get_cluster(&dir.dir_entry);
	return 0;
}

static int fat32fs_opendir(vfs_fd *vfd, const char *name) {
	MESSAGE_DEBUG("vfd:%p name:%s\n", vfd, name);
	fat_file *dir = fat_file_open(ins, name);
	if (!dir) {
		MESSAGE_DEBUG("return:-1\n");
		return -1;
	}
	vfd->private_data =(void *)dir;
	MESSAGE_DEBUG("return:0\n");
	return 0;
}

static dirent *fat32fs_readdir(vfs_fd *vfd) {
	static dirent d;
	fat_dir_entry dir;

	if (!vfd->private_data)
		return NULL;
	if (fat_dir_read((fat_file *)vfd->private_data, &dir) < 0)
		return NULL;
	memset(&d, 0, sizeof(dirent));
	strcpy(d.d_name, dir.short_name);
	d.d_reclen = strlen(dir.short_name);
	d.d_ino = dir_entry_get_cluster(&dir.dir_entry);
	if(dir.dir_entry.attributes.directory)
		d.d_type = DT_DIR;
	else
		d.d_type = DT_REG;
	MESSAGE_DEBUG("dirent:%p ino:%d off:%u reclen:%d type:%d\n",
	&d, d.d_ino, d.d_off, d.d_reclen, d.d_type);
	return &d;
}

static void fat32fs_seekdir(vfs_fd *vfd, off_t offset) {
	if (!vfd->private_data)
		return -1;
	return fat_file_lseek((fat_file *)vfd->private_data, offset);
}

static off_t fat32fs_telldir(vfs_fd *vfd) {
	if (!vfd->private_data)
		return -1;
	return fat_file_tell((fat_file *)vfd->private_data);
}

static int fat32fs_closedir(vfs_fd *vfd) {
	if (!vfd->private_data)
		return -1;
	return fat_file_close((fat_file *)vfd->private_data);
}

static int fat32fs_open(vfs_fd *vfd, const char *path, int flags, mode_t mode) {
	fat_file *file = fat_file_open(ins, path);
	if (!file)
		return -1;
	vfd->private_data =( void *)file;
	return 0;
}

static ssize_t fat32fs_read(vfs_fd *vfd, void *buf, size_t count) {
	if (!vfd->private_data)
		return -1;
	return fat_file_read((fat_file *)vfd->private_data, buf, count);
}

static off_t fat32fs_lseek(vfs_fd *vfd, off_t offset, int whence) {
	if (!vfd->private_data)
		return -1;
	return fat_file_lseek((fat_file *)vfd->private_data, offset);
}

static int fat32fs_close(vfs_fd *vfd) {
	if (!vfd->private_data)
		return -1;
	return fat_file_close((fat_file *)vfd->private_data);
}

vfs_operations fat32fs_oper = {
.init = fat32fs_init,
.getattr = fat32fs_getattr,
.opendir = fat32fs_opendir,
.readdir = fat32fs_readdir,
.seekdir = fat32fs_seekdir,
.telldir = fat32fs_telldir,
.closedir = fat32fs_closedir,
.open = fat32fs_open,
.read = fat32fs_read,
.lseek = fat32fs_lseek,
.close = fat32fs_close
};

vfs_config fat32fs_config = {
.disk_id = 0,
.partition_no = 0,
.private_config = 0
};
