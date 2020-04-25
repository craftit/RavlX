// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlOS
//! file="Ravl/OS/Text/TextFragment.cc"

#include "Ravl/Text/TextFragment.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  
  //: Append text to the end of the fragment.
  
  bool TextFragmentC::Append(const StringC &text) {
    ONDEBUG(cerr << "TextFragmentC::Append(), Appending '" << text << "' at " << end.PositionTxt() << "\n");
    bool ret = end.Insert(text,true);
    ONDEBUG(cerr << "TextFragmentC::Append(), After " << end.PositionTxt() << " " << ret << "\n");
    return ret;
  }
  
  
  //: Remove this fragment from the buffer.
  
  bool TextFragmentC::Remove() {
    RavlAssert(0); // Not implemented
    return false;
  }
}
  


