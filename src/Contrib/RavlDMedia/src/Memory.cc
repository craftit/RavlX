// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlDMedia
//! file="Ravl/Contrib/DMedia/Memory.cc"

#include <unistd.h>
#include "Ravl/DMedia/Memory.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlImageN
{

  //: Constructor.
  
  VLNodeMemoryPoolBodyC::VLNodeMemoryPoolBodyC(const VLServerC &srv,int type,UIntT nnoFrames,bool nglxMode)
    : VLNodeBodyC(srv,type,VL_MEM,VL_ANY),
      pool(0),
      noFrames(nnoFrames),
      glxMode(nglxMode)
  {
    ONDEBUG(cerr << "VLNodeMemoryPoolBodyC::VLNodeMemoryPoolBodyC(), Called \n");
  }
  
  //: Destructor.
  
  VLNodeMemoryPoolBodyC::~VLNodeMemoryPoolBodyC() {
    ONDEBUG(cerr << "VLNodeMemoryPoolBodyC::~VLNodeMemoryPoolBodyC(), Called \n");
    if(pool != 0) {
      MutexLockC alock(server.AccessLock());
      dmBufferDestroyPool(pool);
    }
  }
  
  //: Initalise componentes.
  
  bool VLNodeMemoryPoolBodyC::Init() {
    UIntT transferSize = vlGetTransferSize(Server(),Path());
    ONDEBUG(cerr << "VLNodeMemoryPoolBodyC::Init(), TransferSize=" << transferSize << "\n");
    DMparams* plist;    
    dmParamsCreate(&plist);
    dmBufferSetPoolDefaults(plist,noFrames, transferSize, DM_FALSE, DM_FALSE);
    
    vlDMGetParams(Server(),Path(),Node(), plist);

    dmParamsSetInt(plist, DM_BUFFER_ALIGNMENT, getpagesize());
    
    if(glxMode) { 
      // Setup to pool to be used with hardware excellerated 
      // image processing...
      
      DMparams* imData;
      dmParamsCreate(&imData);
      
      // This should setup....
      // DM_IMAGE_WIDTH, DM_IMAGE_HEIGHT and DM_IMAGE_PACKING
      
      dmParamsCopyAllElems(plist,imData); // Just copy current path setup..
      
      if(dmBufferGetGLPoolParams(imData ,plist) == DM_FAILURE) {
	cerr << "MemoryPoolBodyC::MemoryPoolBodyC(), Failed to configure pool for use with GLXe.  ";
	// What action....
      }
    }
    
    if(dmBufferCreatePool(plist,&pool) != DM_SUCCESS) {
      cerr << "MemoryPoolBodyC::MemoryPoolBodyC(), Failed to create buffer pool.  ";
      dmParamsDestroy(plist);
      return false;
    }
    dmParamsDestroy(plist);
    
    if (vlDMPoolRegister(Server(), Path(),Node(), pool)) {
      cerr << "MemoryPoolBodyC::MemoryPoolBodyC(), Failed to register pool. ";
      return false;
    }
    return true;
  }

  //: Allocate node from pool associated with node.
  
  VLNodePoolDataC VLNodeMemoryPoolBodyC::Allocate() {
    DMbuffer tmp;
    MutexLockC alock(server.AccessLock());
    if(dmBufferAllocate(pool,&tmp) == DM_FAILURE) {
      cerr << "VLNodeMemoryPoolBodyC::Allocate() Failed. \n";
      return VLNodePoolDataC();
    }
    return VLNodePoolDataC(server,tmp);
  }
  
  //: Get Data from node (VL_DRN only)
  
  VLNodePoolDataC VLNodeMemoryPoolBodyC::Next() {
    DMbuffer tmp;
    MutexLockC alock(server.AccessLock());
    if(vlDMBufferGetValid(Server(), Path(), Node(), &tmp) != VLSuccess) {
      int err = vlGetErrno();
      cerr << "VLNodeMemoryPoolBodyC::Next(), Failed." << err << " : " << vlStrError(err) << "\n";
      return VLNodePoolDataC();
    }
    return VLNodePoolDataC(server,tmp);
  }
  
  //: Send Data to node (VL_SRC only)

  bool VLNodeMemoryPoolBodyC::Send(const VLNodePoolDataC &dat) {
    if(!dat.IsValid()) {
      cerr << "VLNodeMemoryPoolBodyC::Send(), Passed invalid buffer. \n";
      return false; 
    }
    MutexLockC alock(server.AccessLock());
    if(vlDMBufferPutValid(Server(),Path(),Node(),dat.Buff()) < 0) {
      cerr << "VLNodeMemoryPoolBodyC::Send(), Failed to put buffer. \n";
      return false;
    }
    return true;
  }

  void VLNodePoolDataBodyC::Done() { 
    if(!valid)
      return ;
    valid = false;
    MutexLockC alock(server.AccessLock());
    dmBufferFree(buff); 
  }
  
  //: Get a ptr to data in buffer.
  
  void *VLNodePoolDataBodyC::Data() {
    if(!valid)
      return 0;
    MutexLockC alock(server.AccessLock());
    return dmBufferMapData(buff);
  }
  
  //: Get size of buffer in bytes.
  
  UIntT VLNodePoolDataBodyC::Size() {
    if(!valid)
      return 0;
    MutexLockC alock(server.AccessLock());    
    return dmBufferGetSize(buff);
  }
  
  //: Access info on video buffer.
  
  DMBufferVideoInfo &VLNodePoolDataBodyC::VideoInfo() { 
    if(!gotInfo) {
      vlDMBufferGetVideoInfo(buff,&vidInfo);
      gotInfo = true;
    }
    return vidInfo; 
  }
  
  //: Time when frame/field was captures.
  
  DateC VLNodePoolDataBodyC::CaptureTime() {
    if(!gotInfo)
      VideoInfo();
    DMtimecode &tc = vidInfo.vitc[0].tc;
    return DateC(0,0,0,tc.hour,tc.minute,tc.second,tc.frame * (1000000/50));
  }
  
  //: Write buffer to file descriptor.
  // Write raw data to file descriptor.
  
  bool VLNodePoolDataBodyC::WriteFd(int fd,UIntT size) {
    if(size == 0)
      size = Size();
    return (write(fd,Data(),size) == size);
  }
  
  //: Read buffer from file descriptor.
  // Read raw data from file descriptor.
  // No format checking is done.
  
  bool VLNodePoolDataBodyC::ReadFd(int fd,UIntT size) {
    if(size == 0)
      size = Size();
    return (read(fd,Data(),size) == size);
  }
  
  /////////////////////////////////////
  
  //: Allocate node from pool associated with node.
  
  VLNodePoolDataC VLNodeMemoryPoolC::Allocate() {
    return Body().Allocate();
  }
  
  //: Get Data from node (VL_DRN only)
  
  VLNodePoolDataC VLNodeMemoryPoolC::Next() {
    return Body().Next(); 
  }

  
  
  
}
