// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_MINTERMITER_HEADER
#define RAVL_MINTERMITER_HEADER 1
//////////////////////////////////////////////////////
//! rcsid="$Id$"
//! author="Charles Galambos"
//! docentry="Ravl.API.Logic.Condition"
//! lib=RavlLogic
//! file="Ravl/Logic/Base/MinTermIter.hh"

#include "Ravl/Logic/MinTerm.hh"

namespace RavlLogicN {

  //! userlevel=Normal
  //: Iterator though terms in a min term.
  
  class MinTermIterC {
  public:
    MinTermIterC()
    {}
    //: Default constructor.
    
    MinTermIterC(const MinTermC &mt);
    //: Constructor.
    
    bool First();
    //: Goto first element.
    // Returns false if there are no elements to iterate
    // through.
    
    const MinTermIterC &operator=(const MinTermC &mg);
    //: Assign to a minterm.
    
    bool IsElm() const
    { return it.IsElm(); }
    //: At a valid element ?
    
    operator bool() const
    { return IsElm(); }
    //: At a valid element ?
    
    bool Next();
    //: Goto next term.
  
    void operator++(int)
    { Next(); }
    //: Goto next term.
    
    LiteralC &Data()
    { return it.Data(); }
    //: Access current literal.
    
    const LiteralC &Data() const
    { return it.Data(); }
    //: Access current literal.

    LiteralC &operator *()
    { return it.Data(); }
    //: Access current literal.
    
    const LiteralC &operator *() const
    { return it.Data(); }
    //: Access current literal.
    
    bool IsNegated() const
    { return negated; }
    //: Test if this is a negated term.
    
  protected:
    bool negated;
    SArray1dIterC<LiteralC> it;
    SArray1dC<LiteralC> pos;
    SArray1dC<LiteralC> neg;
  };
  
}

#endif
