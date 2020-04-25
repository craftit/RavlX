// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_DVSFORMAT_HEADER
#define RAVLIMAGE_DVSFORMAT_HEADER 1
////////////////////////////////////////////////////////////
//! author="Simon Ennis"
//! docentry="Ravl.API.Images.Video.Video IO"
//! date="29/10/98"
//! rcsid="$Id: dvsFormat.hh 5240 2005-12-06 17:16:50Z plugger $"
//! file="Ravl/DVSFileFormat/dvsFormat.hh"
//! example=exImgSeq.cc
//! lib=RavlRawVidIO

#include "Ravl/DP/FileFormat.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/FloatYPbPr422BT709Value.hh"
#include "Ravl/Image/NewGrabfileReader.hh"
#include "Ravl/Image/NewGrabfileWriter.hh"

using namespace RavlImageN;

namespace RavlImageN {

  //! userlevel=Develop
  //: DVSYUV422 File format information.
  
  class FileFormatDVSYPbPr422BodyC 
    : public FileFormatBodyC 
  {
  public:
    FileFormatDVSYPbPr422BodyC(const StringC &vName,const Index2dC &vSize);
    //: Constructor.
    
    virtual const type_info &ProbeLoad(IStreamC &in,const type_info &obj_type) const;
    //: Is stream in std stream format ?
    
    virtual const type_info &ProbeLoad(const StringC &filename,IStreamC &in,const type_info &obj_type) const;
    //: Probe for load.
    
    virtual const type_info &ProbeSave(const StringC &filename,const type_info &obj_type,bool forceFormat) const;
    //: Probe for Save.
    
    virtual DPIPortBaseC CreateInput(IStreamC &in,const type_info &obj_type) const;
    //: Create a input port for loading.
    // This creates a plain YUV stream.
    // Will create an Invalid port if not supported.
  
    virtual DPOPortBaseC CreateOutput(OStreamC &out,const type_info &obj_type) const;
    //: Create a output port for saving.
    // This creates a plain YUV stream.
    // Will create an Invalid port if not supported.
    
    virtual DPIPortBaseC CreateInput(const StringC &filename,const type_info &obj_type) const;
    //: Create a input port for loading from file 'filename'.
    // Will create an Invalid port if not supported. <p>
    
    virtual DPOPortBaseC CreateOutput(const StringC &filename,const type_info &obj_type) const;
    //: Create a output port for saving to file 'filename'..
    // Will create an Invalid port if not supported. <p>
    
    virtual const type_info &DefaultType() const; 
    //: Get prefered IO type.
    
    virtual IntT Priority() const { return 1; }
    //: Find the priority of the format. the higher the better.
    // Default is zero, this is better than the default (streams.)
    
    virtual bool IsStream() const { return true; }
    //: Test if format is a fully streamable.
    // i.e. check if you can read/write more than object object.

    virtual ~FileFormatDVSYPbPr422BodyC();
    
  protected:
    StringC vName;  // Variant name.
    StringC filename;
    Index2dC vSize; // Variant image size.
    mutable GrabfileReaderC* file_reader_pointer;
    mutable GrabfileWriterC* file_writer_pointer;
    mutable IntT cmode,bformat,vmode;
    //mutable CardModeC mode,modesave;
  };
  
 /* /////////////////////////////
 //! userlevel=Develop
  //: DVSRGB File format information.
  
  class FileFormatDVSRGBBodyC 
    : public FileFormatBodyC 
  {
  public:
    FileFormatDVSRGBBodyC(const StringC &vName,const Index2dC &vSize);
    //: Constructor.
    
    virtual const type_info &ProbeLoad(IStreamC &in,const type_info &obj_type) const;
    //: Is stream in std stream format ?
    
    virtual const type_info &ProbeLoad(const StringC &filename,IStreamC &in,const type_info &obj_type) const;
    //: Probe for load.
    
    virtual const type_info &ProbeSave(const StringC &filename,const type_info &obj_type,bool forceFormat) const;
    //: Probe for Save.
    
    virtual DPIPortBaseC CreateInput(IStreamC &in,const type_info &obj_type) const;
    //: Create a input port for loading.
    // This creates a plain YUV stream.
    // Will create an Invalid port if not supported.
  
    virtual DPOPortBaseC CreateOutput(OStreamC &out,const type_info &obj_type) const;
    //: Create a output port for saving.
    // This creates a plain YUV stream.
    // Will create an Invalid port if not supported.
    
    virtual DPIPortBaseC CreateInput(const StringC &filename,const type_info &obj_type) const;
    //: Create a input port for loading from file 'filename'.
    // Will create an Invalid port if not supported. <p>
    
    virtual DPOPortBaseC CreateOutput(const StringC &filename,const type_info &obj_type) const;
    //: Create a output port for saving to file 'filename'..
    // Will create an Invalid port if not supported. <p>
    
    virtual const type_info &DefaultType() const; 
    //: Get prefered IO type.
    
    virtual IntT Priority() const { return 1; }
    //: Find the priority of the format. the higher the better.
    // Default is zero, this is better than the default (streams.)
    
    virtual bool IsStream() const { return true; }
    //: Test if format is a fully streamable.
    // i.e. check if you can read/write more than object object.

    virtual ~FileFormatDVSRGBBodyC();
    
  protected:
    StringC vName;  // Variant name.
    StringC filename;
    Index2dC vSize; // Variant image size.
    mutable GrabfileReader* file_reader_pointer;
    mutable GrabfileWriter* file_writer_pointer;
    mutable CardModeC mode,modesave;
  };
*/
////////////////////////////////////////////////


  //! userlevel=Advanced
  //: Create an instance of a DVSYUV422 File Format.
  
  class FileFormatDVSYPbPr422C : public FileFormatC<ImageC<FloatYPbPr422BT709ValueC> > {
  public:
    FileFormatDVSYPbPr422C(const StringC &vName,const Index2dC &vSize)
      : FileFormatC<ImageC<FloatYPbPr422BT709ValueC> >(*new FileFormatDVSYPbPr422BodyC(vName,vSize))
      {}
  };
/*
  //! userlevel=Advanced
  //: Create an instance of a DVSYUV422 File Format.
  class FileFormatDVSRGBC : public FileFormatC<ImageC<ByteRGBValueC> > {
  public:
    FileFormatDVSRGBC(const StringC &vName,const Index2dC &vSize)
      : FileFormatC<ImageC<ByteRGBValueC> >(*new FileFormatDVSRGBBodyC(vName,vSize))
      {}
  };
*/
  
}
#endif
