// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLFILEFORMATGEN_HEADER
#define RAVLFILEFORMATGEN_HEADER 1
//////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Core/IO/FileFormatGen.hh"
//! lib=RavlIO
//! docentry="Ravl.API.Core.IO.Formats" 
//! author="Charles Galambos"
//! date="17/07/2000"
//! userlevel=Default

#include "Ravl/DP/FileFormat.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Generic format loader.
  // This class provides a simple way of adding new file formats to
  // the system, all it needs is a few pointers to functions which
  // create the appropriate I/O streams.
  
  class FileFormatGenericBodyC  
    : public FileFormatBodyC
  {
  public:
    typedef DPIPortBaseC (*LoadFuncT)(IStreamC &in);
    typedef DPOPortBaseC (*SaveFuncT)(OStreamC &in);
    typedef DPIPortBaseC (*LoadFuncFnT)(const StringC &);
    typedef DPOPortBaseC (*SaveFuncFnT)(const StringC &);
    
    FileFormatGenericBodyC(const StringC &nformatName,
			   const std::type_info &formatType,
			   LoadFuncT nLoadFunc,
			   SaveFuncT nSaveFunc,
			   LoadFuncFnT loadFuncFn,
			   SaveFuncFnT saveFuncFn,
			   const StringC &nDesc = "None.",
			   const StringC &nExt = "",
			   bool naStream = false,
			   IntT priority = 0,
			   const StringC &nHdrData = "" );
    //: Constructor.
    
    FileFormatGenericBodyC(const StringC &nformatName,
			   const std::type_info &formatType,
			   LoadFuncFnT nLoadFunc,
			   SaveFuncFnT nSaveFunc,
			   
			   const StringC &nDesc = "None.",
			   const StringC &nExt = "",
			   bool naStream = false,
			   IntT priority = 0,
			   const StringC &nHdrData = "" );
    //: Constructor.
    
    virtual const std::type_info &ProbeLoad(IStreamC &in,const type_info &obj_type) const;
    //: Is stream in this format ?
    // This asks the question: Can stream 'in' be loaded into object 'obj_type' ?
    // typeid(void) == no, otherwise returns the prefered load type.
    
    virtual const std::type_info &ProbeLoad(const StringC &filename,IStreamC &in,const type_info &obj_type) const;
    //: Is stream in this format ?
    // This asks the question: Can stream 'in' be loaded into object 'obj_type' ?
    // If used with a empty filename asks: Can load this type of object ?
    // typeid(void) == no, otherwise returns the prefered load type.
    
    
    virtual const std::type_info &ProbeSave(const StringC &filename,const type_info &obj_type,bool forceFormat) const;
    //: Is Filename in this format ?
    // This asks the question: Can file 'filename' be saved with object 'obj_type' ?
    // If used with a empty filename asks: Can save this type of object ?
    // typeid(void) == no, otherwise returns the prefered save type.
    
    virtual DPIPortBaseC CreateInput(const StringC &filename,const std::type_info &obj_type) const;
    //: Create a input port for loading from file 'filename'.
    // Will create an Invalid port if not supported. <p>
    // The default version of this functions opens the file and passes
    // the stream to CreateInput(IStreamC ....)
  
    virtual DPOPortBaseC CreateOutput(const StringC &filename,const std::type_info &obj_type) const;
    //: Create a output port for saving to file 'filename'..
    // Will create an Invalid port if not supported. <p>
    // The default version of this functions opens the file and passes
    // the stream to CreateOutput(OStreamC ....)
    
    virtual DPIPortBaseC CreateInput(IStreamC &in,const std::type_info &obj_type) const;
    //: Create a input port for loading from stream 'in'.
    // Will create an Invalid port if not supported.
    
    virtual DPOPortBaseC CreateOutput(OStreamC &out,const std::type_info &obj_type) const;
    //: Create a output port for saving to stream 'out'.
    // Will create an Invalid port if not supported.
    
    virtual const std::type_info &DefaultType() const;
    //: Get preferred IO type.
    // This specifies the native class for representing the data in a file. <p>
    // typeid(void) if none, indicates an indirect driver.
    
    virtual IntT Priority() const;
    //: Find the priority of the format. the higher the better.
    // Default is zero.
    
    virtual bool IsStream() const;
    //: Test if format is a fully streamable.
    // i.e. check if you can read/write more than object object.
    // (Default value of this function is true).
    
    
  protected:
    StringC ext;    // Extension name of file.
    StringC headerData; // Prefix in file header.
    IntT priority;
    LoadFuncT loadFunc;
    SaveFuncT saveFunc;
    LoadFuncFnT loadFuncFn;
    SaveFuncFnT saveFuncFn;
    bool aStream;
    const std::type_info *formatType;
    
  };
  
  //! userlevel=Normal
  //: Generic format loader.
  // This class provides a simple way of adding new file formats to
  // the system, all it needs is a few points to functions which 
  // create the appropriate I/O streams.
  
  class FileFormatGenericC  
   : public FileFormatBaseC
  {
  public:
    FileFormatGenericC()
      {}
    //: Default constructor.
    // Creates an invalid object.
    
    FileFormatGenericC(const StringC &nformatName,
		       const std::type_info &formatType,
		       FileFormatGenericBodyC::LoadFuncT nLoadFunc,
		       FileFormatGenericBodyC::SaveFuncT nSaveFunc,
		       FileFormatGenericBodyC::LoadFuncFnT nLoadFuncFn,
		       FileFormatGenericBodyC::SaveFuncFnT nSaveFuncFn,
		       const StringC &nDesc = "None.",
		       const StringC &nExt = "",
		       bool naStream = false,
		       IntT priority = 0,
		       const StringC &nHdrData = "" )
      : FileFormatBaseC(*new FileFormatGenericBodyC(nformatName,
						    formatType,
						    nLoadFunc,
						    nSaveFunc,
						    nLoadFuncFn,
						    nSaveFuncFn,
						    nDesc,
						    nExt,
						    naStream,
						    priority,
						    nHdrData
						    ))
      {}
    //: Constructor.
    
    
    FileFormatGenericC(const StringC &nformatName,
		       const std::type_info &formatType,
		       FileFormatGenericBodyC::LoadFuncT nLoadFunc,
		       FileFormatGenericBodyC::SaveFuncT nSaveFunc,
		       const StringC &nDesc = "None.",
		       const StringC &nExt = "",
		       bool naStream = false,
		       IntT priority = 0,
		       const StringC &nHdrData = "" )
      : FileFormatBaseC(*new FileFormatGenericBodyC(nformatName,
						    formatType,
						    nLoadFunc,
						    nSaveFunc,
						    0,0,
						    nDesc,
						    nExt,
						    naStream,
						    priority,
						    nHdrData
						    ))
      {}
    //: Constructor.
    
    FileFormatGenericC(const StringC &nformatName,
		       const std::type_info &formatType,
		       FileFormatGenericBodyC::LoadFuncFnT nLoadFunc,
		       FileFormatGenericBodyC::SaveFuncFnT nSaveFunc,
		       const StringC &nDesc = "None.",
		       const StringC &nExt = "",
		       bool naStream = false,
		       IntT priority = 0,
		       const StringC &nHdrData = "" )
      : FileFormatBaseC(*new FileFormatGenericBodyC(nformatName,
						    formatType,
						    nLoadFunc,
						    nSaveFunc,
						    nDesc,
						    nExt,
						    naStream,
						    priority,
						    nHdrData
						    ))
      {}
    //: Constructor.
  };
  
}
#endif

