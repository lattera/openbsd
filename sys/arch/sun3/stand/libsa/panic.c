/*	$OpenBSD: src/sys/arch/sun3/stand/libsa/Attic/panic.c,v 1.3.14.1 2001/10/31 03:08:00 nate Exp $	*/


#include <stdarg.h>
#include "stand.h"

extern __dead void abort();

__dead void
panic(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vprintf(fmt, ap);
	printf("\n");
	va_end(ap);
	abort();
}
