// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Base/StrStream.cc"

#include "Ravl/StrStream.hh"
#include "Ravl/Calls.hh"

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

namespace RavlN {
  
  ///////////////////
  //: Default constructor.

  StrOStreamC::StrOStreamC()
    :
#if RAVL_HAVE_STRINGSTREAM
  OStreamC(*(oss = new std::ostringstream(std::ostringstream::binary)),true)
#else
    OStreamC(*(oss = new ostrstream()),true)
#endif
  {}
  

#if RAVL_HAVE_STRINGSTREAM
  static bool CallUserTrigger(ostringstream *oss,TriggerC &trigger) {
    CallFunc1C<const char *> call(trigger);
    // Make sure there's a null terminator
#if RAVL_COMPILER_VISUALCPP
    // Make sure there's a null terminator
    char term = 0;
    oss->write(&term,1);
#endif
    string str = oss->str();
    const char *data = str.data(); 
    if(call.IsValid())
      call.Call(data);
    else
      trigger.Invoke();
    return true;
  }
#else
  static bool CallUserTrigger(ostrstream *oss,TriggerC &trigger) {
    CallFunc1C<const char *> call(trigger);
#if RAVL_COMPILER_VISUALCPP
    // Make sure there's a null terminator
    char term = 0;
    oss->write(&term,1);
#endif
    const char *data = &(oss->str()[0]);  
    if(call.IsValid())
      call.Call(data);
    else
      trigger.Invoke();
    return true;
  }
#endif  
  //: Construct an output stream with a trigger to call upon its destruction.
  // If trigger is CallFunc1C<char *> or
  // one of its derived classes the first argument is set to the
  // contents of the stream. 
  
  StrOStreamC::StrOStreamC(const TriggerC &sendto) 
    : 
#if RAVL_HAVE_STRINGSTREAM
    OStreamC(*(oss = new ostringstream()),true)
#else
    OStreamC(*(oss = new ostrstream()),true)
#endif
  {
    if(sendto.IsValid())
      AddDestructionOp(Trigger(&CallUserTrigger,oss,sendto)); 
  }
  
  
  //: Get the number of bytes written so far.
  
  UIntT StrOStreamC::Size() const {
#if RAVL_HAVE_STRINGSTREAM
    return oss->str().size();
#else
	// FIXME:- Should really return a SizeT
    return static_cast<UIntT>(oss->pcount());
#endif
  }
  
  ///////////////////
  //: Get text written to stream so far.
  // NB. This does NOT clean the buffer.
  
  StringC StrOStreamC::String() {
    UIntT count = Size();
#if RAVL_HAVE_STRINGSTREAM
    string str = oss->str();
    const char *data = str.data(); 
#else
    const char *data = &(oss->str()[0]); 
#endif
    // This is a bit ugly, we have to copy the 
    // string into another buffer.
    StringC ret(data,count,count);
    return ret;
  }
  
  ////////////////////////////////////////////////////

#if !RAVL_HAVE_STRINGSTREAM
  static bool RemoveBuffer(const StringC &buff){
    return true;
  }
#endif

  //: Default constructor
  
  StrIStreamC::StrIStreamC()
    : iss(0)
  {}

  //: Default constructor.
  
  StrIStreamC::StrIStreamC(const StringC &dat)
    :
#if RAVL_HAVE_STRINGSTREAM
    IStreamC(*(iss = new std::istringstream(std::string(dat.chars(),dat.length()),std::istringstream::binary)),true),
#else
#if RAVL_COMPILER_VISUALCPP
    IStreamC(*(iss = new istrstream(const_cast<char *>(dat.chars()),dat.length())),true),
#else
    IStreamC(*(iss = new istrstream(dat.chars(),dat.length())),true),
#endif
#endif
    buff(dat)
  {
#if !RAVL_HAVE_STRINGSTREAM
    // This will ensure a handle is held to 'dat' until we're finished with it.
    // its not completely clear if this is needed, but better to be safe.
    AddDestructionOp(Trigger(&RemoveBuffer,dat));
#endif
  }
}

