/*	$OpenBSD: src/sys/arch/mvmeppc/stand/libbug/Attic/rtc_rd.c,v 1.1.2.2 2004/02/19 10:49:10 niklas Exp $	*/

/*
 * bug routines -- assumes that the necessary sections of memory
 * are preserved.
 */
#include <sys/types.h>

#include "libbug.h"

void
mvmeprom_rtc_rd(ptime)
	struct mvmeprom_time *ptime;
{
	asm volatile ("mr 3, %0": : "r" (ptime));
	MVMEPROM_CALL(MVMEPROM_RTC_RD);
}
