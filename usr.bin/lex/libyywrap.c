/*	$OpenBSD: src/usr.bin/lex/libyywrap.c,v 1.4 1996/12/10 22:22:03 millert Exp $	*/

/* libyywrap - flex run-time support library "yywrap" function */

/* $Header: /home/daffy/u0/vern/flex/RCS/libyywrap.c,v 1.1 93/10/02 15:23:09 vern Exp $ */

#include <sys/cdefs.h>

int yywrap __P((void));

int
yywrap()
	{
	return 1;
	}
