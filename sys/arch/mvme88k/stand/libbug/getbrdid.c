/*	$OpenBSD: src/sys/arch/mvme88k/stand/libbug/getbrdid.c,v 1.1.8.2 2004/02/19 10:49:09 niklas Exp $	*/

/*
 * bug routines -- assumes that the necessary sections of memory
 * are preserved.
 */
#include <sys/types.h>
#include <machine/prom.h>
#include "prom.h"

/* BUG - query board routines */
struct mvmeprom_brdid *
mvmeprom_brdid()
{
	struct mvmeprom_brdid *id;

	MVMEPROM_CALL(MVMEPROM_GETBRDID);
	asm volatile ("or %0,r0,r2": "=r" (id):);
	return (id);
}
