#include "vfs_fd.h"

static vfs_fd *vfs_fd_construct(void);
static list_node fd_list;

void vfs_fd_init(void)
{
	memset(&fd_list, 0, sizeof(list_node));
}

vfs_fd *vfs_fd_construct(void)
{
	static int count = 0;
	vfs_fd *vfd = kalloc(sizeof(vfs_fd));
	memset(vfd, 0, sizeof(vfs_fd));
	vfd->no = count++;
	vfd->using = false;
	list_insert_tail(fd_list, vfd->list);
	return vfd;
}

vfs_fd vfs_fd_new(void)
{
	list_node *lp = fd_list.next;
	LIST_FOR_EACH(lp)
	{
		vfs_fd *vfd = LIST_GET(lp, list, vfs_fd);
		if(!vfd->using)
			return vfs_fd_open(vfd);
	}
	return vfs_fd_construct();
}

vfs_fd vfs_fd_get(int no)
{
	list_node *lp = fd_list.next;
	LIST_FOR_EACH(lp)
	{
		vfs_fd *vfd = LIST_GET(lp, list, vfs_fd);
		if(vfd->no == no)
			return vfd;
	}
	return NULL;
}

void vfs_fd_open(vfs_fd *fd)
{
	fd->using = true;
}

void vfs_fd_close(vfs_fd *fd)
{
	fd->using = false;
}

}