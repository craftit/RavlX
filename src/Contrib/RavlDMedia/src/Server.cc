// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlDMedia
//! file="Ravl/Contrib/DMedia/Server.cc"

#include "Ravl/DMedia/NodePath.hh"
#include "Ravl/OS/Date.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlImageN
{
    // VLServer //////////////////////////////////////////////////////
  
  //: Constructor.
  
  VLServerBodyC::VLServerBodyC(const char *nm) {
    MutexLockC alock(accessLock);
    server = vlOpenVideo(nm);
    if(server == 0) 
      cerr << "Warning: Failed to open connection to video server. \n";
  }
  
  //: Destructor.
  
  VLServerBodyC::~VLServerBodyC() { 
    MutexLockC alock(accessLock);
    vlCloseVideo(server); 
  }

  //: Register path to recieve events..
  
  void VLServerBodyC::Register(const VLPathC &path) {
    eventTab[path.Path()] = const_cast<VLPathBodyC *>(&path.Body());
  }
  
  //: Deregister path to recieve events..
  
  void VLServerBodyC::Deregister(const VLPathBodyC &path) {
    eventTab.Del(path.Path());
  }
  
  //: Check for pending events.
  
  int VLServerBodyC::CheckEvents(bool block) {
    if(server == 0)
      return 0;
    VLEvent event;
    // If block is true only need 1 event.
    //ONDEBUG(cerr << "VLServerBodyC::CheckEvents(), Called \n");
    int eCount = 0;
    MutexLockC alock(accessLock);
    while(vlPending(server) > 0 || block) {
      block = false; 
      if(vlNextEvent(server,&event) != 0) {
	cerr << "VLServerBodyC::CheckEvents(), Error reading event. \n";
	return eCount;
      }
      //ONDEBUG(cerr << "VLServerBodyC::CheckEvents(), Got event : " << event.reason << "\n");
      if(!eventTab.IsElm(event.vlany.path)) {
	cerr << "VLServerBodyC::CheckEvents(), Got event for unknown path. \n";
	continue;
      }
      alock.Unlock();
      eventTab[event.vlany.path]->DeliverEvent(&event);
      alock.Lock();
      eCount++;
    }
    return eCount;
  }
  
  //: Start up event loop.
  
  bool VLServerBodyC::RunEventLoop() {
    while(1) {
      if(CheckEvents(0) > 0)
	continue;
      Sleep(0.01);
    }
    return true;
  }
  
  //: List available devices.
  
  DListC<VLDeviceC> VLServerBodyC::DeviceList() {
    DListC<VLDeviceC> ret;
    VLDevList devList;
    devList.devices = 0;
    MutexLockC alock(accessLock);
    if(vlGetDeviceList(server,&devList) < 0) {
      cerr << "Failed to get device list. \n";
      return ret;
    }
    alock.Unlock();
    for(int i = 0;i < devList.numDevices;i++)
      ret.InsLast(VLDeviceC(VLServerC(*this),devList.devices[i]));
    return ret;
  }
  
  //: Get device of given name.
  // If none, invalid handle is returned.
  
  VLDeviceC VLServerBodyC::FindDevice(const StringC &nm) {
    VLDeviceC ret;
    DListC<VLDeviceC> devs(DeviceList());
    for(DLIterC<VLDeviceC> it(devs);it.IsElm();it.Next()) {
      if(nm == it.Data().Name())
	return it.Data();
    }
    return ret;
  }

  DListC<VLDeviceC> VLServerC::DeviceList()
  { return Body().DeviceList(); }

  //: Get device of given name.
  // If none, invalid handle is returned.
  
  VLDeviceC VLServerC::FindDevice(const StringC &nm) 
  { return Body().FindDevice(nm); }
  
}
