/*	$OpenBSD: src/usr.bin/patch/mkpath.c,v 1.1 2005/05/16 15:22:46 espie Exp $ */
/*
 * Copyright (c) 1983, 1992, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <err.h>
#include <errno.h>
#include <string.h>

/* Code taken directly from mkdir(1).

 * mkpath -- create directories.
 *	path     - path
 *	mode     - file mode of terminal directory
 *	dir_mode - file mode of intermediate directories
 */
int
mkpath(char *path, mode_t mode, mode_t dir_mode)
{
	struct stat sb;
	char *slash;
	int done = 0;

	slash = path;

	while (!done) {
		slash += strspn(slash, "/");
		slash += strcspn(slash, "/");

		done = (*slash == '\0');
		*slash = '\0';

		if (stat(path, &sb)) {
			if (errno != ENOENT ||
			    (mkdir(path, done ? mode : dir_mode) &&
			    errno != EEXIST)) {
				warn("%s", path);
				return (-1);
			}
		} else if (!S_ISDIR(sb.st_mode)) {
			warnx("%s: %s", path, strerror(ENOTDIR));
			return (-1);
		}

		*slash = '/';
	}

	return (0);
}

