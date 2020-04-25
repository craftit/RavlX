// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_BUFSTREAM_HEADER
#define RAVL_BUFSTREAM_HEADER 1
//////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Core/System/BufStream.hh"
//! lib=RavlCore
//! author="Charles Galambos"
//! date="25/02/1999"
//! docentry="Ravl.API.Core.IO.Streams"

#include "Ravl/Stream.hh"
#include "Ravl/SArray1d.hh"

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
  // Wraps the standard library ostringstream class.
  
  class BufOStreamC 
    : public OStreamC
  {
  public:
    BufOStreamC();
    //: Default constructor.

    BufOStreamC(const TriggerC &sendto);
    //: Construct an output stream with a trigger to call upon its destruction.
    // If trigger is CallFunc1C<SArray1dC<char> > (returning a bool) or
    // one of its derived classes the first argument is set to the
    // contents of the stream.

    SArray1dC<char> &Data();
    //: Get data written to stream. 
    // Note, this will reset the buffer.

  protected:
    SArray1dC<char> data;
#if RAVL_HAVE_STRINGSTREAM
    std::ostringstream *oss; // Output string stream.
#else
    ostrstream *oss; // Output string stream.
#endif
  };
  
  ////////////////////////////
  //! userlevel=Normal
  //: Input stream from memory.
  // Wraps the standard library std::istringstream class.
  
  class BufIStreamC 
    : public IStreamC
  {
  public:
    BufIStreamC()
      : iss(0)
      {}
    //: Default constructor
    
    BufIStreamC(const SArray1dC<char> &dat);
    //: Constructor.
    
    inline SArray1dC<char> &Data() { return data; }
    //: Get array used as buffer.

    inline const SArray1dC<char> &Data() const { return data; }
    //: Get array used as buffer.

  protected:
    SArray1dC<char> data;
#if RAVL_HAVE_STRINGSTREAM
    std::istringstream *iss; // Output string stream.
#else
    istrstream *iss; // Output string stream.
#endif
  };
  

}

#endif
