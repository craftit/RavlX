// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLTEXTFRAGMENT_HEADER
#define RAVLTEXTFRAGMENT_HEADER 1
/////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/OS/Text/TextFragment.hh"
//! lib=RavlOS
//! docentry="Ravl.API.OS.Text Processing"
//! author="Charles Galambos"
//! date="23/02/2000"

#include "Ravl/Text/TextCursor.hh"

namespace RavlN {

  //! userlevel=Advanced
  //: Fragment of text in a buffer.
  
  class TextFragmentC {
  public:
    TextFragmentC()
      {}
    //: Default constructor.
    
    TextFragmentC(const TextCursorC &nstart,const TextCursorC &nend)
      : start(nstart),
      end(nend)
    {}
    //: Constructor.
  
    TextCursorC &Start()
      { return start; }
    //: Start of fragment.
    
    TextCursorC &End()
      { return end; }
    //: End of fragment.
    
    bool Append(const StringC &text);
    //: Append text to the end of the fragment.
    
    bool Remove();
    //: Remove this fragment from the buffer.
    
  protected:
    TextCursorC start;
    TextCursorC end;
  };
  

}
#endif
