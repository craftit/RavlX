// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2019, ReactAI Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_IMGIOOPENCVFORMAT_HEADER
#define RAVLIMAGE_IMGIOOPENCVFORMAT_HEADER 1
////////////////////////////////////////////////////////////
//! file="Ravl/src/Contrib/OpenCV/ImgIOOpenCVFormat.hh"
//! lib=RavlOpenCV
//! author="Charles Galambos"
//! docentry="Ravl.API.Images.IO.Formats"
//! date="12/02/2019"
//! userlevel=Develop

#include "Ravl/DP/FileFormat.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteBGRValue.hh"

namespace RavlImageN {
  //! userlevel=Develop
  //: OpenCV format information.

  class FileFormatOpenCVBodyC
    : public FileFormatBodyC
  {
  public:
    FileFormatOpenCVBodyC();
    //: Default constructor.

    virtual const std::type_info &ProbeLoad(IStreamC &in,const std::type_info &obj_type) const;
    //: Is stream in std stream format ?

    virtual const std::type_info &ProbeLoad(const StringC &filename,IStreamC &in,const std::type_info &obj_type) const;
    //: Probe for load.

    virtual const std::type_info &ProbeSave(const StringC &filename,const std::type_info &obj_type,bool forceFormat) const;
    //: Probe for Save.

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
    //: Create a input port for loading.
    // Will create an Invalid port if not supported.

    virtual DPOPortBaseC CreateOutput(OStreamC &out,const std::type_info &obj_type) const;
    //: Create a output port for saving.
    // Will create an Invalid port if not supported.

    virtual const std::type_info &DefaultType() const;
    //: Get preferred IO type.

    virtual IntT Priority() const { return -1; }
    //: Find the priority of the format. the higher the better.
    // Default is zero, this is better than the default (streams.)

    virtual bool IsStream() const { return true; }
    //: Test if format is a fully streamable.
    // i.e. check if you can read/write more than object object.

  };

  /////////////////////////////
  //! userlevel=Advanced
  //: Create an instance of opencv File Format.

  class FileFormatOpenCVC : public FileFormatC<ImageC<ByteBGRValueC> > {
  public:
    FileFormatOpenCVC()
      : FileFormatC<ImageC<ByteBGRValueC> >(*new FileFormatOpenCVBodyC())
      {}
  };

}
#endif
