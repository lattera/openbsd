/*	$OpenBSD: src/sys/arch/mvme88k/include/varargs.h,v 1.3.14.2 2001/12/05 00:39:12 niklas Exp $	*/

#ifndef _M88K_VARARGS_H_
#define _M88K_VARARGS_H_

#include <machine/va-m88k.h>

/* Define va_list from __gnuc_va_list.  */
typedef	__gnuc_va_list	va_list;
typedef	_BSD_VA_LIST_	__gnuc_va_list

#endif	/* _M88K_VARARGS_H_ */
