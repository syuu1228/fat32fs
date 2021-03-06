#ifndef TYPE_H_
#define TYPE_H_
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>

typedef uint8_t __attribute__ ((aligned (1))) byte_t;
typedef uint16_t __attribute__ ((aligned (1))) word_t;
typedef uint32_t __attribute__ ((aligned (1))) dword_t;
typedef uint64_t __attribute__ ((aligned (1))) qword_t;
#endif /*TYPE_H_ */
