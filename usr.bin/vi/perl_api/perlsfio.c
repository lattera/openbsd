/*	$OpenBSD: src/usr.bin/vi/perl_api/perlsfio.c,v 1.4 2009/10/27 23:59:47 deraadt Exp $	*/

/*-
 * Copyright (c) 1996
 *	Keith Bostic.  All rights reserved.
 * Copyright (c) 1996
 *	Sven Verdoolaege. All rights reserved.
 *
 * See the LICENSE file for redistribution information.
 */

#include "config.h"

#include <sys/param.h>
#include <sys/queue.h>
#include <sys/time.h>

#include <bitstring.h>
#include <ctype.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "../common/common.h"

#include <EXTERN.h>
#include <perl.h>
#include <XSUB.h>

#include "perl_extern.h"

/*
 * PUBLIC: #ifdef USE_SFIO
 */
#ifdef USE_SFIO

#define NIL(type)       ((type)0)

static int
sfnviwrite(f, buf, n, disc)
Sfio_t* f;      /* stream involved */
char*           buf;    /* buffer to read into */
int             n;      /* number of bytes to read */
Sfdisc_t*       disc;   /* discipline */        
{
	SCR *scrp;

	scrp = (SCR *)SvIV((SV*)SvRV(perl_get_sv("curscr", FALSE)));
	msgq(scrp, M_INFO, "%.*s", n, buf);
	return n;
}

/*
 * sfdcnewnvi --
 *	Create nvi discipline
 *
 * PUBLIC: Sfdisc_t* sfdcnewnvi(SCR*);
 */

Sfdisc_t *
sfdcnewnvi(scrp)
	SCR *scrp;
{
	Sfdisc_t*   disc;

	MALLOC(scrp, disc, Sfdisc_t*, sizeof(Sfdisc_t));
	if (!disc) return disc;

	disc->readf = (Sfread_f)NULL;
	disc->writef = sfnviwrite;
	disc->seekf = (Sfseek_f)NULL;
	disc->exceptf = (Sfexcept_f)NULL;
	return disc;
}

/*
 * PUBLIC: #endif
 */
#endif /* USE_SFIO */
