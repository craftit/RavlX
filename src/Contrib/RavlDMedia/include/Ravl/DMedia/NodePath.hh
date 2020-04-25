// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_DMEDIANODEPATH_HEADER
#define RAVLIMAGE_DMEDIANODEPATH_HEADER 1
/////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Contrib/DMedia/NodePath.hh"
//! lib=RavlDMedia
//! userlevel=Default
//! docentry="Ravl.API.Images.Video.Video IO.SGI DMedia"
//! author="Charles Galambos"
//! date="22/11/1999"

#include <sys/time.h>
#include <vl/vl.h>

#include "Ravl/Image/Image.hh"
#include "Ravl/RefCounter.hh"
#include "Ravl/Threads/Signal1.hh"
#include "Ravl/DMedia/Server.hh"

namespace RavlImageN
{
  class VLServerC;
  class VLNodeC;
  class VLPathC;
  class VLPathBodyC;
  class VLDeviceC;
    
  /////////////////////////////////////////////////
  
  //! userlevel=Develop
  //: Video component body
  
  class VLComponentBodyC 
    : public RCBodyVC
  {
  public:
    VLComponentBodyC(const VLServerC &srv)
      : server(srv)
    {}
    //: Constructor.
    
    virtual bool Init()
    { return true; }
    //: Initalise componentes.
    // Server MUST be locked when calling this function.
    
    VLServer Server()
    { return server.Server(); }
    //: Access server.
  protected:    
    
    VLServerC server;
  };

  //! userlevel=Normal
  //: Video component handle.
  
  class VLComponentC 
    : public RCHandleC<VLComponentBodyC>
  {
  public:
    VLComponentC()
    {}
    //: Default constructor.
    
  protected:
    VLComponentC(VLComponentBodyC &bod)
      : RCHandleC<VLComponentBodyC>(bod)
    {}
    //: Default constructor.
    
    VLComponentBodyC &Body()
    { return RCHandleC<VLComponentBodyC>::Body(); }
    //: Access body.
    
    const VLComponentBodyC &Body() const
    { return RCHandleC<VLComponentBodyC>::Body(); }
    //: Access body.

  public:
    bool Init()
    { return Body().Init(); }
    //: Initalise componentes.
    
    VLServer Server()
    { return Body().Server(); }
    //: Access server.
    
  };
  
  /////////////////////////////////////////////////
  
  class VLDeviceBodyC 
    : public VLComponentBodyC
  {
  public:
    VLDeviceBodyC(const VLServerC &src,const VLDevice &dinfo);
    //: Constructor.
    
    VLDev Device() const
    { return info.dev; }
    //: Access device.
    
    const char *Name() const
    { return info.name; }
    //: Get name of device.
    
  protected:
    VLDevice info; // Information on device...
  };
  
  class VLDeviceC 
    : public VLComponentC
  {
  public:
    VLDeviceC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    VLDeviceC(const VLServerC &srv,const VLDevice &ndevice)
      : VLComponentC(*new VLDeviceBodyC(srv,ndevice))
    {}
    //: Constructor.
    
  protected:
    VLDeviceBodyC &Body()
    { return static_cast<VLDeviceBodyC &>(VLComponentC::Body()); }
    //: Access body.
    
    const VLDeviceBodyC &Body() const
    { return static_cast<const VLDeviceBodyC &>(VLComponentC::Body()); }
    //: Access body.
    
  public:

    VLDev Device() const
    { return Body().Device(); }
    //: Access device.
    
    const char *Name() const
    { return Body().Name(); }
    //: Get name of device.
    
  };
  
  
  /////////////////////////////////////////////////
  
  //! userlevel=Develop
  //: Node body
  
  class VLNodeBodyC 
    : public VLComponentBodyC
  {
  public:
    VLNodeBodyC(const VLServerC &srv,int type,int tclass,int number);
    //: Constructor.
    // type  = VL_SRC,VL_DRN,VL_INTERNAL,VL_DEVICE.
    
    VLNode Node()
    { return node; }
    //: Access node.
    
    VLPath Path() 
    { return path; }
    //: Access path node it on.
    
    int Type() const { return vtype; }
    //: Access node type.
    
    int Class() const { return vclass; }
    //: Access node type.
    
    int Number() const { return vnumber; }
    //: Access node type.
    
    void SetPath(VLPath npath) 
    { path = npath; }
    //: Set path node is on.
    
    bool SetControl(VLControlType ct,VLControlValue *cv);
    //: Set and a control value.
    
    bool SetControl(VLControlType ct,int ival);
    //: Set and check a integer control value 
    
    bool GetControl(VLControlType ct,int &ival);
    //: Get a integer control value 
    
    bool GetSize(int &x,int &y);
    //: Get size of image being transfered.
    
  protected:
    
    VLPath path; // Path node it on.
    VLNode node;

    int vtype;
    int vclass;
    int vnumber;
  };

  //! userlevel=Normal
  //: Node handle.
  
  class VLNodeC 
    : public VLComponentC
  {
  public:
    VLNodeC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    VLNodeC(const VLServerC &srv,int type,int tclass = VL_ANY,int number = VL_ANY)
      : VLComponentC(*new VLNodeBodyC(srv,type,tclass,number))
    {}
    //: Constructor.
    // See 'vlGetNode' man page for details of these.
    // type  = VL_SRC,VL_DRN,VL_INTERNAL,VL_DEVICE.
    // class = VL_VIDEO,VL_GFX,VL_MEM,VL_SCREEN,VL_TEXTURE,VL_BLENDER,
    //  VL_CSC,VL_FB

    
  protected:
    VLNodeC(VLNodeBodyC & bod)
      : VLComponentC(bod)
    {}
    //: Body constructor.
    
    VLNodeBodyC &Body()
    { return static_cast<VLNodeBodyC &>(VLComponentC::Body()); }
    //: Access body.
    
    const VLNodeBodyC &Body() const
    { return static_cast<const VLNodeBodyC &>(VLComponentC::Body()); }
    //: Access body.
    
    void SetPath(VLPath npath) 
    { Body().SetPath(npath); }
    //: Set path node it on.
    
  public:
    VLNode Node()
    { return Body().Node(); }
    //: Get the node...
    
    bool SetControl(VLControlType ct,int ival)
    { return Body().SetControl(ct,ival); }
    
    //: Set and check a integer control value 
    
    bool GetControl(VLControlType ct,int &ival)
    { return Body().GetControl(ct,ival); }
    //: Get a integer control value 
    
    bool GetSize(int &x,int &y)
    { return Body().GetSize(x,y); }
    //: Get size of image being transfered.
    
    friend class VLPathBodyC;
  };
  
  /////////////////////////////////////////////////

  //! userlevel=Develop
  //: Video path body
  
  class VLPathBodyC 
    : public VLComponentBodyC
  {
  public:
    VLPathBodyC(const VLServerC &srv);
    //: Constructor.
    
    VLPathBodyC(const VLServerC &srv,const VLNodeC &src,const VLNodeC &dest,const VLDeviceC &dev = VLDeviceC());
    //: Constructor.
    
    ~VLPathBodyC();
    //: Destructor.

    bool AddNode(const VLNodeC &nd);
    //: Add node to path.
    
    bool BeginTransfer(int totalFrames = 0);
    //: Start transfer of data.
    // totalFrames = number of frames to transfer.
    //  0 == Continuous.
    
    bool EndTransfer();
    //: End transfer of data.
    
    void ResetErrors();
    //: Reset error count.

    VLPath Path() 
    { return path; }
    //: Access path.
    
    VLPath Path() const
    { return path; }
    //: Access path.
    
    bool IsActive() const
    { return isActive; }
    //: Test is path is active.
    // True while transfer is running.
    
    bool WaitForTransferEnd();
    //: Wait for the transfer to end.
    
    Signal1C<VLEvent> &PathEvent()
    { return pathEvent; }
    //: Access path event.
    
    Signal0C &DataReady()
    { return dataReady; }
    // Data ready event.
    
  protected:
    virtual bool Init();
    //: Initalise componentes.
    
    void DeliverEvent(VLEvent *event);
    //: Process an event.
    
    VLPath path;
    bool doneInit;
    bool isActive;
    VLDeviceC device;
    VLDev dev;
    VLNodeC src;
    DListC<VLNodeC> filter;
    VLNodeC dest;
    VLTransferDescriptor xferDesc;
    bool verbose;
    int errSeqLost; // Count of sync lost events.
    Signal1C<VLEvent> pathEvent;
    Signal0C dataReady; // Data ready event.
    
    friend class VLPathC;
    friend class VLServerBodyC;
  };
  
  //! userlevel=Normal
  //: Video path handle.
  
  class VLPathC 
    : public VLComponentC
  {
  public:
    VLPathC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
        
    VLPathC(const VLServerC &srv)
      : VLComponentC(*new VLPathBodyC(srv))
    {}
    //: Constructor.

    VLPathC(const VLServerC &srv,const VLNodeC &src,const VLNodeC &dest,const VLDeviceC & dev)
      : VLComponentC(*new VLPathBodyC(srv,src,dest,dev))
    {}
    //: Constructor.
    
  protected:
    VLPathC(VLPathBodyC &bod)
      : VLComponentC(bod)
    {}
    //: Body constructor.
    
    VLPathBodyC &Body()
    { return static_cast<VLPathBodyC &>(VLComponentC::Body()); }
    //: Access body.

    const VLPathBodyC &Body() const
    { return static_cast<const VLPathBodyC &>(VLComponentC::Body()); }
    //: Access body.

  public:
    bool AddNode(const VLNodeC &nd)
    { return Body().AddNode(nd); }
    //: Add node to path.
    
    VLPath Path() 
    { return Body().Path(); }
    //: Access path.
    
    VLPath Path() const
    { return Body().Path(); }
    //: Access path.

    bool BeginTransfer(int totalFrames = 0)
    { return Body().BeginTransfer(totalFrames); }
    //: Start transfer of data.
    // totalFrames = number of frames to transfer.
    //  0 == Continuous.
    
    bool EndTransfer()
    { return Body().EndTransfer(); }
    //: End transfer of data.
    
    bool IsActive() const
    { return Body().IsActive(); }
    //: Test is path is active.
    // True while transfer is running.
    
    bool WaitForTransferEnd()
    { return Body().WaitForTransferEnd(); }
    //: Wait for the transfer to end.
    
    Signal1C<VLEvent> &PathEvent()
    { return Body().PathEvent(); }
    //: Access path event.
    
    Signal0C &DataReady() 
    { return Body().DataReady(); }
    // Data ready event.
    
    friend class VLPathBodyC;
    friend class VLServerBodyC;
  };
  
  /////////////////////////////////////////////////
}
#endif
