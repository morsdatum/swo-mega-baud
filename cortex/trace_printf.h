#ifndef _TRACE_PRINTF_H_
#define _TRACE_PRINTF_H_
#include <string.h>

typedef unsigned int size_t;

void trace_init(void);
size_t trace_write(const char *buf, size_t nbyte);
int trace_printf(const char *format, ...);
int trace_puts(const char *strz);

#define trace_putchar(c) trace_write((const char *)&c, 1)
#define trace_strz(str) trace_write((const char *)str,strlen((const char *)str))

#endif
