// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlDMedia
//! file="Ravl/Contrib/DMedia/RingMemory.cc"

#include "Ravl/DMedia/RingMemory.hh"
#include "Ravl/DMedia/RingData.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlImageN
{

  // VLNodeRingMemoryC //////////////////////////////////////////////////////

  //: Constructor.
  
  VLNodeRingMemoryBodyC::VLNodeRingMemoryBodyC(const VLServerC &srv,int type,UIntT nnoFrames)
    : VLNodeBodyC(srv,type,VL_MEM,VL_ANY),
      noFrames(nnoFrames),
      buffer(0),
      seqCount(1),
      nextFree(1)
  {
    ONDEBUG(cerr << "VLNodeRingMemoryBodyC::VLNodeRingMemoryBodyC(), Frames: " << nnoFrames << "\n");
  }

  //: Destructor.
  
  VLNodeRingMemoryBodyC::~VLNodeRingMemoryBodyC() {
    if(buffer != 0) {
      vlDeregisterBuffer(Server(),Path(),Node(),buffer);
      vlDestroyBuffer(Server(),buffer);
    }
  }
  
  //: Initalise componentes.
  
  bool VLNodeRingMemoryBodyC::Init() {
    transferSize = vlGetTransferSize(Server(),Path());
    buffer = vlCreateBuffer(Server(),Path(),Node(),noFrames);
    if(buffer == 0) {
      int err = vlGetErrno();
      cerr << "VLNodeRingMemoryBodyC::Init(), Failed. for " << noFrames << " fields. " << err << " : " << vlStrError(err) << "\n";
      return false;
    }
    if(vlRegisterBuffer(Server(),Path(),Node(),buffer) != 0) {
      cerr << "VLNodeRingMemoryBodyC::Init(), Failed to register buffer. \n";
      return false;
    }
    cerr << "VLNodeRingMemoryBodyC::Init(),  Done. Buffer:" << buffer << "\n";
    return true;
  }
  
  //: Get next piece of data.
  
  VLRingDataC VLNodeRingMemoryBodyC::Next() {
    VLInfoPtr info = 0;
    do {
      if(Type() == VL_DRN) {
	//cerr << "NextValid.... \n";
	if(!skipFrames)
	  info = vlGetNextValid(Server(),buffer);
	else
	  info = vlGetLatestValid(Server(),buffer);
      } else { // VL_SRC
	//cerr << "NextFree.... \n";
	//info = vlGetNextFree(Server(),buffer,1);
	info = vlGetNextFree(Server(),buffer,transferSize);
      }
    } while(info == 0) ;
    //ONDEBUG(cerr << "Next " << seqCount << " " << Type() << "\n");
    return VLRingDataC(VLNodeRingMemoryC(*this),info,transferSize,seqCount++);
  }
  
  //: Tell buffer we're done with this data.
  
  void VLNodeRingMemoryBodyC::DoneWith(VLRingDataBodyC &dat) {
    if(dat.SeqNo() != nextFree) {
      toFree.Insert(dat.SeqNo());
      return ;
    }
    //ONDEBUG(cerr << "DoneWith " << nextFree << " " << Type() << "\n");
    if(Type() == VL_DRN)
      vlPutFree(Server(),buffer);
    else
      vlPutValid(Server(),buffer);
    nextFree++;
    
    // See what can be free'd up....
    while(toFree.IsMember(nextFree)) {
      toFree.Remove(nextFree);
      //ONDEBUG(cerr << "DoneWith (back)" << nextFree << "\n");
      if(Type() == VL_SRC)
	vlPutFree(Server(),buffer);
      else
	vlPutValid(Server(),buffer);
      nextFree++;
    }
    return ;
  }
  
  //: Tell buffer we're done with this data.
  
  void VLNodeRingMemoryBodyC::DoneWith(VLRingDataC &dat) 
  { DoneWith(dat.Body()); }

  //: Get next piece of data.
  
  VLRingDataC VLNodeRingMemoryC::Next()
   { return Body().Next(); }

}
