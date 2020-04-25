// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_FILEFORMATSTREAM_HEADER
#define RAVL_FILEFORMATSTREAM_HEADER 1
////////////////////////////////////////////////////////
//! docentry="Ravl.API.Core.IO.Formats" 
//! lib=RavlIO
//! file="Ravl/Core/IO/FileFormatStream.hh"
//! author="Charles Galambos"
//! date="12/08/1998"
//! userlevel=Default

#include "Ravl/DP/FileFormat.hh"
#include "Ravl/DP/FileIO.hh"
#include "Ravl/TypeName.hh"

namespace RavlN {
  //: Register file format.
  bool RegisterFormatStreamMeta(FileFormatBaseC &fileformat);
  
  /////////////////////////////
  //: Stream File Format.
  //! userlevel=Develop
  
  template<class DataT>
  class FileFormatStreamBodyC 
    : public FileFormatBodyC 
  {
  public:
    FileFormatStreamBodyC()
      : FileFormatBodyC("stream","Standard C++ iostream headed with the class name. ",false)
    {}
    //: Default constructor.
    
    virtual const std::type_info &ProbeLoad(IStreamC &in,const type_info &obj_type) const  {
      if(!in.good())
	return typeid(void);
      StringC classname;
      if(!ReadString(in,classname))
	return typeid(void);
      //cout << "Stream Probe: " << classname << " Looking for:" << TypeName(obj_type) << endl;
      if(classname != StringC(TypeName(typeid(DataT))))
	return typeid(void); // Don't know how to load this.
      return typeid(DataT);
    }
    //: Is stream in std stream format ?
    
    virtual const std::type_info &ProbeLoad(const StringC &filename,IStreamC &in,const type_info &obj_type) const  {
      //cout << "File Probe '" << filename << "' Looking for:" << TypeName(obj_type) << endl;
      if(filename == "") 
	return typeid(DataT); // Yep, can handle load to DataT.
      return ProbeLoad(in,obj_type); // Check load from stream.
    }
    
    virtual const std::type_info &ProbeSave(const StringC &filename,const type_info &/*obj_type*/,bool forceFormat) const {
      StringC ext = Extension(filename);
      // If there's no extention or the extention is 'strm' we can handle it.
      if(forceFormat)
	return typeid(DataT); // Yep, can save in format.      
      if(filename.IsEmpty())
	return typeid(void); // Nope.
      if(filename[0] == '@')
	return typeid(void); // Nope.
      if(ext == ""  || ext == "strm" || ext == "txt") 
	return typeid(DataT); // Yep, can save in format.
      return typeid(void); // Nope.
    }
    
    virtual DPIPortBaseC CreateInput(IStreamC &in,const std::type_info &obj_type) const {
      if(obj_type != typeid(DataT))
	return DPIPortBaseC();
      return DPIFileC<DataT>(in,true);
    }
    //: Create a input port for loading.
    // Will create an Invalid port if not supported.
    
    virtual DPOPortBaseC CreateOutput(OStreamC &out,const std::type_info &obj_type) const {
      if(obj_type != typeid(DataT))
	return DPOPortBaseC();
      return DPOFileC<DataT>(out,true);
    }
    //: Create a output port for saving.
    // Will create an Invalid port if not supported.
    
    virtual const std::type_info &DefaultType() const 
    { return typeid(DataT); }
    //: Get prefered IO type.
    
    virtual bool IsStream() const
    { return true; }
    //: Just to make it clear its a streamable format.
    
  };
  
  
  /////////////////////////////
  //! userlevel=Normal
  //: Create an instance of a stream File Format.
  
  template<class DataT>
  class FileFormatStreamC : public FileFormatC<DataT> {
  public:
    FileFormatStreamC()
      : FileFormatC<DataT>(*new FileFormatStreamBodyC<DataT>())
    { RegisterFormatStreamMeta(*this); }

    FileFormatStreamC(const char *typeName)
      : FileFormatC<DataT>(*new FileFormatStreamBodyC<DataT>())
    {
      AddTypeName(this->DefaultType(),typeName);
      RegisterFormatStreamMeta(*this);
    }
  };
}


#endif
