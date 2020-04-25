// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_STREAMTYPE_HEADER
#define RAVL_STREAMTYPE_HEADER 1
////////////////////////////////////////////////////
//! lib=RavlCore
//! docentry="Ravl.API.Core.IO.Streams"
//! userlevel=Develop
//! author="Charles Galambos"
//! date="15/02/2000"
//! file="Ravl/Core/Base/StreamType.hh"
//! rcsid="$Id$"

#include "Ravl/Stream.hh"

namespace RavlN {
  
  class IStreamC;
  class OStreamC;
  
  //: Type of stream.
  // used for implementing network pipes and exec pipes.
  
  class StreamTypeC  {
  public:
    StreamTypeC();
    //: Default constructor.
    
    virtual ~StreamTypeC();
    //: Destructor.
    
    virtual const char *TypeName();
    //: Get type of stream.
    
    virtual IStreamC OpenI(const StringC &strm,bool binary = false,bool buffered = true);
    //: Open input stream.
    
    virtual OStreamC OpenO(const StringC &strm,bool binary = false,bool buffered = true);
    //: Open ouput stream.
    
    static StreamTypeC *Find(const char *atypename);
    //: Find a stream of the named type.
    // Returns 0 if not found.
    
  protected:
    static StreamTypeC *list; // List of registered stream types.
    StreamTypeC *next;        // Next stream type in list.
  };
  
}

#endif
