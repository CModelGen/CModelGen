/*
 * Copyright (c) 2002 Michael Ruff (mruff at chiaro.com)
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
#ifdef HAVE_CVS_IDENT
#ident "$Id: a_close.c,v 1.4 2003/06/17 16:55:07 steve Exp $"
#endif

# include  <acc_user.h>
# include  "priv.h"

void acc_close(void)
{
      if (pli_trace) {
	    fprintf(pli_trace, "acc_close()\n");
      }
}

/*
 * $Log: a_close.c,v $
 * Revision 1.4  2003/06/17 16:55:07  steve
 *  1) setlinebuf() for vpi_trace
 *  2) Addes error checks for trace file opens
 *  3) removes now extraneous flushes
 *  4) fixes acc_next() bug
 *
 * Revision 1.3  2003/05/18 00:16:35  steve
 *  Add PLI_TRACE tracing of PLI1 modules.
 *
 *  Add tf_isetdelay and friends, and add
 *  callback return values for acc_vcl support.
 *
 * Revision 1.2  2002/08/12 01:35:02  steve
 *  conditional ident string using autoconfig.
 *
 * Revision 1.1  2002/05/23 03:46:42  steve
 *  Add the acc_user.h header file.
 *
 */
