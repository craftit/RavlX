// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_STRSTREAM_HEADER
#define RAVL_STRSTREAM_HEADER 1
//////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Core/Base/StrStream.hh"
//! lib=RavlCore
//! author="Charles Galambos"
//! date="25/02/1999"
//! docentry="Ravl.API.Core.IO.Streams"
//! userlevel=Default

#include "Ravl/Stream.hh"
#include "Ravl/String.hh"

#if RAVL_HAVE_STREAMASCLASS
#if RAVL_HAVE_STDNAMESPACE
namespace std {
  class ostrstream;
  class istrstream;
}
#else
class ostrstream;
class istrstream;
#endif
#else

#if RAVL_HAVE_ANSICPPHEADERS
#if RAVL_HAVE_STRINGSTREAM
#include <sstream>
#else
#include <strstream>
#endif
#else
#if !RAVL_COMPILER_VISUALCPP
#include <strstream.h>
#else
#include <strstrea.h>
#endif
#endif

#endif

namespace RavlN {
  
  ////////////////////////////
  //! userlevel=Normal
  //: Output stream to memory.
  // Wraps the standard library std::ostrstream class.
  
  class StrOStreamC 
    : public OStreamC
  {
  public:
    StrOStreamC();
    //: Default constructor.
    // Creates an output string stream ready for use.
    
    StrOStreamC(const TriggerC &sendto);
    //: Construct an output stream with a trigger to call upon its destruction.
    // If trigger is CallFunc1C<const char *> (returning a bool) or
    // one of its derived classes the first argument is set to the
    // contents of the stream. 
    
    StringC String();
    //: Get text written to stream.
    // Note, this will reset the buffer. 
    
    UIntT Size() const;
    //: Get the number of bytes written so far.
  protected:

#if RAVL_HAVE_STRINGSTREAM
    std::ostringstream *oss; // Output string stream.
#else
    ostrstream *oss; // Output string stream.
#endif
  };
  
  ////////////////////////////
  //! userlevel=Normal
  //: Input stream from memory.
  // Wraps the standard library std::istrstream class.
  
  class StrIStreamC 
    : public IStreamC
  {
  public:
    StrIStreamC();
    //: Default constructor
    
    StrIStreamC(const StringC &dat);
    //: Constructor.
    
    inline StringC &String() { return buff; }
    //: Get string used as buffer.
    
  protected:
#if RAVL_HAVE_STRINGSTREAM
    std::istringstream *iss; // Output string stream.
#else
    istrstream *iss; // Output string stream.
#endif
    StringC buff;
  };
  
  //! Convert an object to a string.
  template<class DataT>
  RavlN::StringC StringOf(const DataT &data)
  {
    RavlN::StrOStreamC ss;
    ss << data;
    return ss.String();
  }
  
}




#endif
