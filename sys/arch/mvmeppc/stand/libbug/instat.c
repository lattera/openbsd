/*	$OpenBSD: src/sys/arch/mvmeppc/stand/libbug/Attic/instat.c,v 1.1.2.2 2004/02/19 10:49:10 niklas Exp $	*/

/*
 * bug routines -- assumes that the necessary sections of memory
 * are preserved.
 */
#include <sys/types.h>

#include "libbug.h"

/* returns 0 if no characters ready to read */
int
mvmeprom_instat()
{
	int ret;

	MVMEPROM_CALL(MVMEPROM_INSTAT);
	asm volatile ("mr %0, 3" :  "=r" (ret));
	return (!(ret & 0x4));
}
