/*	$OpenBSD: src/sys/arch/mvmeppc/stand/libbug/Attic/netwr.c,v 1.1.2.1 2001/10/31 03:01:21 nate Exp $	*/

/*
 * bug routines -- assumes that the necessary sections of memory
 * are preserved.
 */
#include <sys/types.h>
#include <machine/prom.h>

/* returns 0: success, nonzero: error */
int
mvmeprom_netwr(arg)
	struct mvmeprom_netio *arg;
{
	asm volatile ("mr 3, %0": : "r" (arg));
	MVMEPROM_CALL(MVMEPROM_NETWR);
	return (arg->status);
}
