/*	$OpenBSD: src/sys/arch/mvme88k/stand/libbug/rtc_rd.c,v 1.1.8.2 2004/02/19 10:49:09 niklas Exp $	*/

/*
 * bug routines -- assumes that the necessary sections of memory
 * are preserved.
 */
#include <sys/types.h>
#include <machine/prom.h>
#include "prom.h"

void
mvmeprom_rtc_rd(ptime)
	struct mvmeprom_time *ptime;
{
	asm volatile ("or r2,r0,%0": : "r" (ptime));
	MVMEPROM_CALL(MVMEPROM_RTC_RD);
}
