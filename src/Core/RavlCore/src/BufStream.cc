// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/System/BufStream.cc"

#include "Ravl/BufStream.hh"
#include "Ravl/config.h"
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


  class BufferStringBodyC 
    : public BufferBodyC<char>
  {
  public:
    BufferStringBodyC(std::string &nstr,UIntT size)
      : str(nstr)
    {
      RavlAssert(size <= str.size());
      Attach(SizeBufferAccessC<char>(const_cast<char *>(str.data()),size));
    }
    //: Constructor.
    
  protected:
    std::string str;
  };

  class BufferStringC 
    : public BufferC<char> 
  {
  public:
    BufferStringC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    BufferStringC(std::string &str,UIntT size)
      : BufferC<char>(*new BufferStringBodyC(str,size))
    {}
    //: Constructor.
  };
    
  ///////////////////
  //: Default constructor.
  
  BufOStreamC::BufOStreamC()
    : 
#if RAVL_HAVE_STRINGSTREAM
    OStreamC(*(oss = new std::ostringstream(ostringstream::out|ostringstream::binary)),true)
#else
    OStreamC(*(oss = new ostrstream(0,0,ios_base::app | ios_base::binary)),true)
#endif
  {}
  
#if RAVL_HAVE_STRINGSTREAM
  static bool CallUserTrigger(ostringstream *oss,TriggerC &trigger) {
    CallFunc1C<SArray1dC<char> > call(trigger);
    // Make sure there's a null terminator
#if RAVL_COMPILER_VISUALCPP
    // Make sure there's a null terminator
    char term = 0;
    oss->write(&term,1);
#endif
    if(call.IsValid()) {
      std::string astr = oss->str();
      SizeT size = astr.size();
      BufferStringC buf(astr,size);
      SArray1dC<char> data(buf,size);

      call.Call(data);
    } else
      trigger.Invoke();
    return true;
  }
#else
  static bool CallUserTrigger(ostrstream *oss,TriggerC &trigger) {
    CallFunc1C<SArray1dC<char> > call(trigger);
#if RAVL_COMPILER_VISUALCPP
    // Make sure there's a null terminator
    char term = 0;
    oss->write(&term,1);
#endif
    if(call.IsValid()) {
      std::string astr = oss->str();
      SizeT size = astr.size();
      BufferStringC buf(astr,size);
      SArray1dC<char> data(buf,size);

      call.Call(data);
    } else {
      trigger.Invoke();
    }
    return true;
  }
#endif


  //: Construct an output stream with a trigger to call upon its destruction.
  // If trigger is CallFunc1C<SArray1dC<char> > (returning a bool) or
  // one of its derived classes the first argument is set to the
  // contents of the stream.

  BufOStreamC::BufOStreamC(const TriggerC &sendto)
  :
#if RAVL_HAVE_STRINGSTREAM
  OStreamC(*(oss = new std::ostringstream(ostringstream::binary)),true)
#else
  OStreamC(*(oss = new ostrstream(0,0,ios_base::app | ios_base::binary)),true)
#endif
  {
    if(sendto.IsValid())
      AddDestructionOp(Trigger(&CallUserTrigger,oss,sendto));
  }

  ///////////////////
  //: Get text written to stream so far.
  // NB. This does NOT clean the buffer.
  
  SArray1dC<char> &BufOStreamC::Data() {
#if RAVL_HAVE_STRINGSTREAM
    string astr = oss->str();
    SizeT size = astr.size();
    BufferStringC buf(astr,size);
    data = SArray1dC<char>(buf,size);
#else

    // Fix horrible bug in irix implementation. 
    // pcount gets incremented when str() is called
#if RAVL_COMPILER_MIPSPRO
    char * astr = oss->str() ;  
    int    size = oss->pcount() ;
    data = SArray1dC<char> ( astr, --size ) ;
#else
    data = SArray1dC<char>((char *) oss->str(),static_cast<size_t>(oss->pcount()),true);
#endif // mips pro fix 
#endif
    return data;
  }
  
  ////////////////////////////////////////////////////
  
  //: Default constructor.
  
  BufIStreamC::BufIStreamC(const SArray1dC<char> &dat) 
    :
#if RAVL_COMPILER_VISUALCPP && !RAVL_HAVE_STRINGSTREAM
    IStreamC(*(iss = new istrstream(const_cast<char *>(dat.ReferenceElm()),dat.Size())),true),
#else
#if RAVL_HAVE_STRINGSTREAM
    IStreamC(*(iss = new std::istringstream(std::string(dat.ReferenceElm(),static_cast<size_t>(dat.Size())),istringstream::binary)),true),
#else
    IStreamC(*(iss = new istrstream(dat.ReferenceElm(),static_cast<size_t>(dat.Size()))),true),
#endif
#endif
      data(dat)
  {}
}
