// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2012, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_FILEFORMATSTREAMMETA_HEADER
#define RAVL_FILEFORMATSTREAMMETA_HEADER 1
////////////////////////////////////////////////////////
//! docentry="Ravl.API.Core.IO.Formats" 
//! lib=RavlIO
//! file="Ravl/Core/IO/FileFormatStreamMeta.hh"
//! author="Charles Galambos"
//! date="04/07/2012"
//! userlevel=Default

#include "Ravl/DP/FileFormat.hh"
#include "Ravl/DP/FileIO.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/Hash.hh"

namespace RavlN {
  
  /////////////////////////////
  //: Stream File Format.
  //! userlevel=Develop
  
  class FileFormatStreamMetaBodyC
    : public FileFormatBodyC 
  {
  public:
    FileFormatStreamMetaBodyC();
    //: Default constructor.
    
    virtual const std::type_info &ProbeLoad(IStreamC &in,const std::type_info &obj_type) const;
    //: Is stream in std stream format ?
    
    virtual const std::type_info &ProbeLoad(const StringC &filename,IStreamC &in,const std::type_info &obj_type) const;
    
    virtual const std::type_info &ProbeSave(const StringC &filename,const std::type_info &/*obj_type*/,bool forceFormat) const;
    
    virtual DPIPortBaseC CreateInput(IStreamC &in,const std::type_info &obj_type) const;
    //: Create a input port for loading.
    // Will create an Invalid port if not supported.
    
    virtual DPOPortBaseC CreateOutput(OStreamC &out,const std::type_info &obj_type) const;
    //: Create a output port for saving.
    // Will create an Invalid port if not supported.
    
    virtual const std::type_info &DefaultType() const;
    //: Get preferred IO type.
    
    virtual bool IsStream() const;
    //: Just to make it clear its a streamable format.
    
    bool RegisterFormat(FileFormatBaseC &fileformat);
    //: Register format

  protected:
    HashC<StringC,FileFormatBaseC> m_class2format;
    mutable HashC<const char *,const std::type_info *> m_type2use;
  };
  
  
  /////////////////////////////
  //! userlevel=Normal
  //: Create an instance of a stream File Format.
  
  class FileFormatStreamMetaC : public FileFormatBaseC {
  public:
    FileFormatStreamMetaC()
      : FileFormatBaseC(*new FileFormatStreamMetaBodyC())
    {}

  protected:
    inline FileFormatStreamMetaBodyC &Body()
    { return dynamic_cast<FileFormatStreamMetaBodyC &>(DPEntityC::Body()); }

    inline const FileFormatStreamMetaBodyC &Body() const
    { return dynamic_cast<const FileFormatStreamMetaBodyC &>(DPEntityC::Body()); }

  public:
    bool RegisterFormat(FileFormatBaseC &fileformat)
    { return Body().RegisterFormat(fileformat); }
    //: Register format

  };

  FileFormatStreamMetaC &DefaultFormatStreamMeta();
  //: Access default format

  bool RegisterFormatStreamMeta(FileFormatBaseC &fileformat);
  //: Register file format.

}


#endif
