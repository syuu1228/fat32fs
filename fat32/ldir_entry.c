#include "fat32/ldir_entry.h"
#include "string.h"
#include <assert.h>

void ldir_entry_dump(ldir_entry * ldir)
{
	assert (sizeof (ldir_entry) == 32);
	assert (sizeof (lfn_sequence_number) == 1);
	printf ("sequenceNumber:\n");
	lfn_sequence_number_dump(&(ldir->sequence_number));
	printf ("checkCode:%p\n", ldir->check_code);
	printf ("lname:");
	outnws ((wchar_t *) ldir->name1, 5);
	outnws ((wchar_t *) ldir->name2, 6);
	outnws ((wchar_t *) ldir->name3, 2);
	printf("\n");
}

void lfn_sequence_number_dump(lfn_sequence_number * seq)
{
	printf ("\tdeleted:");
	printf ("%u\n", seq->deleted);
	printf ("\tisEnd:");
	printf ("%u\n", seq->is_end);
	printf ("\tnumber:");
	printf ("%p\n", seq->number);
}
