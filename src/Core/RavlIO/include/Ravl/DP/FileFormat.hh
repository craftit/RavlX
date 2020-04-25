// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_FILEFORMAT_HEADER
#define RAVL_FILEFORMAT_HEADER 1
//////////////////////////////////////////////////
//! example=exFileFormat.cc
//! docentry="Ravl.API.Core.IO.Formats" 
//! lib=RavlIO
//! file="Ravl/Core/IO/FileFormat.hh"
//! author="Charles Galambos"
//! date="13/08/1998"
//! rcsid="$Id$"
//! userlevel=Default

#include "Ravl/Stream.hh"
#include "Ravl/DP/Entity.hh"
#include "Ravl/DP/Port.hh"
#include "Ravl/DP/Pipes.hh"

namespace RavlN {
  template <class DataT> class DListC;
  template <class KeyT,class DataT> class HashC;
  template <class DataT> class FileFormatC;
  
  class FileFormatBaseC;
  class FileFormatDescC;
  class DPConverterBaseC;
  
  ///////////////////////////////////
  //! userlevel=Develop
  //: File format implementation.
  // To add new file formats to the system derive a new class from this one and
  // provide new Probe(), CreateInput/CreateOutput, and DefaultType functions. Then
  // create a new handle class for your format and declare it as a global variable.
  
  class FileFormatBodyC 
    : public DPEntityBodyC 
  {
  public:
    FileFormatBodyC(StringC nformatName = "",StringC nDesc = "None.",bool pubList = true);
    //: Constructor.
    // 'nformatName' is the name of the file format being registered.
    // pubList, Indicates if format should be entered into the public 
    // database.
    
    ~FileFormatBodyC();
    //: Destructor
    
    const StringC &Name() const { return formatName; }
    //: Name of file format.
    
    const StringC &Description() const { return desc; }
    //: Description of fileformat.
    
    virtual const std::type_info &ProbeLoad(IStreamC &in,const std::type_info &obj_type) const;
    //: Is stream in this format ?
    // This asks the question: Can stream 'in' be loaded into object 'obj_type' ?
    // typeid(void) == no, otherwise returns the preferred load type.
    
    virtual const std::type_info &ProbeLoad(const StringC &filename,IStreamC &in,const std::type_info &obj_type) const;
    //: Is stream in this format ?
    // This asks the question: Can stream 'in' be loaded into object 'obj_type' ?
    // If used with a empty filename asks: Can load this type of object ?
    // typeid(void) == no, otherwise returns the preferred load type.
    
    inline const std::type_info &ProbeLoad(const StringC &filename,const std::type_info &obj_type) const;
    //: Is stream in this format ?
    // This asks the question: Can stream 'in' be loaded into object 'obj_type' ?
    // If used with a empty filename asks: Can load this type of object ?
    // typeid(void) == no, otherwise returns the preferred load type.
    
    inline const std::type_info &ProbeLoad(const std::type_info &obj_type) const;
    //: Is stream in this format ?
    // Alias for ProbeLoad("",IStreamC(),obj_type);
    // typeid(void) == no, otherwise returns the preferred load type.
    
    virtual const std::type_info &ProbeSave(const StringC &filename,const std::type_info &obj_type,bool forceFormat) const;
    //: Is Filename in this format ?
    // This asks the question: Can file 'filename' be saved with object 'obj_type' ?
    // If used with a empty filename asks: Can save this type of object ?
    // typeid(void) == no, otherwise returns the preferred save type.
    
    inline const std::type_info &ProbeSave(const std::type_info &obj_type) const;
    //: Alias for ProbeSave("",IStreamC(),obj_type);
    // typeid(void) == no, otherwise returns the preferred save type.
  
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
    
    virtual StringC TypenameAlias() const;
    //: Alias for typename of default type.
    // Returns empty string if not set.

    static bool ReadString(IStreamC &is,StringC &buff,UIntT maxLen = 256);
    //: read a string form the input.
    // returns false if the stream contains non text charters or is longer
    // than the given length.
    // This will also restore the file read position after the read.
    
    static StringC ExtractDevice(const StringC &nm);
    //: Get the device from the full name.
    // For use in '@X:' style device formats.
    
    static StringC ExtractParams(const StringC &nm);
    //: Get the device from the full name.
    // For use in '@X:' style device formats.

    static StringC Extension(const StringC &afn);
    //: Return the extension of filename 'afn'.
    
  protected:
    StringC formatName;  // File format name.
    StringC desc;        // Description of file format.
  };
  
  ///////////////////////////////////////
  //! userlevel=Advanced
  //: File format base handle.
  // This class acts as a handle for all Ravl file formats.
  
  class FileFormatBaseC : public DPEntityC {
  public:
    FileFormatBaseC() 
      : DPEntityC(true)
      {}
    //: Default constructor.
    
    FileFormatBaseC(bool forLoad,StringC name,const std::type_info &objtype);
    //: Save constructor.
    // Find a file format which supports the requested format from the system
    // registry.
    // if not suitable format exists it will create an invalid handle. <p>
    
    FileFormatBaseC(bool forLoad,const StringC &filename,StringC formName,const std::type_info &objtype);
    //: Load constructor.
    // Find a file format which supports the requested format from the system
    // registry.
    // if not suitable format exists it will create an invalid handle. <p>
    
    FileFormatBaseC(IStreamC &in,StringC formName,const std::type_info &objtype);
    //: Load constructor.
    // Find a file format which supports the requested format from the system
    // registry.
    // if not suitable format exists it will create an invalid handle. <p>
    
    inline const StringC &Name() const;
    //: Name of file format.
    
    inline const StringC &Description() const;
    //: Description of fileformat.
    
    inline const std::type_info &ProbeLoad(IStreamC &in,const std::type_info &obj_type) const;
    //: Is stream in this format ?
    // This asks the question: Can stream 'in' be loaded into object 'obj_type' ?
    // Returns a suggested load type. (Which may or may not match 'obj_type'.),
    // This will be typeid(void), if failed.
    
    inline const std::type_info &ProbeLoad(const StringC &filename,IStreamC &in,const std::type_info &obj_type) const;
    //: Is Filename in this format (for load) ?
    // This asks the question: Can file 'filename' be loaded with object 'obj_type' ?
    // If used with a empty filename then in is invalid and it
    //   asks: Can load this type of object ?
    // Returns a suggested load type. (Which may or may not match 'obj_type'.),
    // This will be typeid(void), if failed.
    
    inline const std::type_info &ProbeLoad(const StringC &filename,const std::type_info &obj_type) const;
    //: Is Filename in this format (for load) ?
    // Returns a suggested load type. (Which may or may not match 'obj_type'.),
    // This will be typeid(void), if failed.
    
    inline const std::type_info &ProbeSave(const StringC &filename,const std::type_info &obj_type,bool forceFormat) const;
    //: Is Filename in this format ?
    // This asks the question: Can file 'filename' be saved with object 'obj_type' ?
    // This uses extension to check consistency.
    // If used with a empty filename asks: Can save this type of object ?
    // Returns a suggested load type. (Which may or may not match 'obj_type'.),
    // This will be typeid(void), if failed.
    
    inline const std::type_info &ProbeLoad(const std::type_info &obj_type) const;
    //: Is Filename in this format (for load) ?
    // Alias for ProbeLoad("",IStreamC(),obj_type);
    // Returns a suggested load type. (Which may or may not match 'obj_type'.),
    // This will be typeid(void), if failed.
    
    inline const std::type_info &ProbeSave(const std::type_info &obj_type) const;
    //: Is Filename in this format ?
    // Alias for ProbeSave("",obj_type);
    
    inline DPIPortBaseC CreateInput(const StringC &filename,const std::type_info &obj_type) const;
    //: Create a input port for loading.
    // Will create an Invalid port if not supported. <p>
    // The default version of this functions opens the file and passes
    // the stream to CreateInput(IStreamC ....)
    
    inline DPOPortBaseC CreateOutput(const StringC &filename,const std::type_info &obj_type) const;
    //: Create a output port for saving.
    // Will create an Invalid port if not supported. <p>
    // The default version of this functions opens the file and passes
    // the stream to CreateOutput(OStreamC ....)
    
    inline DPIPortBaseC CreateInput(IStreamC &in,const std::type_info &obj_type) const;
    //: Create a input port for loading.
    // Will create an Invalid port if not supported.
    
    inline DPOPortBaseC CreateOutput(OStreamC &out,const std::type_info &obj_type) const;
    //: Create a output port for saving.
    // Will create an Invalid port if not supported.
    
    inline const std::type_info &DefaultType() const;
    //: Get preferred IO type.
    // typeid(void) if none.
    
    inline IntT Priority() const;
    //: Find the priority of the format. the higher the better.
    // Default is zero.
    
    inline bool IsStream() const;
    //: Test if format is a fully streamable.
    // i.e. check if you can read/write more than object object.

    StringC TypenameAlias() const
    { return Body().TypenameAlias(); }
    //: Alias for typename of default type.
    // Returns empty string if not set.

  protected:
    FileFormatBaseC(FileFormatBodyC &bod) 
      : DPEntityC(bod)
      {}
    //: Body constructor.
  
    inline FileFormatBodyC &Body() 
      { return static_cast<FileFormatBodyC &>(DPEntityC::Body()); }
    
    inline const FileFormatBodyC &Body() const
      { return static_cast<const FileFormatBodyC &>(DPEntityC::Body()); }
    
    friend class FileFormatBodyC;
  };
  
  //////////////////////////////////////////////////
  //! userlevel=Normal
  //: Templated File format
  // This class provides access to all file formats which deal with a 
  // particular type.
  
  template<class DataT>
  class FileFormatC : public FileFormatBaseC {
  public:
    FileFormatC() {}
    //: Default constructor.
    // This creates an invalid handle.
    
    FileFormatC(bool forLoad,StringC name)
      : FileFormatBaseC(forLoad,name,typeid(DataT))
      {}
    //: Construct from format name.
    // Find a file format which supports the requested format.
    // if not suitable format exists it will create an invalid handle. <p>
    // Uses FileFormatBodyC::FindFormat(...).
    
    FileFormatC(bool forLoad,const StringC &filename,const StringC &formName)
      : FileFormatBaseC(forLoad,filename,formName,typeid(DataT))
      {}
    //: Load constructor.
    // Find a file format which supports the requested format.
    // if not suitable format exists it will create an invalid handle. <p>
    // Uses FileFormatBodyC::FindFormat(...).
    
    FileFormatC(IStreamC &in,const StringC &formName)
      : FileFormatBaseC(in,formName,typeid(DataT))
      {}
    //: Load constructor.
    // Find a file format which supports the requested format.
    // if not suitable format exists it will create an invalid handle. <p>
    // Uses FileFormatBodyC::FindFormat(...).
    
    FileFormatC(FileFormatBodyC &bod)
      : FileFormatBaseC(bod)
      {}
    //: Body Constructor.
    
    inline const std::type_info &ProbeLoad(IStreamC &in) const;
    //: Is stream in this format ?
    // This asks the question: Can stream 'in' be loaded into 'DataT' type ?
    
    inline const std::type_info &ProbeLoad(const StringC &filename,IStreamC &in) const;
    //: Is Filename in this format ?
    // This asks the question: Can file 'filename' be loaded with object 'DataT' ?
    // If used with a empty filename asks: Can load this type of object ?
    
    inline const std::type_info &ProbeSave(const StringC &filename,bool forceFmt = false) const;
    //: Is Filename in this format ?
    // This asks the question: Can file 'filename' be saved with object 'DataT' ?
    // If used with a empty filename asks: Can save this type of object ?
    
    inline DPIPortC<DataT> CreateInput(const StringC &filename) const;
    //: Create a input port for loading.
    // Will create an Invalid port if not supported.
    
    inline DPOPortC<DataT> CreateOutput(const StringC &filename) const;
    //: Create a output port for saving.
    // Will create an Invalid port if not supported.
    
    inline DPIPortC<DataT> CreateInput(IStreamC &in) const;
    //: Create a input port for loading.
    // Will create an Invalid port if not supported.
    
    inline DPOPortC<DataT> CreateOutput(OStreamC &out) const;
    //: Create a output port for saving.
    // Will create an Invalid port if not supported.
    
  };
  
  
  //////////////////////////////////////////////////
  
  inline 
  const std::type_info &
  FileFormatBodyC::ProbeLoad(const std::type_info &obj_type) const {
    IStreamC tmp;
    return ProbeLoad("",tmp,obj_type);
  }
  
  inline 
  const std::type_info &
  FileFormatBodyC::ProbeLoad(const StringC &filename,const std::type_info &obj_type) const {
    if(filename.IsEmpty())
      return ProbeLoad(obj_type);
    IStreamC in(filename);
    return ProbeLoad(filename,in,obj_type);
  }
  
  inline 
  const std::type_info &
  FileFormatBodyC::ProbeSave(const std::type_info &obj_type) const
  { return ProbeSave("",obj_type,true); }
  
  //////////////////////////////////////////////////
  
  inline 
  const StringC &
  FileFormatBaseC::Name() const
  { return Body().Name(); }
  
  inline 
  const StringC &
  FileFormatBaseC::Description() const
  { return Body().Description(); }
  
  inline 
  const std::type_info &FileFormatBaseC::ProbeLoad(IStreamC &in,const std::type_info &obj_type) const
  { return Body().ProbeLoad(in,obj_type); }
  
  inline 
  const std::type_info &FileFormatBaseC::ProbeLoad(const StringC &filename,IStreamC &in,const std::type_info &obj_type) const
  { return Body().ProbeLoad(filename,in,obj_type); }
  
  inline 
  const std::type_info &FileFormatBaseC::ProbeLoad(const StringC &filename,const std::type_info &obj_type) const
  { return Body().ProbeLoad(filename,obj_type); }
  
  inline 
  const std::type_info &FileFormatBaseC::ProbeSave(const StringC &filename,const std::type_info &obj_type,bool forceFormat) const
  { return Body().ProbeSave(filename,obj_type,forceFormat); }
  
  inline 
  const std::type_info &FileFormatBaseC::ProbeLoad(const std::type_info &obj_type) const
  { return Body().ProbeLoad(obj_type); }
  
  inline 
  const std::type_info &FileFormatBaseC::ProbeSave(const std::type_info &obj_type) const
  { return Body().ProbeSave(obj_type); }
  
  inline 
  DPIPortBaseC FileFormatBaseC::CreateInput(const StringC &filename,const std::type_info &obj_type) const
  { return Body().CreateInput(filename,obj_type); }
  
  inline 
  DPOPortBaseC FileFormatBaseC::CreateOutput(const StringC &filename,const std::type_info &obj_type) const
  { return Body().CreateOutput(filename,obj_type); }

  inline 
  DPIPortBaseC FileFormatBaseC::CreateInput(IStreamC &in,const std::type_info &obj_type) const
  { return Body().CreateInput(in,obj_type); }
  
  inline
  DPOPortBaseC FileFormatBaseC::CreateOutput(OStreamC &out,const std::type_info &obj_type) const
  { return Body().CreateOutput(out,obj_type); }

  inline
  const std::type_info &FileFormatBaseC::DefaultType() const
  { return Body().DefaultType(); }

  inline 
  IntT FileFormatBaseC::Priority() const
  { return Body().Priority(); }

  inline 
  bool FileFormatBaseC::IsStream() const
  { return Body().IsStream(); }

  //////////////////////////////////////////////////
  template<class DataT>
  inline 
  const std::type_info &
  FileFormatC<DataT>::ProbeLoad(IStreamC &in) const
  { return FileFormatBaseC::ProbeLoad(in,typeid(DataT)); }

  template<class DataT>
  inline 
  const std::type_info &
  FileFormatC<DataT>::ProbeLoad(const StringC &fn,IStreamC &in) const
  { return FileFormatBaseC::ProbeLoad(fn,in,typeid(DataT)); }

  template<class DataT>
  inline 
  const std::type_info &
  FileFormatC<DataT>::ProbeSave(const StringC &filename,bool forceFormat) const
  { return FileFormatBaseC::ProbeSave(filename,typeid(DataT),forceFormat); }

  template<class DataT>
  inline 
  DPIPortC<DataT> 
  FileFormatC<DataT>::CreateInput(const StringC &filename) const
  { return FileFormatBaseC::CreateInput(filename,typeid(DataT)); }

  template<class DataT>
  inline 
  DPOPortC<DataT> 
  FileFormatC<DataT>::CreateOutput(const StringC &filename) const
  { return FileFormatBaseC::CreateOutput(filename,typeid(DataT)); }

  template<class DataT>
  inline 
  DPIPortC<DataT> FileFormatC<DataT>::CreateInput(IStreamC &in) const
  { return FileFormatBaseC::CreateInput(in,typeid(DataT)); }
  
  template<class DataT>
  inline 
  DPOPortC<DataT>  FileFormatC<DataT>::CreateOutput(OStreamC &out) const
  { return FileFormatBaseC::CreateOutput(out,typeid(DataT)); }

}

#endif
