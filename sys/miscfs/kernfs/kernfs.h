/*	$OpenBSD: src/sys/miscfs/kernfs/Attic/kernfs.h,v 1.2.4.2 1997/05/29 00:52:38 mickey Exp $	*/

/*
 * Copyright (c) 1996, 1997 Michael Shalayeff
 * Copyright (c) 1992, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software donated to Berkeley by
 * Jan-Simon Pendry.
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
 *	@(#)kernfs.h	8.5 (Berkeley) 6/15/94
 */

#define	_PATH_KERNFS	"/kern"		/* Default mountpoint */
/* #define KERNFS_DIAGNOSTIC */

#ifdef _KERNEL
struct kernfs_mount {
	struct vnode	*kf_root;	/* Root node */
};

typedef
enum {
	Kroot,		/* root dir */
	Kvar,		/* kernel variable */
	Ksym,		/* symbol table's dir */
	Ksymtab,	/* symbol table */

}	kernfs_type;

typedef
struct kern_target {
	u_char kt_type;
	u_char kt_namlen;
	char *kt_name;
	void *kt_data;
#define KTT_NULL         1
#define KTT_TIME         5
#define KTT_INT         17
#define KTT_STRING      31
#define KTT_HOSTNAME    47
#define KTT_AVENRUN     53
#define KTT_DEVICE      71
#define KTT_MSGBUF      89
#define KTT_USERMEM     91
#define KTT_DOMAIN      97
#define KTT_SYMTAB      101
	u_char kt_tag;
	kernfs_type kt_ktype;
	u_char kt_vtype;
	mode_t kt_mode;
} kern_target_t;

struct kernfs_node {
	TAILQ_ENTRY(kernfs_node) list;
	kernfs_type	kf_type;
	u_long		kf_mode;
	u_long		kf_flags;
	struct vnode	*kf_vnode;
	kern_target_t	*kf_kt;
	/* this is for Ksymtab only */
	struct db_symtab*kf_st;
	struct exec	kf_xh;
};

#define VFSTOKERNFS(mp)	((struct kernfs_mount *)((mp)->mnt_data))
#define	VTOKERN(vp)	((struct kernfs_node *)(vp)->v_data)
#define KFSTOV(kfs)	((kfs)->kf_vnode)

extern int (**kernfs_vnodeop_p) __P((void *));
extern struct vfsops kernfs_vfsops;
extern dev_t rrootdev;

int kernfs_freevp __P((struct vnode *));
int kernfs_allocvp
	__P((struct mount *, struct vnode **, kern_target_t *, kernfs_type));

void	kernfs_init __P((void));
int	kernfs_root __P((struct mount *, struct vnode **));

#endif /* _KERNEL */
