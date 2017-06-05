#ifndef IVL_library_H
#define IVL_library_H
/*
 * Copyright (c) 2011-2014 Stephen Williams (steve@icarus.com)
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
 *    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

class Subprogram;

extern void library_set_work_path(const char*work_path);
extern void library_add_directory(const char*directory);

extern Subprogram*library_find_subprogram(perm_string name);

extern void emit_std_types(ostream&out);
extern int emit_packages(void);

#endif /* IVL_library_H */

