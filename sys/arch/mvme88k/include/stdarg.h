/*	$OpenBSD: src/sys/arch/mvme88k/include/stdarg.h,v 1.5.4.1 2002/06/11 03:37:10 art Exp $	*/

#ifndef _M88K_STDARGS_H_
#define _M88K_STDARGS_H_

#include <machine/ansi.h>

#ifndef	_STDARG_H
#define	_STDARG_H
#endif

#include <machine/va-m88k.h>

typedef	_BSD_VA_LIST_	va_list;

#endif	/* _M88K_STDARGS_H_ */
