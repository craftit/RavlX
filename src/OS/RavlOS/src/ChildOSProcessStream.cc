// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlOS
//! file="Ravl/OS/Exec/ChildOSProcessStream.cc"

#include "Ravl/OS/ChildOSProcess.hh"
#include "Ravl/StreamType.hh"

namespace RavlN {
  
  void StreamType_ChildProc_MustLink()
    {}
  
  //: This allows streams to be used like the 'C' popen command.
  // Syntax  @exec:[command]
  
  static class StreamType_ChildProcC 
    : public StreamTypeC
  {
  public:
    StreamType_ChildProcC()
      {}
    //: Default constructor.
    
    virtual const char *TypeName()
      { return "exec"; }
    //: Get type of stream.
    
    virtual IStreamC OpenI(const StringC &strm,bool /*binary = false*/,bool /*buffered =true*/) { 
	ChildOSProcessC aproc(strm,(bool) true,false,false);
	return aproc.StdOut();
      }
    //: Open input stream.
    
    virtual OStreamC OpenO(const StringC &strm,bool /*binary = false*/,bool /*buffered = true*/) { 
	ChildOSProcessC aproc(strm,(bool) false,false,true);
	return aproc.StdIn();
      }
    //: Open ouput stream.
    
  } Inst_StreamType_ChildProc;

}


