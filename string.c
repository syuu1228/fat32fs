#include "string.h"

void outns(const char *str, int n)
{
	while (n--)
		outc (*str++);
}

int
outw (wchar_t wc)
{
  byte_t *mb = (byte_t *) & wc;
  if (!mb[1])
    outc (mb[0]);
  else
    outc ('?');
  return 0;
}

int
outws (wchar_t * wstr)
{
  while(*wstr != 0 && *wstr != 0xFFFF)
    outw (*wstr++);
  return 0;
}

int
outnws (wchar_t * wstr, int len)
{
  int i;
  for (i = 0; i < len; i++)
    {
      if (wstr[i] != 0 && wstr[i] != 0xFFFF)
	outw (wstr[i]);
    }
  return 0;
}

char *
utf8_to_ascii (char *dst, const wchar_t * src, int len)
{
  int i;
  char *mb;
  for (i = 0; i < len; i++)
    {
      if (src[i] == 0xFFFF)
	break;
      mb = (char *) &(src[i]);
      if (mb[1] == 0x00)
	dst[i] = mb[0];
      else
	dst[i] = '?';
    }
  dst[i] = '\0';
  return dst;
}
