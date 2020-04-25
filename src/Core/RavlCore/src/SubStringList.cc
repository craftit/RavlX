// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/System/SubStringList.cc"

#include <string.h>
#include "Ravl/SubStringList.hh"
#include "Ravl/CDLIter.hh"

namespace RavlN {
  
  SubStringListC::SubStringListC (const SubStringC &csstring, const char* rdelim) {
    SubStringC &string = const_cast<SubStringC &>(csstring);
    if(string.length() < 1)
      return ;
    RavlAssert(rdelim != 0);
    bool delim[256];
    for(int i = 0;i < 256;i++)
      delim[i] = false;
    const char *place,*eos,*lstart = 0;
    // Fill out delim table.
    for(place = rdelim;*place != 0;place++)
      delim[(int) *place] = true;
    // Begin search of string.
    eos = &string.chars()[string.length()];
    for(place = string.chars();place != eos;) {
      // Skip spaces.
      if(delim[(int) *place]) {
	place++;
	continue;
      }
      // Found string.
      lstart = place;
      SizeT len = 0;
      for(;place != eos;place++) {
	if(delim[(int) *place])
	  break;
	len++;
      }
      //cerr << "Str: " << len << " = " << lstart << std::endl;
      // At end of string.
      int startpos = lstart - string.chars();
      InsLast(string.at(startpos,len));
    }
    
  }
  
  StringC SubStringListC::Cat (const char *sep) const {
    StringC ret;
    ConstDLIterC<SubStringC> i(*this);
    if(!i.IsElm())
      return ret;
    ret += i.Data();
    i.Next();
    for (; i.IsElm(); i.Next())
      ret += sep + i.Data();
    return ret;
  }
  
  std::ostream & operator<<(std::ostream & s, const SubStringListC & list) {
    s << list.Cat();
    return s;
  }
  
}
