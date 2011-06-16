/*
 * THIS FILE IS AUTOMATICALLY GENERATED
 * DONT EDIT THIS FILE
 */

/*	$OpenBSD: src/sys/arch/octeon/dev/cn30xxnpireg.h,v 1.1 2011/06/16 11:22:30 syuu Exp $	*/

/*
 * Copyright (c) 2007 Internet Initiative Japan, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
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
 */

/*
 * Cavium Networks OCTEON CN30XX Hardware Reference Manual
 * CN30XX-HM-1.0
 * 9.15 NPI Registers
 */

#ifndef _CN30XXNPIREG_H_
#define _CN30XXNPIREG_H_

#define	MPI_CFG					0x0001070000001000ULL

#define	NPI_RSL_INT_BLOCKS			0x00011f0000000000ULL
#define	NPI_DBG_SELECT				0x00011f0000000008ULL
#define	NPI_CTL_STATUS				0x00011f0000000010ULL
#define	NPI_INT_SUM				0x00011f0000000018ULL
#define	NPI_INT_ENB				0x00011f0000000020ULL
#define	NPI_MEM_ACCESS_SUBID3			0x00011f0000000028ULL
#define	NPI_MEM_ACCESS_SUBID4			0x00011f0000000030ULL
#define	NPI_MEM_ACCESS_SUBID5			0x00011f0000000038ULL
#define	NPI_MEM_ACCESS_SUBID6			0x00011f0000000040ULL
#define	NPI_PCI_READ_CMD			0x00011f0000000048ULL
#define	NPI_NUM_DESC_OUTPUT0			0x00011f0000000050ULL
#define	NPI_BASE_ADDR_INPUT0			0x00011f0000000070ULL
#define	NPI_SIZE_INPUT0				0x00011f0000000078ULL
#define	PCI_READ_TIMEOUT			0x00011f00000000b0ULL
#define	NPI_BASE_ADDR_OUTPUT0			0x00011f00000000b8ULL
#define	NPI_PCI_BURST_SIZE			0x00011f00000000d8ULL
#define	NPI_BUFF_SIZE_OUTPUT0			0x00011f00000000e0ULL
#define	NPI_OUTPUT_CONTROL			0x00011f0000000100ULL
#define	NPI_LOWP_IBUFF_SADDR			0x00011f0000000108ULL
#define	NPI_HIGHP_IBUFF_SADDR			0x00011f0000000110ULL
#define	NPI_LOWP_DBELL				0x00011f0000000118ULL
#define	NPI_HIGHP_DBELL				0x00011f0000000120ULL
#define	NPI_DMA_CONTROL				0x00011f0000000128ULL
#define	NPI_PCI_INT_ARB_CFG			0x00011f0000000130ULL
#define	NPI_INPUT_CONTROL			0x00011f0000000138ULL
#define	NPI_DMA_LOWP_COUNTS			0x00011f0000000140ULL
#define	NPI_DMA_HIGHP_COUNTS			0x00011f0000000148ULL
#define	NPI_DMA_LOWP_NADDR			0x00011f0000000150ULL
#define	NPI_DMA_HIGHP_NADDR			0x00011f0000000158ULL
#define	NPI_P0_PAIR_CNTS			0x00011f0000000160ULL
#define	NPI_P0_DBPAIR_ADDR			0x00011f0000000180ULL
#define	NPI_P0_INSTR_CNTS			0x00011f00000001a0ULL
#define	NPI_P0_INSTR_ADDR			0x00011f00000001c0ULL
#define	NPI_WIN_READ_TO				0x00011f00000001e0ULL
#define	DBG_DATA				0x00011f00000001e8ULL
#define	NPI_PORT_BP_CONTROL			0x00011f00000001f0ULL
#define	NPI_PORT32_INSTR_HDR			0x00011f00000001f8ULL
#define	NPI_BIST_STATUS				0x00011f00000003f8ULL

#define	NPI_MSI_RCV				0x00011f0000001190ULL

#define	NPI_RSL_INT_BLOCKS_XXX_63_31		0xffffffff80000000ULL
#define	NPI_RSL_INT_BLOCKS_IOB			0x0000000040000000ULL
#define	NPI_RSL_INT_BLOCKS_XXX_29_23		0x000000003f800000ULL
#define	NPI_RSL_INT_BLOCKS_ASX0			0x0000000000400000ULL
#define	NPI_RSL_INT_BLOCKS_XXX_21		0x0000000000200000ULL
#define	NPI_RSL_INT_BLOCKS_PIP			0x0000000000100000ULL
#define	NPI_RSL_INT_BLOCKS_XXX_19_18		0x00000000000c0000ULL
#define	NPI_RSL_INT_BLOCKS_LMC			0x0000000000020000ULL
#define	NPI_RSL_INT_BLOCKS_L2C			0x0000000000010000ULL
#define	NPI_RSL_INT_BLOCKS_XXX_15_13		0x000000000000e000ULL
#define	NPI_RSL_INT_BLOCKS_POW			0x0000000000001000ULL
#define	NPI_RSL_INT_BLOCKS_TIM			0x0000000000000800ULL
#define	NPI_RSL_INT_BLOCKS_PKO			0x0000000000000400ULL
#define	NPI_RSL_INT_BLOCKS_IPD			0x0000000000000200ULL
#define	NPI_RSL_INT_BLOCKS_XXX_8_6		0x00000000000001c0ULL
#define	NPI_RSL_INT_BLOCKS_FPA			0x0000000000000020ULL
#define	NPI_RSL_INT_BLOCKS_XXX_4		0x0000000000000010ULL
#define	NPI_RSL_INT_BLOCKS_NPI			0x0000000000000008ULL
#define	NPI_RSL_INT_BLOCKS_GMX1			0x0000000000000004ULL
#define	NPI_RSL_INT_BLOCKS_GMX0			0x0000000000000002ULL
#define	NPI_RSL_INT_BLOCKS_MIO			0x0000000000000001ULL

#define	NPI_DBG_SELECT_XXX_63_16		0xffffffffffff0000ULL
#define	NPI_DBG_SELECT_DBG_SEL			0x000000000000ffffULL

#define	NPI_CTL_STATUS_XXX_63			0x8000000000000000ULL
#define	NPI_DBG_SELECT_DBG_SEL			0x000000000000ffffULL

#define	NPI_CTL_STATUS_XXX_63			0x8000000000000000ULL
#define	NPI_CTL_STATUS_CHIP_REV			0x7f80000000000000ULL
#define	NPI_CTL_STATUS_DIS_PNIW			0x0040000000000000ULL
#define	NPI_CTL_STATUS_SPR5			0x0020000000000000ULL
#define	NPI_CTL_STATUS_SPR4			0x0010000000000000ULL
#define	NPI_CTL_STATUS_SPR8			0x0008000000000000ULL
#define	NPI_CTL_STATUS_OUT0_ENB			0x0004000000000000ULL
#define	NPI_CTL_STATUS_SPR3			0x0002000000000000ULL
#define	NPI_CTL_STATUS_SPR2			0x0001000000000000ULL
#define	NPI_CTL_STATUS_SPR7			0x0000800000000000ULL
#define	NPI_CTL_STATUS_INS0_ENB			0x0000400000000000ULL
#define	NPI_CTL_STATUS_SPR1			0x0000200000000000ULL
#define	NPI_CTL_STATUS_SPR0			0x0000100000000000ULL
#define	NPI_CTL_STATUS_SPR6			0x0000080000000000ULL
#define	NPI_CTL_STATUS_INS0_64B			0x0000040000000000ULL
#define	NPI_CTL_STATUS_PCI_WDIS			0x0000020000000000ULL
#define	NPI_CTL_STATUS_WAIT_COM			0x0000010000000000ULL
#define	NPI_CTL_STATUS_SPARES1			0x000000e000000000ULL
#define	NPI_CTL_STATUS_MAX_WORD			0x0000001f00000000ULL
#define	NPI_CTL_STATUS_SPARES0			0x00000000fffffc00ULL
#define	NPI_CTL_STATUS_TIMER			0x00000000000003ffULL

#define	NPI_INT_SUM_XXX_63_62			0xc000000000000000ULL
#define	NPI_INT_SUM_Q1_A_F			0x2000000000000000ULL
#define	NPI_INT_SUM_Q1_S_E			0x1000000000000000ULL
#define	NPI_INT_SUM_PDF_P_F			0x0800000000000000ULL
#define	NPI_INT_SUM_PDF_P_E			0x0400000000000000ULL
#define	NPI_INT_SUM_PCF_P_F			0x0200000000000000ULL
#define	NPI_INT_SUM_PCF_P_E			0x0100000000000000ULL
#define	NPI_INT_SUM_RDX_S_E			0x0080000000000000ULL
#define	NPI_INT_SUM_RWX_S_E			0x0040000000000000ULL
#define	NPI_INT_SUM_PNC_A_F			0x0020000000000000ULL
#define	NPI_INT_SUM_PNC_S_F			0x0010000000000000ULL
#define	NPI_INT_SUM_COM_A_F			0x0008000000000000ULL
#define	NPI_INT_SUM_COM_S_E			0x0004000000000000ULL
#define	NPI_INT_SUM_Q3_A_F			0x0002000000000000ULL
#define	NPI_INT_SUM_Q3_S_E			0x0001000000000000ULL
#define	NPI_INT_SUM_Q2_A_F			0x0000800000000000ULL
#define	NPI_INT_SUM_Q2_S_E			0x0000400000000000ULL
#define	NPI_INT_SUM_PCR_A_F			0x0000200000000000ULL
#define	NPI_INT_SUM_PCR_S_E			0x0000100000000000ULL
#define	NPI_INT_SUM_FCR_A_F			0x0000080000000000ULL
#define	NPI_INT_SUM_FCR_S_E			0x0000040000000000ULL
#define	NPI_INT_SUM_IOBDMA			0x0000020000000000ULL
#define	NPI_INT_SUM_P_DPERR			0x0000010000000000ULL
#define	NPI_INT_SUM_WIN_RTO			0x0000008000000000ULL
#define	NPI_INT_SUM_SPR17			0x0000004000000000ULL
#define	NPI_INT_SUM_SPR16			0x0000002000000000ULL
#define	NPI_INT_SUM_SPR26			0x0000001000000000ULL
#define	NPI_INT_SUM_IO_PPERR			0x0000000800000000ULL
#define	NPI_INT_SUM_SPR15			0x0000000400000000ULL
#define	NPI_INT_SUM_SPR14			0x0000000200000000ULL
#define	NPI_INT_SUM_SPR25			0x0000000100000000ULL
#define	NPI_INT_SUM_P0_PTOUT			0x0000000080000000ULL
#define	NPI_INT_SUM_SPR13			0x0000000040000000ULL
#define	NPI_INT_SUM_SPR12			0x0000000020000000ULL
#define	NPI_INT_SUM_SPR24			0x0000000010000000ULL
#define	NPI_INT_SUM_P0_PPERR			0x0000000008000000ULL
#define	NPI_INT_SUM_SPR11			0x0000000004000000ULL
#define	NPI_INT_SUM_SPR10			0x0000000002000000ULL
#define	NPI_INT_SUM_SPR23			0x0000000001000000ULL
#define	NPI_INT_SUM_G0_RTOUT			0x0000000000800000ULL
#define	NPI_INT_SUM_SPR9			0x0000000000400000ULL
#define	NPI_INT_SUM_SPR8			0x0000000000200000ULL
#define	NPI_INT_SUM_SPR22			0x0000000000100000ULL
#define	NPI_INT_SUM_P0_PERR			0x0000000000080000ULL
#define	NPI_INT_SUM_SPR7			0x0000000000040000ULL
#define	NPI_INT_SUM_SPR6			0x0000000000020000ULL
#define	NPI_INT_SUM_SPR21			0x0000000000010000ULL
#define	NPI_INT_SUM_P0_RTOUT			0x0000000000008000ULL
#define	NPI_INT_SUM_SPR5			0x0000000000004000ULL
#define	NPI_INT_SUM_SPR4			0x0000000000002000ULL
#define	NPI_INT_SUM_SPR20			0x0000000000001000ULL
#define	NPI_INT_SUM_IO_OVERF			0x0000000000000800ULL
#define	NPI_INT_SUM_SPR3			0x0000000000000400ULL
#define	NPI_INT_SUM_SPR2			0x0000000000000200ULL
#define	NPI_INT_SUM_SPR19			0x0000000000000100ULL
#define	NPI_INT_SUM_IO_RTOUT			0x0000000000000080ULL
#define	NPI_INT_SUM_SPR1			0x0000000000000040ULL
#define	NPI_INT_SUM_SPR0			0x0000000000000020ULL
#define	NPI_INT_SUM_SPR18			0x0000000000000010ULL
#define	NPI_INT_SUM_PO0_2SML			0x0000000000000008ULL
#define	NPI_INT_SUM_PCI_RSL			0x0000000000000004ULL
#define	NPI_INT_SUM_RML_TWO			0x0000000000000002ULL
#define	NPI_INT_SUM_RML_RTO			0x0000000000000001ULL

#define	NPI_INT_ENB_XXX_63_62			0xc000000000000000ULL
#define	NPI_INT_ENB_Q1_A_F			0x2000000000000000ULL
#define	NPI_INT_ENB_Q1_S_E			0x1000000000000000ULL
#define	NPI_INT_ENB_PDF_P_F			0x0800000000000000ULL
#define	NPI_INT_ENB_PDF_P_E			0x0400000000000000ULL
#define	NPI_INT_ENB_PCF_P_F			0x0200000000000000ULL
#define	NPI_INT_ENB_PCF_P_E			0x0100000000000000ULL
#define	NPI_INT_ENB_RDX_S_E			0x0080000000000000ULL
#define	NPI_INT_ENB_RWX_S_E			0x0040000000000000ULL
#define	NPI_INT_ENB_PNC_A_F			0x0020000000000000ULL
#define	NPI_INT_ENB_PNC_S_F			0x0010000000000000ULL
#define	NPI_INT_ENB_COM_A_F			0x0008000000000000ULL
#define	NPI_INT_ENB_COM_S_E			0x0004000000000000ULL
#define	NPI_INT_ENB_Q3_A_F			0x0002000000000000ULL
#define	NPI_INT_ENB_Q3_S_E			0x0001000000000000ULL
#define	NPI_INT_ENB_Q2_A_F			0x0000800000000000ULL
#define	NPI_INT_ENB_Q2_S_E			0x0000400000000000ULL
#define	NPI_INT_ENB_PCR_A_F			0x0000200000000000ULL
#define	NPI_INT_ENB_PCR_S_E			0x0000100000000000ULL
#define	NPI_INT_ENB_FCR_A_F			0x0000080000000000ULL
#define	NPI_INT_ENB_FCR_S_E			0x0000040000000000ULL
#define	NPI_INT_ENB_IOBDMA			0x0000020000000000ULL
#define	NPI_INT_ENB_P_DPERR			0x0000010000000000ULL
#define	NPI_INT_ENB_WIN_RTO			0x0000008000000000ULL
#define	NPI_INT_ENB_SPR17			0x0000004000000000ULL
#define	NPI_INT_ENB_SPR16			0x0000002000000000ULL
#define	NPI_INT_ENB_SPR26			0x0000001000000000ULL
#define	NPI_INT_ENB_IO_PPERR			0x0000000800000000ULL
#define	NPI_INT_ENB_SPR15			0x0000000400000000ULL
#define	NPI_INT_ENB_SPR14			0x0000000200000000ULL
#define	NPI_INT_ENB_SPR25			0x0000000100000000ULL
#define	NPI_INT_ENB_P0_PTOUT			0x0000000080000000ULL
#define	NPI_INT_ENB_SPR13			0x0000000040000000ULL
#define	NPI_INT_ENB_SPR12			0x0000000020000000ULL
#define	NPI_INT_ENB_SPR24			0x0000000010000000ULL
#define	NPI_INT_ENB_P0_PPERR			0x0000000008000000ULL
#define	NPI_INT_ENB_SPR11			0x0000000004000000ULL
#define	NPI_INT_ENB_SPR10			0x0000000002000000ULL
#define	NPI_INT_ENB_SPR23			0x0000000001000000ULL
#define	NPI_INT_ENB_G0_RTOUT			0x0000000000800000ULL
#define	NPI_INT_ENB_SPR9			0x0000000000400000ULL
#define	NPI_INT_ENB_SPR8			0x0000000000200000ULL
#define	NPI_INT_ENB_SPR22			0x0000000000100000ULL
#define	NPI_INT_ENB_P0_PERR			0x0000000000080000ULL
#define	NPI_INT_ENB_SPR7			0x0000000000040000ULL
#define	NPI_INT_ENB_SPR6			0x0000000000020000ULL
#define	NPI_INT_ENB_SPR21			0x0000000000010000ULL
#define	NPI_INT_ENB_P0_RTOUT			0x0000000000008000ULL
#define	NPI_INT_ENB_SPR5			0x0000000000004000ULL
#define	NPI_INT_ENB_SPR4			0x0000000000002000ULL
#define	NPI_INT_ENB_SPR20			0x0000000000001000ULL
#define	NPI_INT_ENB_IO_OVERF			0x0000000000000800ULL
#define	NPI_INT_ENB_SPR3			0x0000000000000400ULL
#define	NPI_INT_ENB_SPR2			0x0000000000000200ULL
#define	NPI_INT_ENB_SPR19			0x0000000000000100ULL
#define	NPI_INT_ENB_IO_RTOUT			0x0000000000000080ULL
#define	NPI_INT_ENB_SPR1			0x0000000000000040ULL
#define	NPI_INT_ENB_SPR0			0x0000000000000020ULL
#define	NPI_INT_ENB_SPR18			0x0000000000000010ULL
#define	NPI_INT_ENB_PO0_2SML			0x0000000000000008ULL
#define	NPI_INT_ENB_PCI_RSL			0x0000000000000004ULL
#define	NPI_INT_ENB_RML_TWO			0x0000000000000002ULL
#define	NPI_INT_ENB_RML_RTO			0x0000000000000001ULL

#define	NPI_MEM_ACCESS_SUBIDX_XXX_63_38		0xffffffc000000000ULL
#define	NPI_MEM_ACCESS_SUBIDX_SHORT		0x0000002000000000ULL
#define	NPI_MEM_ACCESS_SUBIDX_NMERGE		0x0000001000000000ULL
#define	NPI_MEM_ACCESS_SUBIDX_ESR		0x0000000c00000000ULL
#define	NPI_MEM_ACCESS_SUBIDX_ESW		0x0000000300000000ULL
#define	NPI_MEM_ACCESS_SUBIDX_NSR		0x0000000080000000ULL
#define	NPI_MEM_ACCESS_SUBIDX_NSW		0x0000000040000000ULL
#define	NPI_MEM_ACCESS_SUBIDX_ROR		0x0000000020000000ULL
#define	NPI_MEM_ACCESS_SUBIDX_ROW		0x0000000010000000ULL
#define	NPI_MEM_ACCESS_SUBIDX_BA		0x000000000fffffffULL

#define	NPI_PCI_READ_CMD_XXX_63_11		0xfffffffffffff800ULL
#define	NPI_PCI_READ_CMD_CMD_SIZE		0x00000000000007ffULL

#define	NPI_NUM_DESC_OUTPUT0_XXX_63_32		0xffffffff00000000ULL
#define	NPI_NUM_DESC_OUTPUT0_SIZE		0x00000000ffffffffULL

#define	NPI_BASE_ADDR_INPUT0_BADDR		0xfffffffffffffff8ULL
#define	NPI_BASE_ADDR_INPUT0_XXX_2_0		0x0000000000000007ULL

#define	NPI_SIZE_INPUT0_XXX_63_32		0xffffffff00000000ULL
#define	NPI_SIZE_INPUT0_SIZE			0x00000000ffffffffULL

#define	PCI_READ_TIMEOUT_XXX_63_32		0xffffffff00000000ULL
#define	PCI_READ_TIMEOUT_ENB			0x0000000080000000ULL
#define	PCI_READ_TIMEOUT_CNT			0x000000007fffffffULL

#define	NPI_BASE_ADDR_OUTPUT0_BADDR		0xfffffffffffffff8ULL
#define	NPI_BASE_ADDR_OUTPUT0_XXX_2_0		0x0000000000000007ULL

#define	NPI_PCI_BURST_SIZE_XXX_63_14		0xffffffffffffc000ULL
#define	NPI_PCI_BURST_SIZE_WR_BRST		0x0000000000003f80ULL
#define	NPI_PCI_BURST_SIZE_RD_BRST		0x000000000000007fULL

#define	NPI_BUFF_SIZE_OUTPUT0_XXX_63_23		0xffffffffff800000ULL
#define	NPI_BUFF_SIZE_OUTPUT0_ISIZE		0x00000000007f0000ULL
#define	NPI_BUFF_SIZE_OUTPUT0_BSIZE		0x000000000000ffffULL

#define	NPI_OUTPUT_CONTROL_XXX_63_48		0xffff000000000000ULL
#define	NPI_OUTPUT_CONTROL_SPR5			0x0000e00000000000ULL
#define	NPI_OUTPUT_CONTROL_P0_BMODE		0x0000100000000000ULL
#define	NPI_OUTPUT_CONTROL_SPR4			0x00000fff00000000ULL
#define	NPI_OUTPUT_CONTROL_O0_ES		0x00000000c0000000ULL
#define	NPI_OUTPUT_CONTROL_O0_NS		0x0000000020000000ULL
#define	NPI_OUTPUT_CONTROL_O0_RO		0x0000000010000000ULL
#define	NPI_OUTPUT_CONTROL_SPR3			0x000000000e000000ULL
#define	NPI_OUTPUT_CONTROL_O0_CSRM		0x0000000001000000ULL
#define	NPI_OUTPUT_CONTROL_SPR2			0x0000000000f00000ULL
#define	NPI_OUTPUT_CONTROL_SPR1			0x00000000000e0000ULL
#define	NPI_OUTPUT_CONTROL_IPTR_O0		0x0000000000010000ULL
#define	NPI_OUTPUT_CONTROL_SPR0			0x000000000000fff0ULL
#define	NPI_OUTPUT_CONTROL_ESR_SL0		0x000000000000000cULL
#define	NPI_OUTPUT_CONTROL_NSR_SL0		0x0000000000000002ULL
#define	NPI_OUTPUT_CONTROL_ROR_SL0		0x0000000000000001ULL

#define	NPI_LOWP_IBUFF_SADDR_XXX_63_36		0xfffffff000000000ULL
#define	NPI_LOWP_IBUFF_SADDR_SADDR		0x0000000fffffffffULL

#define	NPI_HIGHP_IBUFF_SADDR_XXX_63_36		0xfffffff000000000ULL
#define	NPI_HIGHP_IBUFF_SADDR_SADDR		0x0000000fffffffffULL

#define	NPI_LOWP_DBELL_XXX_63_16		0xffffffffffff0000ULL
#define	NPI_LOWP_DBELL_DBELL			0x000000000000ffffULL

#define	NPI_HIGHP_DBELL_XXX_63_16		0xffffffffffff0000ULL
#define	NPI_HIGHP_DBELL_DBELL			0x000000000000ffffULL

#define	NPI_DMA_CONTROL_XXX_63_36		0xfffffff000000000ULL
#define	NPI_DMA_CONTROL_B0_LEND			0x0000000800000000ULL
#define	NPI_DMA_CONTROL_DWB_DENB		0x0000000400000000ULL
#define	NPI_DMA_CONTROL_DWB_ICHK		0x00000003fe000000ULL
#define	NPI_DMA_CONTROL_FPA_QUE			0x0000000001c00000ULL
#define	NPI_DMA_CONTROL_O_ADD1			0x0000000000200000ULL
#define	NPI_DMA_CONTROL_O_RO			0x0000000000100000ULL
#define	NPI_DMA_CONTROL_O_NS			0x0000000000080000ULL
#define	NPI_DMA_CONTROL_O_ES			0x0000000000060000ULL
#define	NPI_DMA_CONTROL_O_MODE			0x0000000000010000ULL
#define	NPI_DMA_CONTROL_HP_ENB			0x0000000000008000ULL
#define	NPI_DMA_CONTROL_LP_ENB			0x0000000000004000ULL
#define	NPI_DMA_CONTROL_CSIZE			0x0000000000003fffULL

#define	NPI_PCI_INT_ARB_CFG_XXX_63_5		0xffffffffffffffe0ULL
#define	NPI_PCI_INT_ARB_CFG_EN			0x0000000000000010ULL
#define	NPI_PCI_INT_ARB_CFG_PARK_MOD		0x0000000000000008ULL
#define	NPI_PCI_INT_ARB_CFG_PARK_DEV		0x0000000000000007ULL

#define	NPI_INPUT_CONTROL_XXX_63_22		0xffffffffffc00000ULL
#define	NPI_INPUT_CONTROL_PBP_DHI		0x00000000003ffe00ULL
#define	NPI_INPUT_CONTROL_D_NSR			0x0000000000000100ULL
#define	NPI_INPUT_CONTROL_D_ESR			0x00000000000000c0ULL
#define	NPI_INPUT_CONTROL_D_ROR			0x0000000000000020ULL
#define	NPI_INPUT_CONTROL_USE_CSR		0x0000000000000010ULL
#define	NPI_INPUT_CONTROL_NSR			0x0000000000000008ULL
#define	NPI_INPUT_CONTROL_ESR			0x0000000000000006ULL
#define	NPI_INPUT_CONTROL_ROR			0x0000000000000001ULL

#define	NPI_DMA_LOWP_COUNTS_XXX_63_39		0xffffff8000000000ULL
#define	NPI_DMA_LOWP_COUNTS_FCNT		0x0000007f00000000ULL
#define	NPI_DMA_LOWP_COUNTS_DBELL		0x00000000ffffffffULL

#define	NPI_DMA_HIGHP_COUNTS_XXX_63_39		0xffffff8000000000ULL
#define	NPI_DMA_HIGHP_COUNTS_FCNT		0x0000007f00000000ULL
#define	NPI_DMA_HIGHP_COUNTS_DBELL		0x00000000ffffffffULL

#define	NPI_DMA_LOWP_NADDR_XXX_63_40		0xffffff0000000000ULL
#define	NPI_DMA_LOWP_NADDR_STATE		0x000000f000000000ULL
#define	NPI_DMA_LOWP_NADDR_ADDR			0x0000000fffffffffULL

#define	NPI_DMA_HIGHP_NADDR_XXX_63_40		0xffffff0000000000ULL
#define	NPI_DMA_HIGHP_NADDR_STATE		0x000000f000000000ULL
#define	NPI_DMA_HIGHP_NADDR_ADDR		0x0000000fffffffffULL

#define	NPI_P0_PAIR_CNTS_XXX_63_37		0xffffffe000000000ULL
#define	NPI_P0_PAIR_CNTS_FCNT			0xffffffff00000000ULL
#define	NPI_P0_PAIR_CNTS_AVAIL			0x00000000c0000000ULL

#define	NPI_P0_DBPAIR_ADDR_XXX_63		0x8000000000000000ULL
#define	NPI_P0_DBPAIR_ADDR_STATE		0x6000000000000000ULL
#define	NPI_P0_DBPAIR_ADDR_NADDR		0x1fffffffffffffffULL

#define	NPI_P0_INSTR_CNTS_XXX_63_38		0xffffffc000000000ULL
#define	NPI_P0_INSTR_CNTS_FCNT			0x0000003f00000000ULL
#define	NPI_P0_INSTR_CNTS_AVAIL			0x00000000ffffffffULL

#define	NPI_P0_INSTR_ADDR_STATE			0xe000000000000000ULL
#define	NPI_P0_INSTR_ADDR_NADDR			0x1fffffffffffffffULL

#define	NPI_WIN_READ_TO_XXX_63_32		0xffffffff00000000ULL
#define	NPI_WIN_READ_TO_TIME			0x00000000ffffffffULL

#define	DBG_DATA_XXX_63_31			0xffffffff80000000ULL
#define	DBG_DATA_PLL_MUL			0x0000000070000000ULL
#define	DBG_DATA_XXX_27_23			0x000000000f800000ULL
#define	DBG_DATA_C_MUL				0x00000000007c0000ULL
#define	DBG_DATA_DSEL_EXT			0x0000000000020000ULL
#define	DBG_DATA_DATA				0x000000000001ffffULL

#define	NPI_PORT_BP_CONTROL_XXX_63_5		0xffffffffffffffe0ULL
#define	NPI_PORT_BP_CONTROL_BP_ON		0x0000000000000010ULL
#define	NPI_PORT_BP_CONTROL_ENB			0x000000000000000fULL

#define	NPI_PORT32_INSTR_HDR_XXX_63_44		0xfffff00000000000ULL
#define	NPI_PORT32_INSTR_HDR_PBP		0x0000080000000000ULL
#define	NPI_PORT32_INSTR_HDR_XXX_42_38		0x000007c000000000ULL
#define	NPI_PORT32_INSTR_HDR_RPARMODE		0x0000003000000000ULL
#define	NPI_PORT32_INSTR_HDR_XXX_35		0x0000000800000000ULL
#define	NPI_PORT32_INSTR_HDR_RSKP_LEN		0x00000007f0000000ULL
#define	NPI_PORT32_INSTR_HDR_XXX_27_22		0x000000000fc00000ULL
#define	NPI_PORT32_INSTR_HDR_USE_IHDR		0x0000000000200000ULL
#define	NPI_PORT32_INSTR_HDR_XXX_20_16		0x00000000001f0000ULL
#define	NPI_PORT32_INSTR_HDR_PAR_MODE		0x000000000000c000ULL
#define	NPI_PORT32_INSTR_HDR_XXX_13		0x0000000000002000ULL
#define	NPI_PORT32_INSTR_HDR_SKP_LEN		0x0000000000001fc0ULL
#define	NPI_PORT32_INSTR_HDR_XXX_5_0		0x000000000000003fULL

#define	NPI_BIST_STATUS_XXX_63_20		0xfffffffffff00000ULL
#define	NPI_BIST_STATUS_CSR_BS			0x0000000000080000ULL
#define	NPI_BIST_STATUS_DIF_BS			0x0000000000040000ULL
#define	NPI_BIST_STATUS_RDP_BS			0x0000000000020000ULL
#define	NPI_BIST_STATUS_PCNC_BS			0x0000000000010000ULL
#define	NPI_BIST_STATUS_PCN_BS			0x0000000000008000ULL
#define	NPI_BIST_STATUS_RDN_BS			0x0000000000004000ULL
#define	NPI_BIST_STATUS_PCAC_BS			0x0000000000002000ULL
#define	NPI_BIST_STATUS_PCAD_BS			0x0000000000001000ULL
#define	NPI_BIST_STATUS_RDNL_BS			0x0000000000000800ULL
#define	NPI_BIST_STATUS_PGF_BS			0x0000000000000400ULL
#define	NPI_BIST_STATUS_PIG_BS			0x0000000000000200ULL
#define	NPI_BIST_STATUS_POF0_BS			0x0000000000000100ULL
#define	NPI_BIST_STATUS_POF1_BS			0x0000000000000080ULL
#define	NPI_BIST_STATUS_POF2_BS			0x0000000000000040ULL
#define	NPI_BIST_STATUS_POF3_BS			0x0000000000000020ULL
#define	NPI_BIST_STATUS_POS_BS			0x0000000000000010ULL
#define	NPI_BIST_STATUS_NUS_BS			0x0000000000000008ULL
#define	NPI_BIST_STATUS_DOB_BS			0x0000000000000004ULL
#define	NPI_BIST_STATUS_PDF_BS			0x0000000000000002ULL
#define	NPI_BIST_STATUS_DPI_BS			0x0000000000000001ULL

#endif /* _CN30XXNPIREG_H_ */
