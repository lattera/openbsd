/*
 * Copyright (c) 2004 Todd C. Miller <Todd.Miller@courtesan.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "includes.h"

#include <sys/types.h>
#include <sys/param.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <stddef.h>
#include <dirent.h>

RCSID("$Id: bsd-closefrom.c,v 1.1 2004/08/15 08:41:00 djm Exp $");

#ifndef lint
static const char sudorcsid[] = "$Sudo: closefrom.c,v 1.6 2004/06/01 20:51:56 millert Exp $";
#endif /* lint */

/*
 * Close all file descriptors greater than or equal to lowfd.
 */
void
closefrom(int lowfd)
{
    long fd, maxfd;
    {
	/*
	 * Fall back on sysconf().  We avoid checking resource limits since
	 * it is possible to open a file descriptor and then drop the rlimit
	 * such that it is below the open fd.
	 */
	maxfd = sysconf(_SC_OPEN_MAX);
	if (maxfd < 0)
	    maxfd = OPEN_MAX;

	for (fd = lowfd; fd < maxfd; fd++)
	    (void) close((int) fd);
    }
}
