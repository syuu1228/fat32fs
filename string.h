#ifndef STRING_H_
#define STRING_H_
#include "./type.h"

#define outc putchar
void outns(const char *str, int n);
int outw (wchar_t wc);
int outws (wchar_t * wstr);
int outnws (wchar_t * wstr, int len);
char *utf8_to_ascii (char *dst, const wchar_t * src, int len);
#endif /*STRING_H_ */
