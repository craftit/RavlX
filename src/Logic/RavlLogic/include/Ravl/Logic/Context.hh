// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLLOGIC_CONTEXT_HEADER
#define RAVLLOGIC_CONTEXT_HEADER 1
//! userlevel=Normal
//! author="Charles Galambos"
//! docentry="Ravl.API.Logic"
//! rcsid="$Id$"
//! lib=RavlLogic
//! file="Ravl/Logic/Base/Context.hh"

#include "Ravl/Logic/Literal.hh"
#include "Ravl/RCHash.hh"
#include "Ravl/Tuple2.hh"

namespace RavlLogicN {

  //! userlevel=Normal
  //: Comunication context.
  // This provides a method for indentifying common symbols over a comunication link.
  // This class should always be send before any other objects.
  
  class ContextC {
  public:
    ContextC(const char *name = "RavlLogicN");
    //: Default constructor.
    
    bool Register(const StringC &name,const LiteralC &lit,bool builtinLit = false);
    //: Register literal.
    
    bool Send(BinOStreamC &strm);
    //: Send context information.
    
    bool Receive(BinIStreamC &strm);
    //: Recieve context information.
    
    bool Lookup(const StringC &name,LiteralC &lit) {
      Tuple2C<LiteralC,bool> x;
      if(!map.Lookup(name,x))
	return false;
      lit = x.Data1();
      return true;
    }
    //: Lookup literal.
    
  protected:
    StringC name;
    RCHashC<StringC,Tuple2C<LiteralC,bool> > map;
  };
  
  BinIStreamC &operator>>(BinIStreamC &strm,ContextC &obj);
  //: Load from a binary stream.
  
  BinOStreamC &operator<<(BinOStreamC &out,ContextC &obj);
  //: Save to a binary stream.
  
}



#endif
