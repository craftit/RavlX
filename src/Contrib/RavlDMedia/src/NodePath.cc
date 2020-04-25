// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlDMedia
//! file="Ravl/Contrib/DMedia/NodePath.cc"

#include "Ravl/DMedia/NodePath.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlImageN
{
  // VLDevice //////////////////////////////////////////////////////
  
  //: Constructor.
  
  VLDeviceBodyC::VLDeviceBodyC(const VLServerC &srv,const VLDevice &ndevice)
    : VLComponentBodyC(srv),
      info(ndevice)
  {}
  
  
  // VLNode //////////////////////////////////////////////////////
  
  //: Constructor.
  
  VLNodeBodyC::VLNodeBodyC(const VLServerC &srv,int type,int tclass,int number)
    : VLComponentBodyC(srv),
      path(0),
      vtype(type),
      vclass(tclass),
      vnumber(number)
  { 
    MutexLockC alock(server.AccessLock());
    node = vlGetNode(Server(),type,tclass,number); 
    if(node < 0) {
      cerr << "Error creating node. \n";
    }
  }
  
  //: Set and check a control value.
  
  bool VLNodeBodyC::SetControl(VLControlType ct,VLControlValue *cv) {
    MutexLockC alock(server.AccessLock());
    return (vlSetControl(Server(),Path(),Node(),ct,cv) == 0);
  }
  
  //: Set and check a integer control value 
  
  bool VLNodeBodyC::SetControl(VLControlType ct,int ival) {
    RavlAssert(Path() != 0); // Path must be set before setcontrol is used.
    VLControlValue val,val2;
    val.intVal = ival;
    //cerr << "SetControl " <<Server() << " " << Path() << " " << Node() << "\n";
    MutexLockC alock(server.AccessLock());
    if(vlSetControl(Server(),Path(),Node(),ct,&val) < 0) {
      int err = vlGetErrno();
      cerr << "Failed to set control " << ct << " to " << ival << "\n";
      cerr << "VLPathBodyC::SetControl(),  " << err << " : " << vlStrError(err) << "\n";
      return false;
    }
    if(vlGetControl(Server(),Path(),Node(),ct,&val2) < 0) {
      int err = vlGetErrno();
      cerr << "Failed to get control " << ct << " to " << ival << "\n";
      cerr << "VLPathBodyC::SetControl(),  " << err << " : " << vlStrError(err) << "\n";
      return false;
    }
    if(val.intVal != val2.intVal) {
      cerr << "Failed to set control correctly " << ct << " to " << ival << " (" << val2.intVal << ")\n";
      return false;
    }
    return true;
  }
  
  //: Get size of image being transfered.
  
  bool VLNodeBodyC::GetSize(int &x,int &y) {
    VLControlValue vlSize;
    if(vlGetControl(Server(),Path(),Node(),VL_SIZE,&vlSize) < 0) {
      int err = vlGetErrno();
      cerr << "Failed to get image size.\n";
      cerr << "VLPathBodyC::GetSize(),  " << err << " : " << vlStrError(err) << "\n";
      return false;
    }
    y = vlSize.xyVal.x;
    x = vlSize.xyVal.y;
    return true;
  }
  
  //: Get a integer control value 
  
  bool VLNodeBodyC::GetControl(VLControlType ct,int &ival) {
    RavlAssert(Path() != 0); // Path must be set before setcontrol is used.
    VLControlValue val;
    //cerr << "SetControl " <<Server() << " " << Path() << " " << Node() << "\n";
    MutexLockC alock(server.AccessLock());
    if(vlGetControl(Server(),Path(),Node(),ct,&val) < 0) {
      int err = vlGetErrno();
      cerr << "Failed to get control " << ct << " to " << ival << "\n";
      cerr << "VLPathBodyC::GetControl(),  " << err << " : " << vlStrError(err) << "\n";
      return false;
    }
    alock.Unlock();
    ival = val.intVal;
    return true;    
  }


  // VLPath //////////////////////////////////////////////////////

  //: Constructor.
  
  VLPathBodyC::VLPathBodyC(const VLServerC &srv) 
    : VLComponentBodyC(srv),
      dev(VL_ANY),
      path(0),
      doneInit(false),
      isActive(false),
      verbose(false),
      errSeqLost(0),
      pathEvent(VLEvent()),
      dataReady(true)
  {}
  
  //: Constructor.
  
  VLPathBodyC::VLPathBodyC(const VLServerC &srv,
			   const VLNodeC &nsrc,
			   const VLNodeC &ndest,
			   const VLDeviceC &ndev
			   )
    : VLComponentBodyC(srv),
      path(0),
      doneInit(false),
      isActive(false),
      device(ndev),
      dev(VL_ANY),
      src(nsrc),
      dest(ndest),
#if DODEBUG
      verbose(true),
#else
      verbose(false),
#endif
      errSeqLost(0),
      pathEvent(VLEvent()),
      dataReady(true)
  {
    if(device.IsValid())
      dev = device.Device();
    
    if(!src.IsValid() || !dest.IsValid()) {
      cerr << "ERROR: No source or destination defined. \n";
      return ;
    }
    MutexLockC alock(server.AccessLock());
    path = vlCreatePath(Server(),dev,src.Node(),dest.Node());
    if(path < 0) {
      int err = vlGetErrno();
      cerr << "VLPathBodyC(), Failed to creat path. " << err << " : " << vlStrError(err) << "\n";
      return ;
    }
    
    src.SetPath(path);  
    dest.SetPath(path);
    
    if(vlSetupPaths(Server(), (VLPathList)&path,1,VL_SHARE,VL_SHARE) < 0) {
      int err = vlGetErrno();
      cerr << "VLPathBodyC::Init(), Failed to setup paths. " << err << " : " << vlStrError(err) << "\n";
      return ;
    }
  }
  
  //: Destructor.
  
  VLPathBodyC::~VLPathBodyC() {
    server.Deregister(*this);
    if(path != 0) {
      MutexLockC alock(server.AccessLock());
      vlDestroyPath(Server(),path);
    }
  }
  
  //: Add node to path.
  
  bool VLPathBodyC::AddNode(const VLNodeC &nd) {
    RavlAssert(0); // Not implemented.
    return false;
  }

  //: Initalise componentes.
  
  bool VLPathBodyC::Init() {
    if(doneInit)
      return true;
    ONDEBUG(cerr << "VLPathBodyC::Init(), PathID : " << path << ".\n");
    
    server.Register(VLPathC(*this)); // Register with server.
    
    // Setup event mask now.
    
    int mask = VLStreamStartedMask
      | VLTransferCompleteMask
      | VLTransferFailedMask
      | VLTransferErrorMask
      | VLStreamStartedMask
      | VLStreamStoppedMask
      | VLSequenceLostMask
      | VLControlChangedMask
#if 0
      | VLOddVerticalRetraceMask
      | VLEvenVerticalRetraceMask
      | VLFrameVerticalRetraceMask
#endif
      ;
    
    MutexLockC alock(server.AccessLock());
    vlSelectEvents(Server(),path, mask);
    
    // Setup nodes...
    
    if(!src.Init()) {
      cerr << "VLPathBodyC(), Failed to init source. \n";
      return false;
    }
    if(!dest.Init()) {
      cerr << "VLPathBodyC(), Failed to init destination. \n";
      return false;
    }
    for(DLIterC<VLNodeC> it(filter);it.IsElm();it.Next()) {
      it.Data().SetPath(path);
      if(!it.Data().Init()) {
	cerr << "Failed to init filter. \n";
	return false; 
      }
    }
    //ONDEBUG(cerr << "VLPathBodyC::Init(), Path constructed. \n");
    doneInit = true;
    return true;
  }
  
  //: Start transfer of data.
  
  bool VLPathBodyC::BeginTransfer(int totalFrames) {
    if(!doneInit)
      if(!Init())
	return false;
    if(totalFrames == 0) {
      xferDesc.mode = VL_TRANSFER_MODE_CONTINUOUS;
      xferDesc.count = 0;
    } else {
      xferDesc.mode = VL_TRANSFER_MODE_DISCRETE;
      xferDesc.count = totalFrames;
    }
    xferDesc.delay = 0;
    xferDesc.sequence = 0;
    xferDesc.ustime = 0;
    xferDesc.trigger = VLTriggerImmediate;
    
    MutexLockC alock(server.AccessLock());
    
    if(vlBeginTransfer(Server(),Path(),1,&xferDesc) == 0) {
      isActive = true;
    } else {
      int err = vlGetErrno();
      cerr << "VLPathBodyC::BeginTransfer(). Transfer of " << totalFrames << " frames failed.  Error=" << err << " : " << vlStrError(err) << "\n";
      return false;
    }
    return isActive;
  }
    
  //: End transfer of data.
  
  bool VLPathBodyC::EndTransfer() {
    if(!doneInit)
      return true; // Not initalised, so must succed.
    MutexLockC alock(server.AccessLock());
    return vlEndTransfer(Server(),Path()) == 0;
  }

  //: Reset error count.
  
  void VLPathBodyC::ResetErrors() {
    errSeqLost = 0;
  }
  
  //: Process an event.
  //: Check for events.
  
  void VLPathBodyC::DeliverEvent(VLEvent *event) {
    ONDEBUG(cerr << "VLPathBodyC::DeliverEvent(), Got event... \n");
    RavlAssert(event != 0);
    pathEvent(*event);
    switch(event->reason)
      {
      case VLStreamStarted:
	isActive = true;
	if(verbose)
	  cerr << "Stream started. \n";
	break;
      case VLSequenceLost:
	errSeqLost++;
	//if(verbose)
	cerr << "Sequence lost. \n";
	break;
      case VLSyncLost:
	if(verbose)
	  cerr << "Sync signal lost. \n";
	break;
      case VLTransferError:
      case VLTransferFailed:
	//if(verbose)
	cerr << "Transfer failed.. \n";
	break;
      case VLStreamStopped:
	isActive = false;
	if(verbose)
	  cerr << "Transfer stopped\n";
	break;
      case VLTransferComplete:
	if(verbose)
	  cerr << "Transfer complete\n";
	dataReady();
	break;
      case VLControlChanged:
	if(verbose)
	  cerr << "Control changed\n";
	break;
      case VLOddVerticalRetrace:
      case VLEvenVerticalRetrace:
      case VLFrameVerticalRetraceMask:
	ONDEBUG(cerr << "Retrace:" << event->reason << "\n");
	break; // Ignore these...
      default:
	if(verbose)
	  cerr << "Unknown event " << event->reason << "\n";
	break;
      }
  }

  //: Wait for the transfer to end.

  bool VLPathBodyC::WaitForTransferEnd() {
    do {
      server.CheckEvents(true);
    } while(IsActive());
    return true;
  }
    
};
