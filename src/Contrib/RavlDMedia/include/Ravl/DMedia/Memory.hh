// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_DMEDIADMMEMORY_HEADER
#define RAVLIMAGE_DMEDIADMMEMORY_HEADER 1
/////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Contrib/DMedia/Memory.hh"
//! lib=RavlDMedia
//! userlevel=Default
//! docentry="Ravl.API.Images.Video.Video IO.SGI DMedia"
//! author="Charles Galambos"
//! date="22/11/1999"

#include <dmedia/vl.h>
#include "Ravl/DMedia/NodePath.hh"
#include "Ravl/OS/Date.hh"

namespace RavlImageN
{
  class VLNodePoolDataC;
  
  ////// Memory Node ///////////////////////////////////////////
  
  //! userlevel=Develop
  //: Memory node body.
  
  class VLNodeMemoryPoolBodyC 
    : public VLNodeBodyC 
  {
  public:
    VLNodeMemoryPoolBodyC(const VLServerC &srv,int type,UIntT noFrames,bool nglxMode = false);
    //: Constructor.
    
    ~VLNodeMemoryPoolBodyC();
    //: Destructor.
    
    DMbufferpool &Pool()
      { return pool; }
    //: Access pool
    
    VLNodePoolDataC Allocate();
    //: Allocate node from pool associated with node.
    
    VLNodePoolDataC Next();
    //: Get Data from node (VL_DRN only)
    
    bool Send(const VLNodePoolDataC &dat);
    //: Send Data to node (VL_SRC only)
    
  protected:
    virtual bool Init();
    //: Initalise componentes.

    DMbufferpool pool;
    UIntT noFrames;
    bool glxMode;
  };


  //! userlevel=Normal
  //: Memory node body.
  
  class VLNodeMemoryPoolC 
    : public VLNodeC
  {
  public:
    VLNodeMemoryPoolC()
      {}
    //: Default constructor.
    // Creates an invalid handle.
    
    VLNodeMemoryPoolC (const VLServerC &srv,int type,UIntT noFrames,bool nglxMode = false) 
      : VLNodeC(*new VLNodeMemoryPoolBodyC(srv,type,noFrames,nglxMode))
      {}
    //: Constructor.

  protected:

    VLNodeMemoryPoolBodyC &Body()
      { return static_cast<VLNodeMemoryPoolBodyC &>(VLComponentC::Body()); }
    //: Access body.

    const VLNodeMemoryPoolBodyC &Body() const
      { return static_cast<const VLNodeMemoryPoolBodyC &>(VLComponentC::Body()); }
    //: Access body.
    
  public:
    DMbufferpool &Pool()
      { return Body().Pool(); }
    //: Access pool
    
    VLNodePoolDataC Allocate();
    //: Allocate node from pool associated with node.
    
    VLNodePoolDataC Next();
    //: Get Data from node (VL_DRN only)
    
    bool Send(const VLNodePoolDataC &dat)
      { return  Body().Send(dat); }
    //: Send Data to node (VL_SRC only)
    
  };

  ////// Memory Data ///////////////////////////////////////////

  //! userlevel=Develop
  //: Memory buffer body.
  
  class VLNodePoolDataBodyC
    : public RCBodyVC
  {
  public:
    VLNodePoolDataBodyC(const VLServerC &nserver,DMbuffer nbuff)
      : server(nserver),
	valid(true),
	buff(nbuff),
	gotInfo(false)
      {}
    //: Constructor.
    
    ~VLNodePoolDataBodyC()
      { Done(); }
    //: Destructor.
    
    DMbuffer Buff()
      { return buff; }
    //: Access buffer.

    DMbuffer Buff() const
      { return buff; }
    //: Access buffer.
    
    void Done();
    //: Finished with buffer.
    // The buffer will be invalid after.
    // ONLY use this if you know what your
    // doing. 
    
    UIntT Size();
    //: Get size of buffer in bytes.
    
    void *Data();
    //: Get a ptr to data in buffer.
    
    DMBufferVideoInfo &VideoInfo();
    //: Access info on video buffer.
    
    DateC CaptureTime();
    //: Time when frame/field was captures.
    
    RealT Duration() const
      { return 1.0/50.0; }
    //: Duration of data.
    
    bool WriteFd(int fd,UIntT size = 0);
    //: Write buffer to file descriptor.
    // Write raw data to file descriptor.
    
    bool ReadFd(int fd,UIntT size = 0);
    //: Read buffer from file descriptor.
    // Read raw data from file descriptor.
    // No format checking is done.
    
  protected:
    VLServerC server; // Need to for lock on dealloc.
    bool valid;
    DMbuffer buff;
    bool gotInfo;
    DMBufferVideoInfo vidInfo;
  };
  
  //! userlevel=Normal
  //: Memory buffer handle.
  
  class VLNodePoolDataC
    : public RCHandleC<VLNodePoolDataBodyC>
  {
  public:
    VLNodePoolDataC()
      {}
    //: Default constructor.
    // Creates an invalid handle.

    VLNodePoolDataC(const VLServerC &nserver,DMbuffer nbuff)
      : RCHandleC<VLNodePoolDataBodyC>(*new VLNodePoolDataBodyC(nserver,nbuff))
      {}
    //: Default constructor.
    // Creates an invalid handle.
    
    DMbuffer Buff()
      { return Body().Buff(); }
    //: Access buffer.

    DMbuffer Buff() const
      { return Body().Buff(); }
    //: Access buffer.
    
    UIntT Size() 
      { return Body().Size(); }
    //: Get size of buffer in bytes.
    
    void *Data()
      { return Body().Data(); }
    //: Get a ptr to data in buffer.
    
    void Done() 
      { Body().Done(); }
    //: Finished with buffer.
    // The buffer will be invalid after.
    // ONLY use this if you know what your
    // doing. 
    
    DMBufferVideoInfo &VideoInfo()
      { return Body().VideoInfo(); }
    //: Access info on video buffer.

    DateC CaptureTime() 
      { return Body().CaptureTime(); }
    //: Time when frame/field was captures.
    
    RealT Duration() const
      { return Body().Duration(); }

    bool WriteFd(int fd,UIntT size = 0)
      { return Body().WriteFd(fd); }
    //: Write buffer to file descriptor.
    // Write raw data to file descriptor.
    
    bool ReadFd(int fd,UIntT size = 0)
      { return Body().ReadFd(fd); }
    //: Read buffer from file descriptor.
    // Read raw data from file descriptor.
    // No format checking is done.

  };
  
}


#endif
