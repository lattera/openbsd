#	$OpenBSD: src/sys/arch/mvmeppc/stand/Attic/Makefile.inc,v 1.1.6.1 2003/05/19 21:49:43 tedu Exp $

MDEC_DIR?=/usr/mdec
CFLAGS+=	-fno-stack-protector

# Load addresses for first and second stage bootstraps 
STAGE1_RELOC=4000
STAGE2_RELOC=6000
STAGE3_RELOC=6000


