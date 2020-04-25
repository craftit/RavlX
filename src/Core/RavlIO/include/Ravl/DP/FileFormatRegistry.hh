// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_FORMATREGISTRY_HEADER
#define RAVL_FORMATREGISTRY_HEADER 1
/////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Core.IO.Formats" 
//! author="Charles Galambos"
//! lib=RavlIO
//! file="Ravl/Core/IO/FileFormatRegistry.hh"

#include "Ravl/config.h"
#if RAVL_USE_IO_AUTO_TYPECONVERTER
#include "Ravl/DP/TypeConverter.hh"
#endif
#include "Ravl/DP/FileFormat.hh"
#include "Ravl/DP/FileFormatIO.hh"
#include "Ravl/DList.hh"
#include "Ravl/Hash.hh"

namespace RavlN {

  class RCWrapAbstractC;
  
  //! userlevel=Develop
  //: Registry of file formats.
  
  class FileFormatRegistryBodyC 
    : public RCBodyC
  {
  public:
    FileFormatRegistryBodyC();
    //: Default constructor.

#if RAVL_USE_IO_AUTO_TYPECONVERTER
    FileFormatRegistryBodyC(const TypeConverterC &aTypeConveter);
    //: Constructor.
#endif

    bool Insert(FileFormatBaseC &ff);
    //: Add a new format to the registry.

    bool Remove(FileFormatBaseC &ff);
    //: Remove an old format from the registry.
    
    DListC<FileFormatBaseC> &Formats() 
      { return formats; }
    //: Access list of available file formats.
    
    HashC<StringC,DListC<FileFormatBaseC> > &FormatByName()
      { return formatByName; }
    //: Access list of available file formats by name.
    
    FileFormatBaseC FindFormat(bool forLoad,const StringC &formatName,const std::type_info &atype = typeid(void),bool useInDirect = true);
    //: Find a format. 
    // This can be input or output.
    
    FileFormatBaseC FindFormatFile(bool forLoad,const StringC &filename,const std::type_info &atype = typeid(void),StringC formatName = "",bool useInDirect = true);
    //: Find the format of the file.
    // 'stream' is opened by this function where appropriate.
    // This can be input or output.
    
    FileFormatBaseC FindFormat(IStreamC &in,const std::type_info &atype =typeid(void),StringC formatName = "",bool useInDirect = true);
    //: Find the format of the file.
    // This can only be input.
    
    bool FindInputFormat(FileFormatDescC  &fmtInfo,StringC filename,IStreamC &stream,StringC format,const std::type_info &obj_type,bool verbose = false);
    //: Search for input format.
    
    bool FindOutputFormat(FileFormatDescC &fmtInfo,StringC filename,StringC format,const std::type_info &obj_type,bool verbose = false);
    //: Search for output format.
    
    DPIPortBaseC CreateInput(StringC filename,StringC format,const std::type_info &obj_type,bool verbose = false);
    //: Create an input pipe.
    // if obj_type is typeid(void), then the default load type of the file is used.
    
    DPOPortBaseC CreateOutput(StringC filename,StringC format,const std::type_info &obj_type,bool verbose = false);
    //: Create an input pipe.
    
    DPIPortBaseC CreateInput(IStreamC &from,StringC format,const std::type_info &obj_type,bool verbose = false);
    //: Create an input pipe.
    // if obj_type is typeid(void), then the default load type of the file is used.
    
    DPOPortBaseC CreateOutput(OStreamC &to,StringC format,const std::type_info &obj_type,bool verbose = false);
    //: Create an input pipe.

    bool Load(const StringC &filename,RCWrapAbstractC &obj,StringC fileformat,bool verbose);
    //: Load to an abstract object handle.
    // NB. an instace of TypeInfoInstC must exists for the contained class if this
    // is to work.
    
    bool Save(const StringC &filename,const RCWrapAbstractC &obj,StringC fileformat,bool verbose);
    //: Save an abstract object handle.
    // NB. an instace of TypeInfoInstC must exists for the contained class if this
    // is to work.
    
    bool Load(IStreamC &strm,RCWrapAbstractC &obj,StringC fileformat,bool verbose);
    //: Load to an abstract object handle.
    // NB. an instace of TypeInfoInstC must exists for the contained class if this
    // is to work.
    
    bool Save(OStreamC &strm,const RCWrapAbstractC &obj,StringC fileformat,bool verbose);
    //: Save an abstract object handle.
    // NB. an instace of TypeInfoInstC must exists for the contained class if this
    // is to work.
    
    DListC<FileFormatBaseC> ListFormats(bool forLoad,const StringC &fileFormat = StringC(""),const std::type_info &typespec = typeid(void));
    //: List all file formats that support the given type.
    // forLoad == true, then for load. forLoad == false, then for save. <p>  
    // If fileFormat string is empty then all file formats are listed. <p>
    // If typespec is void then all types are listed.
    
    FileFormatBaseC Identify(const StringC &afile);
    //: Identify a file.
    // If file format is unrecognised the returned FileFormatBaseC will be an invalid handle.
    
    FileFormatBaseC Identify(IStreamC &is);
    //: Identify a stream.
    // If file format is unrecognised the returned FileFormatBaseC will be an invalid handle.
    //! userlevel=Develop
    
  protected:
    DListC<FileFormatBaseC> formats;
    HashC<StringC,DListC<FileFormatBaseC> > formatByName;
    
#if RAVL_USE_IO_AUTO_TYPECONVERTER
    TypeConverterC typeConverter;
#endif
    //: Type converter to use.
  };

  //! userlevel=Advanced
  //: Registry of file formats.
  
  class FileFormatRegistryC
    : public RCHandleC<FileFormatRegistryBodyC>
  {
  public:
    FileFormatRegistryC()
      {}
    //: Default constructor.
    // Creates an invalid handle.

    FileFormatRegistryC(bool)
      : RCHandleC<FileFormatRegistryBodyC>(*new FileFormatRegistryBodyC())
      {}
    //: Default constructor.
    // Creates an invalid handle.

#if RAVL_USE_IO_AUTO_TYPECONVERTER
    FileFormatRegistryC(const TypeConverterC &aTypeConveter)
      : RCHandleC<FileFormatRegistryBodyC>(*new FileFormatRegistryBodyC(aTypeConveter))
      {}
    //: Constructor.
#endif
    
    bool Insert(FileFormatBaseC &ff)
      { return Body().Insert(ff); }
    //: Add a new format to the registry.
    
    bool Remove(FileFormatBaseC &ff)
      { return Body().Remove(ff); }
    //: Remove an old format from the registry.

    DListC<FileFormatBaseC> &Formats() 
      { return Body().Formats(); }
    //: Access list of available file formats.
    
    HashC<StringC,DListC<FileFormatBaseC> > &FormatByName()
      { return Body().FormatByName(); }
    //: Access list of available file formats by name.
    
    FileFormatBaseC FindFormat(bool forLoad,const StringC &formatName,const std::type_info &atype = typeid(void),bool useInDirect = true)
      { return Body().FindFormat(forLoad,formatName,atype,useInDirect); }
    //: Find a format. 
    // This can be input or output.
    
    FileFormatBaseC FindFormatFile(bool forLoad,const StringC &filename,const std::type_info &atype = typeid(void),StringC formatName = "",bool useInDirect = true)
      { return Body().FindFormatFile(forLoad,filename,atype,formatName,useInDirect); }
    //: Find the format of the file.
    // This can be input or output.
    
    FileFormatBaseC FindFormat(IStreamC &in,const std::type_info &atype =typeid(void),StringC formatName = "",bool useInDirect = true) 
      { return Body().FindFormat(in,atype,formatName,useInDirect); }
    //: Find the format of the file.
    // This can only be input.
    
    bool FindInputFormat(FileFormatDescC  &fmtInfo,StringC filename,IStreamC &inStream,StringC format,const std::type_info &obj_type,bool verbose = false)
    { return Body().FindInputFormat(fmtInfo,filename,inStream,format,obj_type,verbose); }
    //: Search for input format.
    
    bool FindOutputFormat(FileFormatDescC &fmtInfo,StringC filename,StringC format,const std::type_info &obj_type,bool verbose = false)
      { return Body().FindOutputFormat(fmtInfo,filename,format,obj_type,verbose); }
    //: Search for output format.
    
    DPIPortBaseC CreateInput(StringC filename,StringC format,const std::type_info &obj_type,bool verbose = false)
      { return Body().CreateInput(filename,format,obj_type,verbose); }
    //: Create an input pipe.
    // if obj_type is typeid(void), then the default load type of the file is used.
    
    DPOPortBaseC CreateOutput(StringC filename,StringC format,const std::type_info &obj_type,bool verbose = false)
      { return Body().CreateOutput(filename,format,obj_type,verbose); }
    //: Create an input pipe.
    
    DPIPortBaseC CreateInput(IStreamC &from,StringC format,const std::type_info &obj_type,bool verbose = false)
      { return Body().CreateInput(from,format,obj_type,verbose); }
    //: Create an input pipe.
    // if obj_type is typeid(void), then the default load type of the file is used.
    
    DPOPortBaseC CreateOutput(OStreamC &to,StringC format,const std::type_info &obj_type,bool verbose = false)
      { return Body().CreateOutput(to,format,obj_type,verbose); }
    //: Create an input pipe.
    

    template<class DataT>
      bool Load(const StringC &filename,DataT &obj,StringC fileformat = "",bool verbose = false) {
      DPIPortC<DataT> in(RavlN::BaseLoad(filename,fileformat,typeid(DataT),verbose));
      if(!in.IsValid()) {
#if RAVL_CHECK
	if(verbose) 
#endif
	  cerr << "RavlN::Load(), Failed to load object from file '" << filename << "'\n";
	return false;
      }
      return in.Get(obj);
    }
    //: Load a single object.
    //!param: filename - name of file containing object
    //!param: obj - name of C++ object
    //!param: fileformat - format of object file. If fileformat string is empty, the file format is detected automatically.  To get a list of file formats, type <code>conv -lf</code>
    //!param: verbose -  if true, a message is printed to stdout describing the filename, the format used, the object type being loaded and any status information. 
    // More information <a href="../Tree/Ravl.API.Core.IO.html#LoadSave">here</a>
    
    template<class DataT>
      bool Save(const StringC &filename,const DataT &obj,StringC fileformat = "",bool verbose = false) {
      DPOPortC<DataT> out(RavlN::BaseSave(filename,fileformat,typeid(DataT),verbose));
      if(!out.IsValid()) {
#if RAVL_CHECK
	if(verbose) 
#endif
	  cerr << "RavlN::Save(), Failed to save object to file '" << filename << "'\n";
	return false; // Failed to find format.
      }
      if(!out.Put(obj)) {
	cerr << "RavlN::Save(), Put failed! \n";
	RavlAssert(0);
	return false;
      }
      return true;
    }
    //: Save a single object.
    //!param: filename - name of file containing object
    //!param: obj - name of C++ object
    //!param: fileformat - format of object file. If fileformat string is empty, the file format is detected automatically.  To get a list of file formats, type <code>conv -lf</code>
    //!param: verbose -  if true, a message is printed to stdout describing the filename, the format used, the object type being loaded and any status information. 
    // More information <a href="../Tree/Ravl.API.Core.IO.html#LoadSave">here</a>
    
    template<class DataT>
      bool Load(IStreamC &is,DataT &obj,StringC fileformat = "",bool verbose = false) {
      DPIPortC<DataT> in(RavlN::BaseLoad(is,fileformat,typeid(DataT),verbose));
      if(!in.IsValid()) {
      if(verbose) 
	cerr << "RavlN::Load(), Failed to load object from stream.\n";
      return false;
      }
      return in.Get(obj);
    }
    //: Load a single object from a stream
    //!param: os - stream from which to load object.
    //!param: obj - name of C++ object
    //!param: fileformat - format of object file. If fileformat string is empty, the file format is detected automatically.  To get a list of file formats, type <code>conv -lf</code>
    //!param: verbose -  if true, a message is printed to stdout describing the filename, the format used, the object type being loaded and any status information. 
    // More information <a href="../Tree/Ravl.API.Core.IO.html#LoadSave">here</a>
    
    template<class DataT>
      bool Save(OStreamC &os,const DataT &obj,StringC fileformat = "",bool verbose = false) {
      DPOPortC<DataT> out(RavlN::BaseSave(os,fileformat,typeid(DataT),verbose));
      if(!out.IsValid()) {
	if(verbose) 
	  cerr << "RavlN::Save(), Failed to save object to stream\n";
	return false; // Failed to find format.
      }
      if(!out.Put(obj)) {
	cerr << "RavlN::Save(), Put failed! \n";
	RavlAssert(0);
	return false;
      }
    return true;
    }
    //: Save a single object to a stream
    //!param: os - Where to save the stream to.
    //!param: obj - name of C++ object
    //!param: fileformat - format of object file. If fileformat string is empty, the file format is detected automatically.  To get a list of file formats, type <code>conv -lf</code>
    //!param: verbose -  if true, a message is printed to stdout describing the filename, the format used, the object type being loaded and any status information. 
    // More information <a href="../Tree/Ravl.API.Core.IO.html#LoadSave">here</a>
    
    bool Load(const StringC &filename,RCWrapAbstractC &obj,StringC fileformat,bool verbose)
    { return Body().Load(filename,obj,fileformat,verbose); }
    //: Load to an abstract object handle.
    // NB. an instace of TypeInfoInstC must exists for the contained class if this
    // is to work.
    
    bool Save(const StringC &filename,const RCWrapAbstractC &obj,StringC fileformat,bool verbose)
    { return Body().Save(filename,obj,fileformat,verbose); }
    //: Save an abstract object handle.
    // NB. an instace of TypeInfoInstC must exists for the contained class if this
    // is to work.
    
    bool Load(IStreamC &strm,RCWrapAbstractC &obj,StringC fileformat,bool verbose)
    { return Body().Load(strm,obj,fileformat,verbose); }
    //: Load to an abstract object handle.
    // NB. an instace of TypeInfoInstC must exists for the contained class if this
    // is to work.
    
    bool Save(OStreamC &strm,const RCWrapAbstractC &obj,StringC fileformat,bool verbose)
    { return Body().Save(strm,obj,fileformat,verbose); }
    //: Save an abstract object handle.
    // NB. an instace of TypeInfoInstC must exists for the contained class if this
    // is to work.
    
    DListC<FileFormatBaseC> ListFormats(bool forLoad,const StringC &fileFormat = StringC(""),const std::type_info &typespec = typeid(void))
      { return Body().ListFormats(forLoad,fileFormat,typespec); }
    //: List all file formats that support the given type.
    // forLoad == true, then for load. forLoad == false, then for save. <p>  
    // If fileFormat string is empty then all file formats are listed. <p>
    // If typespec is void then all types are listed.
    
    FileFormatBaseC Identify(const StringC &afile)
      { return Body().Identify(afile); }
    //: Identify a file.
    // If file format is unrecognised the returned FileFormatBaseC will be an invalid handle.
    
    FileFormatBaseC Identify(IStreamC &is)
      { return Body().Identify(is); }
    //: Identify a stream.
    // If file format is unrecognised the returned FileFormatBaseC will be an invalid handle.
    //! userlevel=Develop
    
  };

  FileFormatRegistryC &SystemFileFormatRegistry();
  //: Access the system's format registry.
}

#endif
