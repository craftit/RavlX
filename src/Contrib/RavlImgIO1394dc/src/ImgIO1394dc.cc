// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlImgIO1394dc
//! author="Charles Galambos"
//! file="Ravl/Contrib/dc1394/ImgIO1394dc.cc"

#include "Ravl/Image/ImgIO1394dc.hh"
#include "Ravl/Image/ByteYUVValue.hh"
#include <string.h>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlImageN {

  const FrameRate1394dcC frameRates[] =
    {
      { 1.875  ,FRAMERATE_1_875 },
      { 3.75   ,FRAMERATE_3_75  },
      { 7.5    ,FRAMERATE_7_5   },
      { 15     ,FRAMERATE_15    },
      { 30     ,FRAMERATE_30    },
      { 60     ,FRAMERATE_60    },
      { -1     ,FRAMERATE_60    }
    };
  
  
  struct Feature1394dcC {
    int featureno;
    const char *name;
    const char *desc;
  };
  
  const Feature1394dcC *ImgIO1394dcBaseC::FindFeature(int featureNo) {
    static const Feature1394dcC featureNames[] =
      { { FEATURE_BRIGHTNESS     ,"brightness"     ,"Brightness control." },
	{ FEATURE_EXPOSURE       ,"exposure"       ,"Exposure time."      },
	{ FEATURE_SHARPNESS      ,"sharpness"      ,"Sharpness filter, enhances edges in the image."   },
	{ FEATURE_WHITE_BALANCE  ,"white_balance"  ,"Control relative gain of the different colour channels." },
	{ FEATURE_HUE            ,"hue"            ,"Colour setup. " },
	{ FEATURE_SATURATION     ,"saturation"     ,"Colour saturation." },
	{ FEATURE_GAMMA          ,"gamma"          ,"Enable/Disable gamma correction." },
	{ FEATURE_SHUTTER        ,"shutter_speed"  ,"Shutter speed. " },
	{ FEATURE_GAIN           ,"gain"           ,"Grey level gain." },
	{ FEATURE_IRIS           ,"iris"           ,"Set the iris, controls the amount of light entering the camera." },
	{ FEATURE_FOCUS          ,"focus"          ,"Set the focus of the camera." },
	{ FEATURE_TEMPERATURE    ,"temperature"    ,"Who knows ?" },
	{ FEATURE_TRIGGER        ,"trigger"        ,"Set the camera trigger mode."},
	{ FEATURE_ZOOM           ,"zoom"           ,"Set the focal length of the optics for the camera." },
	{ FEATURE_PAN            ,"pan"            ,"The camera's horizontally angle." },
	{ FEATURE_TILT           ,"tilt"           ,"The camera's vertical angle." },
	{ FEATURE_OPTICAL_FILTER ,"optical_filter" ,"Who knows ?" },
	{ FEATURE_CAPTURE_SIZE   ,"capture_size"   ,"??" },
	{ FEATURE_CAPTURE_QUALITY,"capture_quality","??" },
	{ FEATURE_CAPTURE_QUALITY,0            ,0 }
      };

    for(int i = 0; featureNames[i].name != 0; i++)
      {
        if(featureNames[i].featureno == featureNo)
          return &featureNames[i];
      }
    return 0;
  }


  //: Default constructor.
  
  ImgIO1394dcBaseC::ImgIO1394dcBaseC(UIntT channel,const std::type_info &pixelType)
    : raw1394handle(0),
      cam_channel(channel),
      cam_format(FORMAT_VGA_NONCOMPRESSED),
      cam_mode(-1),
      cam_speed(SPEED_400),
      cam_framerate(FRAMERATE_30),
      oneShotMode(false)
  {
    if(pixelType == typeid(ByteT)) {
      ONDEBUG(std::cerr << "Open in grayscale mode\n");
      cam_mode = MODE_640x480_MONO;
    } else if(pixelType == typeid(ByteRGBValueC)) {
      ONDEBUG(std::cerr << "Open in RGB mode\n");
      cam_mode = MODE_640x480_RGB;
    } else if(pixelType == typeid(ByteYUV422ValueC)) {
      ONDEBUG(std::cerr << "Open in YUV422 mode\n");
      cam_mode = MODE_640x480_YUV422;
    } else if(pixelType == typeid(ByteYUVValueC)) { 
      ONDEBUG(std::cerr << "Open in YUV444 mode\n");
      cam_mode = MODE_160x120_YUV444;
    } else if(pixelType == typeid(ImageC<UInt16T>)) {
      ONDEBUG(std::cerr << "Open in UInt16T mode\n");
      cam_mode = MODE_640x480_MONO16;
    } else {
      std::cerr << "unsupported image pixel type\n";
    }
  }


  //: Destructor.
  ImgIO1394dcBaseC::~ImgIO1394dcBaseC()
  {
    if(raw1394handle == 0)
      return ;
    if(cam_channel >= 100) // DMA
    {
      if(dc1394_dma_unlisten(raw1394handle, &camera) != DC1394_SUCCESS)
        std::cerr << "couldn't stop the camera?\n";
      if(dc1394_dma_release_camera(raw1394handle, &camera) != DC1394_SUCCESS)
        std::cerr << "couldn't release the camera?\n";
    }
    else
    {
      if(dc1394_stop_iso_transmission(raw1394handle,camera.node)!=DC1394_SUCCESS)
        std::cerr << "couldn't stop the camera?\n";
      dc1394_release_camera(raw1394handle,&camera);
    }
    raw1394_destroy_handle(raw1394handle);
  }

  //: Open camera on device.
  bool ImgIO1394dcBaseC::Open(const StringC &dev)
  {
#if DODEBUG
    std::cerr << "Cam channel = " << cam_channel << '\n';
    if(cam_channel >= 100) // DMA access
      std::cerr << "ImgIO1394dcBaseC::Open(), Called. Dev=" << dev << " using DMA acceess\n";
    else
      std::cerr << "ImgIO1394dcBaseC::Open(), Called. Dev=" << dev << "\n";
#endif
    
    if(cam_mode == -1)
    {
      std::cerr << "unsupported format\n";
      return false;
    }
    MTWriteLockC hold(2);
    raw1394handle = dc1394_create_handle(0); //assume only one port=0
    if(raw1394handle == 0)
    {
      std::cerr << "ERROR: Unable to aqure a raw1394 handle\n"
              "Please check \n"
              "  - if the kernel modules `ieee1394',`raw1394' and `ohci1394' are loaded  \n"
              "  - if you have read/write access to /dev/raw1394";
      return false;
    }
    IntT numNodes = raw1394_get_nodecount(raw1394handle);
    IntT numCameras;
    nodeid_t * camera_nodes = dc1394_get_camera_nodes(raw1394handle,&numCameras,1);
    if (numCameras<1)
    {
      std::cerr << "ERROR: No cameras found. \n";
      return false;
    }
    std::cerr << numCameras << " camera(s) found\n";
    std::cerr << numNodes << " camera node(s) found\n";
    
    if(cam_channel >= 100) // DMA
      camera.node = camera_nodes[cam_channel - 100];
    else
      camera.node = camera_nodes[cam_channel];
    
    // Get some information about the camera....
    dc1394_camerainfo camerainfo;
    if(dc1394_get_camera_info(raw1394handle,camera.node,&camerainfo) < 0)
    {
      std::cerr << "Failed to get camera info. \n";
      return false;
    }

    camera_vendor = StringC(camerainfo.vendor);
    camera_model = StringC(camerainfo.model);
    {
      StrOStreamC strm;
      strm.os() << hex << camerainfo.euid_64;
      camera_euid = strm.String();
    }
    
#if 0
    // Query modes.
    quadlet_t supportedModes;
    if(dc1394_query_supported_modes(raw1394handle, camera.node, cam_format,&supportedModes) != DC1394_SUCCESS) {
      
      std::cerr << "Failed to query modes. \n";
      return false;
    }
    std::cerr << "Modes=" << hex << supportedModes << "\n";
#endif
    
    
    // Query framerates.
    if(dc1394_query_supported_framerates(raw1394handle, camera.node, cam_format,
                                         cam_mode, &available_framerates) != DC1394_SUCCESS)
    {
      std::cerr << "Failed to query rates. \n";
      return false;
    }

    std::cerr << "Rate=" << hex << available_framerates << "\n" << dec;
    
    // Find fastest supported framerate.
    if (available_framerates & (1U << (31-5)))
      cam_framerate = FRAMERATE_60;
    else if (available_framerates & (1U << (31-4)))
      cam_framerate = FRAMERATE_30;
    else if (available_framerates & (1U << (31-3)))
      cam_framerate = FRAMERATE_15;
    else if (available_framerates & (1U << (31-2)))
      cam_framerate = FRAMERATE_7_5;
    else if (available_framerates & (1U << (31-1)))
      cam_framerate = FRAMERATE_3_75;
    else if (available_framerates & (1U << (31-0)))
      cam_framerate = FRAMERATE_1_875;
    
    // Setup capture.
    if(cam_channel >= 100) // DMA
    {
      dc1394_feature_set features;
      if(dc1394_get_camera_feature_set(raw1394handle, camera.node, &features) != DC1394_SUCCESS )
      {
        std::cerr << "unable to get camera features\n";
      }
      else
      {
        std::cerr << "features\n";
        dc1394_print_feature_set(&features);
      }
      const int NUM_BUFFERS = 8;
      const int DROP_FRAMES = 1;
      unsigned int ch, sp;
      if(dc1394_get_iso_channel_and_speed(raw1394handle, camera.node, &ch, &sp) != DC1394_SUCCESS )
      {
         std::cerr << "unable to get channel and speed\n";
      }
      std::cerr << "channel=" << ch << "\nspeed" << sp << "\n";
      std::cerr << "trining to set\nchannel=" << cam_channel-100 << "\nspeed" << cam_speed << "\n";
      if(dc1394_dma_setup_capture(raw1394handle, camera.node, cam_channel-100+1, cam_format,
                                  cam_mode, cam_speed, cam_framerate, NUM_BUFFERS,
                                  DROP_FRAMES, dev.chars(), &camera) != DC1394_SUCCESS)
      {
        std::cerr << "unable to setup camera-\n"
                "check that the video mode, framerate and format are\n"
                "supported by your camera\n";
        return false;
      }
    }
    else
    {
      if(dc1394_setup_capture(raw1394handle, camera.node, cam_channel, cam_format,
                   cam_mode, cam_speed, cam_framerate, &camera)!=DC1394_SUCCESS)
      {
        std::cerr << "unable to setup camera-\n"
                "check that the video mode, framerate and format are\n"
                "supported by your camera\n";
        return false;
      }
    }

    if (dc1394_start_iso_transmission(raw1394handle, camera.node) != DC1394_SUCCESS)
    {
      std::cerr << "unable to start camera iso transmission\n";
      return false;
    }
    std::cerr << "Size=" << camera.frame_width << " " << camera.frame_height << "\n";
    return true;
  }
  
  //: Capture an image.
  
  bool ImgIO1394dcBaseC::CaptureImage(void *buff,UIntT pixelSize)
  {
    MTWriteLockC hold(2);
    if(cam_channel >= 100) { // DMA
      if(dc1394_dma_single_capture(&camera) != DC1394_SUCCESS){
        std::cerr << "unable to capture a frame (dma)\n";
        return false;
      }
    } else {
      if (dc1394_single_capture(raw1394handle,&camera) != DC1394_SUCCESS) {
        std::cerr << "unable to capture a frame (no-dma)\n";
        return false;
      }
    }
    
    UIntT bufferSize = camera.frame_height * camera.frame_width * pixelSize;
    memcpy(buff,camera.capture_buffer,bufferSize);
    
    if(cam_channel > 100) // DMA
      dc1394_dma_done_with_buffer(&camera);
    
    if(oneShotMode) // Reset one shot mode.
      dc1394_set_one_shot(raw1394handle, camera.node);
    
    return true;
  }

  //: Get a stream attribute.
  // Returns false if the attribute name is unknown.
  // This is for handling stream attributes such as frame rate, and compression ratios.
  bool ImgIO1394dcBaseC::HandleGetAttr(const StringC &attrName,StringC &attrValue)
  {
    if(attrName == "vendor")
    {
      attrValue = camera_vendor;
      return true;
    }
    if(attrName == "model")
    {
      attrValue = camera_model;
      return true;
    }
    if(attrName == "euid")
    {
      attrValue = camera_euid;
      return true;
    }
    if(attrName == "capturemode") {
      if(oneShotMode)
	attrValue = "oneshot";
      else
	attrValue = "continuous";
      return true;
    }
    return false;
  }

  //: Set a stream attribute.
  // Returns false if the attribute name is unknown.
  // This is for handling stream attributes such as frame rate, and compression ratios.
  
  bool ImgIO1394dcBaseC::HandleSetAttr(const StringC &attrName,const StringC &attrValue)
  {
    if(attrName == "capturemode") {
      if(attrValue == "oneshot")
	OneShotMode(true);
      else {
	if(attrValue != "continuous")
	  std::cerr << "ImgIO1394dcBaseC::HandleSetAttr(), WARNING: Unrecognised capture mode '" << attrValue << "' \n";
	OneShotMode(false);
      }
      return true;
    }
    return false;
  }
  
  //: Get a stream attribute.
  // Returns false if the attribute name is unknown.
  // This is for handling stream attributes such as frame rate, and compression ratios.
  bool ImgIO1394dcBaseC::HandleGetAttr(const StringC &attrName,IntT &attrValue)
  {
    Tuple2C<IntT,ControlTypeT> featureInfo;
    if(!name2featureid.Lookup(attrName,featureInfo))
      return false;
    
    MTWriteLockC hold(2);
    switch(featureInfo.Data2())
    {
    case CT_IntValue:
      dc1394_get_feature_value(raw1394handle,camera.node,featureInfo.Data1(),(UIntT *) &attrValue);
      break;
    case CT_FloatValue:
    case CT_OnOff:
    case CT_Auto:
      std::cerr << "ImgIO1394dcBaseC::HandleGetAttr(), WARNING: Used for On/Off or Auto. \n";
      return false;
    }
    return true;
  }

  //: Set a stream attribute.
  // Returns false if the attribute name is unknown.
  // This is for handling stream attributes such as frame rate, and compression ratios.
  
  bool ImgIO1394dcBaseC::HandleSetAttr(const StringC &attrName,const IntT &attrValue)
  {
    Tuple2C<IntT,ControlTypeT> featureInfo;
    if(!name2featureid.Lookup(attrName,featureInfo))
      return false;
    MTWriteLockC hold(2);
    switch(featureInfo.Data2())
    {
    case CT_FloatValue:
    case CT_IntValue:
      dc1394_set_feature_value(raw1394handle,camera.node,featureInfo.Data1(),attrValue);
      break;
    case CT_OnOff:
    case CT_Auto:
      std::cerr << "ImgIO1394dcBaseC::HandleSetAttr(), WARNING: Used for On/Off or Auto. \n";
      return false;
    }
    return true;
  }

  //: Get a stream attribute.
  // Returns false if the attribute name is unknown.
  // This is for handling stream attributes such as frame rate, and compression ratios.
  
  bool ImgIO1394dcBaseC::HandleGetAttr(const StringC &attrName,RealT &attrValue)
  {
    MTWriteLockC hold(2);
    if(attrName == "framerate")
    {
      unsigned int setting = 0;
      dc1394_get_video_framerate(raw1394handle, camera.node, &setting);
      for(int i = 0;frameRates[i].speed > 0;i++)
      {
        //cerr << "Checking " << frameRates[i].value << "  " << setting << "\n";
        if(frameRates[i].value == setting)
        {
          attrValue = frameRates[i].speed;
          return true;
        }
      }
      std::cerr << "ImgIO1394dcBaseC::HandleGetAttr(), Unrecognised speed attribute " << setting << "\n";
      attrValue = 15;
      return true;
    }
    Tuple2C<IntT,ControlTypeT> featureInfo;
    if(!name2featureid.Lookup(attrName, featureInfo))
      return false;
    float tmp;
    switch(featureInfo.Data2())
    {
    case CT_IntValue:
      dc1394_query_absolute_feature_value(raw1394handle,camera.node,featureInfo.Data1(),&tmp);
      attrValue = (RealT) tmp;
      break;
    case CT_FloatValue:
    case CT_OnOff:
    case CT_Auto:
      std::cerr << "ImgIO1394dcBaseC::HandleGetAttr(), WARNING: Used for On/Off or Auto. \n";
      return false;
    }
    return true;
  }

  //: Set a stream attribute.
  // Returns false if the attribute name is unknown.
  // This is for handling stream attributes such as frame rate, and compression ratios.
  
  bool ImgIO1394dcBaseC::HandleSetAttr(const StringC &attrName,const RealT &attrValue)
  {
    if(attrName == "framerate")
    {
      SetFrameRate(attrValue);
      return true;
    }
    Tuple2C<IntT,ControlTypeT> featureInfo;
    if(!name2featureid.Lookup(attrName,featureInfo))
      return false;
    MTWriteLockC hold(2);
    switch(featureInfo.Data2())
    {
    case CT_FloatValue:
    case CT_IntValue:
      dc1394_set_absolute_feature_value(raw1394handle,camera.node,featureInfo.Data1(),(float) attrValue);
      break;
    case CT_OnOff:
    case CT_Auto:
      std::cerr << "ImgIO1394dcBaseC::HandleSetAttr(), WARNING: Used for On/Off or Auto. \n";
      return false;
    }
    return true;
  }

  //: Get a stream attribute.
  // Returns false if the attribute name is unknown.
  // This is for handling stream attributes such as frame rate, and compression ratios.
  bool ImgIO1394dcBaseC::HandleGetAttr(const StringC &attrName,bool &attrValue)
  {
    Tuple2C<IntT,ControlTypeT> featureInfo;
    if(!name2featureid.Lookup(attrName,featureInfo))
      return false;
    MTWriteLockC hold(2);
    dc1394bool_t tmp;
    switch(featureInfo.Data2())
    {
    case CT_FloatValue:
    case CT_IntValue:
      return false;
    case CT_OnOff:
      dc1394_is_feature_on(raw1394handle,camera.node,featureInfo.Data1(),&tmp);
      break;
    case CT_Auto:
      dc1394_is_feature_auto(raw1394handle,camera.node,featureInfo.Data1(),&tmp);
      break;
    }
    attrValue = (tmp != DC1394_FALSE);
    return true;
  }

  //: Set a stream attribute.
  // Returns false if the attribute name is unknown.
  // This is for handling stream attributes such as frame rate, and compression ratios.
  
  bool ImgIO1394dcBaseC::HandleSetAttr(const StringC &attrName,const bool &attrValue)
  {
    Tuple2C<IntT,ControlTypeT> featureInfo;
    if(!name2featureid.Lookup(attrName,featureInfo))
      return false;
    MTWriteLockC hold(2);
    switch(featureInfo.Data2())
    {
    case CT_FloatValue:
    case CT_IntValue:
      return false;
    case CT_OnOff:
      dc1394_feature_on_off(raw1394handle,camera.node,featureInfo.Data1(),
                            (attrValue == DC1394_FALSE) ? false : true);
      break;
    case CT_Auto:
      dc1394_auto_on_off(raw1394handle,camera.node,featureInfo.Data1(),
                         (attrValue == DC1394_FALSE) ? false : true);
      break;
    }
    return true;
  }
  
  //: Build attribute list.

  void ImgIO1394dcBaseC::BuildAttrList(AttributeCtrlBodyC &attrCtrl)
  {
    MTWriteLockC hold(2);

    attrCtrl.RegisterAttribute(AttributeTypeStringC("vendor","Supplier of device",true,false));
    attrCtrl.RegisterAttribute(AttributeTypeStringC("model","model of device",true,false));
    attrCtrl.RegisterAttribute(AttributeTypeStringC("euid","Unique id for device",true,false));
    
    DListC<StringC> captureModeList;
    captureModeList.InsLast("continuous");
    captureModeList.InsLast("oneshot");
    attrCtrl.RegisterAttribute(AttributeTypeEnumC("capturemode","How to capture images from source. ",true,true,captureModeList,StringC("continuous")));
    
    // libdc1394 BUG FIX, clear available flags before calling get_feature_set.
    dc1394_feature_set featureSet;
    for(int i = 0;i < NUM_FEATURES;i++)
      featureSet.feature[i].available = DC1394_FALSE;

    dc1394_get_camera_feature_set(raw1394handle,camera.node,&featureSet);

    // Go through feature list.
    for(int i = 0;i < NUM_FEATURES;i++)
    {
      const dc1394_feature_info &feature = featureSet.feature[i];
      if(feature.available != DC1394_TRUE)
        continue; // Feature is not avalable on this camera.
      const Feature1394dcC *featureInfo = FindFeature(feature.feature_id);
      const char *cfeatName = featureInfo->name;
      if(cfeatName == 0)
      {
        std::cerr << "WARNING: Unknown featureid " << feature.feature_id << "\n";
        continue;
      }
      StringC featName(cfeatName);

      if((feature.abs_control > 0) && (feature.absolute_capable > 0))
      {
        ONDEBUG(std::cerr << "Setting up " << featName << " Absolute. Min=" << feature.abs_min << " Max=" << feature.abs_max << " Value=" << feature.abs_value << "\n");
        RealT diff = feature.abs_max - feature.abs_min;
        AttributeTypeNumC<RealT> attr(featName,featureInfo->desc,feature.readout_capable,feature.manual_capable,
                    feature.abs_min,feature.abs_max,diff/1000,feature.abs_value);
        attrCtrl.RegisterAttribute(attr);
        name2featureid[featName] = Tuple2C<IntT,ControlTypeT>(feature.feature_id,CT_IntValue);
      }
      else
      {
        ONDEBUG(std::cerr << "Setting up " << featName << " Int. Min=" << feature.min << " Max=" << feature.max << " Value=" << feature.value << "\n");
        AttributeTypeNumC<IntT> attr(featName,featureInfo->desc,feature.readout_capable,feature.manual_capable,
                   feature.min,feature.max,1,feature.value);
        attrCtrl.RegisterAttribute(attr);
        name2featureid[featName] = Tuple2C<IntT,ControlTypeT>(feature.feature_id,CT_IntValue);
      }
      if(feature.on_off_capable)
      {
        StringC name = featName + "_enable";
        AttributeTypeBoolC attr(name,featureInfo->desc,true,true,feature.is_on);
        attrCtrl.RegisterAttribute(attr);
        name2featureid[name] = Tuple2C<IntT,ControlTypeT>(feature.feature_id,CT_OnOff);
      }
      if(feature.auto_capable)
      {
        StringC name = featName + "_auto";
        AttributeTypeBoolC attr(name,featureInfo->desc,true,true,feature.auto_active);
        attrCtrl.RegisterAttribute(attr);
        name2featureid[name] = Tuple2C<IntT,ControlTypeT>(feature.feature_id,CT_Auto);

        //AttributeTypeNumC<IntT> attr(name,featureInfo->desc,true,true,feature.auto_active);
      }
    }

    // Setup framerate attribute.
    RealT minspeed = 1e8;
    RealT maxspeed = 0;
    for(int i = 0;frameRates[i].speed > 0;i++)
    {
      //cerr << "Rate=" << frameRates[i].speed << " " << !(available_framerates & (1U << (31-(frameRates[i].value - FRAMERATE_MIN)))) << "\n";
      if ((available_framerates & (1U << (31-(frameRates[i].value - FRAMERATE_MIN)))) == 0)
        continue; // framerate not supported.
      if(frameRates[i].value < minspeed)
        minspeed = frameRates[i].value;
      if(frameRates[i].value > maxspeed)
        maxspeed = frameRates[i].value;
    }

    AttributeTypeNumC<RealT> frameRateAttr("framerate","Number of images a second to capture",true,true,minspeed,maxspeed,1,(maxspeed + minspeed)/2);
    attrCtrl.RegisterAttribute(frameRateAttr);
  }

  //: Set oneshot mode.
  
  bool ImgIO1394dcBaseC::OneShotMode(bool enable) {
    int ret;
    MTWriteLockC hold(2);
    if(enable)
      ret = dc1394_set_one_shot(raw1394handle, camera.node);
    else
      ret = dc1394_unset_one_shot(raw1394handle, camera.node);
    if(ret != DC1394_SUCCESS)
      return false;
    oneShotMode = enable;
    return true;
  }
  
  //: Set capture framerate.
  // Returns the actual frame rate.
  
  RealT ImgIO1394dcBaseC::SetFrameRate(RealT speed)
  {
    // Find closest setting.
    RealT err = 1000000;
    int setting = -1;
    for(int i = 0;frameRates[i].speed > 0;i++)
    {
      //cerr << "Rate=" << frameRates[i].speed << " " << !(available_framerates & (1U << (31-(frameRates[i].value - FRAMERATE_MIN)))) << "\n";
      if ((available_framerates & (1U << (31-(frameRates[i].value - FRAMERATE_MIN)))) == 0)
        continue; // framerate not supported.
      RealT nerr = Abs(frameRates[i].speed - speed);
      if(nerr < err)
      {
        setting = i;
        err = nerr;
      }
    }
    if(setting < 0)
    {
      std::cerr << "ERROR: failed to find appropriate framerate. \n";
      return -1;
    }
    unsigned int current = 0;
    unsigned int newsetting = (unsigned int) frameRates[setting].value;
    MTWriteLockC hold(2);
    
    dc1394_get_video_framerate(raw1394handle,camera.node,&current);

    if(current != newsetting) { // Need to change
      if(dc1394_set_video_framerate(raw1394handle, camera.node, newsetting) != DC1394_SUCCESS)
      {
        std::cerr << "Failed to setup camera for new framerate. \n";
        return 0;
      }
/*      if (dc1394_stop_iso_transmission(raw1394handle,camera.node)!=DC1394_SUCCESS)
        std::cerr << "ERROR: couldn't stop the camera?\n";
      //dc1394_set_video_framerate(raw1394handle,camera.node,);
      dc1394_release_camera(raw1394handle,&camera);
      if (dc1394_setup_capture(raw1394handle,cameraNode,
			       cam_channel,
			       cam_format,
			       cam_mode,
			       cam_speed,
			       newsetting,
			       &camera)!=DC1394_SUCCESS)
      {
        std::cerr << "Failed to setup camera for new framerate. \n";
        return 0;
      }
      cam_framerate = newsetting;
      if (dc1394_start_iso_transmission(raw1394handle,camera.node) != DC1394_SUCCESS)
      {
        std::cerr << "ERROR: unable to restart camera iso transmission\n";
        return 0;
      }*/
    }
    return frameRates[setting].speed; // Return actual speed.
  }
  
}

