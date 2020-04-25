// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_FILEFORMATCOMPOSITE_HEADER 
#define RAVL_FILEFORMATCOMPOSITE_HEADER 1
////////////////////////////////////////////////////////
//! docentry="Ravl.API.Core.Data Processing.Composite" 
//! lib=RavlOSIO
//! rcsid="$Id$"
//! file="Ravl/OS/IO/FileFormatComposite.hh"
//! author="Charles Galambos"
//! date="12/08/1998"
//! userlevel=Default

#include "Ravl/DP/FileFormat.hh"
#include "Ravl/DP/FileFormatIO.hh"
#include "Ravl/DP/CompositeStream.hh"
#include "Ravl/TypeName.hh"

namespace RavlN {  

  ////////////////////////////
  //: Stream File Format.
  //! userlevel=Develop
  
  template<class DataT>
  class FileFormatCompositeBodyC 
    : public FileFormatBodyC 
  {
  public:
    FileFormatCompositeBodyC()
      : FileFormatBodyC("composite","Composte file sequences. (@COMPOSITE)")
    {}
    //: Default constructor.
    
    virtual const type_info &ProbeLoad(IStreamC &in,const type_info &/*obj_type*/) const 
    { return typeid(void); }
    //: Is stream in std stream format ?
    
    virtual const type_info &ProbeLoad(const StringC &filename,IStreamC &in,const type_info &obj_type) const {
      if(filename.length() == 0)
	return typeid(void);
      if(filename[0] != '@')
	return typeid(void);
      if(ExtractDevice(filename) != "COMPOSITE")
	return typeid(void);
      return typeid(DataT);
    }
    
    virtual const type_info &ProbeSave(const StringC &filename,const type_info &/*obj_type*/,bool forceFormat) const {
      if(filename.length() == 0)
	return typeid(void);
      if(filename[0] != '@')
	return typeid(void);
      if(ExtractDevice(filename) != "COMPOSITE")
	return typeid(void);
      return typeid(DataT);    
    }
    
    virtual DPIPortBaseC CreateInput(IStreamC &in,const type_info &obj_type) const
    { return DPIPortBaseC(); }
    //: Create a input port for loading.
    // Will create an Invalid port if not supported.
    
    virtual DPOPortBaseC CreateOutput(OStreamC &out,const type_info &obj_type) const
    { return DPOPortBaseC(); }
    //: Create a output port for saving.
    // Will create an Invalid port if not supported.
    
    virtual DPIPortBaseC CreateInput(const StringC &filename,const type_info &obj_type) const {
      PlayListC pl;
      StringC fn = ExtractParams(filename);
      cerr << "Attempting to load play list.  " << fn << "\n";
      if(!Load(fn,pl,"",true)) {
	cerr << "FileFormatCompositeBodyC::CreateInput(), ERROR: Failed to load playlist '" << fn << "' for '" << filename << "' Stream type:" << TypeName(typeid(DataT)) << "\n";
	return DPIPortBaseC();
      }
      return CompositeIStreamC<DataT>(pl);
    }
    //: Create a input port for loading.
    
    virtual DPOPortBaseC CreateOutput(const StringC &filename,const type_info &obj_type) const {
      return CompositeOStreamC<DataT>(filename);
    }
    //: Create a output port for saving.
    
    virtual const type_info &DefaultType() const 
    { return typeid(DataT); }
    //: Get prefered IO type.
    
  };
  
  
  /////////////////////////////
  //! userlevel=Normal
  //: Create an instance of a composite file format
  // This format loads a list of file names and loads one after the other.
  
  template<class DataT>
  class FileFormatCompositeC : public FileFormatC<DataT> {
  public:
    FileFormatCompositeC()
      : FileFormatC<DataT>(*new FileFormatCompositeBodyC<DataT>())
    {}
  };

}

#endif

