/*	$OpenBSD: src/sys/arch/sun3/stand/libsa/Attic/promcons.c,v 1.1.1.1.16.1 2001/10/31 03:08:00 nate Exp $	*/


#include <stdarg.h>
#include <sys/types.h>
#include <machine/mon.h>

int
getchar()
{
	return ( (*romp->getChar)() );
}

int
peekchar()
{
	return ( (*romp->mayGet)() );
}

void
putchar(c)
	int c;
{
	if (c == '\n')
		(*romp->putChar)('\r');
	(*romp->putChar)(c);
}

