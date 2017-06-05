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

# include  "subprogram.h"
# include  "entity.h"
# include  "vtype.h"
# include  "sequential.h"
# include  "ivl_assert.h"
# include  "compiler.h"
# include  <cassert>

using namespace std;

Subprogram::Subprogram(perm_string nam, list<InterfacePort*>*ports,
		       const VType*return_type)
: name_(nam), parent_(0), ports_(ports), return_type_(return_type), statements_(0)
{
}

Subprogram::~Subprogram()
{
}

void Subprogram::set_parent(const ScopeBase*par)
{
      ivl_assert(*this, parent_ == 0);
      parent_ = par;
}

void Subprogram::set_program_body(list<SequentialStmt*>*stmt)
{
      ivl_assert(*this, statements_==0);
      statements_ = stmt;
}

bool Subprogram::unbounded() const {
    if(return_type_->is_unbounded())
       return true;

    if(ports_) {
        for(std::list<InterfacePort*>::const_iterator it = ports_->begin();
                it != ports_->end(); ++it) {
            if((*it)->type->is_unbounded())
                return true;
        }
    }

    return false;
}

bool Subprogram::compare_specification(Subprogram*that) const
{
      if (name_ != that->name_)
	    return false;

      if (return_type_==0) {
	    if (that->return_type_!=0)
		  return false;
      } else {
	    if (that->return_type_==0)
		  return false;

	    if (! return_type_->type_match(that->return_type_))
		  return false;
      }

      if (ports_==0) {
	    if (that->ports_!=0)
		  return false;

      } else {
	    if (that->ports_==0)
		  return false;

	    if (ports_->size() != that->ports_->size())
		  return false;
      }

      return true;
}

const InterfacePort*Subprogram::find_param(perm_string nam) const
{
      if(!ports_)
        return NULL;

      for (std::list<InterfacePort*>::const_iterator it = ports_->begin()
                ; it != ports_->end(); ++it) {
        if((*it)->name == nam)
            return *it;
      }

      return NULL;
}

const VType*Subprogram::peek_param_type(int idx) const
{
      if(!ports_ || idx < 0 || (size_t)idx >= ports_->size())
        return NULL;

      std::list<InterfacePort*>::const_iterator p = ports_->begin();
      std::advance(p, idx);

      return (*p)->type;
}

Subprogram*Subprogram::make_instance(std::vector<Expression*> arguments, ScopeBase*scope) {
    assert(arguments.size() == ports_->size());

    std::list<InterfacePort*>*ports = new std::list<InterfacePort*>;
    int i = 0;

    // Change the argument types to match the ones that were used during
    // the function call
    for(std::list<InterfacePort*>::iterator it = ports_->begin();
            it != ports_->end(); ++it) {
        InterfacePort*p = new InterfacePort(**it);
        p->type = arguments[i++]->peek_type()->clone();
        assert(p->type);
        ports->push_back(p);
    }

    char buf[80];
    snprintf(buf, sizeof(buf), "__%s_%p", name_.str(), ports);
    perm_string new_name = lex_strings.make(buf);
    Subprogram*instance = new Subprogram(new_name, ports, return_type_);

    // Copy variables
    for(std::map<perm_string,Variable*>::iterator it = new_variables_.begin();
            it != new_variables_.end(); ++it) {
        Variable*v = new Variable(it->first, it->second->peek_type()->clone());
        instance->new_variables_[it->first] = v;
    }

    instance->set_parent(scope);
    instance->set_program_body(statements_);
    instance->fix_return_type();
    scope->bind_subprogram(new_name, instance);

    return instance;
}

struct check_return_type : public SeqStmtVisitor {
    check_return_type(const Subprogram*subp) : subp_(subp), ret_type_(NULL) {}

    void operator() (SequentialStmt*s)
    {
        ReturnStmt*ret;
        if((ret = dynamic_cast<ReturnStmt*>(s))) {
            const Expression*expr = ret->peek_expr();
            const VType*t = NULL;

            if(const ExpName*n = dynamic_cast<const ExpName*>(expr)) {
                if(Variable*v = subp_->find_variable(n->peek_name()))
                    t = v->peek_type();
            } else {
                t = expr->peek_type();
            }

            if(!t) { // cannot determine the type at least in one case
                ret_type_ = NULL;
                return;
            }

            if(!ret_type_) { // this is first processed return statement
                ret_type_ = t;
            } else if(!t->type_match(ret_type_)) {
                // the function can return different types,
                // we cannot have fixed width
                ret_type_ = NULL;
                return;
            }
        }
    }

    const VType*get_type() const { return ret_type_; }

private:
    const Subprogram*subp_;
    const VType*ret_type_;
};

void Subprogram::fix_return_type()
{
    if(!statements_)
        return;

    check_return_type r(this);

    for (std::list<SequentialStmt*>::iterator s = statements_->begin()
        ; s != statements_->end(); ++s) {
        (*s)->visit(r);
    }

    VType*return_type = const_cast<VType*>(r.get_type());
    if(return_type && !return_type->is_unbounded()) {
        // Let's check if the variable length can be evaluated without any scope.
        // If not, then it is depends on information about e.g. function params
        if(return_type->is_variable_length(NULL)) {
            if(VTypeArray*arr = dynamic_cast<VTypeArray*>(return_type))
                arr->evaluate_ranges(this);
        }
        return_type_ = return_type;
    }
}

void Subprogram::write_to_stream(ostream&fd) const
{
      fd << "  function " << name_ << "(";
      if (ports_ && ! ports_->empty()) {
	    list<InterfacePort*>::const_iterator cur = ports_->begin();
	    InterfacePort*curp = *cur;
	    fd << curp->name << " : ";
	    curp->type->write_to_stream(fd);
	    for (++cur ; cur != ports_->end() ; ++cur) {
		  curp = *cur;
		  fd << "; " << curp->name << " : ";
		  curp->type->write_to_stream(fd);
	    }
      }
      fd << ") return ";
      return_type_->write_to_stream(fd);
      fd << ";" << endl;
}

void Subprogram::write_to_stream_body(ostream&fd) const
{
      fd << "function " << name_ << "(";
      if (ports_ && ! ports_->empty()) {
	    list<InterfacePort*>::const_iterator cur = ports_->begin();
	    InterfacePort*curp = *cur;
	    fd << curp->name << " : ";
	    curp->type->write_to_stream(fd);
	    for (++cur ; cur != ports_->end() ; ++cur) {
		  curp = *cur;
		  fd << "; " << curp->name << " : ";
		  curp->type->write_to_stream(fd);
	    }
      }
      fd << ") return ";
      return_type_->write_to_stream(fd);
      fd << " is" << endl;

      for (map<perm_string,Variable*>::const_iterator cur = new_variables_.begin()
         ; cur != new_variables_.end() ; ++cur) {
            cur->second->write_to_stream(fd);
      }

      fd << "begin" << endl;

      if (statements_) {
            for (list<SequentialStmt*>::const_iterator cur = statements_->begin()
                       ; cur != statements_->end() ; ++cur) {
                  (*cur)->write_to_stream(fd);
            }
      } else {
	    fd << "--empty body" << endl;
      }
	    fd << "end function;" << endl;
}
