/*	$OpenBSD: src/sys/arch/mvmeppc/stand/libbug/Attic/diskrd.c,v 1.1.2.1 2001/10/31 03:01:21 nate Exp $	*/

/*
 * bug routines -- assumes that the necessary sections of memory
 * are preserved.
 */
#include <sys/types.h>
#include <machine/prom.h>

/* returns 0: success, nonzero: error */
int
mvmeprom_diskrd(arg)
	struct mvmeprom_dskio *arg;
{
	int ret;

	asm volatile ("mr 3, %0" :: "r"(arg));
	MVMEPROM_CALL(MVMEPROM_NETRD);
	asm volatile ("mr %0, 3" :  "=r" (ret));
	return ((ret & 0x8));
}
