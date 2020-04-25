// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLLOGIC_LITERALFILTERITER_HEADER
#define RAVLLOGIC_LITERALFILTERITER_HEADER 1
///////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlLogic
//! docentry="Ravl.API.Logic"
//! author="Charles Galambos"
//! file="Ravl/Logic/Base/LiteralFilterIter.hh"

#include "Ravl/Logic/LiteralIter.hh"
#include "Ravl/Logic/BindSet.hh"

namespace RavlLogicN {

  //! userlevel=Develop
  //: Filter the results of another iterator.
  
  class LiteralFilterIterBodyC
    : public  LiteralIterBodyC
  {
  public:
    LiteralFilterIterBodyC(const LiteralIterC &iter,const LiteralC &nfilter);
    //: Constructor.
    
    ~LiteralFilterIterBodyC();
    //: Destructor.
    
    virtual bool Next();
    //: Goto next data element.
    // returns true if next element is valid.
    
    virtual bool First();
    //: Goto first data element.
    // returns true if next element is valid.
    
    virtual bool IsElm() const;
    //: At a valid element ?
    // returns true if iterator is at a valid
    // element.
    
    virtual LiteralC Data();
    //: Get current literal.
    
    virtual BindSetC &Binds();
    //: Access binds associated with solution.
    
    virtual const BindSetC &Binds() const;
    //: Access binds associated with solution.
    
  protected:
    bool NextValid();
    //: Goto next valid solution.
    
    BindSetC binds;
    BindMarkT mark;
    LiteralIterC iter;
    LiteralC filter;
  };

  //! userlevel=Normal
  //: Filter the results of another iterator.
  
  class LiteralFilterIterC
    : public LiteralIterC
  {
  public:
    LiteralFilterIterC()
      {}
    //: Default constructor.

    LiteralFilterIterC(const LiteralIterC &iter,const LiteralC &nfilter)
      : LiteralIterC(*new LiteralFilterIterBodyC(iter,nfilter))
      {}
    //: Default constructor.
    
  protected:
  };
  
}

#endif
