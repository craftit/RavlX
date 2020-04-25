// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2012, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_FILEFORMATBINSTREAMMETA_HEADER
#define RAVL_FILEFORMATBINSTREAMMETA_HEADER 1
////////////////////////////////////////////////////////
//! docentry="Ravl.API.Core.IO.Formats"
//! lib=RavlIO
//! file="Ravl/Core/IO/FileFormatBinStreamMeta.hh"
//! author="Charles Galambos"
//! date="4/07/2012"
//! userlevel=Default

#include "Ravl/DP/FileFormat.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/Hash.hh"
#include "Ravl/HSet.hh"

namespace RavlN {
  /////////////////////////////
  //: Binary stream file format.
  //! userlevel=Develop

  class FileFormatBinStreamMetaBodyC
   : public FileFormatBodyC
  {
  public:
    FileFormatBinStreamMetaBodyC(bool pubList);
    //: Constructor.

    FileFormatBinStreamMetaBodyC(const StringC &formatId,const StringC &formatDescriptor,bool pubList = true);
    //: Constructor with full format info.

    virtual const std::type_info &ProbeLoad(IStreamC &in,const std::type_info &/*obj_type*/) const;
    //: Is stream in std stream format ?

    virtual const std::type_info &ProbeLoad(const StringC &filename,IStreamC &in,const std::type_info &obj_type) const;

    virtual const std::type_info &ProbeSave(const StringC &filename,const std::type_info &/*obj_type*/,bool forceFormat) const;

    virtual DPIPortBaseC CreateInput(IStreamC &in,const std::type_info &obj_type) const;
    //: Create a input port for loading.
    // Will create an Invalid port if not supported.

    virtual DPOPortBaseC CreateOutput(OStreamC &out,const std::type_info &obj_type) const;
    //: Create a output port for saving.
    // Will create an Invalid port if not supported.


    DPIPortBaseC CreateInput(const StringC &filename,const std::type_info &obj_type) const;
    //: Create a input port for loading.
    // Will create an Invalid port if not supported.

    DPOPortBaseC CreateOutput(const StringC &filename,const std::type_info &obj_type) const;
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
    HSetC<StringC> m_ext;
    mutable HashC<const char *,const std::type_info *> m_type2use;
  };

  /////////////////////////////
  //! userlevel=Normal
  //: Binary stream file format.

  class FileFormatBinStreamMetaC
    : public FileFormatBaseC
  {
  public:
    FileFormatBinStreamMetaC()
      : FileFormatBaseC(*new FileFormatBinStreamMetaBodyC(true))
    {}

    FileFormatBinStreamMetaC(const StringC &formatId,const StringC &formatDescriptor)
      : FileFormatBaseC(*new FileFormatBinStreamMetaBodyC(formatId,formatDescriptor))
    {}
    //: Construct will format id and descriptor

  protected:
    inline FileFormatBinStreamMetaBodyC &Body()
    { return dynamic_cast<FileFormatBinStreamMetaBodyC &>(DPEntityC::Body()); }

    inline const FileFormatBinStreamMetaBodyC &Body() const
    { return dynamic_cast<const FileFormatBinStreamMetaBodyC &>(DPEntityC::Body()); }

  public:
    bool RegisterFormat(FileFormatBaseC &fileformat)
    { return Body().RegisterFormat(fileformat); }
    //: Register format
  };

  FileFormatBinStreamMetaC &DefaultFormatBinStreamMeta();
  //: Access default format

  bool RegisterFormatBinStreamMeta(FileFormatBaseC &fileformat);
  //: Register file format.
}

#endif
