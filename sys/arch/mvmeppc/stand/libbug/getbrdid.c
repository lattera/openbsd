/*	$OpenBSD: src/sys/arch/mvmeppc/stand/libbug/Attic/getbrdid.c,v 1.1.2.1 2001/10/31 03:01:21 nate Exp $	*/

/*
 * bug routines -- assumes that the necessary sections of memory
 * are preserved.
 */
#include <sys/types.h>
#include <machine/prom.h>

/* BUG - query board routines */
struct mvmeprom_brdid *
mvmeprom_brdid()
{
	struct mvmeprom_brdid *id;

	MVMEPROM_CALL(MVMEPROM_BRD_ID);
	asm volatile ("mr %0, 3": "=r" (id):);
	return (id);
}
