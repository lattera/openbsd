/*	$OpenBSD: src/sys/arch/m88k/include/stdarg.h,v 1.1.2.1 2004/06/05 23:09:48 niklas Exp $	*/

#ifndef _M88K_STDARGS_H_
#define _M88K_STDARGS_H_

#include <machine/ansi.h>

#ifndef	_STDARG_H
#define	_STDARG_H
#endif

#include <machine/va-m88k.h>

typedef	_BSD_VA_LIST_	va_list;

#endif	/* _M88K_STDARGS_H_ */
