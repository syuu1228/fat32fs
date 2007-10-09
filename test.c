#include <assert.h>
#include "fat32/fat_instance.h"
#include "fat32/fat_cluster_list.h"
#include "fat32/dir_entry.h"
#include "fat32/cluster_data.h"
#include "fat32/fat_file.h"
#include "fat32/fat_dir.h" 
#include "fat32/fat_path.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void test_read_dir_by_fat_file(void)
{
	  fat_instance *ins = fat_instance_new(0, 0);
	  assert(ins);
	  dir_entry dent;
	  fat_cluster_list *list = fat_cluster_list_open(ins, 2);
	  assert(list);
	  fat_cluster_list_dump(list);
	  fat_file *file = fat_file_new(ins, list);
	  assert(file);
	  for(;;)
	  {
		  int res;
		  if((res = fat_file_read(file, &dent, sizeof(dir_entry))) != sizeof(dir_entry))
		  {
			  printf("read failed:%d\n", res);
			  break;
		  }
		  if(DIR_ENTRY_IS_END(&dent))
		  {
			  printf("dent is end\n");
			  break;
		  }
		  else if(DIR_ENTRY_IS_DELETED(&dent))
			  printf("dent is deleted\n");
		  else if(DIR_ENTRY_IS_LDIR_ENTRY(&dent))
			  printf("dent is ldir_entry\n");
		  else
		  {
			  dir_entry_dump(&dent);
			  cluster_t cluster_no = dir_entry_get_cluster(&dent);
			  if(!cluster_no)
			  {
				  printf("unallocated file\n");
				  continue;
			  }
			  fat_cluster_list *list = fat_cluster_list_open(ins, cluster_no);
			  fat_cluster_list_dump(list);
			  fat_cluster_list_close(list);
		  }
	  }
	  fat_file_close(file);
	  fat_instance_delete(ins);
}

void test_read_dir_by_fat_dir(void)
{
	  fat_instance *ins = fat_instance_new(0, 0);
	  assert(ins);
	  fat_cluster_list *list = fat_cluster_list_open(ins, 2);
	  assert(list);
	  fat_file *file = fat_file_new(ins, list);
	  assert(file);
	  fat_dir_entry dir;
	  while(!(fat_dir_read(file, &dir)))
		  dir_entry_dump(&dir);
	  fat_file_close(file);
	  fat_instance_delete(ins);
}

void test_find_dir()
{
	fat_instance *ins = fat_instance_new(0, 0);
	fat_cluster_list *list = fat_cluster_list_open(ins, 2);
	fat_file *file = fat_file_new(ins, list);
	fat_dir_entry dir;
	assert(!fat_dir_find(file, "YAMS-1~1.PDF", &dir));
	dir_entry_dump(&dir.dir_entry);
	assert(!fat_path_get_entry(ins, "/HOGE2/HIGE", &dir));
	dir_entry_dump(&dir.dir_entry);
	fat_file_close(file);
	fat_instance_delete(ins);
}

void test_read_file()
{
	char buf[1025];
	fat_instance *ins = fat_instance_new(0, 0);
	fat_dir_entry dir;
	assert(!fat_path_get_entry(ins, "/YAMS-1~1.PDF", &dir));
	dir_entry_dump(&dir.dir_entry);
	fat_file *file = fat_file_open(ins, "/YAMS-1~1.PDF");
	assert(file);
	dword_t cnt = dir.dir_entry.file_size;
	int fd = open("/tmp/yams.pdf", O_CREAT | O_RDWR);
	assert(fd > 0);
	while(cnt >= 1024)
	{
		assert(fat_file_read(file, buf, 1024) == 1024);
		assert(write(fd, buf, 1024) == 1024);
		cnt -= 1024;
	}
	if(cnt)
	{
		assert(fat_file_read(file, buf, cnt) == cnt);
		assert(write(fd, buf, cnt) == cnt);
	}
	fat_file_close(file);
	fat_instance_delete(ins);
	close(fd);
}

int main(int argc, char *argv[])
{
	test_read_dir_by_fat_file();
	test_read_dir_by_fat_dir();
	test_find_dir();
	test_read_file();
}
