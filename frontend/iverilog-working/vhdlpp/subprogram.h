#ifndef IVL_subprogram_H
#define IVL_subprogram_H
/*
 * Copyright (c) 2013-2014 Stephen Williams (steve@icarus.com)
 * Copyright CERN 2013 / Stephen Williams (steve@icarus.com)
 * Copyright CERN 2015
 * @author Maciej Suminski (maciej.suminski@cern.ch)
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

# include  "StringHeap.h"
# include  "LineInfo.h"
# include  "scope.h"
# include  <iostream>
# include  <list>

class InterfacePort;
class SequentialStmt;
class VType;

class Subprogram : public LineInfo, public ScopeBase {

    public:
      Subprogram(perm_string name, std::list<InterfacePort*>*ports,
		 const VType*return_type);
      ~Subprogram();

      void set_parent(const ScopeBase*par);
      inline const ScopeBase*get_parent() const { return parent_; }

      inline const perm_string&name() const { return name_; }

      void set_program_body(std::list<SequentialStmt*>*statements);
      inline bool empty_program_body() const { return !statements_ || statements_->empty(); }

	// Return true if the specification (name, types, ports)
	// matches this subprogram and that subprogram.
      bool compare_specification(Subprogram*that) const;

      const InterfacePort*find_param(perm_string nam) const;
      const VType*peek_param_type(int idx) const;
      const VType*peek_return_type() const { return return_type_; }

      int emit(ostream&out, Entity*ent, ScopeBase*scope);

	// Emit a definition as it would show up in a package.
      int emit_package(std::ostream&fd) const;

      void write_to_stream(std::ostream&fd) const;
      void write_to_stream_body(std::ostream&fd) const;
      void dump(std::ostream&fd) const;

	// Creates a new instance of the function that takes arguments of
	// a different type. It is used to allow VHDL functions that work with
	// unbounded std_logic_vectors, so there can be a separate instance
	// for limited length logic vector.
      Subprogram*make_instance(std::vector<Expression*> arguments, ScopeBase*scope);

	// Checks if either return type or parameters are unbounded vectors.
      bool unbounded() const;

    private:
	// Tries to set the return type to a fixed type. VHDL functions that
	// return std_logic_vectors do not specify its length, as SystemVerilog
	// demands.
      void fix_return_type();

      perm_string name_;
      const ScopeBase*parent_;
      std::list<InterfacePort*>*ports_;
      const VType*return_type_;
      std::list<SequentialStmt*>*statements_;
};

#endif /* IVL_subprogram_H */
