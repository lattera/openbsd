#objdump: -dr --prefix-addresses -mmips:3000
#name: MIPS la-xgot
#as: -mips1 -KPIC -xgot
#source: la.s

# Test the la macro with -KPIC -xgot.

.*: +file format .*mips.*

Disassembly of section .text:
[0-9a-f]+ <[^>]*> li	\$a0,0
[0-9a-f]+ <[^>]*> li	\$a0,1
[0-9a-f]+ <[^>]*> li	\$a0,0x8000
[0-9a-f]+ <[^>]*> li	\$a0,-32768
[0-9a-f]+ <[^>]*> lui	\$a0,0x1
[0-9a-f]+ <[^>]*> lui	\$a0,0x1
[0-9a-f]+ <[^>]*> ori	\$a0,\$a0,0xa5a5
[0-9a-f]+ <[^>]*> addiu	\$a0,\$a1,0
[0-9a-f]+ <[^>]*> addiu	\$a0,\$a1,1
[0-9a-f]+ <[^>]*> li	\$a0,0x8000
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$a1
[0-9a-f]+ <[^>]*> addiu	\$a0,\$a1,-32768
[0-9a-f]+ <[^>]*> lui	\$a0,0x1
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$a1
[0-9a-f]+ <[^>]*> lui	\$a0,0x1
[0-9a-f]+ <[^>]*> ori	\$a0,\$a0,0xa5a5
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$a1
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$gp\)
[ 	]*[0-9a-f]+: R_MIPS_GOT16	.data
[0-9a-f]+ <[^>]*> nop
[0-9a-f]+ <[^>]*> addiu	\$a0,\$a0,0
[ 	]*[0-9a-f]+: R_MIPS_LO16	.data
[0-9a-f]+ <[^>]*> lui	\$a0,0x0
[ 	]*[0-9a-f]+: R_MIPS_GOT_HI16	big_external_data_label
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$gp
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$a0\)
[ 	]*[0-9a-f]+: R_MIPS_GOT_LO16	big_external_data_label
[0-9a-f]+ <[^>]*> lui	\$a0,0x0
[ 	]*[0-9a-f]+: R_MIPS_GOT_HI16	small_external_data_label
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$gp
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$a0\)
[ 	]*[0-9a-f]+: R_MIPS_GOT_LO16	small_external_data_label
[0-9a-f]+ <[^>]*> lui	\$a0,0x0
[ 	]*[0-9a-f]+: R_MIPS_GOT_HI16	big_external_common
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$gp
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$a0\)
[ 	]*[0-9a-f]+: R_MIPS_GOT_LO16	big_external_common
[0-9a-f]+ <[^>]*> lui	\$a0,0x0
[ 	]*[0-9a-f]+: R_MIPS_GOT_HI16	small_external_common
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$gp
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$a0\)
[ 	]*[0-9a-f]+: R_MIPS_GOT_LO16	small_external_common
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$gp\)
[ 	]*[0-9a-f]+: R_MIPS_GOT16	.bss
[0-9a-f]+ <[^>]*> nop
[0-9a-f]+ <[^>]*> addiu	\$a0,\$a0,0
[ 	]*[0-9a-f]+: R_MIPS_LO16	.bss
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$gp\)
[ 	]*[0-9a-f]+: R_MIPS_GOT16	.bss
[0-9a-f]+ <[^>]*> nop
[0-9a-f]+ <[^>]*> addiu	\$a0,\$a0,1000
[ 	]*[0-9a-f]+: R_MIPS_LO16	.bss
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$gp\)
[ 	]*[0-9a-f]+: R_MIPS_GOT16	.data
[0-9a-f]+ <[^>]*> nop
[0-9a-f]+ <[^>]*> addiu	\$a0,\$a0,1
[ 	]*[0-9a-f]+: R_MIPS_LO16	.data
[0-9a-f]+ <[^>]*> lui	\$a0,0x0
[ 	]*[0-9a-f]+: R_MIPS_GOT_HI16	big_external_data_label
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$gp
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$a0\)
[ 	]*[0-9a-f]+: R_MIPS_GOT_LO16	big_external_data_label
[0-9a-f]+ <[^>]*> nop
[0-9a-f]+ <[^>]*> addiu	\$a0,\$a0,1
[0-9a-f]+ <[^>]*> lui	\$a0,0x0
[ 	]*[0-9a-f]+: R_MIPS_GOT_HI16	small_external_data_label
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$gp
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$a0\)
[ 	]*[0-9a-f]+: R_MIPS_GOT_LO16	small_external_data_label
[0-9a-f]+ <[^>]*> nop
[0-9a-f]+ <[^>]*> addiu	\$a0,\$a0,1
[0-9a-f]+ <[^>]*> lui	\$a0,0x0
[ 	]*[0-9a-f]+: R_MIPS_GOT_HI16	big_external_common
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$gp
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$a0\)
[ 	]*[0-9a-f]+: R_MIPS_GOT_LO16	big_external_common
[0-9a-f]+ <[^>]*> nop
[0-9a-f]+ <[^>]*> addiu	\$a0,\$a0,1
[0-9a-f]+ <[^>]*> lui	\$a0,0x0
[ 	]*[0-9a-f]+: R_MIPS_GOT_HI16	small_external_common
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$gp
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$a0\)
[ 	]*[0-9a-f]+: R_MIPS_GOT_LO16	small_external_common
[0-9a-f]+ <[^>]*> nop
[0-9a-f]+ <[^>]*> addiu	\$a0,\$a0,1
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$gp\)
[ 	]*[0-9a-f]+: R_MIPS_GOT16	.bss
[0-9a-f]+ <[^>]*> nop
[0-9a-f]+ <[^>]*> addiu	\$a0,\$a0,1
[ 	]*[0-9a-f]+: R_MIPS_LO16	.bss
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$gp\)
[ 	]*[0-9a-f]+: R_MIPS_GOT16	.bss
[0-9a-f]+ <[^>]*> nop
[0-9a-f]+ <[^>]*> addiu	\$a0,\$a0,1001
[ 	]*[0-9a-f]+: R_MIPS_LO16	.bss
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$gp\)
[ 	]*[0-9a-f]+: R_MIPS_GOT16	.data
[0-9a-f]+ <[^>]*> lui	\$at,0x1
[0-9a-f]+ <[^>]*> addiu	\$at,\$at,-32768
[ 	]*[0-9a-f]+: R_MIPS_LO16	.data
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$at
[0-9a-f]+ <[^>]*> lui	\$a0,0x0
[ 	]*[0-9a-f]+: R_MIPS_GOT_HI16	big_external_data_label
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$gp
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$a0\)
[ 	]*[0-9a-f]+: R_MIPS_GOT_LO16	big_external_data_label
[0-9a-f]+ <[^>]*> lui	\$at,0x1
[0-9a-f]+ <[^>]*> addiu	\$at,\$at,-32768
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$at
[0-9a-f]+ <[^>]*> lui	\$a0,0x0
[ 	]*[0-9a-f]+: R_MIPS_GOT_HI16	small_external_data_label
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$gp
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$a0\)
[ 	]*[0-9a-f]+: R_MIPS_GOT_LO16	small_external_data_label
[0-9a-f]+ <[^>]*> lui	\$at,0x1
[0-9a-f]+ <[^>]*> addiu	\$at,\$at,-32768
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$at
[0-9a-f]+ <[^>]*> lui	\$a0,0x0
[ 	]*[0-9a-f]+: R_MIPS_GOT_HI16	big_external_common
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$gp
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$a0\)
[ 	]*[0-9a-f]+: R_MIPS_GOT_LO16	big_external_common
[0-9a-f]+ <[^>]*> lui	\$at,0x1
[0-9a-f]+ <[^>]*> addiu	\$at,\$at,-32768
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$at
[0-9a-f]+ <[^>]*> lui	\$a0,0x0
[ 	]*[0-9a-f]+: R_MIPS_GOT_HI16	small_external_common
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$gp
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$a0\)
[ 	]*[0-9a-f]+: R_MIPS_GOT_LO16	small_external_common
[0-9a-f]+ <[^>]*> lui	\$at,0x1
[0-9a-f]+ <[^>]*> addiu	\$at,\$at,-32768
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$at
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$gp\)
[ 	]*[0-9a-f]+: R_MIPS_GOT16	.bss
[0-9a-f]+ <[^>]*> lui	\$at,0x1
[0-9a-f]+ <[^>]*> addiu	\$at,\$at,-32768
[ 	]*[0-9a-f]+: R_MIPS_LO16	.bss
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$at
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$gp\)
[ 	]*[0-9a-f]+: R_MIPS_GOT16	.bss
[0-9a-f]+ <[^>]*> lui	\$at,0x1
[0-9a-f]+ <[^>]*> addiu	\$at,\$at,-31768
[ 	]*[0-9a-f]+: R_MIPS_LO16	.bss
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$at
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$gp\)
[ 	]*[0-9a-f]+: R_MIPS_GOT16	.data
[0-9a-f]+ <[^>]*> nop
[0-9a-f]+ <[^>]*> addiu	\$a0,\$a0,-32768
[ 	]*[0-9a-f]+: R_MIPS_LO16	.data
[0-9a-f]+ <[^>]*> lui	\$a0,0x0
[ 	]*[0-9a-f]+: R_MIPS_GOT_HI16	big_external_data_label
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$gp
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$a0\)
[ 	]*[0-9a-f]+: R_MIPS_GOT_LO16	big_external_data_label
[0-9a-f]+ <[^>]*> nop
[0-9a-f]+ <[^>]*> addiu	\$a0,\$a0,-32768
[0-9a-f]+ <[^>]*> lui	\$a0,0x0
[ 	]*[0-9a-f]+: R_MIPS_GOT_HI16	small_external_data_label
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$gp
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$a0\)
[ 	]*[0-9a-f]+: R_MIPS_GOT_LO16	small_external_data_label
[0-9a-f]+ <[^>]*> nop
[0-9a-f]+ <[^>]*> addiu	\$a0,\$a0,-32768
[0-9a-f]+ <[^>]*> lui	\$a0,0x0
[ 	]*[0-9a-f]+: R_MIPS_GOT_HI16	big_external_common
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$gp
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$a0\)
[ 	]*[0-9a-f]+: R_MIPS_GOT_LO16	big_external_common
[0-9a-f]+ <[^>]*> nop
[0-9a-f]+ <[^>]*> addiu	\$a0,\$a0,-32768
[0-9a-f]+ <[^>]*> lui	\$a0,0x0
[ 	]*[0-9a-f]+: R_MIPS_GOT_HI16	small_external_common
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$gp
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$a0\)
[ 	]*[0-9a-f]+: R_MIPS_GOT_LO16	small_external_common
[0-9a-f]+ <[^>]*> nop
[0-9a-f]+ <[^>]*> addiu	\$a0,\$a0,-32768
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$gp\)
[ 	]*[0-9a-f]+: R_MIPS_GOT16	.bss
[0-9a-f]+ <[^>]*> nop
[0-9a-f]+ <[^>]*> addiu	\$a0,\$a0,-32768
[ 	]*[0-9a-f]+: R_MIPS_LO16	.bss
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$gp\)
[ 	]*[0-9a-f]+: R_MIPS_GOT16	.bss
[0-9a-f]+ <[^>]*> nop
[0-9a-f]+ <[^>]*> addiu	\$a0,\$a0,-31768
[ 	]*[0-9a-f]+: R_MIPS_LO16	.bss
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$gp\)
[ 	]*[0-9a-f]+: R_MIPS_GOT16	.data
[0-9a-f]+ <[^>]*> lui	\$at,0x1
[0-9a-f]+ <[^>]*> addiu	\$at,\$at,0
[ 	]*[0-9a-f]+: R_MIPS_LO16	.data
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$at
[0-9a-f]+ <[^>]*> lui	\$a0,0x0
[ 	]*[0-9a-f]+: R_MIPS_GOT_HI16	big_external_data_label
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$gp
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$a0\)
[ 	]*[0-9a-f]+: R_MIPS_GOT_LO16	big_external_data_label
[0-9a-f]+ <[^>]*> lui	\$at,0x1
[0-9a-f]+ <[^>]*> addiu	\$at,\$at,0
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$at
[0-9a-f]+ <[^>]*> lui	\$a0,0x0
[ 	]*[0-9a-f]+: R_MIPS_GOT_HI16	small_external_data_label
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$gp
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$a0\)
[ 	]*[0-9a-f]+: R_MIPS_GOT_LO16	small_external_data_label
[0-9a-f]+ <[^>]*> lui	\$at,0x1
[0-9a-f]+ <[^>]*> addiu	\$at,\$at,0
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$at
[0-9a-f]+ <[^>]*> lui	\$a0,0x0
[ 	]*[0-9a-f]+: R_MIPS_GOT_HI16	big_external_common
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$gp
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$a0\)
[ 	]*[0-9a-f]+: R_MIPS_GOT_LO16	big_external_common
[0-9a-f]+ <[^>]*> lui	\$at,0x1
[0-9a-f]+ <[^>]*> addiu	\$at,\$at,0
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$at
[0-9a-f]+ <[^>]*> lui	\$a0,0x0
[ 	]*[0-9a-f]+: R_MIPS_GOT_HI16	small_external_common
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$gp
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$a0\)
[ 	]*[0-9a-f]+: R_MIPS_GOT_LO16	small_external_common
[0-9a-f]+ <[^>]*> lui	\$at,0x1
[0-9a-f]+ <[^>]*> addiu	\$at,\$at,0
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$at
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$gp\)
[ 	]*[0-9a-f]+: R_MIPS_GOT16	.bss
[0-9a-f]+ <[^>]*> lui	\$at,0x1
[0-9a-f]+ <[^>]*> addiu	\$at,\$at,0
[ 	]*[0-9a-f]+: R_MIPS_LO16	.bss
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$at
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$gp\)
[ 	]*[0-9a-f]+: R_MIPS_GOT16	.bss
[0-9a-f]+ <[^>]*> lui	\$at,0x1
[0-9a-f]+ <[^>]*> addiu	\$at,\$at,1000
[ 	]*[0-9a-f]+: R_MIPS_LO16	.bss
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$at
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$gp\)
[ 	]*[0-9a-f]+: R_MIPS_GOT16	.data
[0-9a-f]+ <[^>]*> lui	\$at,0x2
[0-9a-f]+ <[^>]*> addiu	\$at,\$at,-23131
[ 	]*[0-9a-f]+: R_MIPS_LO16	.data
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$at
[0-9a-f]+ <[^>]*> lui	\$a0,0x0
[ 	]*[0-9a-f]+: R_MIPS_GOT_HI16	big_external_data_label
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$gp
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$a0\)
[ 	]*[0-9a-f]+: R_MIPS_GOT_LO16	big_external_data_label
[0-9a-f]+ <[^>]*> lui	\$at,0x2
[0-9a-f]+ <[^>]*> addiu	\$at,\$at,-23131
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$at
[0-9a-f]+ <[^>]*> lui	\$a0,0x0
[ 	]*[0-9a-f]+: R_MIPS_GOT_HI16	small_external_data_label
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$gp
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$a0\)
[ 	]*[0-9a-f]+: R_MIPS_GOT_LO16	small_external_data_label
[0-9a-f]+ <[^>]*> lui	\$at,0x2
[0-9a-f]+ <[^>]*> addiu	\$at,\$at,-23131
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$at
[0-9a-f]+ <[^>]*> lui	\$a0,0x0
[ 	]*[0-9a-f]+: R_MIPS_GOT_HI16	big_external_common
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$gp
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$a0\)
[ 	]*[0-9a-f]+: R_MIPS_GOT_LO16	big_external_common
[0-9a-f]+ <[^>]*> lui	\$at,0x2
[0-9a-f]+ <[^>]*> addiu	\$at,\$at,-23131
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$at
[0-9a-f]+ <[^>]*> lui	\$a0,0x0
[ 	]*[0-9a-f]+: R_MIPS_GOT_HI16	small_external_common
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$gp
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$a0\)
[ 	]*[0-9a-f]+: R_MIPS_GOT_LO16	small_external_common
[0-9a-f]+ <[^>]*> lui	\$at,0x2
[0-9a-f]+ <[^>]*> addiu	\$at,\$at,-23131
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$at
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$gp\)
[ 	]*[0-9a-f]+: R_MIPS_GOT16	.bss
[0-9a-f]+ <[^>]*> lui	\$at,0x2
[0-9a-f]+ <[^>]*> addiu	\$at,\$at,-23131
[ 	]*[0-9a-f]+: R_MIPS_LO16	.bss
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$at
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$gp\)
[ 	]*[0-9a-f]+: R_MIPS_GOT16	.bss
[0-9a-f]+ <[^>]*> lui	\$at,0x2
[0-9a-f]+ <[^>]*> addiu	\$at,\$at,-22131
[ 	]*[0-9a-f]+: R_MIPS_LO16	.bss
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$at
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$gp\)
[ 	]*[0-9a-f]+: R_MIPS_GOT16	.data
[0-9a-f]+ <[^>]*> nop
[0-9a-f]+ <[^>]*> addiu	\$a0,\$a0,0
[ 	]*[0-9a-f]+: R_MIPS_LO16	.data
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$a1
[0-9a-f]+ <[^>]*> lui	\$a0,0x0
[ 	]*[0-9a-f]+: R_MIPS_GOT_HI16	big_external_data_label
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$gp
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$a0\)
[ 	]*[0-9a-f]+: R_MIPS_GOT_LO16	big_external_data_label
[0-9a-f]+ <[^>]*> nop
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$a1
[0-9a-f]+ <[^>]*> lui	\$a0,0x0
[ 	]*[0-9a-f]+: R_MIPS_GOT_HI16	small_external_data_label
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$gp
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$a0\)
[ 	]*[0-9a-f]+: R_MIPS_GOT_LO16	small_external_data_label
[0-9a-f]+ <[^>]*> nop
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$a1
[0-9a-f]+ <[^>]*> lui	\$a0,0x0
[ 	]*[0-9a-f]+: R_MIPS_GOT_HI16	big_external_common
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$gp
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$a0\)
[ 	]*[0-9a-f]+: R_MIPS_GOT_LO16	big_external_common
[0-9a-f]+ <[^>]*> nop
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$a1
[0-9a-f]+ <[^>]*> lui	\$a0,0x0
[ 	]*[0-9a-f]+: R_MIPS_GOT_HI16	small_external_common
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$gp
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$a0\)
[ 	]*[0-9a-f]+: R_MIPS_GOT_LO16	small_external_common
[0-9a-f]+ <[^>]*> nop
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$a1
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$gp\)
[ 	]*[0-9a-f]+: R_MIPS_GOT16	.bss
[0-9a-f]+ <[^>]*> nop
[0-9a-f]+ <[^>]*> addiu	\$a0,\$a0,0
[ 	]*[0-9a-f]+: R_MIPS_LO16	.bss
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$a1
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$gp\)
[ 	]*[0-9a-f]+: R_MIPS_GOT16	.bss
[0-9a-f]+ <[^>]*> nop
[0-9a-f]+ <[^>]*> addiu	\$a0,\$a0,1000
[ 	]*[0-9a-f]+: R_MIPS_LO16	.bss
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$a1
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$gp\)
[ 	]*[0-9a-f]+: R_MIPS_GOT16	.data
[0-9a-f]+ <[^>]*> nop
[0-9a-f]+ <[^>]*> addiu	\$a0,\$a0,1
[ 	]*[0-9a-f]+: R_MIPS_LO16	.data
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$a1
[0-9a-f]+ <[^>]*> lui	\$a0,0x0
[ 	]*[0-9a-f]+: R_MIPS_GOT_HI16	big_external_data_label
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$gp
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$a0\)
[ 	]*[0-9a-f]+: R_MIPS_GOT_LO16	big_external_data_label
[0-9a-f]+ <[^>]*> nop
[0-9a-f]+ <[^>]*> addiu	\$a0,\$a0,1
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$a1
[0-9a-f]+ <[^>]*> lui	\$a0,0x0
[ 	]*[0-9a-f]+: R_MIPS_GOT_HI16	small_external_data_label
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$gp
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$a0\)
[ 	]*[0-9a-f]+: R_MIPS_GOT_LO16	small_external_data_label
[0-9a-f]+ <[^>]*> nop
[0-9a-f]+ <[^>]*> addiu	\$a0,\$a0,1
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$a1
[0-9a-f]+ <[^>]*> lui	\$a0,0x0
[ 	]*[0-9a-f]+: R_MIPS_GOT_HI16	big_external_common
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$gp
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$a0\)
[ 	]*[0-9a-f]+: R_MIPS_GOT_LO16	big_external_common
[0-9a-f]+ <[^>]*> nop
[0-9a-f]+ <[^>]*> addiu	\$a0,\$a0,1
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$a1
[0-9a-f]+ <[^>]*> lui	\$a0,0x0
[ 	]*[0-9a-f]+: R_MIPS_GOT_HI16	small_external_common
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$gp
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$a0\)
[ 	]*[0-9a-f]+: R_MIPS_GOT_LO16	small_external_common
[0-9a-f]+ <[^>]*> nop
[0-9a-f]+ <[^>]*> addiu	\$a0,\$a0,1
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$a1
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$gp\)
[ 	]*[0-9a-f]+: R_MIPS_GOT16	.bss
[0-9a-f]+ <[^>]*> nop
[0-9a-f]+ <[^>]*> addiu	\$a0,\$a0,1
[ 	]*[0-9a-f]+: R_MIPS_LO16	.bss
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$a1
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$gp\)
[ 	]*[0-9a-f]+: R_MIPS_GOT16	.bss
[0-9a-f]+ <[^>]*> nop
[0-9a-f]+ <[^>]*> addiu	\$a0,\$a0,1001
[ 	]*[0-9a-f]+: R_MIPS_LO16	.bss
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$a1
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$gp\)
[ 	]*[0-9a-f]+: R_MIPS_GOT16	.data
[0-9a-f]+ <[^>]*> lui	\$at,0x1
[0-9a-f]+ <[^>]*> addiu	\$at,\$at,-32768
[ 	]*[0-9a-f]+: R_MIPS_LO16	.data
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$at
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$a1
[0-9a-f]+ <[^>]*> lui	\$a0,0x0
[ 	]*[0-9a-f]+: R_MIPS_GOT_HI16	big_external_data_label
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$gp
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$a0\)
[ 	]*[0-9a-f]+: R_MIPS_GOT_LO16	big_external_data_label
[0-9a-f]+ <[^>]*> lui	\$at,0x1
[0-9a-f]+ <[^>]*> addiu	\$at,\$at,-32768
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$at
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$a1
[0-9a-f]+ <[^>]*> lui	\$a0,0x0
[ 	]*[0-9a-f]+: R_MIPS_GOT_HI16	small_external_data_label
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$gp
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$a0\)
[ 	]*[0-9a-f]+: R_MIPS_GOT_LO16	small_external_data_label
[0-9a-f]+ <[^>]*> lui	\$at,0x1
[0-9a-f]+ <[^>]*> addiu	\$at,\$at,-32768
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$at
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$a1
[0-9a-f]+ <[^>]*> lui	\$a0,0x0
[ 	]*[0-9a-f]+: R_MIPS_GOT_HI16	big_external_common
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$gp
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$a0\)
[ 	]*[0-9a-f]+: R_MIPS_GOT_LO16	big_external_common
[0-9a-f]+ <[^>]*> lui	\$at,0x1
[0-9a-f]+ <[^>]*> addiu	\$at,\$at,-32768
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$at
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$a1
[0-9a-f]+ <[^>]*> lui	\$a0,0x0
[ 	]*[0-9a-f]+: R_MIPS_GOT_HI16	small_external_common
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$gp
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$a0\)
[ 	]*[0-9a-f]+: R_MIPS_GOT_LO16	small_external_common
[0-9a-f]+ <[^>]*> lui	\$at,0x1
[0-9a-f]+ <[^>]*> addiu	\$at,\$at,-32768
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$at
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$a1
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$gp\)
[ 	]*[0-9a-f]+: R_MIPS_GOT16	.bss
[0-9a-f]+ <[^>]*> lui	\$at,0x1
[0-9a-f]+ <[^>]*> addiu	\$at,\$at,-32768
[ 	]*[0-9a-f]+: R_MIPS_LO16	.bss
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$at
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$a1
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$gp\)
[ 	]*[0-9a-f]+: R_MIPS_GOT16	.bss
[0-9a-f]+ <[^>]*> lui	\$at,0x1
[0-9a-f]+ <[^>]*> addiu	\$at,\$at,-31768
[ 	]*[0-9a-f]+: R_MIPS_LO16	.bss
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$at
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$a1
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$gp\)
[ 	]*[0-9a-f]+: R_MIPS_GOT16	.data
[0-9a-f]+ <[^>]*> nop
[0-9a-f]+ <[^>]*> addiu	\$a0,\$a0,-32768
[ 	]*[0-9a-f]+: R_MIPS_LO16	.data
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$a1
[0-9a-f]+ <[^>]*> lui	\$a0,0x0
[ 	]*[0-9a-f]+: R_MIPS_GOT_HI16	big_external_data_label
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$gp
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$a0\)
[ 	]*[0-9a-f]+: R_MIPS_GOT_LO16	big_external_data_label
[0-9a-f]+ <[^>]*> nop
[0-9a-f]+ <[^>]*> addiu	\$a0,\$a0,-32768
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$a1
[0-9a-f]+ <[^>]*> lui	\$a0,0x0
[ 	]*[0-9a-f]+: R_MIPS_GOT_HI16	small_external_data_label
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$gp
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$a0\)
[ 	]*[0-9a-f]+: R_MIPS_GOT_LO16	small_external_data_label
[0-9a-f]+ <[^>]*> nop
[0-9a-f]+ <[^>]*> addiu	\$a0,\$a0,-32768
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$a1
[0-9a-f]+ <[^>]*> lui	\$a0,0x0
[ 	]*[0-9a-f]+: R_MIPS_GOT_HI16	big_external_common
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$gp
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$a0\)
[ 	]*[0-9a-f]+: R_MIPS_GOT_LO16	big_external_common
[0-9a-f]+ <[^>]*> nop
[0-9a-f]+ <[^>]*> addiu	\$a0,\$a0,-32768
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$a1
[0-9a-f]+ <[^>]*> lui	\$a0,0x0
[ 	]*[0-9a-f]+: R_MIPS_GOT_HI16	small_external_common
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$gp
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$a0\)
[ 	]*[0-9a-f]+: R_MIPS_GOT_LO16	small_external_common
[0-9a-f]+ <[^>]*> nop
[0-9a-f]+ <[^>]*> addiu	\$a0,\$a0,-32768
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$a1
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$gp\)
[ 	]*[0-9a-f]+: R_MIPS_GOT16	.bss
[0-9a-f]+ <[^>]*> nop
[0-9a-f]+ <[^>]*> addiu	\$a0,\$a0,-32768
[ 	]*[0-9a-f]+: R_MIPS_LO16	.bss
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$a1
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$gp\)
[ 	]*[0-9a-f]+: R_MIPS_GOT16	.bss
[0-9a-f]+ <[^>]*> nop
[0-9a-f]+ <[^>]*> addiu	\$a0,\$a0,-31768
[ 	]*[0-9a-f]+: R_MIPS_LO16	.bss
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$a1
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$gp\)
[ 	]*[0-9a-f]+: R_MIPS_GOT16	.data
[0-9a-f]+ <[^>]*> lui	\$at,0x1
[0-9a-f]+ <[^>]*> addiu	\$at,\$at,0
[ 	]*[0-9a-f]+: R_MIPS_LO16	.data
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$at
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$a1
[0-9a-f]+ <[^>]*> lui	\$a0,0x0
[ 	]*[0-9a-f]+: R_MIPS_GOT_HI16	big_external_data_label
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$gp
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$a0\)
[ 	]*[0-9a-f]+: R_MIPS_GOT_LO16	big_external_data_label
[0-9a-f]+ <[^>]*> lui	\$at,0x1
[0-9a-f]+ <[^>]*> addiu	\$at,\$at,0
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$at
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$a1
[0-9a-f]+ <[^>]*> lui	\$a0,0x0
[ 	]*[0-9a-f]+: R_MIPS_GOT_HI16	small_external_data_label
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$gp
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$a0\)
[ 	]*[0-9a-f]+: R_MIPS_GOT_LO16	small_external_data_label
[0-9a-f]+ <[^>]*> lui	\$at,0x1
[0-9a-f]+ <[^>]*> addiu	\$at,\$at,0
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$at
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$a1
[0-9a-f]+ <[^>]*> lui	\$a0,0x0
[ 	]*[0-9a-f]+: R_MIPS_GOT_HI16	big_external_common
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$gp
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$a0\)
[ 	]*[0-9a-f]+: R_MIPS_GOT_LO16	big_external_common
[0-9a-f]+ <[^>]*> lui	\$at,0x1
[0-9a-f]+ <[^>]*> addiu	\$at,\$at,0
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$at
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$a1
[0-9a-f]+ <[^>]*> lui	\$a0,0x0
[ 	]*[0-9a-f]+: R_MIPS_GOT_HI16	small_external_common
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$gp
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$a0\)
[ 	]*[0-9a-f]+: R_MIPS_GOT_LO16	small_external_common
[0-9a-f]+ <[^>]*> lui	\$at,0x1
[0-9a-f]+ <[^>]*> addiu	\$at,\$at,0
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$at
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$a1
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$gp\)
[ 	]*[0-9a-f]+: R_MIPS_GOT16	.bss
[0-9a-f]+ <[^>]*> lui	\$at,0x1
[0-9a-f]+ <[^>]*> addiu	\$at,\$at,0
[ 	]*[0-9a-f]+: R_MIPS_LO16	.bss
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$at
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$a1
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$gp\)
[ 	]*[0-9a-f]+: R_MIPS_GOT16	.bss
[0-9a-f]+ <[^>]*> lui	\$at,0x1
[0-9a-f]+ <[^>]*> addiu	\$at,\$at,1000
[ 	]*[0-9a-f]+: R_MIPS_LO16	.bss
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$at
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$a1
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$gp\)
[ 	]*[0-9a-f]+: R_MIPS_GOT16	.data
[0-9a-f]+ <[^>]*> lui	\$at,0x2
[0-9a-f]+ <[^>]*> addiu	\$at,\$at,-23131
[ 	]*[0-9a-f]+: R_MIPS_LO16	.data
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$at
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$a1
[0-9a-f]+ <[^>]*> lui	\$a0,0x0
[ 	]*[0-9a-f]+: R_MIPS_GOT_HI16	big_external_data_label
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$gp
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$a0\)
[ 	]*[0-9a-f]+: R_MIPS_GOT_LO16	big_external_data_label
[0-9a-f]+ <[^>]*> lui	\$at,0x2
[0-9a-f]+ <[^>]*> addiu	\$at,\$at,-23131
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$at
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$a1
[0-9a-f]+ <[^>]*> lui	\$a0,0x0
[ 	]*[0-9a-f]+: R_MIPS_GOT_HI16	small_external_data_label
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$gp
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$a0\)
[ 	]*[0-9a-f]+: R_MIPS_GOT_LO16	small_external_data_label
[0-9a-f]+ <[^>]*> lui	\$at,0x2
[0-9a-f]+ <[^>]*> addiu	\$at,\$at,-23131
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$at
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$a1
[0-9a-f]+ <[^>]*> lui	\$a0,0x0
[ 	]*[0-9a-f]+: R_MIPS_GOT_HI16	big_external_common
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$gp
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$a0\)
[ 	]*[0-9a-f]+: R_MIPS_GOT_LO16	big_external_common
[0-9a-f]+ <[^>]*> lui	\$at,0x2
[0-9a-f]+ <[^>]*> addiu	\$at,\$at,-23131
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$at
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$a1
[0-9a-f]+ <[^>]*> lui	\$a0,0x0
[ 	]*[0-9a-f]+: R_MIPS_GOT_HI16	small_external_common
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$gp
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$a0\)
[ 	]*[0-9a-f]+: R_MIPS_GOT_LO16	small_external_common
[0-9a-f]+ <[^>]*> lui	\$at,0x2
[0-9a-f]+ <[^>]*> addiu	\$at,\$at,-23131
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$at
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$a1
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$gp\)
[ 	]*[0-9a-f]+: R_MIPS_GOT16	.bss
[0-9a-f]+ <[^>]*> lui	\$at,0x2
[0-9a-f]+ <[^>]*> addiu	\$at,\$at,-23131
[ 	]*[0-9a-f]+: R_MIPS_LO16	.bss
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$at
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$a1
[0-9a-f]+ <[^>]*> lw	\$a0,0\(\$gp\)
[ 	]*[0-9a-f]+: R_MIPS_GOT16	.bss
[0-9a-f]+ <[^>]*> lui	\$at,0x2
[0-9a-f]+ <[^>]*> addiu	\$at,\$at,-22131
[ 	]*[0-9a-f]+: R_MIPS_LO16	.bss
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$at
[0-9a-f]+ <[^>]*> addu	\$a0,\$a0,\$a1
