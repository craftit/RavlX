// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlIO
//! file="Ravl/Core/IO/FileFormatGen.cc"

#include "Ravl/DP/FileFormatGen.hh"

#define DODEBUG 0

#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  
  FileFormatGenericBodyC::FileFormatGenericBodyC(const StringC &nformatName,
						 const std::type_info &nformatType,
						 LoadFuncT nLoadFunc,
						 SaveFuncT nSaveFunc,
						 LoadFuncFnT nLoadFuncFn,
						 SaveFuncFnT nSaveFuncFn,
						 const StringC &nDesc,
						 const StringC &nExt,
						 bool naStream,
						 IntT nPriority,
						 const StringC &nHdrData)
    : FileFormatBodyC(nformatName,nDesc),
      ext(nExt),
      headerData(nHdrData),
      priority(nPriority),
      loadFunc(nLoadFunc),
      saveFunc(nSaveFunc),
      loadFuncFn(nLoadFuncFn),
      saveFuncFn(nSaveFuncFn),
      aStream(naStream),
      formatType(&nformatType)
  {}
  
  //: Constructor.
  
  FileFormatGenericBodyC::FileFormatGenericBodyC(const StringC &nformatName,
						 const std::type_info &nformatType,
						 LoadFuncFnT nLoadFunc,
						 SaveFuncFnT nSaveFunc,
						 const StringC &nDesc,
						 const StringC &nExt,
						 bool naStream,
						 IntT nPriority,
						 const StringC &nHdrData)
    : FileFormatBodyC(nformatName,nDesc),
      ext(nExt),
      headerData(nHdrData),
      priority(nPriority),
      loadFunc(0),
      saveFunc(0),
      loadFuncFn(nLoadFunc),
      saveFuncFn(nSaveFunc),
      aStream(naStream),
      formatType(&nformatType)
  {}
  
  
  //: Is stream in this format ?
  // This asks the question: Can stream 'in' be loaded into object 'obj_type' ?
  // typeid(void) == no, otherwise returns the prefered load type.
  
  const std::type_info &FileFormatGenericBodyC::ProbeLoad(IStreamC &in,const type_info &obj_type) const {
    if(loadFunc == 0)
      return typeid(void); // Don't know how !
    if(headerData.length() > 0) { // Check header data infile.
      if(!in.good())
	return typeid(void); // Don't know how !
      StringC buff("",headerData.length()+1);
      in.read(&(buff[0]),headerData.length());
      in.Unget(&(buff[0]),headerData.length()); // Put them back.
      if(buff != headerData)
	return typeid(void);
    }
    return *formatType;
  }
  
  
  //: Is stream in this format ?
  // This asks the question: Can stream 'in' be loaded into object 'obj_type' ?
  // If used with a empty filename asks: Can load this type of object ?
  // typeid(void) == no, otherwise returns the prefered load type.
  
  const std::type_info &FileFormatGenericBodyC::ProbeLoad(const StringC &filename,IStreamC &in,const type_info &obj_type) const {
    if(loadFuncFn == 0 && loadFunc == 0)
      return typeid(void); // Don't know how !
    if(ext.length() > 0) {
      if(Extension(filename) != ext)
	return typeid(void);
    }
    if(headerData.length() > 0) { // Check header data infile.
      if(!in.good())
	return typeid(void); // Don't know how !
      StringC buff("",headerData.length()+1);
      in.read(&(buff[0]),headerData.length());
      in.Unget(&(buff[0]),headerData.length()); // Put them back.
      if(buff != headerData)
	return typeid(void);
    }
    return *formatType;  
  }
  
  //: Is Filename in this format ?
  // This asks the question: Can file 'filename' be saved with object 'obj_type' ?
  // If used with a empty filename asks: Can save this type of object ?
  // typeid(void) == no, otherwise returns the prefered save type.
  
  const std::type_info &FileFormatGenericBodyC::ProbeSave(const StringC &filename,const type_info &obj_type,bool forceFormat) const {
    if(saveFuncFn == 0 && saveFunc == 0)
      return typeid(void); // Don't know how !
    if(forceFormat)
      return *formatType;
    if(ext.length() > 0) {
      if(Extension(filename) != ext)
	return typeid(void);
    }
    return *formatType;
  }
  

  //: Create a input port for loading from stream 'in'.
  // Will create an Invalid port if not supported.
  
  DPIPortBaseC FileFormatGenericBodyC::CreateInput(IStreamC &in,const std::type_info &obj_type) const {
    if(loadFunc == 0)
      return DPIPortBaseC();
    if(obj_type != *formatType)
      return DPIPortBaseC();
    return loadFunc(in);
  }
  
  //: Create a output port for saving to stream 'out'.
  // Will create an Invalid port if not supported.
  
  DPOPortBaseC FileFormatGenericBodyC::CreateOutput(OStreamC &out,const std::type_info &obj_type) const {
    if(saveFunc == 0)
      return DPOPortBaseC();
    if(obj_type != *formatType)
      return DPOPortBaseC();
    return saveFunc(out);
  }
  
  //: Create a input port for loading from stream 'in'.
  // Will create an Invalid port if not supported.
  
  DPIPortBaseC FileFormatGenericBodyC::CreateInput(const StringC &in,const std::type_info &obj_type) const {
    if(loadFuncFn == 0) {
      IStreamC strm(in);
      if(!strm.good())
	return DPIPortBaseC();
      return CreateInput(strm,obj_type);
    }
    if(obj_type != *formatType)
      return DPIPortBaseC();
    return loadFuncFn(in);
  }
  
  //: Create a output port for saving to stream 'out'.
  // Will create an Invalid port if not supported.
  
  DPOPortBaseC FileFormatGenericBodyC::CreateOutput(const StringC &out,const std::type_info &obj_type) const {
    if(saveFuncFn == 0) {
      OStreamC strm(out);
      if(!strm.good())
	return DPOPortBaseC();
      return CreateOutput(strm,obj_type);
    }
    if(obj_type != *formatType)
      return DPOPortBaseC();
    return saveFuncFn(out);
  }
  
  //: Get prefered IO type.
  // This specifies the native class for representing the data in a file. <p>
  // typeid(void) if none, indicates an indirect driver.
  
  const std::type_info &FileFormatGenericBodyC::DefaultType() const
  { return *formatType; }
  
  //: Find the priority of the format. the higher the better.
  // Default is zero.

  IntT FileFormatGenericBodyC::Priority() const  { 
    return priority; 
  }
  
  //: Test if format is a fully streamable.
  // i.e. check if you can read/write more than object object.
  // (Default value of this function is true).
  
  bool FileFormatGenericBodyC::IsStream() const {  
    return aStream; 
  }
  
  
}
