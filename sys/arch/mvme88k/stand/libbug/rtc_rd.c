/*	$OpenBSD: src/sys/arch/mvme88k/stand/libbug/rtc_rd.c,v 1.1.8.1 2001/10/31 03:01:20 nate Exp $	*/

/*
 * bug routines -- assumes that the necessary sections of memory
 * are preserved.
 */
#include <sys/types.h>
#include <machine/prom.h>

void
mvmeprom_rtc_rd(ptime)
	struct mvmeprom_time *ptime;
{
	asm volatile ("or r2,r0,%0": : "r" (ptime));
	MVMEPROM_CALL(MVMEPROM_RTC_RD);
}
