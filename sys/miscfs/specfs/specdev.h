/*	$OpenBSD: src/sys/miscfs/specfs/Attic/specdev.h,v 1.11.2.2 2002/11/04 18:02:31 art Exp $	*/
/*	$NetBSD: specdev.h,v 1.12 1996/02/13 13:13:01 mycroft Exp $	*/

/*
 * Copyright (c) 1990, 1993
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
 *	@(#)specdev.h	8.3 (Berkeley) 8/10/94
 */

/*
 * This structure defines the information maintained about
 * special devices. It is allocated in checkalias and freed
 * in vgone.
 */
struct specinfo {
	struct	vnode **si_hashchain;
	struct	vnode *si_specnext;
	struct  mount *si_mountpoint;
	dev_t	si_rdev;
	struct	lockf *si_lockf;
	daddr_t si_lastr;
};
/*
 * Exported shorthand
 */
#define v_rdev v_specinfo->si_rdev
#define v_hashchain v_specinfo->si_hashchain
#define v_specnext v_specinfo->si_specnext
#define v_specmountpoint v_specinfo->si_mountpoint
#define v_speclockf v_specinfo->si_lockf

/*
 * Special device management
 */
#define	SPECHSZ	64
#if	((SPECHSZ&(SPECHSZ-1)) == 0)
#define	SPECHASH(rdev)	(((rdev>>5)+(rdev))&(SPECHSZ-1))
#else
#define	SPECHASH(rdev)	(((unsigned)((rdev>>5)+(rdev)))%SPECHSZ)
#endif

struct vnode *speclisth[SPECHSZ];

/*
 * Prototypes for special file operations on vnodes.
 */
extern	int (**spec_vnodeop_p)(void *);
struct	nameidata;
struct	componentname;
struct	ucred;
struct	flock;
struct	buf;
struct	uio;

int	spec_badop(void *);
int	spec_ebadf(void *);

int	spec_lookup(void *);
#define	spec_create	spec_badop
#define	spec_mknod	spec_badop
int	spec_open(void *);
int	spec_close(void *);
#define	spec_access	spec_ebadf
#define	spec_getattr	spec_ebadf
#define	spec_setattr	spec_ebadf
int	spec_read(void *);
int	spec_write(void *);
#define	spec_lease_check nullop
int	spec_ioctl(void *);
int	spec_select(void *);
int	spec_kqfilter(void *);
int	spec_fsync(void *);
#define	spec_remove	spec_badop
#define	spec_link	spec_badop
#define	spec_rename	spec_badop
#define	spec_mkdir	spec_badop
#define	spec_rmdir	spec_badop
#define	spec_symlink	spec_badop
#define	spec_readdir	spec_badop
#define	spec_readlink	spec_badop
#define	spec_abortop	spec_badop
int spec_inactive(void *);
#define	spec_reclaim	nullop
#define spec_lock       vop_generic_lock
#define spec_unlock     vop_generic_unlock
#define spec_islocked   vop_generic_islocked
int	spec_bmap(void *);
int	spec_strategy(void *);
int	spec_print(void *);
int	spec_pathconf(void *);
int	spec_advlock(void *);
#define	spec_reallocblks spec_badop
#define	spec_bwrite	vop_generic_bwrite
#define spec_revoke     vop_generic_revoke
#define	spec_mmap	vop_generic_mmap
#define spec_getpages	genfs_getpages
#define spec_putpages	genfs_putpages

/*
 * Since most of the vnode op vectors for spec files share a bunch of
 * operations, we maintain them here instead of duplicating them everywhere.
 *
 * XXX - vnodeop inheritance would be nice.
 */
#define SPEC_VNODEOP_DESCS \
	{ &vop_open_desc, spec_open },			\
	{ &vop_lookup_desc, spec_lookup },		\
	{ &vop_create_desc, spec_create },		\
	{ &vop_mknod_desc, spec_mknod },		\
	{ &vop_select_desc, spec_select },		\
	{ &vop_kqfilter_desc, spec_kqfilter },		\
	{ &vop_ioctl_desc, spec_ioctl },		\
	{ &vop_revoke_desc, spec_revoke },		\
	{ &vop_remove_desc, spec_remove },		\
	{ &vop_link_desc, spec_link },			\
	{ &vop_rename_desc, spec_rename },		\
	{ &vop_mkdir_desc, spec_mkdir },		\
	{ &vop_rmdir_desc, spec_rmdir },		\
	{ &vop_symlink_desc, spec_symlink },		\
	{ &vop_readdir_desc, spec_readdir },		\
	{ &vop_readlink_desc, spec_readlink },		\
	{ &vop_abortop_desc, spec_abortop },		\
	{ &vop_bmap_desc, spec_bmap },			\
	{ &vop_strategy_desc, spec_strategy },		\
	{ &vop_lease_desc, spec_lease_check },		\
	{ &vop_bwrite_desc, spec_bwrite },		\
	{ &vop_pathconf_desc, spec_pathconf }, 		\
	{ &vop_advlock_desc, spec_advlock },		\
	{ &vop_reallocblks_desc, spec_reallocblks },	\
	{ &vop_mmap_desc, spec_mmap },			\
	{ &vop_getpages_desc, spec_getpages },		\
	{ &vop_putpages_desc, spec_putpages }
	
