// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_FILEFORMATXMLSTREAM_HEADER
#define RAVL_FILEFORMATXMLSTREAM_HEADER 1
////////////////////////////////////////////////////////
//! docentry="Ravl.API.Core.IO.Formats" 
//! lib=RavlIO
//! file="Ravl/Core/IO/FileFormatXMLStream.hh"
//! author="Charles Galambos"
//! date="21/10/2002"
//! rcsid="$Id$"
//! userlevel=Default

#include "Ravl/DP/FileFormat.hh"
#include "Ravl/DP/XMLFileIO.hh"
#include "Ravl/TypeName.hh"

namespace RavlN {
  /////////////////////////////
  //: Binary stream file format.
  //! userlevel=Develop
  
  template<class DataT>
  class FileFormatXMLStreamBodyC : public FileFormatBodyC {
  public:
    FileFormatXMLStreamBodyC(bool deletable)
      : FileFormatBodyC("xml","XML stream. ",deletable)
      {}
    //: Default constructor.
    
    virtual const std::type_info &ProbeLoad(IStreamC &in,const std::type_info &/*obj_type*/) const  {
      if(!in.good())
	return typeid(void);
      streampos mark = in.Tell();
      XMLIStreamC ix(in);
      if(!ix.ReadHeader()) {
        //cerr << "FileFormatXMLStreamBodyC, No XML header found. \n";
        in.is().clear();
	in.Seek(mark);
	return typeid(void); // Don't know how to load this.
      }
      // Read RAVL processing instruction.
      StringC name;
      RCHashC<StringC,StringC> attr;
      XMLTagOpsT top = ix.ReadTag(name,attr);
      if(name != "RAVL" || top != XML_PI) {
        //cerr << "FileFormatXMLStreamBodyC, No xml RAVL header found. \n";
	in.Seek(mark);
	return typeid(void);
      }
      // Extract class name.
      StringC classname = attr["class"];
      in.Seek(mark);
      //cerr << "FileFormatXMLStreamBodyC, Got class '" << classname << "' Looking for='" << TypeName(typeid(DataT)) << "'\n";
      if(classname != StringC(TypeName(typeid(DataT)))) 
	return typeid(void); // Don't know how to load this.
      return typeid(DataT);
    }
    //: Is stream in std stream format ?
    
    virtual const std::type_info &ProbeLoad(const StringC &filename,IStreamC &in,const std::type_info &obj_type) const {
      //cout << "File Probe '" << filename << "' Looking for:" << TypeName(obj_type) << endl;
      if(filename == "") 
	return typeid(DataT); // Yep, can handle load to DataT.
      return ProbeLoad(in,obj_type); // Check load from stream.
    }
    
    virtual const std::type_info &ProbeSave(const StringC &filename,const std::type_info &/*obj_type*/,bool forceFormat) const {
      if(forceFormat)
	return typeid(DataT); // If we're forced just accept it.
      StringC ext = Extension(filename);
      // If there's no extension or the extension is 'xml' we can handle it.
      if(filename.IsEmpty())
	return typeid(void); // Nope.
      if(filename[0] == '@')
	return typeid(void); // Nope.
      if(ext == ""  || ext == "xml") 
	return typeid(DataT); // Yep, can save in format.
      return typeid(void); // Nope.
    }
    
    virtual DPIPortBaseC CreateInput(IStreamC &in,const std::type_info &obj_type) const {
      if(obj_type != typeid(DataT))
	return DPIPortBaseC();
      XMLIStreamC bs(in);
      return DPIXMLFileC<DataT>(bs,true);
    }
    //: Create a input port for loading.
    // Will create an Invalid port if not supported.
    
    virtual DPOPortBaseC CreateOutput(OStreamC &out,const std::type_info &obj_type) const {
      if(obj_type != typeid(DataT))
	return DPOPortBaseC();
      XMLOStreamC bs(out);
      return DPOXMLFileC<DataT>(bs,true);
    }
    //: Create a output port for saving.
    // Will create an Invalid port if not supported.
    
    
    DPIPortBaseC CreateInput(const StringC &filename,const std::type_info &obj_type) const  {
      if(obj_type != typeid(DataT))
	return DPIPortBaseC();
      XMLIStreamC bs(filename);
      return DPIXMLFileC<DataT>(bs,true);
    }
    
    //: Create a input port for loading.
    // Will create an Invalid port if not supported.
    
    DPOPortBaseC CreateOutput(const StringC &filename,const std::type_info &obj_type) const { 
      if(obj_type != typeid(DataT))
	return DPOPortBaseC();
      XMLOStreamC bs(filename);
      return DPOXMLFileC<DataT>(bs,true);
    }
    //: Create a output port for saving.
    // Will create an Invalid port if not supported.
    
    virtual const std::type_info &DefaultType() const 
    { return typeid(DataT); }
    //: Get preferred IO type.

    virtual bool IsStream() const
    { return true; }
    //: Just to make it clear its a streamable format.
  
  };

  
  /////////////////////////////
  //! userlevel=Normal
  //: Binary stream file format.
  
  template<class DataT>
  class FileFormatXMLStreamC : public FileFormatC<DataT> {
  public:
    FileFormatXMLStreamC()
      : FileFormatC<DataT>(*new FileFormatXMLStreamBodyC<DataT>(true))
    {}
  };

}

#endif
