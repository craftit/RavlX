// This file is part of CxxDoc, The RAVL C++ Documentation tool 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU General 
// Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlSourceTools
//! file="Ravl/SourceTools/CodeManager/ProgInfo.cc"

#include "Ravl/SourceTools/ProgInfo.hh"
#include "Ravl/SourceTools/DefsFile.hh"
#include "Ravl/StringList.hh"
#include "Ravl/OS/Filename.hh"

namespace RavlN {
  
  //: Constructor.
  ProgInfoBodyC::ProgInfoBodyC(const StringC &ln,DefsMkFileC &defs,const StringC &fromDir)   { 
    progSource = ln;
    progName = StringC(ln).before('.',-1); // Strip .cc or .c from progName.
    //cerr << "ProgInfoBodyC::ProgInfoBodyC() Source= "<< progSource << "\n";
    Add(defs,fromDir); 
  }
  
  //: Add information from defs file 'defs'.
  // True is returned if operation succeeded.
  
  bool ProgInfoBodyC::Add(DefsMkFileC &defs,const StringC &dir) {
    if(!defs.IsValid())
      return false;
    sources += dir + filenameSeperator + progSource;
    // We could check for duplications here, but there
    // really shouldn't be.
    if(defs["PLIB"].IsEmpty()) { // Are these just for the exe ?
      StringListC sl(defs["SOURCES"]);
      for(DLIterC<StringC> it(sl);it;it++) 
	sources += dir + filenameSeperator + *it;
      StringListC pl(defs["USESLIBS"]);
      {for(DLIterC<StringC> it(pl);it;it++) {
	if(!useslibs.Contains(*it))
	  useslibs += *it;
      }}
    } else
      useslibs += defs["PLIB"];
      
    // We need to check for repeated inclusions in USESLIBS.
    {
      StringListC pl(defs["PROGLIBS"]);
      for(DLIterC<StringC> it(pl);it;it++) {
	if(!useslibs.Contains(*it))
	  useslibs += *it;
      }
    }
    return true;
  }

}
