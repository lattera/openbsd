/*	$OpenBSD: src/usr.bin/lex/libyywrap.c,v 1.7 2012/12/05 23:20:25 deraadt Exp $	*/

/* libyywrap - flex run-time support library "yywrap" function */

/* $Header: /cvs/src/usr.bin/lex/libyywrap.c,v 1.6 2003/07/28 20:38:31 deraadt Exp $ */

int yywrap(void);

int
yywrap(void)
{
	return 1;
}
