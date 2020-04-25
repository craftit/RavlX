// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlDMedia
//! userlevel=Normal
//! docentry="Ravl.API.Images.Video.Video IO.SGI DMedia"
//! author="Charles Galambos"
//! file="Ravl/Contrib/DMedia/exDMedia.cc"

#include "Ravl/DMedia/NodePath.hh"
#include "Ravl/DMedia/Video.hh"
#include "Ravl/DMedia/RingMemory.hh"
#include "Ravl/DMedia/RingData.hh"

using namespace RavlImageN;

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

int main()
{
  VLServerC server(""); // Open connection to video server.
  
  DListC<VLDeviceC> devs = server.DeviceList();
  
  for(DLIterC<VLDeviceC> it(devs);it.IsElm();it.Next())
    cerr << "Device " << it.Data().Name() << "\n";
  
  ONDEBUG(cerr << "Setting up input. \n");
  
  VLNodeVideoC  videoIn(server,VL_SRC,VL_ANY);
  VLNodeRingMemoryC memIn(server,VL_DRN,10);  
  VLPathC pathIn(server,videoIn,memIn,devs.First());

  ONDEBUG(cerr << "Setting up output 1. \n");
  
  VLNodeVideoC  videoOut(server,VL_DRN,VL_ANY);
  VLNodeRingMemoryC memOut(server,VL_SRC,10);  
  VLPathC pathOut(server,videoOut,memOut,devs.First());

  ONDEBUG(cerr << "Setting up output 2. \n");
  
  VLNodeVideoC  videoOut2(server,VL_DRN,VL_ANY);
  VLNodeRingMemoryC memOut2(server,VL_SRC,10);  
  VLPathC pathOut2(server,videoOut2,memOut2,devs.Last());
  
  ONDEBUG(cerr << "Setting up stream format. \n");
  
  memIn.SetControl(VL_COLORSPACE,VL_COLORSPACE_RGB);  
  memOut.SetControl(VL_COLORSPACE,VL_COLORSPACE_RGB);  
  memOut2.SetControl(VL_COLORSPACE,VL_COLORSPACE_RGB);  
  
  memIn.SetControl(VL_CAP_TYPE,VL_CAPTURE_FIELDS);  
  memOut.SetControl(VL_CAP_TYPE,VL_CAPTURE_FIELDS);  
  memOut2.SetControl(VL_CAP_TYPE,VL_CAPTURE_FIELDS);  
  
  memIn.SetControl(VL_PACKING,VL_PACKING_444_8);  
  memOut.SetControl(VL_PACKING,VL_PACKING_444_8);
  memOut2.SetControl(VL_PACKING,VL_PACKING_444_8);

  memIn.SetControl(VL_COMPRESSION,VL_COMPRESSION_NONE);  
  memOut.SetControl(VL_COMPRESSION,VL_COMPRESSION_NONE);
  memOut2.SetControl(VL_COMPRESSION,VL_COMPRESSION_NONE);
  
  memIn.SetControl(VL_TIMING,VL_TIMING_625_CCIR601);
  memOut.SetControl(VL_TIMING,VL_TIMING_625_CCIR601);
  memOut2.SetControl(VL_TIMING,VL_TIMING_625_CCIR601);
  
  videoIn.SetControl(VL_TIMING,VL_TIMING_625_CCIR601);
  videoOut.SetControl(VL_TIMING,VL_TIMING_625_CCIR601);
  videoOut2.SetControl(VL_TIMING,VL_TIMING_625_CCIR601);
  
  videoIn.SetControl(VL_FORMAT,VL_FORMAT_DIGITAL_COMPONENT_SERIAL);
  videoOut.SetControl(VL_FORMAT,VL_FORMAT_DIGITAL_COMPONENT_SERIAL);
  videoOut2.SetControl(VL_FORMAT,VL_FORMAT_DIGITAL_COMPONENT_SERIAL);
  
  ONDEBUG(cerr << "Beginning output 1 transfer. \n");
  
  if(!pathOut.BeginTransfer()) {
    cerr << "Failed to begin transfer. \n";
    return 1;
  }
  
  ONDEBUG(cerr << "Beginning output 2 transfer. \n");
  
  if(!pathOut2.BeginTransfer()) {
    cerr << "Failed to begin transfer. \n";
    return 1;
  }
  
  ONDEBUG(cerr << "Beginning input transfer. \n");
  
  if(!pathIn.BeginTransfer()) {
    cerr << "Failed to begin transfer. \n";
    return 1;
  }
  
  cerr << "Waiting for transfer to complete. \n";
  
  while(pathIn.IsActive()) {
    ONDEBUG(cerr << "Getting buffers. \n");
    VLRingDataC itmp = memIn.Next();
    VLRingDataC otmp = memOut.Next();
    VLRingDataC otmp2 = memOut2.Next();
    RavlAssert(itmp.IsValid());
    RavlAssert(otmp.IsValid());
    ONDEBUG(cerr << "Copy data. \n");
    otmp.CopyFrom(itmp);
    otmp2.CopyFrom(itmp);
    ONDEBUG(cerr << "Done.... \n");
    otmp.Done();
    otmp2.Done();
    itmp.Done();
  }
  
  //path.WaitForTransferEnd();
  cerr << "Done. \n";
  return 0;
}
