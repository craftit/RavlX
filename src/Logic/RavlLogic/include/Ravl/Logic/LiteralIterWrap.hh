// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLLOGIC_LITERALITERWRAP_HEADER
#define RAVLLOGIC_LITERALITERWRAP_HEADER 1
///////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Logic"
//! lib=RavlLogic
//! file="Ravl/Logic/Base/LiteralIterWrap.hh"

#include "Ravl/Logic/LiteralIter.hh"
#include "Ravl/RCAbstract.hh"

namespace RavlLogicN {
  using namespace RavlN;
  
  //! userlevel=Develop
  //: Abstract iterator through a set of literals.
  
  template<class IterT>
  class LiteralIterWrapBodyC 
    : public LiteralIterBodyC 
  {
  public:
    LiteralIterWrapBodyC(const IterT &nit)
      : it(nit),
      binds(true)
      {}
    //: Default constructor.

    LiteralIterWrapBodyC(const IterT &nit,const BindSetC &nbs)
      : it(nit),
      binds(nbs)
      {}
    //: Default constructor.
    
    virtual bool Next()
      {  it.Next(); return it.IsElm(); }
    //: Goto next data element.
    // returns true if next element is valid.
    
    virtual bool First()
      { it.First(); return it.IsElm(); }
    //: Goto first data element.
    // returns true if next element is valid.
    
    virtual bool IsElm() const
    { return it.IsElm(); }
    //: At a valid element ?
    // returns true if iterator is at a valid
    // element.
    
    virtual LiteralC Data()
    { return it.Data(); }
    //: At a valid element.
    
    virtual RCAbstractC MapTo()
      { return RCAbstractC(); }
    //: Used for iterating mappings.
    // If no valid mapping exists, an invalid handle
    // will be returned
    
    IterT &Iter()
    { return it; }
    //: Access iterator.
    
    const IterT &Iter() const
    { return it; }
    //: Access iterator.

    virtual BindSetC &Binds()
      { return binds; }
    //: Access binds associated with solution.
    
    virtual const BindSetC &Binds() const
      { return binds; }
    //: Access binds associated with solution.

  protected:
    IterT it;
    BindSetC binds;
  };

  //! userlevel=Advanced
  //: Wrap a simple iterator with one derived from LiteralIterC.
  // This can intended to be used with class such as DLIterC, Array1dIterC.
  
  template<class IterT>
  class LiteralIterWrapC 
    : public LiteralIterC
  {
  public:
    LiteralIterWrapC(const IterT &nit)
      : LiteralIterC(*new LiteralIterWrapBodyC<IterT>(nit))
      {}
    //: Constructor.

    LiteralIterWrapC(const IterT &nit,const BindSetC &nbs)
      : LiteralIterC(*new LiteralIterWrapBodyC<IterT>(nit,nbs))
      {}
    //: Constructor.
    
  protected:
    LiteralIterWrapC(LiteralIterWrapBodyC<IterT> &bod)
      : LiteralIterC(bod)
      {}
    //: Body constructor.
    
    LiteralIterWrapBodyC<IterT> &Body()
      { return static_cast<LiteralIterWrapBodyC<IterT> &>(LiteralIterC::Body()); }
    //: Access body.

    const LiteralIterWrapBodyC<IterT> &Body() const
      { return static_cast<const LiteralIterWrapBodyC<IterT> &>(LiteralIterC::Body()); }
    //: Access body.
    
  public:

    IterT &Iter()
    { return Body().Iter(); }
    //: Access iterator.
    
    const IterT &Iter() const
    { return Body().Iter(); }
    //: Access iterator.
    
  };

  
}

#endif
