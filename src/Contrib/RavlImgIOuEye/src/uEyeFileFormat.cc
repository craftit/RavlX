// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2008, OmniPerception Ltd.
// This code may be redistributed under the terms of the MIT
// License. See http://www.opensource.org/licenses/mit-license.html
// file-header-ends-here
//////////////////////////////////////////////////////////////////
//! author = "Charles Galambos"
//! lib=RavlImgIOuEye

#include "Ravl/Image/ByteYUVValue.hh"
#include "Ravl/Image/uEyeFileFormat.hh"
#include "Ravl/Image/ImgIOuEye.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/Image/RealRGBValue.hh"

#define DPDEBUG 0
#if DPDEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlImageN
{
  
  FileFormatuEyeBodyC::FileFormatuEyeBodyC() 
    : FileFormatBodyC("ueye", "uEye input (@UEYE)")
  {}
  
  
  
  const type_info &FileFormatuEyeBodyC::ProbeLoad(IStreamC &in, const type_info &obj_type) const
  {
    ONDEBUG(cerr << "FileFormatuEyeBodyC::ProbeLoad not a uEye input" << endl;)
    return typeid(void); 
  }
  
  
  const type_info &FileFormatuEyeBodyC::ProbeLoad(const StringC &filename, IStreamC &in, const type_info &obj_type) const
  {
    ONDEBUG(cerr << "FileFormatuEyeBodyC::ProbeLoad filename(" << filename << ") type(" << obj_type.name() << ")" << endl);
    // Look for the device symbol
    if(filename.length() == 0)
      return typeid(void);
    if(filename[0] != '@')
      return typeid(void);
    
    // Check it's a uEye device
    StringC deviceId = ExtractDevice(filename);
    if (deviceId != "UEYE")
      return typeid(void);
    
    if (obj_type == typeid(ImageC<ByteYUVValueC>))
      return typeid(ImageC<ByteRGBValueC>);
    
    if (obj_type == typeid(ImageC<ByteT>) || obj_type == typeid(ImageC<FloatT>) || obj_type == typeid(ImageC<RealT>))
      return typeid(ImageC<ByteT>);
    
    return typeid(ImageC<ByteRGBValueC>);
  }
  
  
  
  const type_info &FileFormatuEyeBodyC::ProbeSave(const StringC &filename, const type_info &obj_type, bool forceFormat ) const
  {
    ONDEBUG(cerr << "FileFormatuEyeBodyC::ProbeSave unsupported" << endl;)
    return typeid(void);   
  }
  
  DPIPortBaseC FileFormatuEyeBodyC::CreateInput(IStreamC &in, const type_info &obj_type) const
  { 
    ONDEBUG(cerr << "FileFormatuEyeBodyC::CreateInput(OStreamC) unsupported" << endl);
    return DPIPortBaseC();
  }
  
  DPOPortBaseC FileFormatuEyeBodyC::CreateOutput(OStreamC &out, const type_info &obj_type) const
  {
    ONDEBUG(cerr << "FileFormatuEyeBodyC::CreateOutput(OStreamC) unsupported" << endl);
    return DPOPortBaseC();  
  }
  
  DPIPortBaseC FileFormatuEyeBodyC::CreateInput(const StringC &filename, const type_info &obj_type) const
  {
    ONDEBUG(cerr << "FileFormatuEyeBodyC::CreateInput filename(" << filename << ") type(" << obj_type.name() << ")" << endl);

    if(filename.length() == 0)
      return DPIPortBaseC();
    if(filename[0] != '@')
      return DPIPortBaseC();
    
    StringC deviceId = ExtractDevice(filename);
    if (deviceId != "UEYE")
      return DPIPortBaseC();
    
    // Get the device and optional channel
    StringC params = ExtractParams(filename);
    UIntT cameraId = params.UIntValue();
    
    // Create the relevant port
    try {
      if (obj_type == typeid(ImageC<ByteT>))
        return ImgIOuEyeC<ByteT>(cameraId);
      if (obj_type == typeid(ImageC<ByteRGBValueC>))
        return ImgIOuEyeC<ByteRGBValueC>(cameraId);
    } catch(RavlN::ExceptionOperationFailedC &ex) {
      std::cerr << "ERROR: Requested camera not found. \n";
    }
    
    return DPIPortBaseC();
  }
  
  
  
  DPOPortBaseC FileFormatuEyeBodyC::CreateOutput(const StringC &filename, const type_info &obj_type) const
  {
    ONDEBUG(cerr << "FileFormatuEyeBodyC::CreateOutput(const StringC) unsupported" << endl);
    return DPOPortBaseC();  
  }
  
  
  const type_info &FileFormatuEyeBodyC::DefaultType() const
  {
    return typeid(ImageC<ByteT>); 
  }
  
  
  static FileFormatuEyeC Init;  
  
  void InituEyeFormat()
  {}

}
