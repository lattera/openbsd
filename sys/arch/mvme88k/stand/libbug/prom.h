/*	$OpenBSD: src/sys/arch/mvme88k/stand/libbug/prom.h,v 1.1.8.1 2001/10/31 03:01:20 nate Exp $	*/

#define MVMEPROM_CALL(x)	\
	asm volatile ( __CONCAT("or r9,r0," __STRING(x)) ); \
	asm volatile ("tb0 0,r0,496");
