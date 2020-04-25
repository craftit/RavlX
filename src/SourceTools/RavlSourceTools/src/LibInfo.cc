// This file is part of CxxDoc, The RAVL C++ Documentation tool 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU General 
// Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
///////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlSourceTools
//! file="Ravl/SourceTools/CodeManager/LibInfo.cc"

#include "Ravl/SourceTools/LibInfo.hh"
#include "Ravl/SourceTools/DefsFile.hh"
#include "Ravl/StringList.hh"
#include "Ravl/OS/Filename.hh"

namespace RavlN {
  
  //: Add information from defs file 'defs'.
  // True is returned if operation succeeded.
  
  bool LibInfoBodyC::Add(DefsMkFileC &defs,const StringC &dir) {
    if(!defs.IsValid())
      return false;
    if(defs["PLIB"] != libName) {
      return false;
    }
    // We could check for duplications here, but there
    // really shouldn't be.
    {
      StringListC sl(defs["SOURCES"]);
      for(DLIterC<StringC> it(sl);it;it++) 
	sources += dir + filenameSeperator + *it;
    }
    {
      StringListC hl(defs["HEADERS"]);
      StringC package = defs["PACKAGE"];
      for(DLIterC<StringC> it(hl);it;it++) 
	headers += HeaderInfoC(*it,package,dir + filenameSeperator + *it);
    }
    // We need to check for repeated inclusions in USESLIBS.
    {
      StringListC ul(defs["USESLIBS"]);
      for(DLIterC<StringC> it(ul);it;it++) {
	if(*it == libName)
	  continue; // Don't make a circular dependancy.
	if(!useslibs.Contains(*it))
	  useslibs += *it;
      }
    }
    {
      StringListC ul(defs["AUXFILES"]);
      for(DLIterC<StringC> it(ul);it;it++) {
	if(!auxfiles.Contains(*it))
	  auxfiles += dir + filenameSeperator + *it;
      }
      auxdir = defs["AUXDIR"];
    }
    if(defs.IsDefined("MUSTLINK")) {
      mustLink = dir + filenameSeperator + defs["MUSTLINK"];
    } else
      mustLink = StringC();
    return true;
  }
 
}
