// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Base/StreamType.cc"

#define USE_STRSTREAM 1

#include "Ravl/StreamType.hh"
#include "Ravl/MTLocks.hh"


#if USE_STRSTREAM
#include "Ravl/StrStream.hh"
#endif

namespace RavlN { 
  
  StreamTypeC *StreamTypeC::list = 0; // List of registered stream types.
  
  //: Default constructor.
  
  StreamTypeC::StreamTypeC() {
    // Add into list.
    MTWriteLockC lock; // Lock list.
    next = list;
    list = this;
  }
  
  //: Destructor.
  
  // Simple remove from list ?
  
  StreamTypeC::~StreamTypeC() {
    MTWriteLockC lock; // Lock list.
    if(list == this) // Is this going to be easy ?
      list = next;
    else {
      StreamTypeC *place = list;
      for(;place != 0;) {
	if(place->next == this) {
	  place->next = next;
	  break;
	}
	place = place->next;
      }
    }
  }
  
  //: Get type of stream.
  
  const char *StreamTypeC::TypeName() { 
    IssueError(__FILE__,__LINE__,"Abstract method called 'StreamTypeC::TypeName()'.");
    return "Abstract"; 
  }
  
  //: Open input stream.
  
  IStreamC StreamTypeC::OpenI(const StringC &strm,bool binary,bool buffered) {
    IssueError(__FILE__,__LINE__,"Abstract method called 'StreamTypeC::OpenI()'.");
    return IStreamC();
  }
  
  //: Open ouput stream.
  
  OStreamC StreamTypeC::OpenO(const StringC &strm,bool binary,bool buffered)  {
    IssueError(__FILE__,__LINE__,"Abstract method called 'StreamTypeC::OpenO()'.");
    return OStreamC();
  }
  
  //: Find a stream of the named type.
  
  StreamTypeC *StreamTypeC::Find(const char *atypename) {
    StreamTypeC *place = list;
    StringC ant(atypename);
    MTReadLockC lock;
    for(;place != 0;place = place->next) {
      if(ant == place->TypeName())
	break;
    }
    return place;
  }
  
#if USE_STRSTREAM
  ///////////////// StrStream, so there's at least one ////////////////////////////////
  

  static class StreamType_StrStreamC 
    : public StreamTypeC
  {
  public:
    StreamType_StrStreamC()
      {}
    //: Default constructor.
    
    virtual const char *TypeName()
      { return "str"; }
    //: Get type of stream.
    
    virtual IStreamC OpenI(const StringC &strm,bool /*binary = false*/,bool /*buffered =true*/)
    { return StrIStreamC(strm); }
    //: Open input stream.
    
    virtual OStreamC OpenO(const StringC &/*strm*/,bool /*binary = false*/,bool /*buffered = true*/)
      { return StrOStreamC(); }
    //: Open ouput stream.
    // Not sure the point of this, but its implemented anyway.
    
  } Inst_StreamType_StrStream;
#endif  
}
