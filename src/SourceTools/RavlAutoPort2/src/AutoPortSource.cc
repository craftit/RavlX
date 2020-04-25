// This file is part of VCPP, Visual C++ porting tool for QMake projects 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU General 
// Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
///////////////////////////////////////////////////
//! rcsid="$Id: AutoPortSource.cc 5298 2006-01-06 11:56:18Z kier $"
//! file="Ravl/SourceTools/VisualCPP/AutoPortSource.cc"
//! lib=RavlAutoPort2

#include "Ravl/config.h"

#if RAVL_COMPILER_LLVM
#include <iostream>
#include <fstream>
#endif

#include "Ravl/AutoPort2/AutoPortSource.hh"
#include "Ravl/SourceTools/SourceCodeManager.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/OS/Date.hh"
#include "Ravl/CallMethods.hh"
#include "Ravl/HashIter.hh"

#if !RAVL_COMPILER_LLVM
#if RAVL_HAVE_ANSICPPHEADERS
#include <fstream>
#else
#include <fstream.h>
#endif
#endif

#define DODEBUG 0
#if DODEBUG 
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  
  //: Constructor.
  
  AutoPortSourceBodyC::AutoPortSourceBodyC(StringC &where, DListC<StringC> & dl,const HSetC<StringC> &resources)
    : verbose(false), 
      doLibs(dl),
      m_resources(resources)
  {}
  
  //: Scan a tree for info.
  
  bool AutoPortSourceBodyC::ScanTree(StringC &where) {
    SourceCodeManagerC chkit(where);
    if(verbose)
      chkit.SetVerbose(true);
    chkit.ForAllDirs(CallMethod2C<AutoPortSourceC,StringC &,DefsMkFileC &,bool>(AutoPortSourceC(*this) ,
										&AutoPortSourceC::ScanDirectory));
										
    //check if all libraries in list were found
    if(!doLibs.IsEmpty()) 
    {
      for(DLIterC<StringC>it(doLibs);it;it++) 
      {
	if(!libs.IsElm(*it))
          cerr << "ERROR: Can't find library : " << *it << "\n";
      }
    }    
    										
    return true;
  }

  //: Scan the contents of a directory.
  
  bool AutoPortSourceBodyC::ScanDirectory(StringC &where,DefsMkFileC &defs) {
    if(verbose)
      cerr << "Scanning '" << where << "'\n";
    StringC libName = defs["PLIB"].TopAndTail();
    
    //: If we have been supplied with an external list of libraries to do
    if(!doLibs.IsEmpty()) {
      bool found=false;
      for(DLIterC<StringC> it(doLibs);it;it++) {
	if(*it == libName)
	  found=true;
      }
      if(!found)
      {
        //cout << "My skipping library " << libName << endl;
	return true;
      } 
    }
    
    StringListC libRequires(defs.Value("REQUIRES"));
    for(DLIterC<StringC> it(libRequires);it;it++) {
      if(!m_resources.IsMember(*it)) {
        cerr << "Missing required resource '" << *it << "' for '" << libName <<"' \n";
        return true;
      }
    }
    
    // Check this directory supports VCPP.
    if(defs.Value("DONOT_SUPPORT").contains("VCPP")) 
    {
      cerr << "WARNING: Library " << libName << " does not support Visual Studio\n";
      return true;
    }
    StringC supportOnly = defs.Value("SUPPORT_ONLY");
    if(!supportOnly.IsEmpty()) {
      if(!supportOnly.contains("VCPP")) 
      {
        cerr << "WARNING: Library " << libName << " does not support Visual Studio\n";
	return true;
      }
    }
    
    if(!libName.IsEmpty()) {
      LibInfoC &li = libs[libName];
      if(!li.IsValid()) // Need to create it?
	li = LibInfoC(libName);
      li.Add(defs,where);
    } else {
      if(defs["HEADERS"].TopAndTail().length() == 0)
	cerr << "WARNING: defs.mk has header files but no library in " << where << " \n";
    }
    {
      StringListC lmains(defs["MAINS"]);
      for(DLIterC<StringC> it(lmains);it;it++) {
	ONDEBUG(cerr << "Inc Main=" << *it << "\n");
	mains += ProgInfoC(*it,defs,where);
      }
    }
    {
      StringListC lexam(defs["EXAMPLES"]);
      for(DLIterC<StringC> it(lexam);it;it++) {
	ONDEBUG(cerr << "Inc Example=" << *it << "\n");
	examples += ProgInfoC(*it,defs,where);
      }
    }
    {
      StringListC ltests(defs["TESTEXES"]);
      for(DLIterC<StringC> it(ltests);it;it++) {
	ONDEBUG(cerr << "Inc Textexes=" << *it << "\n");
	tests += ProgInfoC(*it,defs,where);
      }
    }

    //: we want to build up the lib deps
    if(!libName.IsEmpty()) {
      DListC<StringC> usesLibs = libs[libName].UsesLibs();
      
      //: need to expand any libraries we already know about
      DListC<StringC> update;
      for(DLIterC<StringC>it(usesLibs);it;it++) {
	it.Data().del(".opt");
	if(deps.IsElm(*it)) { 
	  for(DLIterC<StringC> inIt(deps[*it]);inIt;inIt++) 
	    if(!update.Contains(*inIt)) update.InsLast(*inIt);
	}
	if(!update.Contains(*it)) update.InsLast(*it);
      }
      
      if(!deps.IsElm(libName)) {
	deps.Insert(libName, update);
      }
    }


    return true;
  }

  //: Update a file.
  // vcpp, is the file in the Visual C++ tree.
  // src, is the unix version. <p>

  bool AutoPortSourceBodyC::Unix2Dos(const StringC &src,const StringC &vcpp) {
    // Install new files.
    FilenameC srcFn(src);
    if(!srcFn.Exists()) {
      cerr << "WARNING: Can't find src file : " << srcFn << "\n";
      return false;
    }
    FilenameC vcppFn(vcpp);
    
    if(vcppFn.Exists()) {
      if(vcppFn.LastModTime() > srcFn.LastModTime())
	return true; // File is upto date.
    }
    // Check the destination is writeable.
    
    if(vcppFn.Exists()) {
      if(!vcppFn.IsWritable()) {
	FilePermissionC perm = vcppFn.Permissions();
	perm.SetUserReadWrite(); // Make sure user has read/write.
	if(!vcppFn.SetPermissions(perm)) {
	  cerr << "ERROR: Failed to get write permission for file : " << vcpp << "\n";
	  return false;
	}
      }
    }
    // Do the copy.
    if(!srcFn.CopyTo(vcpp)) {
      cerr << "ERROR: Failed to update file : " << vcpp << "\n";
      return false;
    }
    return true;
  }
  
  //: Update a file.
  // vcpp, is the file in the Visual C++ tree.
  // src, is the unix version. <p>
  
  bool AutoPortSourceBodyC::Dos2Unix(const StringC &src,const StringC &vcpp) {    
    FilenameC vcppFn(vcpp);
    if(!vcppFn.Exists()) {
      std::cerr << "ERROR: Failed to checkout file " << src << " for update. \n";
      return false;
    }
    std::ifstream in1(vcpp.c_str());
    std::ofstream in2(src.c_str());
    if(!in1) {
      std::cerr << "ERROR: Failed to open file :" << vcpp << " for update." <<endl;
      return false;
    }
    if(!in2) {
      std::cerr << "ERROR: Failed to open file :" << src << " for update." << endl;
      return false;
    }
    
    // As we all know DOS has additional junk \CR\LF
    // this neatly removes them on the way back to Unix
    // (they are not inserted on the way there)
    
    const int buffsize = 4096;
    char buff[buffsize];
    do {
      in1.read(buff,buffsize);
      int at = 0;
      char *place = buff;
      char *start = buff;
      char *end = &buff[in1.gcount()];
      for(;place != end;place++) {
	if(*place == 0x0d) { // Cut out these chars.
	  if(at != 0) 
	    in2.write(start,at); // Write upto here.
	  start = place;
	  start++; // Skip this char.
	  at = 0;
	} else
	  at++;
      }
      in2.write(start,at); // Write out the rest.
    } while(in1) ;
    
    return true;
  }
  
  //: Dump contents of db to stdout.
  
  void AutoPortSourceBodyC::Dump() {
    cerr << "Dump:\n";
    for(HashIterC<StringC,LibInfoC> it(libs);it;it++)
      cerr << "Lib=" << it.Key() << " \n";
    for(DLIterC<ProgInfoC> it(mains);it;it++)
      cerr << "Main=" << it->Name() << " Src=" << it->MainSource() << "\n";
    for(DLIterC<ProgInfoC> it(tests);it;it++)
      cerr << "Test=" << it->Name() << " Src=" << it->MainSource() << "\n";
    for(DLIterC<ProgInfoC> it(examples);it;it++)
      cerr << "Example=" << it->Name() << " Src=" << it->MainSource() << "\n";
    cerr << "Dump: Done.\n";
  }

  

}
