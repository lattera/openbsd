/*	$OpenBSD: src/sys/arch/amd64/include/exec.h,v 1.1.2.1 2004/06/05 23:09:25 niklas Exp $	*/
/*
 * Written by Artur Grabowski <art@openbsd.org> Public Domain
 */

#ifndef _AMD64_EXEC_H_
#define _AMD64_EXEC_H_

#define __LDPGSZ 4096

#define NATIVE_EXEC_ELF

#define ARCH_ELFSIZE 64

#define ELF_TARG_CLASS		ELFCLASS64
#define ELF_TARG_DATA		ELFDATA2LSB
#define ELF_TARG_MACH		EM_AMD64

#define _NLIST_DO_ELF
#define _KERN_DO_ELF64

#endif
