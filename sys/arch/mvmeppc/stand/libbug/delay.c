/*	$OpenBSD: src/sys/arch/mvmeppc/stand/libbug/Attic/delay.c,v 1.1.2.2 2004/02/19 10:49:10 niklas Exp $	*/

/*
 * bug routines -- assumes that the necessary sections of memory
 * are preserved.
 */
#include <sys/types.h>

#include "libbug.h"

/* BUG - timing routine */
void
mvmeprom_delay(msec)
	int msec; /* This is r3 */
{
	asm volatile ("mr 3, %0" :: "r"(msec));
	MVMEPROM_CALL(MVMEPROM_DELAY);
}
