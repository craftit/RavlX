// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlIO
//! file="Ravl/Core/IO/FileFormat.cc"

#include "Ravl/DP/FileFormat.hh"
#include "Ravl/DP/FileFormatRegistry.hh"
#include "Ravl/DList.hh"
#include "Ravl/DLIter.hh"
#include "Ravl/Hash.hh"
#include "Ravl/DP/DataConv.hh"
#include "Ravl/DP/Converter.hh"
#include "Ravl/MTLocks.hh"
#include <ctype.h>
#include <stdio.h>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  
  
  ///////////////////////////
  //: Default constructor.
  
  FileFormatBodyC::FileFormatBodyC(StringC nformatName,StringC nDesc,bool pubList)
    : formatName(nformatName),
      desc(nDesc)
  {
    if(pubList) { // Put in public list ?
      FileFormatBaseC me(*this);
      SystemFileFormatRegistry().Insert(me);
      ONDEBUG(cerr << "Fileformat: " << nformatName << " registered\n");
    }
  }
  
  ///////////////////////////
  //: Destructor
  // Remove from all lists.
  
  FileFormatBodyC::~FileFormatBodyC() 
  {}
  
  ///////////////////////////////
  //: Is stream in this format ?
  
  const std::type_info &
  FileFormatBodyC::ProbeLoad(IStreamC &/*in*/,const std::type_info &/*obj_type*/) const 
  { return typeid(void); }

  //////////////////////////////
  //: Is Filename in this format ?
  
  const std::type_info &
  FileFormatBodyC::ProbeLoad(const StringC &filename,IStreamC &in,const std::type_info &obj_type) const  { 
    if(!in.good())
      return typeid(void);
    return ProbeLoad(in,obj_type);
  }
  
  //////////////////////////////
  //: Is Filename in this format ?
  
  const std::type_info &
  FileFormatBodyC::ProbeSave(const StringC &/*filename*/,const std::type_info &/*obj_type*/,bool /*forceFormat*/) const 
  { return typeid(void); }

  //////////////////////////////
  //: Create a input port for loading.
  // Will create an Invalid port if not supported.
  
  DPIPortBaseC FileFormatBodyC::CreateInput(const StringC &filename,const std::type_info &obj_type) const  { 
    IStreamC in(filename,true); // Open in binary mode by default.
    if(!in) // File opened ok ?
      return DPIPortBaseC();
    return CreateInput(in,obj_type);
  }
  
  //////////////////////////////
  //: Create a output port for saving.
  // Will create an Invalid port if not supported.
  
  DPOPortBaseC FileFormatBodyC::CreateOutput(const StringC &filename,const std::type_info &obj_type) const  { 
    OStreamC out(filename,true); // Open in binary mode by default.
    if(!out) // File opened ok ?
      return DPOPortBaseC();
    return CreateOutput(out,obj_type);
  }
  
  //////////////////////////////
  //: Create a input port for loading.
  // Will create an Invalid port if not supported.
  
  DPIPortBaseC FileFormatBodyC::CreateInput(IStreamC &,const std::type_info &) const
  { return DPIPortBaseC(); }
  
  //////////////////////////////
  //: Create a output port for saving.
  // Will create an Invalid port if not supported.
  
  DPOPortBaseC FileFormatBodyC::CreateOutput(OStreamC &,const std::type_info &) const
  { return DPOPortBaseC(); }
  
  //////////////////////////////
  //: Get preferred IO type.
  // typeid(void) if none.
  
  const std::type_info &FileFormatBodyC::DefaultType() const
  { return typeid(void); }
  

  //: Find the priority of the format. the higher the better.
  // Default is zero.

  IntT FileFormatBodyC::Priority() const {
    return 0;
  }

  ///////////////////////////////
  //: Test if format is a fully streamable.
  // i.e. check if you can read/write more than object object.
  // (Default value of this function is true).
  
  bool FileFormatBodyC::IsStream() const
  { return true; }
  
  //: Alias for typename of default type.
  // Returns empty string if not set.
  
  StringC FileFormatBodyC::TypenameAlias() const
  { return StringC(); }

  //: read a string from the input.
  
  bool FileFormatBodyC::ReadString(IStreamC &s,StringC &buff,UIntT maxLen) {
    if (!s)
      return false;
    std::streamoff pos = s.Tell();
    bool ret = true;  
    int ch;
    streambuf *sb = s.is().rdbuf();
    buff = StringC();
    while ((ch = sb->sbumpc()) != EOF)
    {
      if (ch == '\n' || ch == ' ' || ch == '\t')
        break;
      if(!isprint(ch)) {
        ONDEBUG(cerr << "FileFormatBodyC::ReadString(), Found non printable char " << ((int) ch) << "\n");
        ret = false;
        break;
      }
      if(buff.length() >= maxLen) {
        ONDEBUG(cerr << "FileFormatBodyC::ReadString(), String to long '" << maxLen << "'\n");
        ret = false;
        break;
      }
      buff += ch;
    }
    if (ch == EOF) 
      s.is().clear(ios::eofbit|s.is().rdstate());
    s.Seek(pos);
    ONDEBUG(cerr << "FileFormatBodyC::ReadString(), Ret:'" << buff << "' MaxLen:" << maxLen << " Ok:" << ret << "\n");
    return ret;
  }
  
  //: Get the device from the name.
  
  StringC FileFormatBodyC::ExtractDevice(const StringC &nm) {
    StringC dataName = StringC(nm).after(0);
    if(dataName.contains(':'))
      dataName = dataName.before(':');
    return dataName;
  }
  
  StringC FileFormatBodyC::ExtractParams(const StringC &nm) {
    StringC dataName(nm);
    if(!dataName.contains(':'))
      return ""; // Empty
    return dataName.after(':');
  }
  
  /////////////////////////////////////////////////////////////////////////////
  
  StringC FileFormatBodyC::Extension(const StringC &afn) {
    StringC fn(afn);
    int i;
    for(i = fn.length()-1;i >= 0;i--) {
      if(fn[i] == '.')
	break;
      if(fn[i] == '/') // Found file separator.
	return StringC();
    }
    if(i < 0)
      return StringC(); // No extension found.
    return fn.after(i);
  }

  
  //////////////////////////////////////////////////////////////
  
  //: Save constructor.
  
  FileFormatBaseC::FileFormatBaseC(bool forLoad,StringC name,const std::type_info &objtype) 
    : DPEntityC(true)
  { (*this) = SystemFileFormatRegistry().FindFormat(forLoad,name,objtype); }

  //: Load constructor.

  FileFormatBaseC::FileFormatBaseC(bool forLoad,const StringC &filename,StringC formName,const std::type_info &objtype) 
    : DPEntityC(true)
  {  (*this) = SystemFileFormatRegistry().FindFormatFile(forLoad,filename,objtype,formName); }
 
  //: Load constructor.
  
  FileFormatBaseC::FileFormatBaseC(IStreamC &in,StringC formName,const std::type_info &objtype) 
    : DPEntityC(true)
  { (*this) = SystemFileFormatRegistry().FindFormat(in,objtype,formName); }
}
