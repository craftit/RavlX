// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/Contrib/V4L2/V4L2Format.cc"
//! lib=RavlImgIOV4L2
//! author = "Warren Moore"

#include "Ravl/Image/ByteYUVValue.hh"
#include "Ravl/Image/V4L2Format.hh"
#include "Ravl/Image/ImgIOV4L2.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/Image/RealRGBValue.hh"
#include "Ravl/SysLog.hh"

#define DPDEBUG 0
#if DPDEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlImageN
{
  
  const static StringC g_defaultDevice("/dev/video0");
  //: Default V4L2 device
  
  const static IntT g_defaultChannel(0);
  //: Default V4L2 channel
  
  void InitV4L2Format()
  {}
  
  FileFormatV4L2BodyC::FileFormatV4L2BodyC() :
    FileFormatBodyC("v4l2", "V4L2 input (@V4L2)")
  {}
  
  const type_info &FileFormatV4L2BodyC::ProbeLoad(IStreamC &in, const type_info &obj_type) const
  {
    ONDEBUG(RavlDebug("ProbeLoad not a V4L2 input"));
    return typeid(void); 
  }
  
  const type_info &FileFormatV4L2BodyC::ProbeLoad(const StringC &filename, IStreamC &in, const type_info &obj_type) const
  {
    ONDEBUG(RavlDebug("ProbeLoad filename(%s) type(%s)",filename.data(),obj_type.name()));

    // Get the parameters
    StringC device;
    IntT channel;
    if (!FindParams(filename, device, channel))
      return typeid(void);
    ONDEBUG(RavlDebug("ProbeLoad device(%s) channel(%d)",device.data(),channel));

    // Create the V4L2 object (will not be open after construction if not supported)
    ImgIOV4L2BaseC v4l2(device, channel);
    if(!v4l2.IsOpen())
      return typeid(void);
    
    ONDEBUG(RavlDebug("ProbeLoad format supported(%s)  ",(v4l2.IsOpen() ? "Y" : "N")));
    if (obj_type == typeid(ImageC<ByteYUVValueC>) || obj_type == typeid(ImageC<ByteRGBValueC>)) {
      ONDEBUG(RavlDebug("Targeting a colour image "));
      if(v4l2.CheckFormat(typeid(ImageC<ByteRGBValueC>)))
        return typeid(ImageC<ByteRGBValueC>);
      if(v4l2.CheckFormat(typeid(ImageC<ByteYUV422ValueC>)))
        return typeid(ImageC<ByteYUV422ValueC>);
      if(v4l2.CheckFormat(typeid(ImageC<ByteT>)))
        return typeid(ImageC<ByteT>);
      return typeid(void);
    }
    
    if (obj_type == typeid(ImageC<ByteT>) || obj_type == typeid(ImageC<FloatT>) || obj_type == typeid(ImageC<RealT>)) {
      ONDEBUG(RavlDebug("Targeting a grey level image "));
      if(v4l2.CheckFormat(typeid(ImageC<ByteT>)))
        return typeid(ImageC<ByteT>);
      if(v4l2.CheckFormat(typeid(ImageC<ByteYUV422ValueC>)))
        return typeid(ImageC<ByteYUV422ValueC>);
      if(v4l2.CheckFormat(typeid(ImageC<ByteRGBValueC>)))
        return typeid(ImageC<ByteRGBValueC>);
      return typeid(void);
    }
    
    if (obj_type == typeid(ImageC<ByteYUV422ValueC>)) {
      ONDEBUG(RavlDebug("Targeting a YUV422 image. "));
      if(v4l2.CheckFormat(typeid(ImageC<ByteYUV422ValueC>)))
        return typeid(ImageC<ByteYUV422ValueC>);
      if(v4l2.CheckFormat(typeid(ImageC<ByteRGBValueC>)))
        return typeid(ImageC<ByteRGBValueC>);
      if(v4l2.CheckFormat(typeid(ImageC<ByteT>)))
        return typeid(ImageC<ByteT>);
      return typeid(void);
    }
    
    ONDEBUG(RavlDebug("Falling back to default format. "));
    if(v4l2.CheckFormat(typeid(ImageC<ByteRGBValueC>))) {
      ONDEBUG(RavlDebug("ByteRGBValueC supported. "));
      return typeid(ImageC<ByteRGBValueC>);
    }
    if(v4l2.CheckFormat(typeid(ImageC<ByteYUV422ValueC>))) {
      ONDEBUG(RavlDebug("ByteYUV422ValueC supported. "));
      return typeid(ImageC<ByteYUV422ValueC>);
    }
    if(v4l2.CheckFormat(typeid(ImageC<ByteT>))) {
      ONDEBUG(RavlDebug("ByteT supported. "));
      return typeid(ImageC<ByteT>);
    }
    ONDEBUG(RavlDebug("Format not supported. "));
    return typeid(void);
  }
  
  
  
  const type_info &FileFormatV4L2BodyC::ProbeSave(const StringC &filename, const type_info &obj_type, bool forceFormat ) const
  {
    ONDEBUG(RavlDebug("FileFormatV4L2BodyC::ProbeSave unsupported"));
    return typeid(void);   
  }
  
  
  
  DPIPortBaseC FileFormatV4L2BodyC::CreateInput(IStreamC &in, const type_info &obj_type) const
  { 
    ONDEBUG(cerr << "FileFormatV4L2BodyC::CreateInput(OStreamC) unsupported" << endl;)
    return DPIPortBaseC();
  }
  
  
  
  DPOPortBaseC FileFormatV4L2BodyC::CreateOutput(OStreamC &out, const type_info &obj_type) const
  {
    ONDEBUG(cerr << "FileFormatV4L2BodyC::CreateOutput(OStreamC) unsupported" << endl;)
    return DPOPortBaseC();  
  }
  
  
  
  DPIPortBaseC FileFormatV4L2BodyC::CreateInput(const StringC &filename, const type_info &obj_type) const
  {
    ONDEBUG(cerr << "FileFormatV4L2BodyC::CreateInput filename(" << filename << ") type(" << obj_type.name() << ")" << endl;)

    // Get the parameters
    StringC device;
    IntT channel;
    if (!FindParams(filename, device, channel))
      return DPIPortBaseC();
    ONDEBUG(cerr << "FileFormatV4L2BodyC::CreateInput device(" << device << ") channel(" << channel << ")" << endl);
    
    // Create the relevant port
    
    if (obj_type == typeid(ImageC<ByteT>))
      return ImgIOV4L2C<ByteT>(device, channel);
    
    if (obj_type == typeid(ImageC<ByteRGBValueC>))
      return ImgIOV4L2C<ByteRGBValueC>(device, channel);
    
    if (obj_type == typeid(ImageC<ByteYUV422ValueC>))
      return ImgIOV4L2C<ByteYUV422ValueC>(device, channel);
    
    return DPIPortBaseC();
  }
  
  
  
  DPOPortBaseC FileFormatV4L2BodyC::CreateOutput(const StringC &filename, const type_info &obj_type) const
  {
    ONDEBUG(cerr << "FileFormatV4L2BodyC::CreateOutput(const StringC) unsupported" << endl;)
    return DPOPortBaseC();  
  }
  
  
  
  const type_info &FileFormatV4L2BodyC::DefaultType() const
  { 
    return typeid(ImageC<ByteRGBValueC>); 
  }
  
  
  
  bool FileFormatV4L2BodyC::FindParams(const StringC &filename, StringC &device, IntT &channel) const
  {
    // Look for the device symbol
    if(filename.length() == 0)
      return false;
    if(filename[0] != '@')
      return false;
    
    // Check it's a V4L2 device
    StringC deviceId = ExtractDevice(filename);
    if (deviceId != "V4L2")
      return false;
    
    // Set the defaults
    device = g_defaultDevice;
    channel = g_defaultChannel;

    // Get the device and optional channel
    StringC params = ExtractParams(filename);
    if (params.Size() > 0)
    {
      IntT delim = params.index('#');
      if(delim >= 0)
      {
        device = params.before(delim);
        channel = params.after(delim).IntValue();
      }
      else
        if (params.length() > 0)
          device = params;
    }
    
    return true;
  }
  
  
  
  static FileFormatV4L2C Init;  
}
