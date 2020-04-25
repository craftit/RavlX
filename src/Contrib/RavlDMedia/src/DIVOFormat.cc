// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlDMedia
//! file="Ravl/Contrib/DMedia/DIVOFormat.cc"

#include "Ravl/DMedia/DIVOFormat.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/Image/VYUValue.hh"
#include "Ravl/DMedia/NodePath.hh"
#include "Ravl/Threads/LaunchThread.hh"
#include "Ravl/Image/ByteYUV422Value.hh"
#include "Ravl/Image/ByteYUVValue.hh"
//#include "Ravl/Image/SubSamp2Intl.hh"
#include "Ravl/StringList.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#include "Ravl/TypeName.hh"
#else
#define ONDEBUG(x) 
#endif

namespace RavlImageN
{
  
  void InitFormatDIVO()
  {}
  
  // Charles' temporary stuff
  // bool subSample2 = false;
  // bool subSample2sum = false;
  // bool glxMode = false;
  
  static void ReadConfig(const StringC &fn,
			 bool &subSample2,
			 bool &subSample2sum,
			 bool &glxMode,
			 bool &frame,
			 IntT& nBufferSize) {
    StringListC strl(fn,":");
    DLIterC<StringC> it(strl);
    if(!it.IsElm())
      return ;
    for(it.Next();it.IsElm();it.Next()) {
      if(it.Data() == "SS2") {
	subSample2 = true;
	continue;
      }
      if(it.Data() == "SS2s") {
	subSample2 = true;
	subSample2sum = true;
	continue;
      }
      if(it.Data() == "glx") {
	glxMode = true;
	continue;
      }
      if(it.Data() == "frame") {
	frame = true;
	continue;
      }

      // Joel's bit for setting the buffer size
      if (sscanf(it.Data(), "buffer%d", &nBufferSize) == 1) {
	if (nBufferSize < 1)
	  nBufferSize = 1;
	continue;
      }

      cerr << "IOFormatDIVOBodyC::CreateInput(), Unknown mode : '" << it.Data() << "'. Ignoring.\n";
    }
  }
  
  //: Constructor.

  IOFormatDIVOBodyC::IOFormatDIVOBodyC() 
    : FileFormatBodyC("DIVO","Hardware interface to SDI ports.")
  {
    ONDEBUG(cerr << "IOFormatDIVOBodyC::IOFormatDIVOBodyC() Format registered. \n");
  }
  
  //: Find the best type for capture.
  
  const type_info &IOFormatDIVOBodyC::GetBestType(const type_info &obj_type) const {
    if(obj_type == typeid(ImageC<ByteRGBValueC>))
      return obj_type;
    if(obj_type == typeid(ImageC<ByteT>))
      return obj_type;
    if(obj_type == typeid(ImageC<ByteVYUValueC>)) 
      return obj_type;
    // Some hints.
    if(obj_type == typeid(ImageC<ByteYUVValueC>)) 
      return typeid(ImageC<ByteVYUValueC>);
    if(obj_type == typeid(ImageC<RealT>))
      return typeid(ImageC<ByteT>);
    if(obj_type == typeid(ImageC<IntT>))
      return typeid(ImageC<ByteT>);
    if(obj_type == typeid(ImageC<UIntT>))
      return typeid(ImageC<ByteT>);
    // Else do what CCIR does.
    return typeid(ImageC<ByteYUV422ValueC>);
  }
  
  //: Check if device 'nm' exists.
  
  bool IOFormatDIVOBodyC::HaveDevice(const StringC &nm) const {
    // Check for hardware we support.
    // Otherwise don't even open a connection to the video server.
    if(!nm.contains("DIVO") && !nm.contains("SIRIUS"))
      return false; // No idea.
    
    // Server open ?
    if(!vidServer.IsValid())
      const_cast<IOFormatDIVOBodyC *>(this)->OpenServer();
    return devSet.IsMember(nm);
  }
  
  //: Open video server.
  
  void IOFormatDIVOBodyC::OpenServer() {
    ONDEBUG(cerr << "Opening connection to server. \n");
    if(vidServer.IsValid())
      return ; // Open already.
    vidServer = VLServerC("");
    // Get list of available devices.
    DListC<VLDeviceC> lst = vidServer.DeviceList();
    for(DLIterC<VLDeviceC> it(lst);it.IsElm();it.Next()) {
      ONDEBUG(cerr << "DIVO Found device:" << it.Data().Name() << "\n");
      devSet.Insert(it.Data().Name());
    }
    // Start event loop.
    ONDEBUG(cerr << "IOFormatDIVOBodyC::OpenServer(), Launching event loop. \n");
    LaunchThread(vidServer,&VLServerC::RunEventLoop);
    ONDEBUG(cerr << "IOFormatDIVOBodyC::OpenServer(), Done. \n");
  }
    
  //: Is stream in std stream format ?
  // Not supported.

  const type_info &IOFormatDIVOBodyC::ProbeLoad(IStreamC &in,const type_info &obj_type) const {
    ONDEBUG(cerr << "IOFormatDIVOBodyC::ProbeLoad(), Stream. \n");
    return typeid(void); 
  }
  
  //: Probe for load.
    
  const type_info &IOFormatDIVOBodyC::ProbeLoad(const StringC &filename,IStreamC &,const type_info &obj_type) const {
    ONDEBUG(cerr << "IOFormatDIVOBodyC::ProbeLoad(), Filename:" << filename << "  Type:" << TypeName(obj_type) << "\n");
    if(filename[0] != '@') 
      return typeid(void);
    StringC dataName = ExtractDevice(filename);
    ONDEBUG(cerr << "IOFormatDIVOBodyC::ProbeLoad(), Device:" << dataName << "\n");
    if(!HaveDevice(dataName)) {
      ONDEBUG(cerr << "No device:" << dataName << " found.\n");
      return typeid(void);
    }
    return GetBestType(obj_type);
  }
  
  //: Probe for Save.
    
  const type_info &IOFormatDIVOBodyC::ProbeSave(const StringC &filename,const type_info &obj_type,bool /*forceFormat*/) const {
    if(filename[0] != '@') 
      return typeid(void);
    StringC dataName = ExtractDevice(filename);
    ONDEBUG(cerr << "IOFormatDIVOBodyC::ProbeSave(), Device:" << dataName << "\n");
    if(!HaveDevice(dataName)) {
      ONDEBUG(cerr << "No device:" << dataName << " found.\n");
      return typeid(void);
    }
    return GetBestType(obj_type);
  }
  
  //: Create a input port for loading.
  // This creates a plain DIVO stream.
  // Will create an Invalid port if not supported.
  
  DPIPortBaseC IOFormatDIVOBodyC::CreateInput(IStreamC &,const type_info &) const {
    ONDEBUG(cerr << "IOFormatDIVOBodyC::CreateInput(IStreamC &) Called. \n");
    return DPIPortBaseC();
  }
  
  //: Create a output port for saving.
  // This creates a plain DIVO stream.
  // Will create an Invalid port if not supported.
  
  DPOPortBaseC IOFormatDIVOBodyC::CreateOutput(OStreamC &,const type_info &) const {
    ONDEBUG(cerr << "IOFormatDIVOBodyC::CreateOutput(OStreamC &) Called. \n");
    return DPOPortBaseC();
  }
  
  //: Create a input port for loading from file 'filename'.
  // Will create an Invalid port if not supported. <p>
  
  DPIPortBaseC IOFormatDIVOBodyC::CreateInput(const StringC &filename,const type_info &obj_type) const {
    ONDEBUG(cerr << "IOFormatDIVOBodyC::CreateInput(StringC &) " << filename << ". \n");
    StringC dataName = ExtractDevice(filename);
    if(!HaveDevice(dataName)) 
      return DPIPortBaseC();
    
    // Check paramiters.
    
    bool subSample2 = false;
    bool subSample2sum = false;
    bool glxMode = false;
    bool frame = true;
    IntT nBufferSize = 10;
    ReadConfig(filename,subSample2,subSample2sum,glxMode,frame, nBufferSize);
    
    const type_info &useType = GetBestType(obj_type);    
    ONDEBUG(cerr << "IOFormatDIVOBodyC::CreateInput() " << TypeName(useType) << " with device '" << dataName << "' \n");
    if(useType == typeid(ImageC<ByteYUV422ValueC>)) {
      DPIVideoDIVOC<ByteYUV422ValueC> ret(vidServer,dataName.chars(),frame,glxMode, nBufferSize);
#if 0
      if(subSample2)
	return ret >> DPSubSample2IntlC<ByteYUV422ValueC>(subSample2sum);
#endif
      return ret;
    }
    if(useType == typeid(ImageC<ByteVYUValueC>)) {
      DPIVideoDIVOC<ByteVYUValueC> ret(vidServer,dataName.chars(),frame,glxMode, nBufferSize);
#if 0
      if(subSample2)
	return ret >> DPSubSample2IntlC<ByteVYUValueC>(subSample2sum);
#endif
      return ret;
    }
    if(useType == typeid(ImageC<ByteRGBValueC>)) {
      DPIVideoDIVOC<ByteRGBValueC> ret(vidServer,dataName.chars(),frame,glxMode, nBufferSize);
#if 0
      if(subSample2)
	return ret >> DPSubSample2IntlC<ByteRGBValueC>(subSample2sum);
#endif
      return ret;
    }
    if(useType == typeid(ImageC<ByteT>)) {
      DPIVideoDIVOC<ByteT> ret(vidServer,dataName.chars(),frame,glxMode, nBufferSize);
#if 0
      if(subSample2)
	return ret >> DPSubSample2IntlC<ByteT>(subSample2sum);
#endif
      return ret;
    }
    cerr << "IOFormatDIVOBodyC::CreateInput(), ERROR: Best type not implemented :" << TypeName(useType) << "\n";
    RavlAssert(0); // 'BestType' not implemented.
    return DPIPortBaseC();
  }
  
  //: Create a output port for saving to file 'filename'..
  // Will create an Invalid port if not supported. <p>

  DPOPortBaseC IOFormatDIVOBodyC::CreateOutput(const StringC &filename,const type_info &obj_type) const {
    ONDEBUG(cerr << "IOFormatDIVOBodyC::CreateOutput(StringC &) " << filename << ". \n");
    StringC dataName = ExtractDevice(filename);
    if(!HaveDevice(dataName)) 
      return DPOPortBaseC();    
    const type_info &useType = GetBestType(obj_type);    
    
    bool subSample2 = false;
    bool subSample2sum = false;
    bool glxMode = false;
    bool frame = false;
    IntT nBufferSize = 1;
    ReadConfig(filename,subSample2,subSample2sum,glxMode,frame, nBufferSize);
    
    ONDEBUG(cerr << "IOFormatDIVOBodyC::CreateOutput() " << TypeName(useType) << " with device '" << dataName << "' \n");
    
    if(useType == typeid(ImageC<ByteYUV422ValueC>)) {
      return DPOVideoDIVOC<ByteYUV422ValueC>(vidServer,dataName.chars(),frame,glxMode);
    }
    if(useType == typeid(ImageC<ByteVYUValueC>)) {
      return DPOVideoDIVOC<ByteVYUValueC>(vidServer,dataName.chars(),frame,glxMode);
    }    
    if(useType == typeid(ImageC<ByteRGBValueC>)) {
      return DPOVideoDIVOC<ByteRGBValueC>(vidServer,dataName.chars(),frame,glxMode);
    }
    if(useType == typeid(ImageC<ByteT>)) {
      return DPOVideoDIVOC<ByteT>(vidServer,dataName.chars(),frame,glxMode);
    }
    cerr << "IOFormatDIVOBodyC::CreateOutput(), ERROR: Best type not implemented :" << TypeName(useType) << "\n";
    RavlAssert(0); // 'BestType' not implemented.
    return DPOPortBaseC();
  }
  
  //: Get prefered IO type.
  
  const type_info &IOFormatDIVOBodyC::DefaultType() const
  { return typeid(ImageC<ByteYUV422ValueC>); }
  
  //: Find the priority of the format. the higher the better.
  // Default is zero, this is better than the default (streams.)
  
  IntT IOFormatDIVOBodyC::Priority() const 
   { return -1; }
  
  //: Test if format is a fully streamable.
  // i.e. check if you can read/write more than object object.

  bool IOFormatDIVOBodyC::IsStream() const 
  { return true; }
  
  // Add format to the data base.
  
  IOFormatDIVOC divoIOformat(true);
}
