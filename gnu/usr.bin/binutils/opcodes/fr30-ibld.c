/* Instruction building/extraction support for fr30. -*- C -*-

THIS FILE IS MACHINE GENERATED WITH CGEN: Cpu tools GENerator.
- the resultant file is machine generated, cgen-ibld.in isn't

Copyright (C) 1996, 1997, 1998, 1999 Free Software Foundation, Inc.

This file is part of the GNU Binutils and GDB, the GNU debugger.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software Foundation, Inc.,
59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

/* ??? Eventually more and more of this stuff can go to cpu-independent files.
   Keep that in mind.  */

#include "sysdep.h"
#include <ctype.h>
#include <stdio.h>
#include "ansidecl.h"
#include "dis-asm.h"
#include "bfd.h"
#include "symcat.h"
#include "fr30-desc.h"
#include "fr30-opc.h"
#include "opintl.h"

#undef min
#define min(a,b) ((a) < (b) ? (a) : (b))
#undef max
#define max(a,b) ((a) > (b) ? (a) : (b))

/* Used by the ifield rtx function.  */
#define FLD(f) (fields->f)

static const char * insert_normal
     PARAMS ((CGEN_CPU_DESC, long, unsigned int, unsigned int, unsigned int,
	      unsigned int, unsigned int, unsigned int, CGEN_INSN_BYTES_PTR));
static const char * insert_insn_normal
     PARAMS ((CGEN_CPU_DESC, const CGEN_INSN *,
	      CGEN_FIELDS *, CGEN_INSN_BYTES_PTR, bfd_vma));

static int extract_normal
     PARAMS ((CGEN_CPU_DESC, CGEN_EXTRACT_INFO *, CGEN_INSN_INT,
	      unsigned int, unsigned int, unsigned int, unsigned int,
	      unsigned int, unsigned int, bfd_vma, long *));
static int extract_insn_normal
     PARAMS ((CGEN_CPU_DESC, const CGEN_INSN *, CGEN_EXTRACT_INFO *,
	      CGEN_INSN_INT, CGEN_FIELDS *, bfd_vma));

/* Operand insertion.  */

#if ! CGEN_INT_INSN_P

/* Subroutine of insert_normal.  */

static CGEN_INLINE void
insert_1 (cd, value, start, length, word_length, bufp)
     CGEN_CPU_DESC cd;
     unsigned long value;
     int start,length,word_length;
     unsigned char *bufp;
{
  unsigned long x,mask;
  int shift;
  int big_p = CGEN_CPU_INSN_ENDIAN (cd) == CGEN_ENDIAN_BIG;

  switch (word_length)
    {
    case 8:
      x = *bufp;
      break;
    case 16:
      if (big_p)
	x = bfd_getb16 (bufp);
      else
	x = bfd_getl16 (bufp);
      break;
    case 24:
      /* ??? This may need reworking as these cases don't necessarily
	 want the first byte and the last two bytes handled like this.  */
      if (big_p)
	x = (bufp[0] << 16) | bfd_getb16 (bufp + 1);
      else
	x = bfd_getl16 (bufp) | (bufp[2] << 16);
      break;
    case 32:
      if (big_p)
	x = bfd_getb32 (bufp);
      else
	x = bfd_getl32 (bufp);
      break;
    default :
      abort ();
    }

  /* Written this way to avoid undefined behaviour.  */
  mask = (((1L << (length - 1)) - 1) << 1) | 1;
  if (CGEN_INSN_LSB0_P)
    shift = (start + 1) - length;
  else
    shift = (word_length - (start + length));
  x = (x & ~(mask << shift)) | ((value & mask) << shift);

  switch (word_length)
    {
    case 8:
      *bufp = x;
      break;
    case 16:
      if (big_p)
	bfd_putb16 (x, bufp);
      else
	bfd_putl16 (x, bufp);
      break;
    case 24:
      /* ??? This may need reworking as these cases don't necessarily
	 want the first byte and the last two bytes handled like this.  */
      if (big_p)
	{
	  bufp[0] = x >> 16;
	  bfd_putb16 (x, bufp + 1);
	}
      else
	{
	  bfd_putl16 (x, bufp);
	  bufp[2] = x >> 16;
	}
      break;
    case 32:
      if (big_p)
	bfd_putb32 (x, bufp);
      else
	bfd_putl32 (x, bufp);
      break;
    default :
      abort ();
    }
}

#endif /* ! CGEN_INT_INSN_P */

/* Default insertion routine.

   ATTRS is a mask of the boolean attributes.
   WORD_OFFSET is the offset in bits from the start of the insn of the value.
   WORD_LENGTH is the length of the word in bits in which the value resides.
   START is the starting bit number in the word, architecture origin.
   LENGTH is the length of VALUE in bits.
   TOTAL_LENGTH is the total length of the insn in bits.

   The result is an error message or NULL if success.  */

/* ??? This duplicates functionality with bfd's howto table and
   bfd_install_relocation.  */
/* ??? This doesn't handle bfd_vma's.  Create another function when
   necessary.  */

static const char *
insert_normal (cd, value, attrs, word_offset, start, length, word_length,
	       total_length, buffer)
     CGEN_CPU_DESC cd;
     long value;
     unsigned int attrs;
     unsigned int word_offset, start, length, word_length, total_length;
     CGEN_INSN_BYTES_PTR buffer;
{
  static char errbuf[100];
  /* Written this way to avoid undefined behaviour.  */
  unsigned long mask = (((1L << (length - 1)) - 1) << 1) | 1;

  /* If LENGTH is zero, this operand doesn't contribute to the value.  */
  if (length == 0)
    return NULL;

  if (CGEN_INT_INSN_P
      && word_offset != 0)
    abort ();

  if (word_length > 32)
    abort ();

  /* For architectures with insns smaller than the base-insn-bitsize,
     word_length may be too big.  */
  if (cd->min_insn_bitsize < cd->base_insn_bitsize)
    {
      if (word_offset == 0
	  && word_length > total_length)
	word_length = total_length;
    }

  /* Ensure VALUE will fit.  */
  if (! CGEN_BOOL_ATTR (attrs, CGEN_IFLD_SIGNED))
    {
      unsigned long maxval = mask;
      if ((unsigned long) value > maxval)
	{
	  /* xgettext:c-format */
	  sprintf (errbuf,
		   _("operand out of range (%lu not between 0 and %lu)"),
		   value, maxval);
	  return errbuf;
	}
    }
  else
    {
      long minval = - (1L << (length - 1));
      long maxval = (1L << (length - 1)) - 1;
      if (value < minval || value > maxval)
	{
	  sprintf
	    /* xgettext:c-format */
	    (errbuf, _("operand out of range (%ld not between %ld and %ld)"),
	     value, minval, maxval);
	  return errbuf;
	}
    }

#if CGEN_INT_INSN_P

  {
    int shift;

    if (CGEN_INSN_LSB0_P)
      shift = (start + 1) - length;
    else
      shift = word_length - (start + length);
    *buffer = (*buffer & ~(mask << shift)) | ((value & mask) << shift);
  }

#else /* ! CGEN_INT_INSN_P */

  {
    unsigned char *bufp = (unsigned char *) buffer + word_offset / 8;

    insert_1 (cd, value, start, length, word_length, bufp);
  }

#endif /* ! CGEN_INT_INSN_P */

  return NULL;
}

/* Default insn builder (insert handler).
   The instruction is recorded in CGEN_INT_INSN_P byte order
   (meaning that if CGEN_INT_INSN_P BUFFER is an int * and thus the value is
   recorded in host byte order, otherwise BUFFER is an array of bytes and the
   value is recorded in target byte order).
   The result is an error message or NULL if success.  */

static const char *
insert_insn_normal (cd, insn, fields, buffer, pc)
     CGEN_CPU_DESC cd;
     const CGEN_INSN * insn;
     CGEN_FIELDS * fields;
     CGEN_INSN_BYTES_PTR buffer;
     bfd_vma pc;
{
  const CGEN_SYNTAX *syntax = CGEN_INSN_SYNTAX (insn);
  unsigned long value;
  const unsigned char * syn;

  CGEN_INIT_INSERT (cd);
  value = CGEN_INSN_BASE_VALUE (insn);

  /* If we're recording insns as numbers (rather than a string of bytes),
     target byte order handling is deferred until later.  */

#if CGEN_INT_INSN_P

  *buffer = value;

#else

  cgen_put_insn_value (cd, buffer, min (cd->base_insn_bitsize,
					CGEN_FIELDS_BITSIZE (fields)),
		       value);

#endif /* ! CGEN_INT_INSN_P */

  /* ??? It would be better to scan the format's fields.
     Still need to be able to insert a value based on the operand though;
     e.g. storing a branch displacement that got resolved later.
     Needs more thought first.  */

  for (syn = CGEN_SYNTAX_STRING (syntax); * syn != '\0'; ++ syn)
    {
      const char *errmsg;

      if (CGEN_SYNTAX_CHAR_P (* syn))
	continue;

      errmsg = (* cd->insert_operand) (cd, CGEN_SYNTAX_FIELD (*syn),
				       fields, buffer, pc);
      if (errmsg)
	return errmsg;
    }

  return NULL;
}

/* Operand extraction.  */

#if ! CGEN_INT_INSN_P

/* Subroutine of extract_normal.
   Ensure sufficient bytes are cached in EX_INFO.
   OFFSET is the offset in bytes from the start of the insn of the value.
   BYTES is the length of the needed value.
   Returns 1 for success, 0 for failure.  */

static CGEN_INLINE int
fill_cache (cd, ex_info, offset, bytes, pc)
     CGEN_CPU_DESC cd;
     CGEN_EXTRACT_INFO *ex_info;
     int offset, bytes;
     bfd_vma pc;
{
  /* It's doubtful that the middle part has already been fetched so
     we don't optimize that case.  kiss.  */
  int mask;
  disassemble_info *info = (disassemble_info *) ex_info->dis_info;

  /* First do a quick check.  */
  mask = (1 << bytes) - 1;
  if (((ex_info->valid >> offset) & mask) == mask)
    return 1;

  /* Search for the first byte we need to read.  */
  for (mask = 1 << offset; bytes > 0; --bytes, ++offset, mask <<= 1)
    if (! (mask & ex_info->valid))
      break;

  if (bytes)
    {
      int status;

      pc += offset;
      status = (*info->read_memory_func)
	(pc, ex_info->insn_bytes + offset, bytes, info);

      if (status != 0)
	{
	  (*info->memory_error_func) (status, pc, info);
	  return 0;
	}

      ex_info->valid |= ((1 << bytes) - 1) << offset;
    }

  return 1;
}

/* Subroutine of extract_normal.  */

static CGEN_INLINE long
extract_1 (cd, ex_info, start, length, word_length, bufp, pc)
     CGEN_CPU_DESC cd;
     CGEN_EXTRACT_INFO *ex_info;
     int start,length,word_length;
     unsigned char *bufp;
     bfd_vma pc;
{
  unsigned long x,mask;
  int shift;
  int big_p = CGEN_CPU_INSN_ENDIAN (cd) == CGEN_ENDIAN_BIG;

  switch (word_length)
    {
    case 8:
      x = *bufp;
      break;
    case 16:
      if (big_p)
	x = bfd_getb16 (bufp);
      else
	x = bfd_getl16 (bufp);
      break;
    case 24:
      /* ??? This may need reworking as these cases don't necessarily
	 want the first byte and the last two bytes handled like this.  */
      if (big_p)
	x = (bufp[0] << 16) | bfd_getb16 (bufp + 1);
      else
	x = bfd_getl16 (bufp) | (bufp[2] << 16);
      break;
    case 32:
      if (big_p)
	x = bfd_getb32 (bufp);
      else
	x = bfd_getl32 (bufp);
      break;
    default :
      abort ();
    }

  /* Written this way to avoid undefined behaviour.  */
  mask = (((1L << (length - 1)) - 1) << 1) | 1;
  if (CGEN_INSN_LSB0_P)
    shift = (start + 1) - length;
  else
    shift = (word_length - (start + length));
  return (x >> shift) & mask;
}

#endif /* ! CGEN_INT_INSN_P */

/* Default extraction routine.

   INSN_VALUE is the first base_insn_bitsize bits of the insn in host order,
   or sometimes less for cases like the m32r where the base insn size is 32
   but some insns are 16 bits.
   ATTRS is a mask of the boolean attributes.  We only need `SIGNED',
   but for generality we take a bitmask of all of them.
   WORD_OFFSET is the offset in bits from the start of the insn of the value.
   WORD_LENGTH is the length of the word in bits in which the value resides.
   START is the starting bit number in the word, architecture origin.
   LENGTH is the length of VALUE in bits.
   TOTAL_LENGTH is the total length of the insn in bits.

   Returns 1 for success, 0 for failure.  */

/* ??? The return code isn't properly used.  wip.  */

/* ??? This doesn't handle bfd_vma's.  Create another function when
   necessary.  */

static int
extract_normal (cd, ex_info, insn_value, attrs, word_offset, start, length,
		word_length, total_length, pc, valuep)
     CGEN_CPU_DESC cd;
     CGEN_EXTRACT_INFO *ex_info;
     CGEN_INSN_INT insn_value;
     unsigned int attrs;
     unsigned int word_offset, start, length, word_length, total_length;
     bfd_vma pc;
     long *valuep;
{
  CGEN_INSN_INT value;

  /* If LENGTH is zero, this operand doesn't contribute to the value
     so give it a standard value of zero.  */
  if (length == 0)
    {
      *valuep = 0;
      return 1;
    }

  if (CGEN_INT_INSN_P
      && word_offset != 0)
    abort ();

  if (word_length > 32)
    abort ();

  /* For architectures with insns smaller than the insn-base-bitsize,
     word_length may be too big.  */
  if (cd->min_insn_bitsize < cd->base_insn_bitsize)
    {
      if (word_offset == 0
	  && word_length > total_length)
	word_length = total_length;
    }

  /* Does the value reside in INSN_VALUE?  */

  if (word_offset == 0)
    {
      /* Written this way to avoid undefined behaviour.  */
      CGEN_INSN_INT mask = (((1L << (length - 1)) - 1) << 1) | 1;

      if (CGEN_INSN_LSB0_P)
	value = insn_value >> ((start + 1) - length);
      else
	value = insn_value >> (word_length - (start + length));
      value &= mask;
      /* sign extend? */
      if (CGEN_BOOL_ATTR (attrs, CGEN_IFLD_SIGNED)
	  && (value & (1L << (length - 1))))
	value |= ~mask;
    }

#if ! CGEN_INT_INSN_P

  else
    {
      unsigned char *bufp = ex_info->insn_bytes + word_offset / 8;

      if (word_length > 32)
	abort ();

      if (fill_cache (cd, ex_info, word_offset / 8, word_length / 8, pc) == 0)
	return 0;

      value = extract_1 (cd, ex_info, start, length, word_length, bufp, pc);
    }

#endif /* ! CGEN_INT_INSN_P */

  *valuep = value;

  return 1;
}

/* Default insn extractor.

   INSN_VALUE is the first base_insn_bitsize bits, translated to host order.
   The extracted fields are stored in FIELDS.
   EX_INFO is used to handle reading variable length insns.
   Return the length of the insn in bits, or 0 if no match,
   or -1 if an error occurs fetching data (memory_error_func will have
   been called).  */

static int
extract_insn_normal (cd, insn, ex_info, insn_value, fields, pc)
     CGEN_CPU_DESC cd;
     const CGEN_INSN *insn;
     CGEN_EXTRACT_INFO *ex_info;
     CGEN_INSN_INT insn_value;
     CGEN_FIELDS *fields;
     bfd_vma pc;
{
  const CGEN_SYNTAX *syntax = CGEN_INSN_SYNTAX (insn);
  const unsigned char *syn;

  CGEN_FIELDS_BITSIZE (fields) = CGEN_INSN_BITSIZE (insn);

  CGEN_INIT_EXTRACT (cd);

  for (syn = CGEN_SYNTAX_STRING (syntax); *syn; ++syn)
    {
      int length;

      if (CGEN_SYNTAX_CHAR_P (*syn))
	continue;

      length = (* cd->extract_operand) (cd, CGEN_SYNTAX_FIELD (*syn),
					ex_info, insn_value, fields, pc);
      if (length <= 0)
	return length;
    }

  /* We recognized and successfully extracted this insn.  */
  return CGEN_INSN_BITSIZE (insn);
}

/* machine generated code added here */

/* Main entry point for operand insertion.

   This function is basically just a big switch statement.  Earlier versions
   used tables to look up the function to use, but
   - if the table contains both assembler and disassembler functions then
     the disassembler contains much of the assembler and vice-versa,
   - there's a lot of inlining possibilities as things grow,
   - using a switch statement avoids the function call overhead.

   This function could be moved into `parse_insn_normal', but keeping it
   separate makes clear the interface between `parse_insn_normal' and each of
   the handlers.  It's also needed by GAS to insert operands that couldn't be
   resolved during parsing.
*/

const char *
fr30_cgen_insert_operand (cd, opindex, fields, buffer, pc)
     CGEN_CPU_DESC cd;
     int opindex;
     CGEN_FIELDS * fields;
     CGEN_INSN_BYTES_PTR buffer;
     bfd_vma pc;
{
  const char * errmsg = NULL;
  unsigned int total_length = CGEN_FIELDS_BITSIZE (fields);

  switch (opindex)
    {
    case FR30_OPERAND_CRI :
      errmsg = insert_normal (cd, fields->f_CRi, 0, 16, 12, 4, 16, total_length, buffer);
      break;
    case FR30_OPERAND_CRJ :
      errmsg = insert_normal (cd, fields->f_CRj, 0, 16, 8, 4, 16, total_length, buffer);
      break;
    case FR30_OPERAND_R13 :
      break;
    case FR30_OPERAND_R14 :
      break;
    case FR30_OPERAND_R15 :
      break;
    case FR30_OPERAND_RI :
      errmsg = insert_normal (cd, fields->f_Ri, 0, 0, 12, 4, 16, total_length, buffer);
      break;
    case FR30_OPERAND_RIC :
      errmsg = insert_normal (cd, fields->f_Ric, 0, 16, 12, 4, 16, total_length, buffer);
      break;
    case FR30_OPERAND_RJ :
      errmsg = insert_normal (cd, fields->f_Rj, 0, 0, 8, 4, 16, total_length, buffer);
      break;
    case FR30_OPERAND_RJC :
      errmsg = insert_normal (cd, fields->f_Rjc, 0, 16, 8, 4, 16, total_length, buffer);
      break;
    case FR30_OPERAND_RS1 :
      errmsg = insert_normal (cd, fields->f_Rs1, 0, 0, 8, 4, 16, total_length, buffer);
      break;
    case FR30_OPERAND_RS2 :
      errmsg = insert_normal (cd, fields->f_Rs2, 0, 0, 12, 4, 16, total_length, buffer);
      break;
    case FR30_OPERAND_CC :
      errmsg = insert_normal (cd, fields->f_cc, 0, 0, 4, 4, 16, total_length, buffer);
      break;
    case FR30_OPERAND_CCC :
      errmsg = insert_normal (cd, fields->f_ccc, 0, 16, 0, 8, 16, total_length, buffer);
      break;
    case FR30_OPERAND_DIR10 :
      {
        long value = fields->f_dir10;
        value = ((unsigned int) (value) >> (2));
        errmsg = insert_normal (cd, value, 0, 0, 8, 8, 16, total_length, buffer);
      }
      break;
    case FR30_OPERAND_DIR8 :
      errmsg = insert_normal (cd, fields->f_dir8, 0, 0, 8, 8, 16, total_length, buffer);
      break;
    case FR30_OPERAND_DIR9 :
      {
        long value = fields->f_dir9;
        value = ((unsigned int) (value) >> (1));
        errmsg = insert_normal (cd, value, 0, 0, 8, 8, 16, total_length, buffer);
      }
      break;
    case FR30_OPERAND_DISP10 :
      {
        long value = fields->f_disp10;
        value = ((int) (value) >> (2));
        errmsg = insert_normal (cd, value, 0|(1<<CGEN_IFLD_SIGNED), 0, 4, 8, 16, total_length, buffer);
      }
      break;
    case FR30_OPERAND_DISP8 :
      errmsg = insert_normal (cd, fields->f_disp8, 0|(1<<CGEN_IFLD_SIGNED), 0, 4, 8, 16, total_length, buffer);
      break;
    case FR30_OPERAND_DISP9 :
      {
        long value = fields->f_disp9;
        value = ((int) (value) >> (1));
        errmsg = insert_normal (cd, value, 0|(1<<CGEN_IFLD_SIGNED), 0, 4, 8, 16, total_length, buffer);
      }
      break;
    case FR30_OPERAND_I20 :
      {
{
  FLD (f_i20_4) = ((unsigned int) (FLD (f_i20)) >> (16));
  FLD (f_i20_16) = ((FLD (f_i20)) & (65535));
}
        errmsg = insert_normal (cd, fields->f_i20_4, 0, 0, 8, 4, 16, total_length, buffer);
        if (errmsg)
          break;
        errmsg = insert_normal (cd, fields->f_i20_16, 0, 16, 0, 16, 16, total_length, buffer);
        if (errmsg)
          break;
      }
      break;
    case FR30_OPERAND_I32 :
      errmsg = insert_normal (cd, fields->f_i32, 0|(1<<CGEN_IFLD_SIGN_OPT), 16, 0, 32, 32, total_length, buffer);
      break;
    case FR30_OPERAND_I8 :
      errmsg = insert_normal (cd, fields->f_i8, 0, 0, 4, 8, 16, total_length, buffer);
      break;
    case FR30_OPERAND_LABEL12 :
      {
        long value = fields->f_rel12;
        value = ((int) (((value) - (((pc) + (2))))) >> (1));
        errmsg = insert_normal (cd, value, 0|(1<<CGEN_IFLD_SIGNED)|(1<<CGEN_IFLD_PCREL_ADDR), 0, 5, 11, 16, total_length, buffer);
      }
      break;
    case FR30_OPERAND_LABEL9 :
      {
        long value = fields->f_rel9;
        value = ((int) (((value) - (((pc) + (2))))) >> (1));
        errmsg = insert_normal (cd, value, 0|(1<<CGEN_IFLD_SIGNED)|(1<<CGEN_IFLD_PCREL_ADDR), 0, 8, 8, 16, total_length, buffer);
      }
      break;
    case FR30_OPERAND_M4 :
      {
        long value = fields->f_m4;
        value = ((value) & (15));
        errmsg = insert_normal (cd, value, 0, 0, 8, 4, 16, total_length, buffer);
      }
      break;
    case FR30_OPERAND_PS :
      break;
    case FR30_OPERAND_REGLIST_HI_LD :
      errmsg = insert_normal (cd, fields->f_reglist_hi_ld, 0, 0, 8, 8, 16, total_length, buffer);
      break;
    case FR30_OPERAND_REGLIST_HI_ST :
      errmsg = insert_normal (cd, fields->f_reglist_hi_st, 0, 0, 8, 8, 16, total_length, buffer);
      break;
    case FR30_OPERAND_REGLIST_LOW_LD :
      errmsg = insert_normal (cd, fields->f_reglist_low_ld, 0, 0, 8, 8, 16, total_length, buffer);
      break;
    case FR30_OPERAND_REGLIST_LOW_ST :
      errmsg = insert_normal (cd, fields->f_reglist_low_st, 0, 0, 8, 8, 16, total_length, buffer);
      break;
    case FR30_OPERAND_S10 :
      {
        long value = fields->f_s10;
        value = ((int) (value) >> (2));
        errmsg = insert_normal (cd, value, 0|(1<<CGEN_IFLD_SIGNED), 0, 8, 8, 16, total_length, buffer);
      }
      break;
    case FR30_OPERAND_U10 :
      {
        long value = fields->f_u10;
        value = ((unsigned int) (value) >> (2));
        errmsg = insert_normal (cd, value, 0, 0, 8, 8, 16, total_length, buffer);
      }
      break;
    case FR30_OPERAND_U4 :
      errmsg = insert_normal (cd, fields->f_u4, 0, 0, 8, 4, 16, total_length, buffer);
      break;
    case FR30_OPERAND_U4C :
      errmsg = insert_normal (cd, fields->f_u4c, 0, 0, 12, 4, 16, total_length, buffer);
      break;
    case FR30_OPERAND_U8 :
      errmsg = insert_normal (cd, fields->f_u8, 0, 0, 8, 8, 16, total_length, buffer);
      break;
    case FR30_OPERAND_UDISP6 :
      {
        long value = fields->f_udisp6;
        value = ((unsigned int) (value) >> (2));
        errmsg = insert_normal (cd, value, 0, 0, 8, 4, 16, total_length, buffer);
      }
      break;

    default :
      /* xgettext:c-format */
      fprintf (stderr, _("Unrecognized field %d while building insn.\n"),
	       opindex);
      abort ();
  }

  return errmsg;
}

/* Main entry point for operand extraction.
   The result is <= 0 for error, >0 for success.
   ??? Actual values aren't well defined right now.

   This function is basically just a big switch statement.  Earlier versions
   used tables to look up the function to use, but
   - if the table contains both assembler and disassembler functions then
     the disassembler contains much of the assembler and vice-versa,
   - there's a lot of inlining possibilities as things grow,
   - using a switch statement avoids the function call overhead.

   This function could be moved into `print_insn_normal', but keeping it
   separate makes clear the interface between `print_insn_normal' and each of
   the handlers.
*/

int
fr30_cgen_extract_operand (cd, opindex, ex_info, insn_value, fields, pc)
     CGEN_CPU_DESC cd;
     int opindex;
     CGEN_EXTRACT_INFO *ex_info;
     CGEN_INSN_INT insn_value;
     CGEN_FIELDS * fields;
     bfd_vma pc;
{
  /* Assume success (for those operands that are nops).  */
  int length = 1;
  unsigned int total_length = CGEN_FIELDS_BITSIZE (fields);

  switch (opindex)
    {
    case FR30_OPERAND_CRI :
      length = extract_normal (cd, ex_info, insn_value, 0, 16, 12, 4, 16, total_length, pc, & fields->f_CRi);
      break;
    case FR30_OPERAND_CRJ :
      length = extract_normal (cd, ex_info, insn_value, 0, 16, 8, 4, 16, total_length, pc, & fields->f_CRj);
      break;
    case FR30_OPERAND_R13 :
      break;
    case FR30_OPERAND_R14 :
      break;
    case FR30_OPERAND_R15 :
      break;
    case FR30_OPERAND_RI :
      length = extract_normal (cd, ex_info, insn_value, 0, 0, 12, 4, 16, total_length, pc, & fields->f_Ri);
      break;
    case FR30_OPERAND_RIC :
      length = extract_normal (cd, ex_info, insn_value, 0, 16, 12, 4, 16, total_length, pc, & fields->f_Ric);
      break;
    case FR30_OPERAND_RJ :
      length = extract_normal (cd, ex_info, insn_value, 0, 0, 8, 4, 16, total_length, pc, & fields->f_Rj);
      break;
    case FR30_OPERAND_RJC :
      length = extract_normal (cd, ex_info, insn_value, 0, 16, 8, 4, 16, total_length, pc, & fields->f_Rjc);
      break;
    case FR30_OPERAND_RS1 :
      length = extract_normal (cd, ex_info, insn_value, 0, 0, 8, 4, 16, total_length, pc, & fields->f_Rs1);
      break;
    case FR30_OPERAND_RS2 :
      length = extract_normal (cd, ex_info, insn_value, 0, 0, 12, 4, 16, total_length, pc, & fields->f_Rs2);
      break;
    case FR30_OPERAND_CC :
      length = extract_normal (cd, ex_info, insn_value, 0, 0, 4, 4, 16, total_length, pc, & fields->f_cc);
      break;
    case FR30_OPERAND_CCC :
      length = extract_normal (cd, ex_info, insn_value, 0, 16, 0, 8, 16, total_length, pc, & fields->f_ccc);
      break;
    case FR30_OPERAND_DIR10 :
      {
        long value;
        length = extract_normal (cd, ex_info, insn_value, 0, 0, 8, 8, 16, total_length, pc, & value);
        value = ((value) << (2));
        fields->f_dir10 = value;
      }
      break;
    case FR30_OPERAND_DIR8 :
      length = extract_normal (cd, ex_info, insn_value, 0, 0, 8, 8, 16, total_length, pc, & fields->f_dir8);
      break;
    case FR30_OPERAND_DIR9 :
      {
        long value;
        length = extract_normal (cd, ex_info, insn_value, 0, 0, 8, 8, 16, total_length, pc, & value);
        value = ((value) << (1));
        fields->f_dir9 = value;
      }
      break;
    case FR30_OPERAND_DISP10 :
      {
        long value;
        length = extract_normal (cd, ex_info, insn_value, 0|(1<<CGEN_IFLD_SIGNED), 0, 4, 8, 16, total_length, pc, & value);
        value = ((value) << (2));
        fields->f_disp10 = value;
      }
      break;
    case FR30_OPERAND_DISP8 :
      length = extract_normal (cd, ex_info, insn_value, 0|(1<<CGEN_IFLD_SIGNED), 0, 4, 8, 16, total_length, pc, & fields->f_disp8);
      break;
    case FR30_OPERAND_DISP9 :
      {
        long value;
        length = extract_normal (cd, ex_info, insn_value, 0|(1<<CGEN_IFLD_SIGNED), 0, 4, 8, 16, total_length, pc, & value);
        value = ((value) << (1));
        fields->f_disp9 = value;
      }
      break;
    case FR30_OPERAND_I20 :
      {
        length = extract_normal (cd, ex_info, insn_value, 0, 0, 8, 4, 16, total_length, pc, & fields->f_i20_4);
        length = extract_normal (cd, ex_info, insn_value, 0, 16, 0, 16, 16, total_length, pc, & fields->f_i20_16);
{
  FLD (f_i20) = ((((FLD (f_i20_4)) << (16))) | (FLD (f_i20_16)));
}
      }
      break;
    case FR30_OPERAND_I32 :
      length = extract_normal (cd, ex_info, insn_value, 0|(1<<CGEN_IFLD_SIGN_OPT), 16, 0, 32, 32, total_length, pc, & fields->f_i32);
      break;
    case FR30_OPERAND_I8 :
      length = extract_normal (cd, ex_info, insn_value, 0, 0, 4, 8, 16, total_length, pc, & fields->f_i8);
      break;
    case FR30_OPERAND_LABEL12 :
      {
        long value;
        length = extract_normal (cd, ex_info, insn_value, 0|(1<<CGEN_IFLD_SIGNED)|(1<<CGEN_IFLD_PCREL_ADDR), 0, 5, 11, 16, total_length, pc, & value);
        value = ((((value) << (1))) + (((pc) + (2))));
        fields->f_rel12 = value;
      }
      break;
    case FR30_OPERAND_LABEL9 :
      {
        long value;
        length = extract_normal (cd, ex_info, insn_value, 0|(1<<CGEN_IFLD_SIGNED)|(1<<CGEN_IFLD_PCREL_ADDR), 0, 8, 8, 16, total_length, pc, & value);
        value = ((((value) << (1))) + (((pc) + (2))));
        fields->f_rel9 = value;
      }
      break;
    case FR30_OPERAND_M4 :
      {
        long value;
        length = extract_normal (cd, ex_info, insn_value, 0, 0, 8, 4, 16, total_length, pc, & value);
        value = ((value) | (((-1) << (4))));
        fields->f_m4 = value;
      }
      break;
    case FR30_OPERAND_PS :
      break;
    case FR30_OPERAND_REGLIST_HI_LD :
      length = extract_normal (cd, ex_info, insn_value, 0, 0, 8, 8, 16, total_length, pc, & fields->f_reglist_hi_ld);
      break;
    case FR30_OPERAND_REGLIST_HI_ST :
      length = extract_normal (cd, ex_info, insn_value, 0, 0, 8, 8, 16, total_length, pc, & fields->f_reglist_hi_st);
      break;
    case FR30_OPERAND_REGLIST_LOW_LD :
      length = extract_normal (cd, ex_info, insn_value, 0, 0, 8, 8, 16, total_length, pc, & fields->f_reglist_low_ld);
      break;
    case FR30_OPERAND_REGLIST_LOW_ST :
      length = extract_normal (cd, ex_info, insn_value, 0, 0, 8, 8, 16, total_length, pc, & fields->f_reglist_low_st);
      break;
    case FR30_OPERAND_S10 :
      {
        long value;
        length = extract_normal (cd, ex_info, insn_value, 0|(1<<CGEN_IFLD_SIGNED), 0, 8, 8, 16, total_length, pc, & value);
        value = ((value) << (2));
        fields->f_s10 = value;
      }
      break;
    case FR30_OPERAND_U10 :
      {
        long value;
        length = extract_normal (cd, ex_info, insn_value, 0, 0, 8, 8, 16, total_length, pc, & value);
        value = ((value) << (2));
        fields->f_u10 = value;
      }
      break;
    case FR30_OPERAND_U4 :
      length = extract_normal (cd, ex_info, insn_value, 0, 0, 8, 4, 16, total_length, pc, & fields->f_u4);
      break;
    case FR30_OPERAND_U4C :
      length = extract_normal (cd, ex_info, insn_value, 0, 0, 12, 4, 16, total_length, pc, & fields->f_u4c);
      break;
    case FR30_OPERAND_U8 :
      length = extract_normal (cd, ex_info, insn_value, 0, 0, 8, 8, 16, total_length, pc, & fields->f_u8);
      break;
    case FR30_OPERAND_UDISP6 :
      {
        long value;
        length = extract_normal (cd, ex_info, insn_value, 0, 0, 8, 4, 16, total_length, pc, & value);
        value = ((value) << (2));
        fields->f_udisp6 = value;
      }
      break;

    default :
      /* xgettext:c-format */
      fprintf (stderr, _("Unrecognized field %d while decoding insn.\n"),
	       opindex);
      abort ();
    }

  return length;
}

cgen_insert_fn * const fr30_cgen_insert_handlers[] = 
{
  insert_insn_normal,
};

cgen_extract_fn * const fr30_cgen_extract_handlers[] = 
{
  extract_insn_normal,
};

/* Getting values from cgen_fields is handled by a collection of functions.
   They are distinguished by the type of the VALUE argument they return.
   TODO: floating point, inlining support, remove cases where result type
   not appropriate.  */

int
fr30_cgen_get_int_operand (cd, opindex, fields)
     CGEN_CPU_DESC cd;
     int opindex;
     const CGEN_FIELDS * fields;
{
  int value;

  switch (opindex)
    {
    case FR30_OPERAND_CRI :
      value = fields->f_CRi;
      break;
    case FR30_OPERAND_CRJ :
      value = fields->f_CRj;
      break;
    case FR30_OPERAND_R13 :
      value = 0;
      break;
    case FR30_OPERAND_R14 :
      value = 0;
      break;
    case FR30_OPERAND_R15 :
      value = 0;
      break;
    case FR30_OPERAND_RI :
      value = fields->f_Ri;
      break;
    case FR30_OPERAND_RIC :
      value = fields->f_Ric;
      break;
    case FR30_OPERAND_RJ :
      value = fields->f_Rj;
      break;
    case FR30_OPERAND_RJC :
      value = fields->f_Rjc;
      break;
    case FR30_OPERAND_RS1 :
      value = fields->f_Rs1;
      break;
    case FR30_OPERAND_RS2 :
      value = fields->f_Rs2;
      break;
    case FR30_OPERAND_CC :
      value = fields->f_cc;
      break;
    case FR30_OPERAND_CCC :
      value = fields->f_ccc;
      break;
    case FR30_OPERAND_DIR10 :
      value = fields->f_dir10;
      break;
    case FR30_OPERAND_DIR8 :
      value = fields->f_dir8;
      break;
    case FR30_OPERAND_DIR9 :
      value = fields->f_dir9;
      break;
    case FR30_OPERAND_DISP10 :
      value = fields->f_disp10;
      break;
    case FR30_OPERAND_DISP8 :
      value = fields->f_disp8;
      break;
    case FR30_OPERAND_DISP9 :
      value = fields->f_disp9;
      break;
    case FR30_OPERAND_I20 :
      value = fields->f_i20;
      break;
    case FR30_OPERAND_I32 :
      value = fields->f_i32;
      break;
    case FR30_OPERAND_I8 :
      value = fields->f_i8;
      break;
    case FR30_OPERAND_LABEL12 :
      value = fields->f_rel12;
      break;
    case FR30_OPERAND_LABEL9 :
      value = fields->f_rel9;
      break;
    case FR30_OPERAND_M4 :
      value = fields->f_m4;
      break;
    case FR30_OPERAND_PS :
      value = 0;
      break;
    case FR30_OPERAND_REGLIST_HI_LD :
      value = fields->f_reglist_hi_ld;
      break;
    case FR30_OPERAND_REGLIST_HI_ST :
      value = fields->f_reglist_hi_st;
      break;
    case FR30_OPERAND_REGLIST_LOW_LD :
      value = fields->f_reglist_low_ld;
      break;
    case FR30_OPERAND_REGLIST_LOW_ST :
      value = fields->f_reglist_low_st;
      break;
    case FR30_OPERAND_S10 :
      value = fields->f_s10;
      break;
    case FR30_OPERAND_U10 :
      value = fields->f_u10;
      break;
    case FR30_OPERAND_U4 :
      value = fields->f_u4;
      break;
    case FR30_OPERAND_U4C :
      value = fields->f_u4c;
      break;
    case FR30_OPERAND_U8 :
      value = fields->f_u8;
      break;
    case FR30_OPERAND_UDISP6 :
      value = fields->f_udisp6;
      break;

    default :
      /* xgettext:c-format */
      fprintf (stderr, _("Unrecognized field %d while getting int operand.\n"),
		       opindex);
      abort ();
  }

  return value;
}

bfd_vma
fr30_cgen_get_vma_operand (cd, opindex, fields)
     CGEN_CPU_DESC cd;
     int opindex;
     const CGEN_FIELDS * fields;
{
  bfd_vma value;

  switch (opindex)
    {
    case FR30_OPERAND_CRI :
      value = fields->f_CRi;
      break;
    case FR30_OPERAND_CRJ :
      value = fields->f_CRj;
      break;
    case FR30_OPERAND_R13 :
      value = 0;
      break;
    case FR30_OPERAND_R14 :
      value = 0;
      break;
    case FR30_OPERAND_R15 :
      value = 0;
      break;
    case FR30_OPERAND_RI :
      value = fields->f_Ri;
      break;
    case FR30_OPERAND_RIC :
      value = fields->f_Ric;
      break;
    case FR30_OPERAND_RJ :
      value = fields->f_Rj;
      break;
    case FR30_OPERAND_RJC :
      value = fields->f_Rjc;
      break;
    case FR30_OPERAND_RS1 :
      value = fields->f_Rs1;
      break;
    case FR30_OPERAND_RS2 :
      value = fields->f_Rs2;
      break;
    case FR30_OPERAND_CC :
      value = fields->f_cc;
      break;
    case FR30_OPERAND_CCC :
      value = fields->f_ccc;
      break;
    case FR30_OPERAND_DIR10 :
      value = fields->f_dir10;
      break;
    case FR30_OPERAND_DIR8 :
      value = fields->f_dir8;
      break;
    case FR30_OPERAND_DIR9 :
      value = fields->f_dir9;
      break;
    case FR30_OPERAND_DISP10 :
      value = fields->f_disp10;
      break;
    case FR30_OPERAND_DISP8 :
      value = fields->f_disp8;
      break;
    case FR30_OPERAND_DISP9 :
      value = fields->f_disp9;
      break;
    case FR30_OPERAND_I20 :
      value = fields->f_i20;
      break;
    case FR30_OPERAND_I32 :
      value = fields->f_i32;
      break;
    case FR30_OPERAND_I8 :
      value = fields->f_i8;
      break;
    case FR30_OPERAND_LABEL12 :
      value = fields->f_rel12;
      break;
    case FR30_OPERAND_LABEL9 :
      value = fields->f_rel9;
      break;
    case FR30_OPERAND_M4 :
      value = fields->f_m4;
      break;
    case FR30_OPERAND_PS :
      value = 0;
      break;
    case FR30_OPERAND_REGLIST_HI_LD :
      value = fields->f_reglist_hi_ld;
      break;
    case FR30_OPERAND_REGLIST_HI_ST :
      value = fields->f_reglist_hi_st;
      break;
    case FR30_OPERAND_REGLIST_LOW_LD :
      value = fields->f_reglist_low_ld;
      break;
    case FR30_OPERAND_REGLIST_LOW_ST :
      value = fields->f_reglist_low_st;
      break;
    case FR30_OPERAND_S10 :
      value = fields->f_s10;
      break;
    case FR30_OPERAND_U10 :
      value = fields->f_u10;
      break;
    case FR30_OPERAND_U4 :
      value = fields->f_u4;
      break;
    case FR30_OPERAND_U4C :
      value = fields->f_u4c;
      break;
    case FR30_OPERAND_U8 :
      value = fields->f_u8;
      break;
    case FR30_OPERAND_UDISP6 :
      value = fields->f_udisp6;
      break;

    default :
      /* xgettext:c-format */
      fprintf (stderr, _("Unrecognized field %d while getting vma operand.\n"),
		       opindex);
      abort ();
  }

  return value;
}

/* Stuffing values in cgen_fields is handled by a collection of functions.
   They are distinguished by the type of the VALUE argument they accept.
   TODO: floating point, inlining support, remove cases where argument type
   not appropriate.  */

void
fr30_cgen_set_int_operand (cd, opindex, fields, value)
     CGEN_CPU_DESC cd;
     int opindex;
     CGEN_FIELDS * fields;
     int value;
{
  switch (opindex)
    {
    case FR30_OPERAND_CRI :
      fields->f_CRi = value;
      break;
    case FR30_OPERAND_CRJ :
      fields->f_CRj = value;
      break;
    case FR30_OPERAND_R13 :
      break;
    case FR30_OPERAND_R14 :
      break;
    case FR30_OPERAND_R15 :
      break;
    case FR30_OPERAND_RI :
      fields->f_Ri = value;
      break;
    case FR30_OPERAND_RIC :
      fields->f_Ric = value;
      break;
    case FR30_OPERAND_RJ :
      fields->f_Rj = value;
      break;
    case FR30_OPERAND_RJC :
      fields->f_Rjc = value;
      break;
    case FR30_OPERAND_RS1 :
      fields->f_Rs1 = value;
      break;
    case FR30_OPERAND_RS2 :
      fields->f_Rs2 = value;
      break;
    case FR30_OPERAND_CC :
      fields->f_cc = value;
      break;
    case FR30_OPERAND_CCC :
      fields->f_ccc = value;
      break;
    case FR30_OPERAND_DIR10 :
      fields->f_dir10 = value;
      break;
    case FR30_OPERAND_DIR8 :
      fields->f_dir8 = value;
      break;
    case FR30_OPERAND_DIR9 :
      fields->f_dir9 = value;
      break;
    case FR30_OPERAND_DISP10 :
      fields->f_disp10 = value;
      break;
    case FR30_OPERAND_DISP8 :
      fields->f_disp8 = value;
      break;
    case FR30_OPERAND_DISP9 :
      fields->f_disp9 = value;
      break;
    case FR30_OPERAND_I20 :
      fields->f_i20 = value;
      break;
    case FR30_OPERAND_I32 :
      fields->f_i32 = value;
      break;
    case FR30_OPERAND_I8 :
      fields->f_i8 = value;
      break;
    case FR30_OPERAND_LABEL12 :
      fields->f_rel12 = value;
      break;
    case FR30_OPERAND_LABEL9 :
      fields->f_rel9 = value;
      break;
    case FR30_OPERAND_M4 :
      fields->f_m4 = value;
      break;
    case FR30_OPERAND_PS :
      break;
    case FR30_OPERAND_REGLIST_HI_LD :
      fields->f_reglist_hi_ld = value;
      break;
    case FR30_OPERAND_REGLIST_HI_ST :
      fields->f_reglist_hi_st = value;
      break;
    case FR30_OPERAND_REGLIST_LOW_LD :
      fields->f_reglist_low_ld = value;
      break;
    case FR30_OPERAND_REGLIST_LOW_ST :
      fields->f_reglist_low_st = value;
      break;
    case FR30_OPERAND_S10 :
      fields->f_s10 = value;
      break;
    case FR30_OPERAND_U10 :
      fields->f_u10 = value;
      break;
    case FR30_OPERAND_U4 :
      fields->f_u4 = value;
      break;
    case FR30_OPERAND_U4C :
      fields->f_u4c = value;
      break;
    case FR30_OPERAND_U8 :
      fields->f_u8 = value;
      break;
    case FR30_OPERAND_UDISP6 :
      fields->f_udisp6 = value;
      break;

    default :
      /* xgettext:c-format */
      fprintf (stderr, _("Unrecognized field %d while setting int operand.\n"),
		       opindex);
      abort ();
  }
}

void
fr30_cgen_set_vma_operand (cd, opindex, fields, value)
     CGEN_CPU_DESC cd;
     int opindex;
     CGEN_FIELDS * fields;
     bfd_vma value;
{
  switch (opindex)
    {
    case FR30_OPERAND_CRI :
      fields->f_CRi = value;
      break;
    case FR30_OPERAND_CRJ :
      fields->f_CRj = value;
      break;
    case FR30_OPERAND_R13 :
      break;
    case FR30_OPERAND_R14 :
      break;
    case FR30_OPERAND_R15 :
      break;
    case FR30_OPERAND_RI :
      fields->f_Ri = value;
      break;
    case FR30_OPERAND_RIC :
      fields->f_Ric = value;
      break;
    case FR30_OPERAND_RJ :
      fields->f_Rj = value;
      break;
    case FR30_OPERAND_RJC :
      fields->f_Rjc = value;
      break;
    case FR30_OPERAND_RS1 :
      fields->f_Rs1 = value;
      break;
    case FR30_OPERAND_RS2 :
      fields->f_Rs2 = value;
      break;
    case FR30_OPERAND_CC :
      fields->f_cc = value;
      break;
    case FR30_OPERAND_CCC :
      fields->f_ccc = value;
      break;
    case FR30_OPERAND_DIR10 :
      fields->f_dir10 = value;
      break;
    case FR30_OPERAND_DIR8 :
      fields->f_dir8 = value;
      break;
    case FR30_OPERAND_DIR9 :
      fields->f_dir9 = value;
      break;
    case FR30_OPERAND_DISP10 :
      fields->f_disp10 = value;
      break;
    case FR30_OPERAND_DISP8 :
      fields->f_disp8 = value;
      break;
    case FR30_OPERAND_DISP9 :
      fields->f_disp9 = value;
      break;
    case FR30_OPERAND_I20 :
      fields->f_i20 = value;
      break;
    case FR30_OPERAND_I32 :
      fields->f_i32 = value;
      break;
    case FR30_OPERAND_I8 :
      fields->f_i8 = value;
      break;
    case FR30_OPERAND_LABEL12 :
      fields->f_rel12 = value;
      break;
    case FR30_OPERAND_LABEL9 :
      fields->f_rel9 = value;
      break;
    case FR30_OPERAND_M4 :
      fields->f_m4 = value;
      break;
    case FR30_OPERAND_PS :
      break;
    case FR30_OPERAND_REGLIST_HI_LD :
      fields->f_reglist_hi_ld = value;
      break;
    case FR30_OPERAND_REGLIST_HI_ST :
      fields->f_reglist_hi_st = value;
      break;
    case FR30_OPERAND_REGLIST_LOW_LD :
      fields->f_reglist_low_ld = value;
      break;
    case FR30_OPERAND_REGLIST_LOW_ST :
      fields->f_reglist_low_st = value;
      break;
    case FR30_OPERAND_S10 :
      fields->f_s10 = value;
      break;
    case FR30_OPERAND_U10 :
      fields->f_u10 = value;
      break;
    case FR30_OPERAND_U4 :
      fields->f_u4 = value;
      break;
    case FR30_OPERAND_U4C :
      fields->f_u4c = value;
      break;
    case FR30_OPERAND_U8 :
      fields->f_u8 = value;
      break;
    case FR30_OPERAND_UDISP6 :
      fields->f_udisp6 = value;
      break;

    default :
      /* xgettext:c-format */
      fprintf (stderr, _("Unrecognized field %d while setting vma operand.\n"),
		       opindex);
      abort ();
  }
}

/* Function to call before using the instruction builder tables.  */

void
fr30_cgen_init_ibld_table (cd)
     CGEN_CPU_DESC cd;
{
  cd->insert_handlers = & fr30_cgen_insert_handlers[0];
  cd->extract_handlers = & fr30_cgen_extract_handlers[0];

  cd->insert_operand = fr30_cgen_insert_operand;
  cd->extract_operand = fr30_cgen_extract_operand;

  cd->get_int_operand = fr30_cgen_get_int_operand;
  cd->set_int_operand = fr30_cgen_set_int_operand;
  cd->get_vma_operand = fr30_cgen_get_vma_operand;
  cd->set_vma_operand = fr30_cgen_set_vma_operand;
}
