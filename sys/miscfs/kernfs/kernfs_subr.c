/*	$OpenBSD: src/sys/miscfs/kernfs/Attic/kernfs_subr.c,v 1.1.2.4 1997/05/29 00:52:38 mickey Exp $ */

/*
 * Copyright (c) 1996, 1997 Michael Shalayeff
 * Copyright (c) 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
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
 *	@(#)procfs_subr.c	8.5 (Berkeley) 6/15/94
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/time.h>
#include <sys/kernel.h>
#include <sys/proc.h>
#include <sys/vnode.h>
#include <sys/malloc.h>
#include <sys/exec.h>
#include <miscfs/kernfs/kernfs.h>

static TAILQ_HEAD(, kernfs_node)	kfshead;
static int kfsvplock = 0;
#define KERNFS_LOCKED	0x01
#define KERNFS_WANT	0x02
extern struct exec kernel_xh;


void
kernfs_init(void)
{
	TAILQ_INIT(&kfshead);
}

/*
 * allocate a kernfs_node/vnode pair.  the vnode is
 * referenced, but not locked.
 *
 * type, and mount point uniquely
 * identify a kfsnode.  the mount point is needed
 * because someone might mount this filesystem
 * twice.
 *
 * all kfsnodes are maintained on a singly-linked
 * list.  new nodes are only allocated when they cannot
 * be found on this list.  entries on the list are
 * removed when the vfs reclaim entry is called.
 *
 * a single lock is kept for the entire list.  this is
 * needed because the getnewvnode() function can block
 * waiting for a vnode to become free, in which case there
 * may be more than one process trying to get the same
 * vnode.  this lock is only taken if we are going to
 * call getnewvnode, since the kernel itself is single-threaded.
 *
 * if an entry is found on the list, then call vget() to
 * take a reference.  this is done because there may be
 * zero references to it and so it needs to removed from
 * the vnode free list.
 */
int
kernfs_allocvp(mp, vpp, kt, type)
	struct mount *mp;
	struct vnode **vpp;
	kern_target_t *kt;
	kernfs_type type;
{
	struct kernfs_node *kfs;
	struct vnode *vp;
	int error;

loop:
	for (kfs = kfshead.tqh_first; kfs != NULL; kfs = kfs->list.tqe_next) {
		vp = KFSTOV(kfs);
		if (kfs->kf_type == type && kfs->kf_kt == kt
		    && vp->v_mount == mp) {
			if (vget(vp, 0))
				goto loop;
			*vpp = vp;
			return (0);
		}
	}

	/*
	 * otherwise lock the vp list while we call getnewvnode
	 * since that can block.
	 */ 
	if (kfsvplock & KERNFS_LOCKED) {
		kfsvplock |= KERNFS_WANT;
		sleep((caddr_t) &kfsvplock, PINOD);
		goto loop;
	}
	kfsvplock |= KERNFS_LOCKED;

	if ((error = getnewvnode(VT_KERNFS, mp, kernfs_vnodeop_p, vpp)) != 0)
		goto out;
	vp = *vpp;

	MALLOC(kfs, void *, sizeof(struct kernfs_node), M_TEMP, M_WAITOK);
	vp->v_data = kfs;

	kfs->kf_type = type;
	kfs->kf_vnode = vp;
	kfs->kf_kt = kt;

	switch (type) {
	case Kroot:	/* /kern = dr-xr-xr-x */
		kfs->kf_mode = (VREAD|VEXEC) |
				(VREAD|VEXEC) >> 3 |
				(VREAD|VEXEC) >> 6;
		vp->v_type = VDIR;
		vp->v_flag = VROOT;
		break;

	case Kvar:	/* kern/<files> = -r--r--r-- */
		kfs->kf_mode = ((kern_target_t *)kt)->kt_mode;
		vp->v_type = VREG;
		break;
	case Ksym:	/* /kern/sym = dr-xr-xr-x */
		kfs->kf_mode = ((kern_target_t *)kt)->kt_mode;
		vp->v_type = VDIR;
		break;

	case Ksymtab:	/* /kern/sym/ ... = -rw-r--r-- */
		kfs->kf_mode = (VREAD|VWRITE) |
				(VREAD) >> 3 |
				(VREAD) >> 6;
		vp->v_type = VREG;
		break;
	default:
		panic("kernfs_allocvp: unknown type");
	}

	/* add to procfs vnode list */
	TAILQ_INSERT_TAIL(&kfshead, kfs, list);

out:
	kfsvplock &= ~KERNFS_LOCKED;

	if (kfsvplock & KERNFS_WANT) {
		kfsvplock &= ~KERNFS_WANT;
		wakeup((caddr_t) &kfsvplock);
	}

	return (error);
}

int
kernfs_freevp(vp)
	struct vnode *vp;
{
	struct kernfs_node *kfs = VTOKERN(vp);

	if (kfs != NULL) {
		TAILQ_REMOVE(&kfshead, kfs, list);
		FREE(vp->v_data, M_TEMP);
		vp->v_data = 0;
	}
	return (0);
}

