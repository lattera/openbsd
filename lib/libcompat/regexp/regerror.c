/*	$OpenBSD: src/lib/libcompat/regexp/Attic/regerror.c,v 1.3.4.1 2000/10/06 20:40:26 jason Exp $	*/
#ifndef lint
static char *rcsid = "$OpenBSD: src/lib/libcompat/regexp/Attic/regerror.c,v 1.3.4.1 2000/10/06 20:40:26 jason Exp $";
#endif /* not lint */

#include <regexp.h>
#include <stdio.h>

static void (*_new_regerror)() = NULL;

void
v8_regerror(s)
	const char *s;
{
	if (_new_regerror != NULL)
		_new_regerror(s);
	else
		warnx("%s", s);
	return;
}

void
v8_setregerror(f)
	void (*f)();
{
	_new_regerror = f;
}
