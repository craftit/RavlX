// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_DMEDIASERVER_HEADER
#define RAVLIMAGE_DMEDIASERVER_HEADER 1
/////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Contrib/DMedia/Server.hh"
//! lib=RavlDMedia
//! userlevel=Develop
//! docentry="Ravl.API.Images.Video.Video IO.SGI DMedia"
//! author="Charles Galambos"
//! date="22/11/1999"

#include "Ravl/Threads/Mutex.hh"
#include "Ravl/DList.hh"
#include "Ravl/Hash.hh"
#include "Ravl/String.hh"

extern "C" {
  typedef struct _VLServer *VLServer;
  typedef int VLPath;
};

namespace RavlImageN
{
  using namespace RavlN;
  
  class VLServerC;
  class VLNodeC;
  class VLPathC;
  class VLPathBodyC;
  class VLDeviceC;
  
  //! userlevel=Develop
  //: Server handle body.
  
  class VLServerBodyC 
    : public RCBodyC
  {
  public:
    VLServerBodyC(const char *nm);
    //: Constructor.
    
    ~VLServerBodyC();
    //: Destructor.
    
    VLServer Server()
      { return server; }
    //: Access server.
    
    bool IsOpen() const 
      { return server != 0; }
    //: Is connection to server open ?
    
    void Register(const VLPathC &path);
    //: Register path to recieve events..
    
    int CheckEvents(bool block = false);
    //: Check for pending events.
    // if block == true, the CheckEvents will not
    // return until at least one event is processed.
    
    bool RunEventLoop();
    //: Start up event loop.
    
    DListC<VLDeviceC> DeviceList();
    //: List available devices.
    
    VLDeviceC FindDevice(const StringC &nm);
    //: Get device of given name.
    // If none, invalid handle is returned.
    
    MutexC &AccessLock() 
      { return accessLock; }
    //: Access control.
    
  protected:
    void Deregister(const VLPathBodyC &path);
    //: Deregister path to recieve events..

    MutexC accessLock; // Access control.
    VLServer server;
    HashC<VLPath,VLPathBodyC *> eventTab;
    
    friend class VLPathBodyC;
    friend class VLServerC;
  };
  
  //! userlevel=Normal
  //: Server handle.
  
  class VLServerC 
    : public RCHandleC<VLServerBodyC>
  {
  public:
    VLServerC()
      {}
    //: Default constructor.
    // Creates an invalid handle.
    
    VLServerC(const char *nm)
      : RCHandleC<VLServerBodyC>(*new VLServerBodyC(nm))
      {}
    //: Constructor.
    
    bool IsOpen() const 
      { return Body().IsOpen(); }
    //: Is connection to server open ?

    VLServer Server()
      { return Body().Server(); }
    //: Access server.
    
    void CheckEvents(bool block = false)
      { Body().CheckEvents(block); }
    //: Check for pending events.
    // if block == true, the CheckEvents will not
    // return until at least one event is processed.
    
    bool RunEventLoop()
      { return Body().RunEventLoop(); }
    //: Start up event loop.
    
    DListC<VLDeviceC> DeviceList();
    //: List available devices.
    // return until at least one event is processed.
    
    VLDeviceC FindDevice(const StringC &nm);
    //: Get device of given name.
    // If none, invalid handle is returned.
    
    MutexC &AccessLock() 
      { return Body().AccessLock(); }
    //: Access control.
    
  protected:
    VLServerC(VLServerBodyC &bod)
      : RCHandleC<VLServerBodyC>(bod)
      {}
    //: Body Constructor.
    
    void Register(const VLPathC &path)
      { Body().Register(path); }
    //: Register path to recieve events..
    
    void Deregister(const VLPathBodyC &path)
      { Body().Deregister(path); }
    //: Deregister path to recieve events..
    
    friend class VLPathBodyC;
    friend class VLServerBodyC;
  };

};
#endif
