#	$OpenBSD: src/sys/arch/mvme88k/stand/Makefile.inc,v 1.2.8.3 2004/02/19 10:49:08 niklas Exp $

MDEC_DIR?=/usr/mdec
CFLAGS+=	-fno-stack-protector -mmemcpy

# Load addresses for first and second stage bootstraps
STAGE1_RELOC=0x009F0000
STAGE2_RELOC=0x00AF0000


