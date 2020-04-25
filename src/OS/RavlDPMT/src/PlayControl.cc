// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlDPMT
//! file="Ravl/OS/DataProc/PlayControl.cc"

#include "Ravl/DP/PlayControl.hh"
#include "Ravl/CallMethods.hh"

#define FORCE_AT 0

#define DPDEBUG 0
#if DPDEBUG 
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {

  //: Default constructor.
  
  DPPlayControlBodyC::DPPlayControlBodyC()
    : access(true), // Setup a recursive mutex.
      inc(1),
      ok(true),
      pause(false),
      sema(0),
      passEOS(true),
      start(0),
      end((UIntT) -1),
      playMode(0),
      subStart(0),
      subEnd(0),
      doneRev(false),
      at(0),
      lastFrame(0),
      triggerSizeUpdate(-1),
      triggerStartUpdate(-1)
  {}
  
  //: Constructor.
  
  DPPlayControlBodyC::DPPlayControlBodyC(const DPSeekCtrlC &nCntrl,bool nPassEOS,UIntT nstart,UIntT nend)
    : access(true), // Setup a recursive mutex.
      inc(1),
      ok(true),
      pause(false),
      sema(0),
      ctrl(nCntrl),
      passEOS(nPassEOS),
      start(nstart),
      end(nend),
      playMode(0),
      subStart(1),
      subEnd(1),
      doneRev(false),
      at(nstart),
      lastFrame(0),
      triggerSizeUpdate(-1),
      triggerStartUpdate(-1)
  {
    if(ctrl.IsValid()) {
      UIntT len =  ctrl.Size();
      if(end >= len) {
	if(len != ((UIntT) -1))
	  end = len -1;
	else 
	  end = len;
      }
      MutexLockC lock(access);
      if(start != ((UIntT) -1))
	ctrl.Seek64(start);
      else
	start = ctrl.Start();
      at = ctrl.Tell64();
    }
    if(start == ((UIntT) -1))
      start = 0;
    if(at == -1)
      at = 0;
    lastFrame = start;
    ONDEBUG(cerr << "Start : "<< start << " at:" << at<< " End:" << end <<"\n");
  }
  
  //: Open new video stream.
  // Access must be locked when calling this function.
  bool DPPlayControlBodyC::Open(const DPSeekCtrlC &nCntrl, bool bPlayMode /*=true*/) 
  {
    
    // Free up any existing size and start change triggers.
    if(ctrl.IsValid()) {
      AttributeCtrlC attrCtrl(static_cast<DPEntityC &>(ctrl));
      if(triggerSizeUpdate >= 0) {
	if(!attrCtrl.RemoveChangedSignal(triggerSizeUpdate))
	  cerr << "DPPlayControlBodyC::Open, Internal Error: failed to remove size changed signal. " << triggerStartUpdate << " \n";
	triggerSizeUpdate = -1;
      }
      if(triggerStartUpdate >= 0) {
	if(!attrCtrl.RemoveChangedSignal(triggerStartUpdate))
	  cerr << "DPPlayControlBodyC::Open, Internal Error: failed to remove start changed signal. " << triggerStartUpdate << "\n";
	triggerStartUpdate = -1;
      }
    } else {
      RavlAssert(triggerSizeUpdate < 0);
      RavlAssert(triggerStartUpdate < 0);
    }
    
    // Get on with sorting out the new sequence.
    inc = (bPlayMode == true ? 1 : 0);
    ok = true;
    ctrl = nCntrl;
    playMode = 0;
    subStart = 1;
    subEnd = 1;
    // Preserve sequence limits ???
    if(!ctrl.IsValid()) {
      // Seek ctrl isn't valid.
      start = 0;
      at = 0;
      end = ((UIntT) -1);
      return true;
    }
    
    // Construct new size and start update signal
    AttributeCtrlC attrCtrl(static_cast<DPEntityC &>(ctrl));
    triggerSizeUpdate = attrCtrl.RegisterChangedSignal("size",TriggerR(*this,&DPPlayControlBodyC::CBSequenceSizeChanged));
    if(triggerSizeUpdate < 0)
      cerr << "DPPlayControlBodyC::Open, Internal Error: failed to setup size changed signal. ";
    triggerStartUpdate = attrCtrl.RegisterChangedSignal("start",TriggerR(*this,&DPPlayControlBodyC::CBSequenceStartChanged));
    if(triggerStartUpdate < 0)
      cerr << "DPPlayControlBodyC::Open, Internal Error: failed to setup start changed signal. ";
    
    // Sort out position of start of sequence.
    start = ctrl.Start();
    if(start == ((UIntT) -1))
      start = 0;
    at = start;
    ctrl.Seek64(start);
    
    
    // Sort out end of sequence.
    UIntT sSize = ctrl.Size();
    if(sSize != ((UIntT) -1))
      end = sSize -1;
    else
      end = sSize;
    return true;
  }
  
  //: Destructor.
  
  DPPlayControlBodyC::~DPPlayControlBodyC() {
    // Free up any existing size change triggers.
    if(ctrl.IsValid()) {
      AttributeCtrlC attrCtrl(static_cast<DPEntityC &>(ctrl));
      if(triggerSizeUpdate >= 0 && !attrCtrl.RemoveChangedSignal(triggerSizeUpdate))
	cerr << "DPPlayControlBodyC::~DPPlayControlBodyC, Internal Error: failed to remove size changed signal. ";
      if(triggerStartUpdate >= 0 && !attrCtrl.RemoveChangedSignal(triggerStartUpdate))
	cerr << "DPPlayControlBodyC::~DPPlayControlBodyC, Internal Error: failed to remove start changed signal. ";
      triggerStartUpdate = -1;
      triggerSizeUpdate = -1;
    }
    
    // Try and make sure anything wait on us is free to continue.
    passEOS = true;
    pause = false;
    while(!sema.TryWait())
      sema.Post();
    sema.Post(); // Then post one more.
  }
  
  
  //: Set increments.
  // 1 == Normal play 
  // -1=Reverse play 
  // 2=Double speed forward etc..
  
  IntT DPPlayControlBodyC::Speed(IntT ninc) 
  {
    inc = ninc;  
    return inc;
  }
  
  //: Where are we in the stream ?
  
  UIntT DPPlayControlBodyC::Tell() const { 
#if FORCE_AT
    if(!ctrl.IsValid())
      return 0;
    MutexLockC lock(access);
    UIntT ret =  ctrl.Tell();
    if((IntT) ret != at) {
      cerr << "WARNING: Position mismatch: At:" << at << " Actual:" << ret << "\n";
      const_cast<DPPlayControlBodyC &>(*this).at = ret;
    }
    // Unlock access here.
    return ret;
#else

    //// Lock to make sure value is correct before returning value.
    //MutexLockC lock(access);
    return at;
#endif
  }
  
  //: How long is the stream ?
  
  UIntT DPPlayControlBodyC::Size() const { 
    if(!ctrl.IsValid())
      return 0;
    MutexLockC lock(access);
    return ctrl.Size(); 
    // Unlock access here.
  }
  
  //: Seek to an absolute position in stream
  
  bool DPPlayControlBodyC::Seek(UIntT pos) {
    ONDEBUG(cerr << "bool DPPlayControlBodyC::Seek(UIntT pos)"); 
    if(!ctrl.IsValid())
      return false;
    MutexLockC lock(access);
    IntT seekto = pos;
    
    // Compensate for 'inc'
    if(pos != end) {
      IntT ci = inc-1;
      if(((seekto - ci) >= (int) start) && ((seekto - ci) <= (int)end))
        seekto = pos - ci;
    } else {
      inc = 1;
    }
    
    // Do the seek.
    if(!ctrl.Seek(seekto)) {
      cerr << "WARNING: Seek to " << pos <<" (" << seekto << ") failed. \n";
      return false;
    }
    at = seekto;
    
    // To ensure the results of Tell() and LastFrame() return something consistany
    // update the lastFrame.
    lastFrame = pos;
    
    ONDEBUG(cerr << "DPPlayControlBodyC::Seek(), Seek to :" << pos << " (Comp:" << seekto << ") Tell64:" << ctrl.Tell64() << " Inc:" << inc << " End=" << end << " LastFrame=" << lastFrame << "\n");
    if(pause) { // Make sure it gets displayed if we're paused.
      lock.Unlock();// Unlock access.
      if(sema.Count() < 1)
	sema.Post(); // Display at least one frame.
    }
    return true;
  }
  
  //: Pause playing.
  
  void DPPlayControlBodyC::Pause() { 
    ONDEBUG(cerr << "DPPlayControlBodyC::Pause(), Called\n");
    while(sema.Count() > 0) {
      if(!sema.TryWait())
	break;
    }
    pause = true; 
    
    ONDEBUG(if(ctrl.IsValid()) cerr << "DPPlayControlBodyC::Pause(), Done. @" << ctrl.Tell64() << "\n");
  }
  
  //: Continue playing.
  
  void DPPlayControlBodyC::Continue() { 
    if(!pause)
      return ;
    pause = false; 
    if(sema.Count() < 1)
      sema.Post();
  }
  
  //: Move 'frames' forward or backward.
  
  void DPPlayControlBodyC::Jog(IntT frames) {
    ONDEBUG(cerr << "DPPlayControlBodyC::Jog(), " << frames << "\n");
    if(!pause)
      Pause();
    //MutexLockC lock(access);
    inc = frames;
    
    //CheckUpdate();
    //lock.Unlock(); // Unlock access before post.
    if(sema.Count() < 1)
      sema.Post(); // Display at least one frame.
  }
  
  //: Check state of stream BEFORE get.
  // This assumes the input stream is locked by the calling function.
  
  bool DPPlayControlBodyC::CheckUpdate() {
    ONDEBUG(cerr << "-------------------------------\n@ " << at << " Mode=" << playMode <<  " Inc:" << inc << " Tell64:" << ctrl.Tell64() <<" End=" << end << "\n");
    bool ret = true;
#if FORCE_AT
    if(at != ctrl.Tell64()) {
      cerr << "WARNING: Position mismatch: At:" << at << " Actual:" << ctrl.Tell64() << "\n";    
      at = ctrl.Tell64();
    }
#endif
    switch(playMode) 
      {
      case 0: break; // 0-Once through 
      case 1: // 1-Once through sub seq. 
	if(inc > 0) {
	  if(at >= subEnd) {
	    inc = 0; // Stop.
	    Pause();
	  }
	} else {
	  if(at < subStart) {
	    inc = 0; // Stop.
	    Pause();
	  }
	}
	break;
      case 2: // Loop.
	if(inc > 0) {
	  if(at > subEnd) {
	    if(!ctrl.Seek(subStart)) {
	      cerr << "WARNING: Seek to " << subStart <<" failed. \n";
	    } else
	      at = subStart;
	  }
	} else {
	  if(at < subStart) {
	    if(!ctrl.Seek(subEnd)) {
	      cerr << "WARNING: Seek to " << subEnd <<" failed. \n";
	    } else
	      at = subEnd;
	  }
	} 
	break;
      case 3: // Palindrome.
	if(inc > 0) {
	  if(at > subEnd && !doneRev) {
	    inc *= -1; // Reverse direction
	    doneRev = true;
	  } else
	    doneRev = false;
	} else {
	  if(at <= subStart && !doneRev) {
	    inc *= -1; // Reverse direction
	    doneRev = true;
	  } else
	    doneRev = false;
	}
	break;      
      }
    
    // Deal with normal backward/forward playing.
    
    UIntT oldAt = at;
    if(inc >= 1) 
    {
      if((at + inc) <= ((IntT) end)) // before end ?
      { 
	at += (inc-1);
	if(inc != 1) 
        {
	  if(!ctrl.DSeek(inc-1)) 
          {
	    cerr << "DSeek failed : " << inc -1 << "\n";
	    at = ctrl.Tell64();
	    if(at == -1)
	      at = oldAt;
	  }
	}
      } 
      else 
      { 
	ONDEBUG(cerr << "Hit end : "<< at << " Inc:" << inc <<"\n");
	// Failed, hit end.
	if(end != ((UIntT) -1) && playMode < 2) 
        {
	  if(!ctrl.Seek(end)) 
          { // Show last frame.
	    cerr << "Warning: Seek to end failed. \n";
	    at = ctrl.Tell64();
	    if(at == -1)
	      at = oldAt + 1;
	  } else
	    at = end;
	  inc = 0;
	  Pause();
	}
      }
    } 
    else 
    {
      // inc is negative or zero
      if((UIntT) lastFrame == end)
      {
	at += inc; // If we're at the end the stream won't be one beyond.
      }
      else
      {
	at += (inc-1);
      }
      if(at >= ((IntT) start)) 
      {
	if(!ctrl.DSeek(inc-1)) 
        {
	  ONDEBUG(cerr << "DSeek failed : "<< at << "  Inc:" << inc-1 <<" Start@ " << start << "\n");
	  at = ctrl.Tell64();
	  if(at == -1)
	    at = oldAt + 1;
	}
      } 
      else 
      {
	ONDEBUG(cerr << "Hit start : "<< at << "  Inc:" << inc <<" Start@ " << start << "\n");
	if(start != ((UIntT) -1) && playMode < 2) {
	  if(!ctrl.Seek(start)) {
	    cerr << "Warning: Seek to start failed. \n";
	    at = ctrl.Tell64();
	    if(at == -1)
	      at = oldAt + 1;
	  } else
	    at = start;
	  inc = 0;
	  Pause();
	}
      }
    }
    return ret;
  }


  //: Update lastFrame and at. after read.
  void DPPlayControlBodyC::UpdateAt(void)
  {
    // Place will be incremented by next read.
    lastFrame = at;
    at++; // Allow for frame read.

    // Check for last frame in sequence.
    if(lastFrame == end)
      at = end;
    ONDEBUG(cerr << " Last frame :" << lastFrame << " Tell64:" << ctrl.Tell64() << " At:" << at << "\n");
  }



  //: Callback on sequence size changing.
  
  bool DPPlayControlBodyC::CBSequenceSizeChanged() {
    ONDEBUG(cerr << "DPPlayControlBodyC::CBSequenceSizeChanged, Called. \n");
    RavlAssert(ctrl.IsValid());
    MutexLockC lock(access);
    // Sort out end of sequence.
    UIntT sSize = ctrl.Size();
    UIntT oldEnd = end;
    ONDEBUG(cerr << "DPPlayControlBodyC::CBSequenceSizeChanged, Called. Size=" << sSize << "\n");
    if(sSize != ((UIntT) -1))
      end = sSize -1;
    else
      end = sSize;
    if(lastFrame == oldEnd && end > oldEnd)
      at++;
    if(at >= (IntT) end)
      at = end;
    return true;
  }
  
  //: Callback on sequence start changing.
  
  bool DPPlayControlBodyC::CBSequenceStartChanged() {
    ONDEBUG(cerr << "bool DPPlayControlBodyC::CBSequenceStartChanged()"); 
    RavlAssert(ctrl.IsValid());
    
    MutexLockC lock(access);
    // Sort out position of start of sequence.
    start = ctrl.Start();
    ONDEBUG(cerr << "DPPlayControlBodyC::CBSequenceStartChanged, Called. Start=" << start << " \n");
    if(start == ((UIntT) -1))
      start = 0;
    return true;
  }

  
  //: Goto beginning of sequence. (for GUI)
  
  void DPPlayControlBodyC::ToBeginning() {
    Speed(1);
    ONDEBUG(cerr << "DPPlayControlBodyC::ToBeginning(), Called. Start=" << start << " Mode=" << playMode << "\n");
    switch(playMode) {
    case 0:
      Seek(start);
      break;
    case 1: // Sub sequence.
    case 2: // Loop
    case 3: // Palindrome.
      Seek(subStart);
      break;
    default:
      Seek(start);
      break;
    }
  }
  
  //: Fast forward. (for GUI)
  
  void DPPlayControlBodyC::FastFwd() {
    Speed(4); 
    Continue();
  }
  
  //: Play forward. (for GUI)
  void DPPlayControlBodyC::PlayFwd() {
    Speed(1); 
    Continue();
  }
  
  //: Go forward 1 frame. (for GUI)
  
  void DPPlayControlBodyC::JogFwd()
  { Jog(1); }
  
  //: Stop (for GUI)
  
  void DPPlayControlBodyC::Stop() {
    Speed(0);
    Pause();
  }
  
  //: Go backward 1 frame. (for GUI)
  
  void DPPlayControlBodyC::JogBkw()
  { Jog(-1); }
  
  //: Play backward. (for GUI)
  
  void DPPlayControlBodyC::PlayBkw() {
    Speed(-1); 
    Continue();
  }
  
  //: Play backward. (for GUI)
  
  void DPPlayControlBodyC::FastBkw() {
    Speed(-4); 
    Continue();
  }
  
  //: Goto end of sequence. (for GUI)
  
  void DPPlayControlBodyC::ToEnd() {
    ONDEBUG(cerr << "DPPlayControlBodyC::ToEnd(). Called. End=" << end << "\n");
    if(FixedEnd() == ((UIntT) -1)) 
      return ;
    Pause();  
    
    switch(playMode) {
    case 0:
      Seek(end);
      break;
    case 1: // Sub sequence.
    case 2: // Loop
    case 3: // Palindrome.
      Seek(subEnd);
      break;
    default:
      Seek(end);
      break;
    }
  }
  
  //: Goto position in sequence. (for GUI)
  void DPPlayControlBodyC::SeekTo(UIntT loc) {
    Seek(loc);
  }
  
  
  /////////////////////////////////////////////////
  //: Body constructor.
  
  DPPlayControlC::DPPlayControlC(const DPPlayControlBodyC &bod)
    : DPEntityC((DPPortBodyC &) bod)
  {}
  
}

void HelpDebugger() {
  return ;
}
