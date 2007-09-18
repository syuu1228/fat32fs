#ifndef VFS_FD_H_
#define VFS_FD_H_
#include "type.h"
#include "list.h"

typedef struct
{
	list_node list;
	int no;
	bool using;
	off_t offset;
	void *private_data;
} vfs_fd;

void vfs_fd_init(void);
vfs_fd *vfs_fd_new(void);
vfs_fd *vfs_fd_get(int no);
void vfs_fd_open(vfs_fd *vfd);
void vfs_fd_close(vfs_fd *vfd);
#endif /*VFS_FD_H_*/
