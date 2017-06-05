/*
 * Copyright (c) 2002 Stephen Williams (steve@icarus.com)
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
#ident "$Id: finish.c,v 1.2 2002/08/12 01:35:02 steve Exp $"
#endif

# include  <vpi_user.h>
# include  <veriuser.h>


/*
 * Implement tf_dofinish and tf_dostop using vpi functions.
 */
int tf_dofinish(void)
{
      vpi_control(vpiFinish, 0);
      return 0;
}

int tf_dostop(void)
{
      vpi_control(vpiStop, 0);
      return 0;
}

/*
 * $Log: finish.c,v $
 * Revision 1.2  2002/08/12 01:35:02  steve
 *  conditional ident string using autoconfig.
 *
 * Revision 1.1  2002/05/19 05:21:00  steve
 *  Start the libveriuser library.
 *
 */

