#ifndef TYPE_H_
#define TYPE_H_
#include <stdint.h>
typedef uint8_t __attribute__ ((aligned (1))) byte_t;
typedef uint16_t __attribute__ ((aligned (1))) word_t;
typedef uint32_t __attribute__ ((aligned (1))) dword_t;
typedef uint64_t __attribute__ ((aligned (1))) qword_t;
typedef word_t wchar_t;
#endif /*TYPE_H_ */
