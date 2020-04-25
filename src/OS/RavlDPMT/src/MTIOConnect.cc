// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlDPMT
//! file="Ravl/OS/DataProc/MTIOConnect.cc"

#include "Ravl/DP/MTIOConnect.hh"
#include "Ravl/Threads/LaunchThread.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  
  class DPEventCompleteMTIOConnectBodyC 
    : public DPEventBodyC 
  {
  public:
    DPEventCompleteMTIOConnectBodyC(const DPMTIOConnectBaseC &con)
      : connection(con)
    {}
    //: Constructor.
    
    virtual bool Wait()
    { return connection.Wait(); }
    
    //: Wait for event.
    // Return when event occurres returns true.
    // or if event will never occurre returns false.
    // This will decrement the pending event count by one.
    
    virtual IntT Occurred() {
      if(connection.IsDisconnected())
	return 1;
      return 0;
    }
    //: Has event occured ?
    // Returns number of events since last call.
    
  private:
    DPMTIOConnectBaseC connection;
  };


  //: Default Constructor.

  DPMTIOConnectBaseBodyC::DPMTIOConnectBaseBodyC(bool nuseIsGetReady,UIntT nblockSize)
    : useIsGetReady(nuseIsGetReady),
      terminate(false),
      pause(false),
      resume(0),
      blockSize(nblockSize)
  {}

  //: Generate an event handle 
  // It indicates the completion of processing.
  
  DPEventC DPMTIOConnectBaseBodyC::EventComplete() { 
    return DPEventC(*new DPEventCompleteMTIOConnectBodyC(DPMTIOConnectBaseC(*this))); 
  }
  
  //: Stop connection.
  
  bool DPMTIOConnectBaseBodyC::Disconnect() {
    bool ret = ! terminate;
    terminate = true;
    return ret;
  }

  //: Run until a stream completes.
  
  bool DPMTIOConnectBaseBodyC::Run() {
    ONDEBUG(cerr << "DPMTIOConnectBaseBodyC::Run(), Called. \n");
    if(!IsReady())
      return false;
    if(IsRunning())
      return false;
    terminate = false;
    ONDEBUG(cerr << "DPMTIOConnectBaseBodyC::Run(), Launching thread. \n");
    LaunchThread(DPMTIOConnectBaseC(*this),&DPMTIOConnectBaseC::Start);
    return true;
  }
  
  //: Attempt to stop stream processing.
  
  bool DPMTIOConnectBaseBodyC::Stop() {
    terminate = true;
    return true;
  }
  
  //: Do some async stuff. 
  
  bool DPMTIOConnectBaseBodyC::Start() {
    RavlAssertMsg(0,"DPMTIOConnectBaseBodyC::Start(), Abstract method called. ");
    return true;
  }
  
  //: Check if we're ready to run.
  
  bool DPMTIOConnectBaseBodyC::IsReady() const {
    RavlAssertMsg(0,"DPMTIOConnectBaseBodyC::IsReady(), Abstract method called. ");
    return false;
  }

  //: Stop connection.
  
  bool DPMTIOConnectBaseC::Disconnect() 
  { return Body().Disconnect(); }

  
}
