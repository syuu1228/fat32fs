#define DISK_ID 0
#define PARTITION_NO 0
#include "vfs.h"
#include "fat32/fat_dir.h"
#include "fat32/fat_file.h"

static fat_instance *ins;

static int fat32fs_init(vfs_config *conf)
{
	if(!ins = fat_instance_new(conf->disk_id, conf->partition_no))
	{
		MESSAGE_ERROR("fat32fs initialize failed\n");
		return -1;
	}
	return 0;
}

static int fat32fs_getattr(const char *path, stat *stbuf)
{
    return 0;
}

static int fat32fs_opendir(vfs_fd *vfd, const char *name)
{
	fat_dir *dir = fat_dir_open(ins, name);
	if(!dir)
		return -1;
	vfd->private_data = (void *)dir;
    return 0;
}


static dirent *fat32fs_readdir(vfs_fd *vfd)
{
	static dirent;
    fat_dir_content content;
    
	if(!vfd->private_data)
		return -1;
    if(!fat_dir_read((fat_dir *)vfd_private_data, &content))
    	return -1;
    memset(&dirent, 0, sizeof(dirent));
    return &dirent;
}

static int fat32fs_closedir(vfs_fd *vfd)
{
	if(!vfd->private_data)
		return -1;
	return fat_dir_close((fat_dir *)vfd->private_data);
}

static int fat32fs_open(vfs_fd *vfd, const char *path, int flags, mode_t mode)
{
	fat_file *file = fat_file_open(ins, path);
	if(!file)
		return -1;
	vfd->private_data = (void *)file;
    return 0;
}

static ssize_t fat32fs_read(vfs_fd *vfd, void *buf, size_t count)
{
	if(!vfd->private_data)
		return -1;
	return fat_file_read((fat_file *)vfd_private_data, buf, count);
}

static off_t fat32fs_lseek(vfs_fd *vfd, off_t offset, int whence)
{
	if(!vfd->private_data)
		return -1;
	return fat_file_seek((fat_file *)vfd_private_data, offset);
}

static int fat32fs_close(vfs_fd *vfd)
{
	if(!vfd->private_data)
		return -1;
	return fat_file_close((fat_file *)vfd_private_data);
}

static vfs_operations fat32fs_oper = {
	.init		= fat32fs_init,
    .getattr	= fat32fs_getattr,
    .opendir	= fat32fs_opendir,
    .readdir	= fat32fs_readdir,
    .closedir	= fat32fs_closedir,
    .open		= fat32fs_open,
    .read		= fat32fs_read,
    .lseek		= fat32fs_lseek,
    .close		= fat32fs_close
};

static vfs_config fat32fs_config = {
		.disk_id = 0,
		.partition_no = 0,
		.private_config = 0
};

int main(int argc, char *argv[])
{
	vfs_init(&fat32fs_oper, &fat32fs_config);
}
