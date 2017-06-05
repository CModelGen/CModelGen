/*
 * Copyright (c) 2011 Stephen Williams (steve@icarus.com)
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

# define __STDC_LIMIT_MACROS
# include  "vtype.h"
# include  "expression.h"
# include  <typeinfo>
# include  <stdint.h>
# include  <cassert>

using namespace std;

void VType::write_to_stream(ostream&fd) const
{
      fd << "/* UNKNOWN TYPE: " << typeid(*this).name() << " */";
}

void VType::write_type_to_stream(ostream&fd) const
{
      write_to_stream(fd);
}

void VTypeArray::write_to_stream(ostream&fd) const
{
	// Special cases: std_logic_vector & string
      if (etype_ == &primitive_STDLOGIC) {
	    fd << "std_logic_vector";
	    if (!ranges_.empty() && !ranges_[0].is_box()) {
		  write_range_to_stream_(fd);
	    }
	    return;
      } else if (etype_ == &primitive_CHARACTER) {
	    fd << "string";
	    if (!ranges_.empty() && !ranges_[0].is_box()) {
		  write_range_to_stream_(fd);
	    }
	    return;
      }

      bool typedefed = false;
      if(const VTypeDef*tdef = dynamic_cast<const VTypeDef*>(etype_)) {
            tdef->write_to_stream(fd);
            typedefed = true;
      } else {
            fd << "array ";
      }

      if (! ranges_.empty()) {
	    assert(ranges_.size() < 2);
	    if (ranges_[0].is_box()) {
		  fd << "(INTEGER range <>) ";
	    } else {
		  write_range_to_stream_(fd);
	    }
      }

      if(!typedefed) {
            fd << "of ";
            etype_->write_to_stream(fd);
      }
}

void VTypeArray::write_range_to_stream_(std::ostream&fd) const
{
    assert(ranges_.size() < 2);
    assert(ranges_[0].msb() && ranges_[0].lsb());

    fd << "(";
    if (ranges_[0].msb())
        ranges_[0].msb()->write_to_stream(fd);
    else
        fd << "<>";

    fd << (ranges_[0].is_downto() ? " downto " : " to ");

    if (ranges_[0].lsb())
        ranges_[0].lsb()->write_to_stream(fd);
    else
        fd << "<>";
    fd << ") ";
}

void VTypeArray::write_type_to_stream(ostream&fd) const
{
	// Special case: std_logic_vector
      if (etype_ == &primitive_STDLOGIC) {
	    fd << "std_logic_vector";
	    if (! ranges_.empty() && ! ranges_[0].is_box()) {
		  write_range_to_stream_(fd);
	    }
	    return;
      }
      else if (etype_ == &primitive_CHARACTER) {
	    fd << "string";
	    if (! ranges_.empty() && ! ranges_[0].is_box()) {
		  write_range_to_stream_(fd);
	    }
	    return;
      }

      fd << "array ";

      if (! ranges_.empty()) {
	    assert(ranges_.size() < 2);
	    if (ranges_[0].is_box()) {
		  fd << "(INTEGER range <>) ";
	    } else {
		  write_range_to_stream_(fd);
	    }
      }

      fd << "of ";

      if(const VTypeDef*tdef = dynamic_cast<const VTypeDef*>(etype_)) {
          tdef->write_to_stream(fd);
      } else {
          etype_->write_to_stream(fd);
      }
}

void VTypeDef::write_type_to_stream(ostream&fd) const
{
      type_->write_type_to_stream(fd);
}

void VTypeDef::write_to_stream(ostream&fd) const
{
      fd << name_;
}

void VTypePrimitive::write_to_stream(ostream&fd) const
{
      switch (type_) {
	  case BIT:
	    fd << "bit";
	    break;
	  case INTEGER:
	    fd << "integer";
	    break;
	  case NATURAL:
	    fd << "natural";
	    break;
	  case REAL:
	    fd << "real";
	    break;
	  case STDLOGIC:
	    fd << "std_logic";
	    break;
	  case CHARACTER:
	    fd << "character";
	    break;
	  case BOOLEAN:
	    fd << "boolean";
	    break;
	  default:
	    assert(0);
	    fd << "/* PRIMITIVE: " << type_ << " */";
	    break;
      }
}

void VTypeRange::write_to_stream(ostream&fd) const
{
	// Detect some special cases that can be written as ieee or
	// standard types.
      if (const VTypePrimitive*tmp = dynamic_cast<const VTypePrimitive*> (base_)) {
	    if (start_==0 && end_==INT64_MAX && tmp->type()==VTypePrimitive::INTEGER) {
		  fd << "natural";
		  return;
	    }
      }

      base_->write_to_stream(fd);
      fd << " range " << start_;
      fd << (start_ < end_ ? " to " : " downto ");
      fd << end_;
}

void VTypeRecord::write_to_stream(ostream&fd) const
{
      fd << "record ";
      for (size_t idx = 0 ; idx < elements_.size() ; idx += 1) {
	    elements_[idx]->write_to_stream(fd);
	    fd << "; ";
      }
      fd << "end record";
}

void VTypeRecord::element_t::write_to_stream(ostream&fd) const
{
      fd << name_ << ": ";
      type_->write_to_stream(fd);
}

void VTypeEnum::write_to_stream(std::ostream&fd) const
{
      fd << "(";
      for (vector<perm_string>::const_iterator it = names_.begin();
        it != names_.end(); ++it) {
            if(it != names_.begin())
                fd << ",";

            fd << *it;

      }
      fd << ")";
}

