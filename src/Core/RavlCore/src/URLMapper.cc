// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Base/URLMapper.cc"

#include "Ravl/Stream.hh"
#include "Ravl/StreamType.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  
  extern URLMapperFuncT urlMapper;
  
  //: Map URL's to RAVL special filenames.
  // With the expection of 'file:' specifications this just changes URL's 
  // to a RAVL special file. This allows us to implement handlers in seperate
  // libraries which can link in as required.
  
  static StringC DefaultURLMapper(const StringC &fn) {
    ONDEBUG(cerr << "DefaultURLMapper(), Called on '" << fn << "'\n");
    if(fn.length() < 1)
      return StringC(fn);
    if(fn[0] == '@') // Is this a RAVL special filename ?
      return StringC(fn);
    int sep = fn.index(':');
    if(sep < 0)
      return StringC(fn); // No seperator, take as normal filename.
    
#if RAVL_OS_WIN32
    // Look out for driver letters.
    if(sep == 1) {
      if((fn[0] >= 'A' && fn[0] <= 'Z') ||
         (fn[0] >= 'a' && fn[0] <= 'z')) {
        return StringC(fn);
      }
    }
#endif
    
    SubStringC urltype = StringC(fn).before(sep);
    if(urltype == "file") { // Got a file descriptor.
      StringC theRest = StringC(fn).after(sep);
      theRest.gsub("%20"," "); // Hack to handle spaces...
      ONDEBUG(cerr << "DefaultURLMapper(), Filename= '" << theRest << "'\n");
      if(theRest.length() < 2 || theRest[0] != '/')
	return StringC(theRest);
      if(theRest[1] == '/' && theRest[0] == '/')
	return theRest.after(1);
      // Fall through and use 'StreamTypeC' to handle..
    }
    if(StreamTypeC::Find(urltype.chars()) != 0)
      return StringC("@") + urltype + StringC(fn).from(sep); // Turn it into a RAVL stream type.
    std::cerr << "WARNING: Unsupported URL type '" << urltype << "'. \n";
    return StringC(fn);
  }
  
  //: Enable url mapping of filenames.
  
  bool EnableURLMapper() {
    urlMapper = &DefaultURLMapper;
    return true;
  }
  
  //: Dissable url mapping of filenames.
  
  bool DisableURLMapper() {
    urlMapper = 0;
    return true;
  }
  
}
