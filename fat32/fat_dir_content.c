#include "fat32/fat_dir_content.h"
#include "message.h"

static inline int log_file_name_copy(wchar_t * wstr, ldir_entry * ldir,
		int pos, int len)
{
	assert(pos + 5 + 6 + 2 < len);
#ifdef DEBUG
	outnws((wchar_t *) ldir->name1, 5);
#endif
	memcpy (&(wstr[pos]), ldir->name1, sizeof(wchar_t) * 5);
	//    wcsncpy (&(wstr[pos]), (const wchar_t *) ldir->name1, 5);
	pos += 5;
#ifdef DEBUG
	outnws((wchar_t *) ldir->name2, 6);
#endif
	memcpy (&(wstr[pos]), ldir->name2, sizeof(wchar_t) * 6);
	//    wcsncpy (&(wstr[pos]), (const wchar_t *) ldir->name2, 6);
	pos += 6;
#ifdef DEBUG
	outnws((wchar_t *) ldir->name3, 2);
#endif
	memcpy (&(wstr[pos]), ldir->name3, sizeof(wchar_t) * 2);
	//    wcsncpy (&(wstr[pos]), (const wchar_t *) ldir->name3, 2);
	pos += 2;
	wstr[pos] = 0;
	return pos;
}

static inline void dir_entry_copy(fat_dir_content * content,
		dir_entry * dirEntry)
{
	strncpy (content->name, (const char *) dirEntry->name, 8);
	strncpy (content->extension, (const char *) dirEntry->extension, 3);
	content->cluster_no = dir_entry_cluster (dirEntry);
	memcpy (&(content->attributes), &(dirEntry->attributes),
			sizeof(dir_attributes));
	content->create_time_ms = dirEntry->create_time_ms;
	memcpy (&(content->create_time), &(dirEntry->create_time), sizeof(dir_entry_time));
	memcpy (&(content->create_date), &(dirEntry->create_date), sizeof(dir_entry_date));
	memcpy (&(content->access_date), &(dirEntry->access_date), sizeof(dir_entry_date));
	memcpy (&(content->update_time), &(dirEntry->update_time), sizeof(dir_entry_time));
	memcpy (&(content->update_date), &(dirEntry->update_date), sizeof(dir_entry_date));
	content->file_size = dirEntry->file_size;
}

static inline int sfncpy(char *dest, char *src, int len)
{
	int i;
	for (i = 0; i < len; i++)
	{
		dest[i] = src[i];
		if (src[i] == '\0'||src[i] == ' ')
			break;
	}
	dest[i] = '\0';
	return i;
}

int fat_dir_content_get(fat_dir_content * content, dir_entry * dir_entry,
		int pos, const int len)
{
	//  int lfnPos = 0;
	MESSAGE_DEBUG("pos:%d len:%d\n", pos, len);

	if (pos >= len)
	{
		MESSAGE_ERROR ("NULL pointer reference\n");
		return -1;
	}

	for (; pos < len; pos++)
	{
		if (DIR_ENTRY_IS_END (&(dir_entry[pos])))
		{
			pos = -1;
			break;
		}
		if (DIR_ENTRY_IS_DELETED (&(dir_entry[pos])))
			continue;
		if (DIR_ENTRY_IS_LDIR_ENTRY (&(dir_entry[pos])))
		{
#ifdef DEBUG
			//	  ldir_entry_dump ((ldir_entry *) & dir_entry[pos]);
#endif
			//      ldir_entry *ldir = (ldir_entry *)&(dir_entry[pos]);
			//      lfnPos = log_file_name_copy (content->long_name, ldir, lfnPos, 255);
			//assert (lnPos - content->long_name > 255 - 5 + 6 + 2);
			//assert (&(content->long_name[255]) - lnPos < 5 + 6 + 2);
			//        lnPos = log_file_name_copy (lnPos, ldir);
#ifdef DEBUG
			//       outnws(content->long_name, 255);
#endif
		}
		else
		{
#ifdef DEBUG
			//	  dir_entry_dump (&dir_entry[pos]);
#endif
			dir_entry_copy (content, &dir_entry[pos]);
			pos++;
			break;
		}
	}
	if (pos + 1>= len || DIR_ENTRY_IS_END (&(dir_entry[pos + 1])))
		pos = -1;
	MESSAGE_DEBUG("return:%d\n", pos);
	return pos;
}

void fat_dir_content_combine_short_name(fat_dir_content * content, char *buf)
{
	int pos = sfncpy (buf, content->name, 8);
	if (content->extension[0] != ' ')
	{
		buf[pos++] = '.';
		pos = sfncpy (&(buf[pos]), content->extension, 3);
	}
	//MessageDebug (buf);
}

void fat_dir_content_dump(fat_dir_content * content)
{
	printf ("name:");
	outns (content->name, 8);
	printf ("\nextension:");
	outns (content->extension, 3);
	printf ("\nshortName:");
	char shortName[13];
	fat_dir_content_combine_short_name (content, shortName);
	outns (shortName, 12);
	//  printf ("\nlongName:");
	//  outnws ((wchar_t *) content->long_name, 255);
	printf ("\nclusterNo:");
	printf("%u\n", content->cluster_no);
	puts ("attributes:");
	dir_attributes_dump (&(content->attributes));
	printf ("createTimeMs:");
	printf("%u\n", content->create_time_ms);
	puts ("createTime:");
	time_dump (&(content->create_time));
	puts ("createDate:");
	date_dump (&(content->create_date));
	puts ("accessDate:");
	date_dump (&(content->access_date));
	puts ("updateTime:");
	time_dump (&(content->update_time));
	puts ("updateDate:");
	date_dump (&(content->update_date));
	printf ("fileSize:%u\n\n", content->file_size);
}
