#define DISK_ID 0
#define PARTITION_NO 0

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

static int fat32fs_opendir(const char *name)
{
    return NULL;
}


static dirent *fat32fs_readdir(int fd)
{
    return NULL;
}

static int fat32fs_closedir(int fd)
{
    return 0;
}

static int fat32fs_open(const char *pathname, int flags, mode_t mode)
{
    return 0;
}

static ssize_t fat32fs_read(int fd, void *buf, size_t count)
{
	return 0;
}

static off_t fat32fs_lseek(int fildes, off_t offset, int whence)
{
	return 0;
}

static int fat32fs_close(int fd)
{
    return 0;
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
