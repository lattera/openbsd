/*
 * Author: Tatu Ylonen <ylo@cs.hut.fi>
 * Copyright (c) 1995 Tatu Ylonen <ylo@cs.hut.fi>, Espoo, Finland
 *                    All rights reserved
 * Created: Mon Mar 20 22:09:17 1995 ylo
 *
 * Versions of malloc and friends that check their results, and never return
 * failure (they call fatal if they encounter an error).
 *
 * As far as I am concerned, the code I have written for this software
 * can be used freely for any purpose.  Any derived versions of this
 * software must be clearly marked as such, and if the derived work is
 * incompatible with the protocol description in the RFC file, it must be
 * called by a name other than "ssh" or "Secure Shell".
 */

/* RCSID("$OpenBSD: src/usr.bin/ssh/xmalloc.h,v 1.5.2.1 2001/09/27 00:15:43 miod Exp $"); */

#ifndef XMALLOC_H
#define XMALLOC_H

void	*xmalloc(size_t);
void	*xrealloc(void *, size_t);
void     xfree(void *);
char 	*xstrdup(const char *);

#endif				/* XMALLOC_H */
