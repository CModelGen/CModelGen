/*
 * Copyright (c) 1999-2008 Stephen Williams (steve@icarus.com)
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

# include "config.h"

#include "PTask.h"

PFunction::PFunction(perm_string name, PScope*parent, bool is_auto__)
: PScope(name, parent), ports_(0), statement_(0)
{
      is_auto_ = is_auto__;
      return_type_.type = PTF_NONE;
}

PFunction::~PFunction()
{
}

void PFunction::set_ports(svector<PWire *>*p)
{
      assert(ports_ == 0);
      ports_ = p;
}

void PFunction::set_statement(Statement*s)
{
      assert(s != 0);
      assert(statement_ == 0);
      statement_ = s;
}

void PFunction::set_return(PTaskFuncArg t)
{
      return_type_ = t;
}
