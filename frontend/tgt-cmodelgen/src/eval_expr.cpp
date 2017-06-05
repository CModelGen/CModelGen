/*
 * Copyright (c) 2001-2012 Stephen Williams (steve@icarus.com)
 *
 *    This source code is free software; you can redistribute it
 *    and/or modify it in source code form under the terms of the GNU
 *    General Public License as published by the Free Software
 *    Foundation; either version 2 of the License, or (at your option)
 *    any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

# include  "vvp_priv.h"
# include  <string.h>
# include  <stdlib.h>
# include  <assert.h>
# include  <stdbool.h>
# include  "ivl_alloc.h"

int number_is_unknown(ivl_expr_t expr)
{
      const char*bits;
      unsigned idx;

      if (ivl_expr_type(expr) == IVL_EX_ULONG)
        return 0;

      assert(ivl_expr_type(expr) == IVL_EX_NUMBER);

      bits = ivl_expr_bits(expr);
      for (idx = 0 ;  idx < ivl_expr_width(expr) ;  idx += 1)
        if ((bits[idx] != '0') && (bits[idx] != '1'))
          return 1;

      return 0;
}

/*
 * This function returns TRUE if the number can be represented as a
 * lim_wid immediate value. This amounts to verifying that any upper
 * bits are the same. For a negative value we do not support the most
 * negative twos-complement value since it can not be negated. This
 * code generator always emits positive values, hence the negation
 * requirement.
 */
int number_is_immediate(ivl_expr_t expr, unsigned lim_wid, int negative_ok_flag)
{
      const char *bits;
      unsigned nbits = ivl_expr_width(expr);
      char pad_bit = '0';
      unsigned idx;

    /* We can only convert numbers to an immediate value. */
      if (ivl_expr_type(expr) != IVL_EX_NUMBER
      && ivl_expr_type(expr) != IVL_EX_ULONG
      && ivl_expr_type(expr) != IVL_EX_DELAY)
        return 0;

    /* If a negative value is OK, then we really have one less
     * significant bit because of the sign bit. */
      if (negative_ok_flag) lim_wid -= 1;

    /* This is an unsigned value so it can not have the -2**N problem. */
      if (ivl_expr_type(expr) == IVL_EX_ULONG) {
        unsigned long imm;
        if (lim_wid >= 8*sizeof(unsigned long)) return 1;
          /* At this point we know that lim_wid is smaller than an
           * unsigned long variable. */
        imm = ivl_expr_uvalue(expr);
        if (imm < (1UL << lim_wid)) return 1;
        else return 0;
      }

    /* This is an unsigned value so it can not have the -2**N problem. */
      if (ivl_expr_type(expr) == IVL_EX_DELAY) {
        uint64_t imm;
        if (lim_wid >= 8*sizeof(uint64_t)) return 1;
          /* At this point we know that lim_wid is smaller than a
           * uint64_t variable. */
        imm = ivl_expr_delay_val(expr);
        if (imm < ((uint64_t)1 << lim_wid)) return 1;
        else return 0;
      }

      bits = ivl_expr_bits(expr);

      if (ivl_expr_signed(expr) && bits[nbits-1]=='1') pad_bit = '1';

      if (pad_bit == '1' && !negative_ok_flag) return 0;

      for (idx = lim_wid ;  idx < nbits ;  idx += 1)
        if (bits[idx] != pad_bit) return 0;

    /* If we have a negative number make sure it is not too big. */
      if (pad_bit == '1') {
        for (idx = 0; idx < lim_wid; idx += 1)
          if (bits[idx] == '1') return 1;
        return 0;
      }

      return 1;
}

/*
 * We can return positive or negative values. You must verify that the
 * number is not unknown (number_is_unknown) and is small enough
 * (number_is_immediate).
 */
long get_number_immediate(ivl_expr_t expr)
{
      long imm = 0;
      unsigned idx;

      switch (ivl_expr_type(expr)) {
      case IVL_EX_ULONG:
        imm = ivl_expr_uvalue(expr);
        break;

      case IVL_EX_NUMBER: {
        const char*bits = ivl_expr_bits(expr);
        unsigned nbits = ivl_expr_width(expr);
          /* We can not copy more bits than fit into a long. */
        if (nbits > 8*sizeof(long)) nbits = 8*sizeof(long);
        for (idx = 0 ; idx < nbits ; idx += 1) switch (bits[idx]){
            case '0':
              break;
            case '1':
              imm |= 1L << idx;
              break;
            default:
              assert(0);
        }
        if (ivl_expr_signed(expr) && bits[nbits-1]=='1' &&
            nbits < 8*sizeof(long)) imm |= -1L << nbits;
        break;
      }

      default:
        assert(0);
      }

      return imm;
}
