
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "swo.h"
#include "trace_printf.h"

#define TRACE_PRINTF_BUFSIZE 200


void trace_init(void){
  swo_init();
  trace_puts("Trace started.");
}

size_t trace_write(const char *buf, size_t nbyte){
  size_t cnt;
  for(cnt =0;cnt < nbyte;cnt++){   
      itm_xmit_char(buf[cnt]);
  }

  return cnt;
}

int trace_printf(const char* format, ...){
  int ret;
  va_list ap;

  va_start(ap, format);

  static char buffer[TRACE_PRINTF_BUFSIZE];

  ret = vsnprintf(buffer, sizeof(buffer), format, ap);

  if(ret > 0 )
    ret  = trace_write(buffer, (size_t)ret);

  va_end(ap);

  return ret;
}

int trace_puts(const char *strz){
  trace_write("\n\r", 2);
  return trace_write(strz, strlen(strz));
}

