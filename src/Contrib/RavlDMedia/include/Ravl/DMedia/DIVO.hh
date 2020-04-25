// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_DMEDIADIVO_HEADER
#define RAVLIMAGE_DMEDIADIVO_HEADER 1
///////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Contrib/DMedia/DIVO.hh"
//! lib=RavlDMedia
//! docentry="Ravl.API.Images.Video.Video IO.SGI DMedia"
//! userlevel=Default
//! author="Charles Galambos"
//! date="22/11/1999"

#include "Ravl/DP/SPort.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/Stream.hh"
#include "Ravl/Array2dIter2.hh"
#include "Ravl/DMedia/Memory.hh"
#include "Ravl/DMedia/Buffer.hh"
#include "Ravl/DMedia/NodePath.hh"
#include "Ravl/DMedia/Video.hh"
#include "Ravl/Threads/SemaphoreRC.hh"

namespace RavlImageN
{
  using namespace RavlN;
  
  ///////////////////////////////////////////////////////////////////

  //! userlevel=Develop
  //: Base for DIVO IO classes.
  
  class DPVideoDIVOBaseBodyC {
  public:
    DPVideoDIVOBaseBodyC(const VLServerC &server,const char *nm,const type_info &pixType,bool nframe = false,bool nglxMode=false);
    //: Constructor.
    
    bool SetupParamiters();
    //: Setup path paramiters.
    
    SemaphoreRC &ImageReady()
    { return imageReady; }
    //: Access image ready semaphore.
    
  protected:
    const char *devName;
    VLServerC server;
    VLNodeVideoC vid;
    VLNodeMemoryPoolC node;
    VLPathC path;
    bool active;
    ImageRectangleC rect;
    bool glxMode;
    bool frameMode;
    const type_info &pixType;
    SemaphoreRC imageReady;
  };

  //! userlevel=Develop
  //: Base for DIVO I classes.
  
  class DPIVideoDIVOBaseBodyC
    : public DPVideoDIVOBaseBodyC
  {
  public:
    DPIVideoDIVOBaseBodyC(const VLServerC &server,const char *nm,const type_info &pixType,bool nframe = false,bool nglxMode=false, IntT noFrames=10);
    //: Constructor
    
    IntT SubSample() const
    { return subSample; }
    //: Get the sub sample rate.
    
  protected:
    IntT subSample;
  };

  //! userlevel=Develop
  //: Base for DIVO O classes.
  
  class DPOVideoDIVOBaseBodyC 
    : public DPVideoDIVOBaseBodyC
  {
  public:
    DPOVideoDIVOBaseBodyC(const VLServerC &server,const char *nm,const type_info &pixType,bool nframe = false,bool nglxMode=false);
    //: Constructor
    
  protected:
  };
  
  ///////////////////////////////////////////////////////////////////

  //! userlevel=Develop
  //: Input video body.

  template<class DataT>
  class DPIVideoDIVOBodyC
    : public DPISPortBodyC<ImageC<DataT> >,
      public DPIVideoDIVOBaseBodyC
  {
  public:
    DPIVideoDIVOBodyC(const VLServerC &server,const char *name,bool nframe = false,bool nglxMode=false, IntT noFrames=10) 
      : DPIVideoDIVOBaseBodyC(server,name,typeid(DataT),nframe,nglxMode, noFrames)
    {}
    //: Body constructor.
    
    virtual bool Get(ImageC<DataT> &buff) { 
      VLNodePoolDataC pd;
      if(!imageReady.Wait()) // Wait for ready signal.
	return false;
      
      pd = node.Next();
      RavlAssert(pd.IsValid());
      ImageC<DataT> inImg(rect.Rows(),rect.Cols(),DMBufferC<DataT>(pd));

      // FIXME: should do this in Memory.cc
      USTMSCpair time = dmBufferGetUSTMSCpair(pd.Buff());
      DateC date((long) time.ust / 1000000000,(long) (time.ust / 1000) % 1000);
      
      buff = inImg;
      return true;
    }
    //: Get next image.
    
    virtual ImageC<DataT> Get() {
      ImageC<DataT> buff;
      if(!Get(buff))
	throw DataNotReadyC("Invalid buffer returned from node.");
      return buff;
    }
    //: Get next image.
    
    virtual bool IsGetReady() const 
    { return node.IsValid(); }
    //: Is some data ready ?
    // true = yes.
    // Defaults to !IsGetEOS().
    
    virtual bool IsGetEOS() const 
    { return !node.IsValid(); }
    //: Has the End Of Stream been reached ?
    // true = yes.
    
    bool Seek(UIntT off)
    { return false; }
    //: Seek to location in stream.
    
    bool DSeek(IntT)
    { return false; }
    //: Delta Seek, goto location relative to the current one.
    
    UIntT Tell() const
    { return 0; }
    //: Find current location in stream.
    // May return ((UIntT) (-1)) if not implemented.
    
    UIntT Size() const
    { return ((UIntT) (-1)); }
    //: Find the total size of the stream. (assuming it starts from 0)
    
    UIntT Start() const
    { return 0; }
    //: Find the offset where the stream begins, normally zero.
    
  protected:
  };

  //! userlevel=Advanced
  //: Input video handle.
  
  template<class DataT>
  class DPIVideoDIVOC
    : public DPISPortC<ImageC<DataT> >
  {
  public:
    DPIVideoDIVOC()
      : DPEntityC(true)
    {}
    //: Default constructor.
    
    DPIVideoDIVOC(const VLServerC &server,const char *text,bool nframe = false,bool nglxMode=false, IntT noFrames=10)
      : DPEntityC(*new DPIVideoDIVOBodyC<DataT>(server,text,nframe,nglxMode,noFrames))
    {}
    //: Default constructor.
    
  };
  
  ///////////////////////////////////////////////////////////////////
  
  //! userlevel=Develop
  //: Output video body.
  
  template<class DataT>
  class DPOVideoDIVOBodyC
    : public DPOSPortBodyC<ImageC<DataT> >,
      public DPOVideoDIVOBaseBodyC
  {
  public:
    DPOVideoDIVOBodyC(const VLServerC &server,const char *name,bool nframe = false,bool nglxMode=false)
      : DPOVideoDIVOBaseBodyC(server,name,typeid(DataT),nframe,nglxMode)
    {}
    //: Body constructor.
    
    bool Put(const ImageC<DataT> &dat) { 
      if(!dat.IsValid())
	return false; // Invalid frame.
      ImageC<DataT> img(dat);
      if(!img.IsValid())
	return false; // Invalid image.
      DMBufferC<DataT> obuff(img.Buffer2d().Data());
      VLNodePoolDataC outDat;
      if(obuff.IsValid()) {
	outDat = obuff.PoolData();
      } else {
	outDat = node.Allocate();
	ImageC<DataT> buffImg(rect.Rows(),rect.Cols(),DMBufferC<DataT>(outDat));
	ImageRectangleC clipRect(rect);
	clipRect.ClipBy(img.Rectangle());	  
	ImageC<DataT> clippedBuff(buffImg,clipRect);
	ImageC<DataT> clippedSrc(img,clipRect);
	for(Array2dIter2C<DataT,DataT> it(clippedBuff,clippedSrc);it.IsElm();it.Next()) 
	  it.Data1() = it.Data2();
      }
      node.Send(outDat);
      return true;
    }
    //: Put a frame to 
    
    void PutEOS()
    {}
    //: Put End Of Stream marker.
    
    bool IsPutReady() const
    { return node.IsValid(); }
    //: Is port ready for data ?
    
    bool Seek(UIntT off)
    { return false; }
    //: Seek to location in stream.
    
    bool DSeek(IntT)
    { return false; }
    //: Delta Seek, goto location relative to the current one.
    
    UIntT Tell() const
    { return 0; }
    //: Find current location in stream.
    // May return ((UIntT) (-1)) if not implemented.
    
    UIntT Size() const
    { return ((UIntT) (-1)); }
    //: Find the total size of the stream. (assuming it starts from 0)
    
    UIntT Start() const
    { return 0; }
    //: Find the offset where the stream begins, normally zero.
    
  protected:
  };
  
  //! userlevel=Advanced
  //: Output video handle.
  
  template<class DataT>
  class DPOVideoDIVOC
    : public DPOSPortC<ImageC<DataT> >
  {
  public:
    DPOVideoDIVOC()
      : DPEntityC(true)
    {}
    //: Default constructor.
    
    DPOVideoDIVOC(const VLServerC &server,const char *text,bool frame = false,bool nglxMode=false)
      : DPEntityC(*new DPOVideoDIVOBodyC<DataT>(server,text,frame,nglxMode))
    {}
    //: Default constructor.
    
  };
  
}
  
#endif
