/*	$OpenBSD: src/sys/miscfs/fifofs/fifo.h,v 1.9.2.2 2002/11/04 18:02:31 art Exp $	*/
/*	$NetBSD: fifo.h,v 1.10 1996/02/09 22:40:15 christos Exp $	*/

/*
 * Copyright (c) 1991, 1993
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
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
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
 *
 *	@(#)fifo.h	8.3 (Berkeley) 8/10/94
 */
#ifdef FIFO

#include <miscfs/genfs/genfs.h>

/*
 * Prototypes for fifo operations on vnodes.
 */
int	fifo_badop(void *);
int	fifo_ebadf(void *);

int	fifo_lookup(void *);
#define fifo_create	fifo_badop
#define fifo_mknod	fifo_badop
int	fifo_open(void *);
int	fifo_close(void *);
#define fifo_access	fifo_ebadf
#define fifo_getattr	fifo_ebadf
#define fifo_setattr	fifo_ebadf
int	fifo_read(void *);
int	fifo_write(void *);
#define fifo_lease_check nullop
int	fifo_ioctl(void *);
int	fifo_select(void *);
int	fifo_kqfilter(void *);
#define fifo_fsync	nullop
#define fifo_remove	fifo_badop
#define fifo_revoke     vop_generic_revoke
#define fifo_link	fifo_badop
#define fifo_rename	fifo_badop
#define fifo_mkdir	fifo_badop
#define fifo_rmdir	fifo_badop
#define fifo_symlink	fifo_badop
#define fifo_readdir	fifo_badop
#define fifo_readlink	fifo_badop
#define fifo_abortop	fifo_badop
int fifo_inactive(void *);
#define fifo_reclaim	nullop
#define fifo_lock       vop_generic_lock
#define fifo_unlock     vop_generic_unlock
int	fifo_bmap(void *);
#define fifo_strategy	fifo_badop
int	fifo_print(void *);
#define fifo_islocked	vop_generic_islocked
int	fifo_pathconf(void *);
int	fifo_advlock(void *);
#define fifo_reallocblks fifo_badop
#define fifo_bwrite	nullop
#define fifo_mmap	fifo_badop
#define fifo_putpages	genfs_null_putpages

void 	fifo_printinfo(struct vnode *);

extern int (**fifo_vnodeop_p)(void *);

/*
 * Since most of the vnode op vectors for spec files share a bunch of
 * operations, we maintain them here instead of duplicating them everywhere.
 *
 * XXX - vnodeop inheritance would be nice.
 */
#define FIFO_VNODEOP_DESCS \
	{ &vop_open_desc, fifo_open },			\
	{ &vop_lookup_desc, fifo_lookup },		\
	{ &vop_create_desc, fifo_create },		\
	{ &vop_mknod_desc, fifo_mknod },		\
	{ &vop_remove_desc, fifo_remove },		\
	{ &vop_link_desc, fifo_link },			\
	{ &vop_rename_desc, fifo_rename },		\
	{ &vop_mkdir_desc, fifo_mkdir },		\
	{ &vop_rmdir_desc, fifo_rmdir },		\
	{ &vop_symlink_desc, fifo_symlink },		\
	{ &vop_readdir_desc, fifo_readdir },		\
	{ &vop_readlink_desc, fifo_readlink },		\
	{ &vop_abortop_desc, fifo_abortop },		\
	{ &vop_select_desc, fifo_select },		\
	{ &vop_lease_desc, fifo_lease_check },		\
	{ &vop_ioctl_desc, fifo_ioctl },		\
	{ &vop_select_desc, fifo_select },		\
	{ &vop_revoke_desc, fifo_revoke },		\
	{ &vop_pathconf_desc, fifo_pathconf },		\
	{ &vop_advlock_desc, fifo_advlock },		\
	{ &vop_strategy_desc, fifo_strategy },  	\
	{ &vop_reallocblks_desc, fifo_reallocblks },	\
	{ &vop_mmap_desc, fifo_mmap },			\
	{ &vop_putpages_desc, fifo_putpages }
	

#endif /* FIFO */
