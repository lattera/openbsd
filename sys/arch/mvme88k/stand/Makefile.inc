#	$OpenBSD: src/sys/arch/mvme88k/stand/Makefile.inc,v 1.4.6.1 2003/05/19 21:45:53 tedu Exp $

MDEC_DIR?=/usr/mdec
CFLAGS+=	-fno-stack-protector

# Load addresses for first and second stage bootstraps 
STAGE1_RELOC=0x009F0000
STAGE2_RELOC=0x00AF0000


