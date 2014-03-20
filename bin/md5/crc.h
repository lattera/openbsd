/*	$OpenBSD: src/bin/md5/crc.h,v 1.2 2014/03/20 22:03:56 tedu Exp $	*/

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

#define	CKSUM_DIGEST_LENGTH		4
#define	CKSUM_DIGEST_STRING_LENGTH	(10 + 1 + 19)

typedef struct CKSUMContext {
	u_int32_t crc;
	off_t len;
} CKSUM_CTX;

void	 CKSUM_Init(CKSUM_CTX *);
void	 CKSUM_Update(CKSUM_CTX *, const u_int8_t *, size_t);
void	 CKSUM_Final(CKSUM_CTX *);
char    *CKSUM_End(CKSUM_CTX *, char *);
char    *CKSUM_Data(const u_int8_t *, size_t, char *);
