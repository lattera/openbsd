/*	$OpenBSD: src/sys/arch/mvme88k/include/stdarg.h,v 1.3.14.2 2003/03/27 23:32:18 niklas Exp $	*/

#ifndef _M88K_STDARGS_H_
#define _M88K_STDARGS_H_

#include <machine/ansi.h>

#ifndef	_STDARG_H
#define	_STDARG_H
#endif

#include <machine/va-m88k.h>

typedef	_BSD_VA_LIST_	va_list;

#endif	/* _M88K_STDARGS_H_ */
