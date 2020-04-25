// This file is part of CxxDoc, The RAVL C++ Documentation tool 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU General 
// Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlSourceTools
//! file="Ravl/SourceTools/CodeManager/BuildInfo.cc"


#include "Ravl/SourceTools/BuildInfo.hh"

namespace RavlN {
  
  //: Default constructor.
  
  BuildInfoBodyC::BuildInfoBodyC() {
    libs["Math"] = LibInfoC("m"); // Alias for standard math library
    libs["system"] = LibInfoC("system",true); // Alias for standard math library
  }
  
  //: Add information from directory to database.
  
  bool BuildInfoBodyC::ScanDirectory(StringC &where,DefsMkFileC &defs) {
    std::cout << "BuildInfoBodyC::ScanDirectory:" << where << std::endl;
    StringC libName = defs["PLIB"];
    if(!libName.IsEmpty()) {
      LibInfoC &li = libs[libName];
      if(!li.IsValid()) // Need to create it?
	li = LibInfoC(libName);
      li.Add(defs,where);
      // Got an alias ?
      if(defs.IsDefined("LIBDEP")) {
	StringC alias = defs["LIBDEP"].before(".def");
	alias = alias.TopAndTail();
	if(!libs.IsElm(alias))
	  libs[alias] = li;
      }
    }
    return true;
  }


  //: List libraries the named library depends on.
  
  bool BuildInfoBodyC::LibDepends(StringC &lib,DListC<StringC> &list,HSetC<StringC> &done) {
    if(!libs.IsElm(lib)) {
      if(!done.IsMember(lib)) {
	list.InsFirst(StringC("-l") + lib);
	done += lib;
      }
      return true;
    }
    LibInfoC &info = libs[lib];
    lib = info.Name(); // Check its not an alias.
    done += lib;
    for(DLIterC<StringC> it(info.UsesLibs());it;it++) {
      if(done.IsMember(*it)) // Need to queue it ?
	continue;
      LibDepends(*it,list,done);
    }
    if(!info.IsDummy())
      list.InsFirst(StringC("-l") + lib);
    if(!info.MustLink().IsEmpty()) {
      StringC ml = info.MustLink();
      int i;
      if((i = ml.index(".cc",-1)) >= 0) {
	ml = ml.before(i);
      } else if((i = ml.index(".c",-1)) >= 0) {
	ml = ml.before(i);	
      }
      list.InsFirst(ml + ".o");
    }
    return true;
  }

  //: Given a list of libraries used directly, expand it to a full list.
  
  DListC<StringC> BuildInfoBodyC::Depends(const DListC<StringC> &uses) {
    DListC<StringC> ret;
    HSetC<StringC> done;
    for(DLIterC<StringC> it(uses);it;it++)
      LibDepends(*it,ret,done);
    return ret;
  }
  
  //: List libraries the named library depends on.
  
  DListC<StringC> BuildInfoBodyC::LibDepends(StringC &lib) {
    DListC<StringC> ret;
    HSetC<StringC> done;
    LibDepends(lib,ret,done);
    return ret;
  }

  
}
