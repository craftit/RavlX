// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_FILEFORMATDYNAMIC_HEADER
#define RAVL_FILEFORMATDYNAMIC_HEADER
///////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/OS/IO/FileFormatDynamic.hh"
//! lib=RavlOSIO
//! docentry="Ravl.API.OS"
//! author="Charles Galambos"
//! date="04/07/2000"

#include "Ravl/DP/FileFormat.hh"
#include "Ravl/DList.hh"
#include "Ravl/String.hh"
#include "Ravl/Hash.hh"

namespace RavlN {

  //! userlevel=Develop
  //: Dynamic loading of shared libraries for IO
  // In order to probe a file format dynamically, the shared libraries must be available.

  class FileFormatDynamicBodyC 
    : public FileFormatBodyC 
  {
  public:
    FileFormatDynamicBodyC();
    //: Default constructor.
  
    virtual const type_info &ProbeLoad(IStreamC &in,const type_info &obj_type) const;
    //: Is stream in std stream format ?
  
    virtual const type_info &ProbeLoad(const StringC &filename,IStreamC &in,const type_info &obj_type) const;
    //: Probe for load.
  
    virtual const type_info &ProbeSave(const StringC &filename,const type_info &obj_type,bool forceFormat) const;
    //: Probe for Save.
  
    virtual DPIPortBaseC CreateInput(IStreamC &in,const type_info &obj_type) const;
    //: Create a input port for loading.
    // Will create an Invalid port if not supported.
  
    virtual DPOPortBaseC CreateOutput(OStreamC &out,const type_info &obj_type) const;
    //: Create a output port for saving.
    // Will create an Invalid port if not supported.
  
    virtual const type_info &DefaultType() const; 
    //: Get prefered IO type.
  
    virtual IntT Priority() const { return 0; }
    //: Find the priority of the format. the higher the better.
    // Default is zero, this is worse than the default (streams.)
  
    virtual bool IsStream() const { return false; }
    //: Test if format is a fully streamable.
    // i.e. check if you can read/write more than object object.
  

  protected:

    class DynEntryC {
    public:
      DynEntryC()
	: ltype(-1)
      {}
      //: Default constructor.
    
      DynEntryC(int nltype,const StringC &nformatId,const StringC &nm,const DListC<StringC> &nlibs)
	: ltype(nltype),
	  formatId(nformatId),
	  key(nm),
	  libs(nlibs)
      {}
    
      int ltype;  // 0-FileExt 1-FileExt 2-Symbol 
      StringC formatId;
      StringC key;
      DListC<StringC> libs;
    };
  
    HashC<StringC,DynEntryC> entryBySymb;
    DListC<DynEntryC> prefix;
    DListC<DynEntryC> postfix;
  
    bool LoadFormatInfo(const StringC &fn);
    //: Load a description file.
    
    bool CheckDyn(const StringC &filename,const type_info &obj_type) const;
    // Check info against load 
  
    bool Load(const DynEntryC &entry) const;
    //: Load libraries specified in an entry as needed.
    
  };

  //! userlevel=Advanced
  //: Dynamic loading of shared libraries for IO
  // In order to probe a file format dynamically, the shared libraries must be available.
  
  class FileFormatDynamicC 
    : public FileFormatBaseC
  {
  public:
    FileFormatDynamicC();
  };
}
#endif
