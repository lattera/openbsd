#	$OpenBSD: src/sys/arch/mvme88k/stand/Makefile.inc,v 1.2.8.2 2003/03/27 23:32:19 niklas Exp $

MDEC_DIR?=/usr/mdec
CFLAGS+=	-fno-stack-protector

# Load addresses for first and second stage bootstraps 
STAGE1_RELOC=0x009F0000
STAGE2_RELOC=0x00AF0000


