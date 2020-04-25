// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlOSIO
//! file="Ravl/OS/IO/FileFormatDynamic.cc"

#include "Ravl/DP/FileFormatDynamic.hh"
#include "Ravl/DP/DynamicLink.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/StringList.hh"
#include "Ravl/CDLIter.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/DP/DynamicIO.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x) 
#endif

#ifndef PROJECT_OUT
#define PROJECT_OUT ""
#endif

namespace RavlN {
  
  bool InitDynamicIO() { 
    static FileFormatDynamicC RegisterFileFormatDynamic; 
    return true;
  }
  
  //: Default constructor.

  FileFormatDynamicBodyC::FileFormatDynamicBodyC()
    : FileFormatBodyC("dyn","Dynamic format loader. ")
  {
    ONDEBUG(cerr << "FileFormatDynamicBodyC::FileFormatDynamicBodyC(), Called. \n");
    StringC formatInfo = PROJECT_OUT "/lib/RAVL/libdep/formatInfo";
    if(!LoadFormatInfo(formatInfo)) {
#if DODEBUG
      cerr << "Failed to load default format information from '" << formatInfo << "'\n";
#endif
    }
  }
  
  //: Is stream in std stream format ?

  const type_info &FileFormatDynamicBodyC::ProbeLoad(IStreamC &in,const type_info &obj_type) const
  {
    CheckDyn(StringC(""),obj_type);
    return typeid(void);
  }

  //: Probe for load.
  const type_info &FileFormatDynamicBodyC::ProbeLoad(const StringC &filename,IStreamC &in,const type_info &obj_type) const
  {
    CheckDyn(filename,obj_type);  
    return typeid(void);
  }

  //: Probe for Save.

  const type_info &FileFormatDynamicBodyC::ProbeSave(const StringC &filename,const type_info &obj_type,bool forceFormat) const
  {
    CheckDyn(filename,obj_type);  
    return typeid(void);
  }

  //: Create a input port for loading.
  // Will create an Invalid port if not supported.

  DPIPortBaseC FileFormatDynamicBodyC::CreateInput(IStreamC &in,const type_info &obj_type) const
  {
    RavlAssert(0);
    return DPIPortBaseC();
  }

  //: Create a output port for saving.
  // Will create an Invalid port if not supported.

  DPOPortBaseC FileFormatDynamicBodyC::CreateOutput(OStreamC &out,const type_info &obj_type) const
  {
    RavlAssert(0);
    return DPOPortBaseC();
  }

  //: Get prefered IO type.

  const type_info &FileFormatDynamicBodyC::DefaultType() const
  { return typeid(void); }


  //: Load a description file.

  bool FileFormatDynamicBodyC::LoadFormatInfo(const StringC &fn) {
    ONDEBUG(cerr << "FileFormatDynamicBodyC::LoadFormatInfo() From '" << fn << "'\n");
    IStreamC in(fn);
    if(!in.good()) 
      return false;
    StringC lineBuff;
    int lineNo = 0;
    while(in.good()) {
      StringC formatId,entryType,key;
      lineNo++;
      lineBuff = "";
      readline(in,lineBuff);
      if(lineBuff.length() == 0)
	continue;
      if(lineBuff[0] == '#')
	continue; // Skip comment.
      StringListC sl(lineBuff);
      if(sl.IsEmpty())
	continue;
      formatId = sl.PopFirst();
      if(sl.IsEmpty()) {
	cerr << "Unexpected end of line in '" << fn <<"' at line " << lineNo << " \n";
	continue;
      }
      entryType = sl.PopFirst();
      if(sl.IsEmpty()) {
	cerr << "Unexpected end of line in '" << fn <<"' at line " << lineNo << " \n";
	continue;
      }
      key = sl.PopFirst();
      if(sl.IsEmpty()) {
	cerr << "Unexpected end of line in '" << fn <<"' at line " << lineNo << " \n";
	continue;
      }
      // The remain is a list of libs to load.
      int entryTypeNo = 0; // Default by file...
      //ONDEBUG(cerr <<"Entry:" << entryTypeNo << " : " << formatId << ":" << key <<":" << sl << "\n");
      if(entryType == "fileExt")
	entryTypeNo = 0;
      else if(entryType == "filePrefix")
	entryTypeNo = 1;
      else if(entryType == "symb")
	entryTypeNo = 2;
      else
	cerr << "ERROR: Unknown entry type '" << entryType << "', in '" << fn <<"' at line " << lineNo << " \n";
      DynEntryC newun(entryTypeNo,formatId,key,sl);
      if(entryTypeNo == 0)
	postfix.InsLast(newun);
      if(entryTypeNo == 1)
	prefix.InsLast(newun);
      if(entryTypeNo == 2)
	entryBySymb[key] = newun;
    }
    return true;
  }
  
  bool FileFormatDynamicBodyC::CheckDyn(const StringC &filename,const type_info &obj_type) const {
    ONDEBUG(cerr << "Checking  '" << filename << "' Type:" << TypeName(obj_type) << "\n");
#if 0
    if(obj_type != typeinfo(void)) {
      const DynEntryC *ent = entryBySymb.Lookup(TypeName(obj_type));
      if(ent != 0) 
	Load(*ent);
    }
#endif
    for(DLIterC<DynEntryC> it(postfix);it.IsElm();it.Next()) {
      if(Extension(filename) == it.Data().key)
	Load(it.Data());
    }
    for(DLIterC<DynEntryC> it1(prefix);it1.IsElm();it1.Next()) {
      if(filename.matches(it1.Data().key,0,false)) 
	Load(it1.Data());
    }
    return true;
  }

  bool FileFormatDynamicBodyC::Load(const DynEntryC &entry) const {
    ONDEBUG(cerr << "Requesting load of entry '" << entry.key << "' \n");
    for(DLIterC<StringC> it(entry.libs);it;it++) {
      StringC lib = StringC("lib") + it.Data() + ".so";
      ONDEBUG(cerr << "  loading '" << it.Data() << "' (" << lib << ") \n");
      DynamicLinkLoad(lib);
    }
    return true;
  }

  ///////////////////////////////////////////////////////////

  FileFormatDynamicC::FileFormatDynamicC()
    : FileFormatBaseC(*new FileFormatDynamicBodyC())
  {}
  
#if CPUG_VAR_SHARED
#endif

}
