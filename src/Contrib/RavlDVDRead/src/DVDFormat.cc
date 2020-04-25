// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU
// General Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/Contrib/DVDRead/DVDFormat.cc"
//! lib=RavlDVDRead
//! author = "Warren Moore"

#include <stdint.h>
#include "Ravl/DVDFormat.hh"
#include "Ravl/DVDRead.hh"
#include "Ravl/ImgIOMPEG2DVD.hh"
#include "Ravl/Image/MPEG2Demux.hh"
#include "Ravl/DP/SPortAttach.hh"

#define DPDEBUG 0
#if DPDEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN
{
  
  using namespace RavlImageN;
 
  void InitDVDFormat()
  {}
  
  //: Constructor.
  
  FileFormatDVDBodyC::FileFormatDVDBodyC() :
    FileFormatBodyC("dvd", "DVD device. (@DVD)")
  {}
  
  //: Is stream in std stream format ?
  
  const type_info &
  FileFormatDVDBodyC::ProbeLoad(IStreamC &in, const type_info &obj_type) const
  {
    ONDEBUG(cerr << "FileFormatDVDBodyC::ProbeLoad(), Not a DVD. \n");
    return typeid(void); 
  }
  
  const type_info &
  FileFormatDVDBodyC::ProbeLoad(const StringC &filename, IStreamC &in, const type_info &obj_type) const
  {
    // Look for the device symbol
    if(filename.length() == 0)
      return typeid(void);
    if(filename[0] != '@')
      return typeid(void);
    
    // Check it's a DVD device
    StringC device = ExtractDevice(filename);
    if(device != "DVD")
      return typeid(void);

    // Get the title and optional device
    StringC params = ExtractParams(filename);
    IntT delim = params.index('#');
    IntT title = 1;
    if(delim >= 0)
    {
      title = params.before(delim).IntValue();
      params = params.after(delim);
    }
    else
      if (params.length() > 0)
        title = params.IntValue();
    
    // Check the title
    if (title < 1 || title > 99)
      return typeid(void);
    
    ONDEBUG(cerr << "FileFormatDVDBodyC::ProbeLoad(), Checking file type. param=" << params << ", title=" << title << "\n");
    return typeid(ImageC<ByteRGBValueC>);
  }

  const type_info &
  FileFormatDVDBodyC::ProbeSave(const StringC &filename, const type_info &obj_type, bool forceFormat) const
  {
    return typeid(void);   
  }
  
  //: Create a input port for loading from file 'filename'.
  // Will create an Invalid port if not supported. <p>
  
  DPIPortBaseC FileFormatDVDBodyC::CreateInput(const StringC &filename, const type_info &obj_type) const
  {
    ONDEBUG(cerr << "FileFormatDVDBodyC::CreateInput()\n");
    // Look for the device symbol
    if(filename.length() == 0)
      return DPIPortBaseC();
    if(filename[0] != '@')
      return DPIPortBaseC();
    
    // Check it's a DVD device
    StringC device = ExtractDevice(filename);
    if(device != "DVD")
      return DPIPortBaseC();

    // Get the title and optional device
    StringC params = ExtractParams(filename);
    IntT delim = params.index('#');
    IntT title = 1;
    if(delim >= 0)
    {
      title = params.before(delim).IntValue();
      params = params.after(delim);
    }
    else
    {
      if (params.length() > 0)
        title = params.IntValue();
      params = "/dev/dvd";
    }
    
    // Check the title
    if (title < 1 || title > 99)
      return DPIPortBaseC();
    
    // Create the pipeline
    ONDEBUG(cerr << "FileFormatDVDBodyC::CreateInput(), Checking file type. params=" << params << ", title=" << title << "\n");
    DVDReadC dvd(title, params);
    MPEG2DemuxC demux(0xe0);
    ImgILibMPEG2DVDC mpeg(demux, dvd);
    dvd >> demux >> mpeg;
    
    return SPort(mpeg);
  }
  
  //: Create a output port for saving to file 'filename'..
  // Will create an Invalid port if not supported. <p>
  
  DPOPortBaseC FileFormatDVDBodyC::CreateOutput(const StringC &filename, const type_info &obj_type) const
  {
    return DPOPortBaseC();  
  }
  
  //: Create a input port for loading.
  // Will create an Invalid port if not supported.
  
  DPIPortBaseC FileFormatDVDBodyC::CreateInput(IStreamC &in, const type_info &obj_type) const
  { 
    return DPIPortBaseC();
  }
  
  //: Create a output port for saving.
  // Will create an Invalid port if not supported.
  
  DPOPortBaseC FileFormatDVDBodyC::CreateOutput(OStreamC &out, const type_info &obj_type) const
  {
    return DPOPortBaseC();  
  }
  
  //: Get prefered IO type.
  
  const type_info &FileFormatDVDBodyC::DefaultType() const 
  { 
    return typeid(ImageC<ByteRGBValueC>); 
  }
  
  static FileFormatDVDC Init;  
  
}
