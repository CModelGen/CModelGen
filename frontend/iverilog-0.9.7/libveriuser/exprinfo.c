/*
 * Copyright (c) 2003 Stephen Williams (steve@icarus.com)
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
#ident "$Id: exprinfo.c,v 1.1 2003/10/10 02:57:46 steve Exp $"
#endif

# include  <assert.h>
# include  <ctype.h>
# include  <veriuser.h>
# include  <vpi_user.h>
# include  "priv.h"

struct t_tfexprinfo* tf_exprinfo(PLI_INT32 a, struct t_tfexprinfo*ip)
{
      if (pli_trace) {
	    fprintf(pli_trace, "tf_exprinfo: enter.\n");
	    fflush(pli_trace);
      }

      fprintf(stderr, "tf_exprinfo: XXXX not implemented. XXXX\n");

      if (pli_trace) {
	    fprintf(pli_trace, "tf_exprinfo: return.\n");
	    fflush(pli_trace);
      }
      return 0;
}



/*
 * $Log: exprinfo.c,v $
 * Revision 1.1  2003/10/10 02:57:46  steve
 *  Some PLI1 stubs.
 *
 */

