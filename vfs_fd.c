#include "vfs_fd.h"
#include "message.h"

static vfs_fd *vfs_fd_construct(void);
static list_node fd_list;

#define VFS_FD_DUMP(vfd) \
	MESSAGE_DEBUG("vfd:%p list:%p prev:%p next:%p using:%d offset:%u private_data:%p\n", \
	vfd, (vfd)->list, (vfd)->list.prev, (vfd)->list.next, (vfd)->using, (vfd)->offset, (vfd)->private_data)

void vfs_fd_init(void)
{
	MESSAGE_DEBUG("\n");
	memset(&fd_list, 0, sizeof(list_node));
}

vfs_fd *vfs_fd_construct(void)
{
	MESSAGE_DEBUG("\n");
	static int count = 0;
	vfs_fd *vfd = calloc(1, sizeof(vfs_fd));
	vfd->no = count++;
	vfd->using = false;
	list_insert_tail(&fd_list, &(vfd->list));
	VFS_FD_DUMP(vfd);
	return vfd;
}

vfs_fd *vfs_fd_new(void)
{
	MESSAGE_DEBUG("\n");
	list_node *lp = fd_list.next;
	LIST_FOR_EACH(lp)
	{
		vfs_fd *vfd = LIST_GET(lp, list, vfs_fd);
		VFS_FD_DUMP(vfd);
		if(!vfd->using)
			return vfd;
	}
	return vfs_fd_construct();
}

vfs_fd *vfs_fd_get(int no)
{
	MESSAGE_DEBUG("no:%d\n", no);
	list_node *lp = fd_list.next;
	LIST_FOR_EACH(lp)
	{
		vfs_fd *vfd = LIST_GET(lp, list, vfs_fd);
		VFS_FD_DUMP(vfd);
		if(vfd->no == no)
			return vfd;
	}
	MESSAGE_DEBUG("return:NULL\n");
	return NULL;
}

void vfs_fd_open(vfs_fd *vfd)
{
	VFS_FD_DUMP(vfd);
	vfd->using = true;
}

void vfs_fd_close(vfs_fd *vfd)
{
	VFS_FD_DUMP(vfd);
	vfd->using = false;
}
