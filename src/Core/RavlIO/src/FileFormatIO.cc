// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.Core.IO"
//! lib=RavlIO
//! file="Ravl/Core/IO/FileFormatIO.cc"

// These functions provide an interface to the system file format registry,
// with out having to include 100's of big nasty header files.

#include "Ravl/DP/FileFormatRegistry.hh"

#define DODEBUG 0
#if DODEBUG
#include "Ravl/TypeName.hh"
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  
  class RCWrapAbstactC;
  
  /////////////////////////////////
  //! userlevel=Develop
  //: Base load function.
  // If fileformat string is empty, the file format is detected automaticly.
  // When verbose flag is true, a message is printed to stdout describing the filename, the format
  // used and the object type being loaded. 
  
  DPIPortBaseC BaseLoad(const StringC &filename,
			StringC fformat,
			const std::type_info &obj_type,
			bool verbose) 
  { return SystemFileFormatRegistry().CreateInput(filename,fformat,obj_type,verbose); }
  
  ///////////////////////////////// 
  //! userlevel=Develop
  //: Base save function.
  // If fileformat string is empty, the default file format for the type is used.
  // When verbose flag is true, a message is printed to stdout describing the filename, the format
  // used and the object type being saved. 
  
  DPOPortBaseC BaseSave(const StringC &filename,
			StringC fformat,
			const std::type_info &obj_type,
			bool verbose) 
  { return SystemFileFormatRegistry().CreateOutput(filename,fformat,obj_type,verbose); }
  
  
  /////////////////////////////////
  //! userlevel=Develop
  //: Base load function.
  // If fileformat string is empty, the file format is detected automaticly.
  // When verbose flag is true, a message is printed to stdout describing the filename, the format
  // used and the object type being loaded. 
  
  DPIPortBaseC BaseLoad(IStreamC &is,
			StringC fformat,
			const std::type_info &obj_type,
			bool verbose) 
  { return SystemFileFormatRegistry().CreateInput(is,fformat,obj_type,verbose); }
  
  /////////////////////////////////
  //! userlevel=Develop
  //: Base save function.
  // If fileformat string is empty, the default file format for the type is used.
  // When verbose flag is true, a message is printed to stdout describing the filename, the format
  // used and the object type being saved. 
  
  DPOPortBaseC BaseSave(OStreamC &os,
			StringC fformat,
			const std::type_info &obj_type,
			bool verbose) 
  { return SystemFileFormatRegistry().CreateOutput(os,fformat,obj_type,verbose); }
  
  
  /////////////////////////////////
  //! userlevel=Advanced
  //: Load to an abstract object handle.
  // NB. an instace of TypeInfoInstC must exists for the contained class if this
  // is to work.
  
  bool LoadAbstract(const StringC &filename,RCWrapAbstractC &obj,StringC fileformat,bool verbose)
  { return SystemFileFormatRegistry().Load(filename,obj,fileformat,verbose); }
  
  /////////////////////////////////
  //! userlevel=Advanced
  //: Save an abstract object handle.
  // NB. an instace of TypeInfoInstC must exists for the contained class if this
  // is to work.
  
  bool SaveAbstract(const StringC &filename,const RCWrapAbstractC &obj,StringC fileformat,bool verbose) 
  { return SystemFileFormatRegistry().Save(filename,obj,fileformat,verbose); }
  
  //! userlevel=Advanced
  //: Load to an abstract object handle.
  // NB. an instance of TypeInfoInstC must exists for the contained class if this
  // is to work.
  
  bool LoadAbstract(IStreamC &strm,RCWrapAbstractC &obj,StringC fileformat,bool verbose) { 
    ONDEBUG(std::cerr << "LoadAbstract(IStreamC &,RCWrapAbstractC &), Called.  \n");
    return SystemFileFormatRegistry().Load(strm,obj,fileformat,verbose); 
  }
  
  //! userlevel=Advanced
  //: Save an abstract object handle.
  // NB. an instace of TypeInfoInstC must exists for the contained class if this
  // is to work.
  
  bool SaveAbstract(OStreamC &strm,const RCWrapAbstractC &obj,StringC fileformat,bool verbose) { 
    ONDEBUG(std::cerr << "SaveAbstract(OStreamC &,const RCWrapAbstractC &), Called. Type=" << TypeName(obj.DataType()) << " \n");
    return SystemFileFormatRegistry().Save(strm,obj,fileformat,verbose); 
  }
  
  /////////////////////////////////
  //! userlevel=Advanced
  //: List all file formats that support the given type.
  // If typespec is void then all types are listed.
  
  DListC<FileFormatBaseC> ListFormats(bool forLoad,const StringC &fileFormat,const std::type_info &typespec) 
  { return SystemFileFormatRegistry().ListFormats(forLoad,fileFormat,typespec); }
  
  //////////////////////////////////
  //: Identify a file.
  // If file format is unrecognised FileFormatBaseC will be an invalid handle.
  
  FileFormatBaseC Identify(const StringC &afile)    
  { return SystemFileFormatRegistry().Identify(afile); }
  
  //: Identify a stream.
  // If file format is unrecognised the returned FileFormatBaseC will be an invalid handle.
  
  FileFormatBaseC Identify(IStreamC &istrm) 
  { return SystemFileFormatRegistry().FindFormat(istrm); }
}
