/*	$OpenBSD: src/sys/lib/libz/inffast.h,v 1.4.8.2 2004/02/19 10:57:19 niklas Exp $	*/
/* inffast.h -- header to use inffast.c
 * Copyright (C) 1995-2003 Mark Adler
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

/* WARNING: this file should *not* be used by applications. It is
   part of the implementation of the compression library and is
   subject to change. Applications should only use zlib.h.
 */

void inflate_fast OF((z_streamp strm, unsigned start));
