/*
void test_high_api (void)
{
  fat_instance *ins = fat_instance_new(0, 0);
  printf("bpb_cluster_size:%d\n", bpb_cluster_size(ins->bpb));
  fat_dir *dir = fat_dir_open(ins, "/");
  fat_dir_content content;
  while (fat_dir_read (dir, &content) > 0)
	  fat_dir_content_dump (&content);
  fat_dir_close(dir);
  
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
  fat_file_close(file);
  fat_instance_delete(ins);
  return 0;
}

#include <assert.h>
#include "fat32/fat_instance.h"
#include "fat32/fat_cluster_chain.h"
#include "fat32/dir_entry.h"
#include "fat32/cluster_data.h"
#include "fat32/fat_file.h"
void test_read_dir_by_fat_file(void)
{
	  fat_instance *ins = fat_instance_new(0, 0);
	  dir_entry dent;
	  fat_cluster_chain *chain = fat_cluster_chain_get(ins, 2);
	  fat_cluster_chain_dump(chain);
	  fat_file *file = fat_file_new(ins, chain);
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
			  fat_cluster_chain *chain = fat_cluster_chain_get(ins, dir_entry_cluster(&dent));
			  fat_cluster_chain_dump(chain);
			  fat_cluster_chain_delete(chain);
		  }
	  }
}*/
#include "fat32/fat_instance.h"
#include "fat32/fat_cluster_chain.h"
#include "fat32/fat_file.h"
#include "fat32/fat_dir.h" 
/*void test_read_dir_by_fat_dir(void)
{
	  fat_instance *ins = fat_instance_new(0, 0);
	  fat_cluster_chain *chain = fat_cluster_chain_get(ins, 2);
	  fat_file *file = fat_file_new(ins, chain);
	  dir_entry *dir;
	  while((dir = fat_dir_read(file)))
		  dir_entry_dump(dir);
}*/

void test_find_dir()
{
	fat_instance *ins = fat_instance_new(0, 0);
	fat_cluster_chain *chain = fat_cluster_chain_get(ins, 2);
	fat_file *file = fat_file_new(ins, chain);
	dir_entry *dir = fat_dir_find(file, "YAMS-1~1.PDF");
	dir_entry_dump(dir);
}
int main(int argc, char *argv[])
{
//	test_read_dir();
//	test_read_dir_by_fat_file();
//	test_read_dir_by_fat_dir();
	test_find_dir();
//	test_high_api();
    //return fuse_main(argc, argv, &fuse_oper, NULL);
}
