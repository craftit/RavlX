// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DPPLAYCONTROL_HEADER
#define RAVL_DPPLAYCONTROL_HEADER 1
////////////////////////////////////////////////
//! file="Ravl/OS/DataProc/PlayControl.hh"
//! lib=RavlDPMT
//! author="Charles Galambos"
//! date="16/03/1999"
//! docentry="Ravl.API.Core.Data Processing.Seekable Ports"
//! rcsid="$Id$"

#include "Ravl/Threads/Mutex.hh"
#include "Ravl/Threads/Semaphore.hh"
#include "Ravl/DP/StreamOp.hh"
#include "Ravl/DP/SPort.hh"

#define RAVL_GCC3FIX 0

void HelpDebugger();

namespace RavlN {

  //! userlevel=Develop
  //: Base play control body class.
  
  class DPPlayControlBodyC
    : public virtual DPEntityBodyC
  {
  public:
    DPPlayControlBodyC();
    //: Default constructor.
    
    DPPlayControlBodyC(const DPSeekCtrlC &nCntrl,bool nPassEOS = true,UIntT nstart = ((UIntT) -1),UIntT nend = ((UIntT) -1));
    //: Constructor.
    
    ~DPPlayControlBodyC();
    //: Destructor.
    
    IntT Speed(IntT ninc);
    //: Set increments.
    // 1 == Normal play 
    // -1=Reverse play 
    // 2=Double speed forward etc..
    
    UIntT Tell() const;
    //: Where are we in the stream ?
    // This gives the number of the NEXT frame 
    // that will be processed.
    
    UIntT LastFrame() const 
    { return lastFrame; }
    //: Get number of last frame read.
    
    UIntT Size() const;
    //: How long is the stream ?
    // This is the number of frames in the sequence.
    
    bool Seek(UIntT pos);
    //: Seek to an absolute position in stream
    
    void Pause();
    //: Pause playing.
    
    void Continue();
    //: Continue playing.
    
    void Jog(IntT frames = 1);
    //: Move 'frames' forward or backward.
    
    UIntT FixedStart() const { return start; }
    //: Get start of sub sequence.
    
    UIntT FixedEnd() const { return end; }
    //: Get start of sub sequence.
    
    void ToBeginning();
    //: Goto beginning of sequence. (for GUI)
    
    void FastFwd();
    //: Fast forward. (for GUI)
    
    void PlayFwd();
    //: Play forward. (for GUI)
    
    void JogFwd();
    //: Go forward 1 frame. (for GUI)
    
    void Stop();
    //: Stop (for GUI)
    
    void JogBkw();
    //: Go backward 1 frame. (for GUI)
    
    void PlayBkw();
    //: Play backward. (for GUI)
    
    void FastBkw();
    //: Play backward. (for GUI)
    
    void ToEnd();
    //: Goto end of sequence. (for GUI)
    
    void SeekTo(UIntT loc);
    //: Goto position in sequence. (for GUI)
    
    void PlaySubSequence(int mode,UIntT startfrm,UIntT endfrm) { 
      playMode = mode; subStart = startfrm; subEnd = endfrm; 
      RavlAssert(subStart < subEnd);
    }
    //: Play a sub-sequence.
    // 0-Once through whole sequence.
    // 1-Once through sub seq. 
    // 2-Repeat 
    // 3-back and forth.
    
    IntT &SubSeqStart() 
    { return subStart; }
    //: Set sub sequence start.
    
    IntT &SubSeqEnd() 
    { return subEnd; }
    //: Set sub sequence start.
    
    IntT &SubSeqMode() 
    { return playMode; }
    //: Set sub sequence start.
    
    bool Paused() const
    { return pause; }
    //: Access pause flag
    // true - Play control paused.
    
    IntT Speed() const
    { return inc; }
    //: Current speed setting in frames a cycle.
    // 0 - Effectively paused, -1=backard 1=forward.

  protected:  
    bool Open(const DPSeekCtrlC &nCntrl, bool bPlayMode);
    //: Open new video stream.
    // This assumes the input stream is locked by the calling function.
    
    bool CheckUpdate();
    //: Check state of stream after get.
    // This assumes the input stream is locked by the calling function.

    void UpdateAt();
    //: Update the current position - assumes input stream is locked by the calling function
    
    bool CBSequenceSizeChanged();
    //: Callback on sequence size changing.
    
    bool CBSequenceStartChanged();
    //: Callback on sequence start changing.
    
    MutexC access;    // Access control.
    IntT inc;         // Increments -ve and +ve  0==Stopped.  1=Normal play..
    bool ok;          // Are operations succeding?
    bool pause;       // Actively pause stream ?
    SemaphoreC sema;  // Stream pause semaphore.  
    DPSeekCtrlC ctrl; // Seek control handle.
    bool passEOS;     // Pass along End Of Stream.
    UIntT start;      // Inital frame no.  (this is a valid frame. )
    UIntT end;        // final frame no.   (this is a valid frame. )
    
    IntT playMode;    // 0-Once through 1-Once through sub seq. 2-Repeat 3-back and forth.
    IntT subStart;    // Inital frame no of sub sequence.  (this is a valid frame. )
    IntT subEnd;      // final frame no of sub sequence.   (this is a valid frame. )
    bool doneRev;     // Used in palindrome mode.
    IntT at;          // Cache of input stream position.
    UIntT lastFrame;   // Last frame displayed.
    
    IntT triggerSizeUpdate; // Id for size updated trigger.
    IntT triggerStartUpdate; // Id for start updated trigger.
  };
  
  //! userlevel=Normal
  //: Base play control handle class.
  
  class DPPlayControlC
    : public virtual DPEntityC
  {
  public:
    DPPlayControlC()
      : DPEntityC(true)
    {}
    //: Default constructor.
    
  protected:
    DPPlayControlC(const DPPlayControlBodyC &bod);
    //: Body constructor.
    
    DPPlayControlBodyC &Body() 
    { return dynamic_cast<DPPlayControlBodyC &>(DPEntityC::Body()); }
    //: Access body.
    
    const DPPlayControlBodyC &Body() const
    { return dynamic_cast<const DPPlayControlBodyC &>(DPEntityC::Body()); }
    //: Access body.
    
  public:

    UIntT LastFrame() const 
    { return Body().LastFrame(); }
    //: Get number of last frame read.
    
    inline IntT Speed(IntT ninc) 
    { return Body().Speed(ninc); }
    //: Set increments.
    // 1 == Normal play 
    // -1=Reverse play 
    // 2=Double speed forward etc..
    
    inline UIntT Tell() const 
    { return Body().Tell(); }
    //: Where are we in the stream ?
    
    inline UIntT Size() const 
    { return Body().Size(); }
    //: How long is the stream ?
    
    inline void Pause()  
    { Body().Pause(); }
    //: Pause playing.
    
    inline void Continue()  
    { Body().Continue(); }
    //: Continue playing.
    
    inline void Jog(IntT frames = 1)  
    { Body().Jog(frames); }
    //: Move 'frames' forward or backward.
    
    inline bool Seek(UIntT pos) 
    { return Body().Seek(pos); }
    //: Seek to an absolute position in stream
    
    UIntT FixedStart() const 
    { return Body().FixedStart(); }
    //: Get start of sub sequence.
    
    UIntT FixedEnd() const
    { return Body().FixedEnd(); }
    //: Get start of sub sequence.
    
    void ToBeginning()
    { Body().ToBeginning(); }
    
    //: Goto beginning of sequence. (for GUI)
    
    void FastFwd()
    { Body().FastFwd(); }
    //: Fast forward. (for GUI)
    
    void PlayFwd()
    { Body().PlayFwd(); }
    //: Play forward. (for GUI)
    
    void Stop()
    { Body().Stop(); }
    //: Stop (for GUI)
    
    void PlayBkw()
    { Body().PlayBkw(); }
    //: Play backward. (for GUI)
    
    void FastBkw()
    { Body().FastBkw(); }
    //: Play backward. (for GUI)
    
    void ToEnd()
    { Body().ToEnd(); }
    //: Goto end of sequence. (for GUI)
    
    void SeekTo(UIntT loc)
    { Body().SeekTo(loc); }
    //: Goto position in sequence. (for GUI)
    
    void PlaySubSequence(int mode,UIntT startfrm,UIntT endfrm)
    { Body().PlaySubSequence(mode,startfrm,endfrm); }
    //: Play a sub-sequence.
    // 0-Once through whole sequence.
    // 1-Once through sub seq. 
    // 2-Repeat 
    // 3-back and forth.
    
    IntT &SubSeqStart() 
    { return Body().SubSeqStart(); }
    //: Set sub sequence start.
    
    IntT &SubSeqEnd() 
    { return Body().SubSeqEnd(); }
    //: Set sub sequence start.
    
    IntT &SubSeqMode() 
    { return Body().SubSeqMode(); }
    //: Set sub sequence start.
    
    bool Paused() const
    { return Body().Paused(); }
    //: Access pause flag
    // true - Play control paused.
    
    IntT Speed() const
    { return Body().Speed(); }
    //: Current speed setting in frames a cycle.
    // 0 - Effectively paused, -1=backard 1=forward.
    
  };
  
  ////////////////////////////////////////
  //! userlevel=Develop
  //: Stream operation base class.
  
  template<class DataT>
  class DPIPlayControlBodyC 
    : public DPIPortBodyC<DataT>,
      public DPPlayControlBodyC,
      public DPStreamOpBodyC
  {
  public:
    DPIPlayControlBodyC() {
      this->MapBackChangedSignal("start");
      this->MapBackChangedSignal("size");
    }
    //: Default constructor.
    
    DPIPlayControlBodyC(const DPISPortC<DataT> &nin,bool nPassEOS = true,UIntT nstart = ((UIntT) -1),UIntT nend = ((UIntT) -1))
      : DPPlayControlBodyC(nin,nPassEOS,nstart,nend),
	input(nin)
    {
      this->MapBackChangedSignal("start");
      this->MapBackChangedSignal("size");
    }
    //: Constructor.

    virtual bool Save(ostream &out) const 
    { return DPPlayControlBodyC::Save(out); }
    //: Save to ostream.

    virtual StringC OpName() const
    { return StringC("playcontrol"); }
    //: Op type name.
    
    virtual DataT Get() { 
      if(pause)
	sema.Wait();
      MutexLockC lock(access);
      CheckUpdate();
      RavlAssert(input.IsValid());
      if(!input.IsValid())
	throw DataNotReadyC("No input. ");
      DataT dat = input.Get();  // Possible exception.
      UpdateAt();
      // Place will be incremented by next read.
      return dat;
    }
    // Get next piece of data.
    
    virtual bool Get(DataT &buff) { 
      if(pause)
	sema.Wait();
      // Something is corrupting the stack in gcc-3.2.x, so protect the mutex
      // with some space allocated in n[] and n1[]
#if RAVL_GCC32FIX  
      int n1[20]; // Hacky bug fix.
#endif
      MutexLockC lock(access);
#if RAVL_GCC32FIX  
      int n[20];
#endif
      if(!input.IsValid())
	return false;
      //cerr << "Access a=" << ((void *) & access) << "\n";
      RavlAssert(&(lock.Mutex()) == &access);
      CheckUpdate();
      RavlAssert(&(lock.Mutex()) == &access);
      HelpDebugger();
      if(!input.Get(buff)) {
	cerr << "DPIPlayControlBodyC::Get() ERROR: Failed, attempting to fudge stream position... \n";
	at--;
	return false;
      }
      UpdateAt();
      RavlAssert(&(lock.Mutex()) == &access);
      //cerr << "Access b=" << ((void *) & access) << " " << << "\n";
      // Use n1 and n to avoid warnings.
#if RAVL_GCC32FIX  
      n1[0] =0;
      n[0] = n1[0];
#endif
      lock.Unlock();
      return true;
    }
    //: Try and get next piece of data.
    // If none, return false.
    // else put data into buff.  
    
    virtual bool IsGetReady() const {
      // This really should be false,
      // but this causes older code to exit in confusion.
      if(pause) 
	return true; 
      MutexLockC lock(access);
      RavlAssert(input.IsValid());
      return input.IsGetReady(); 
      // Unlock here.
    }
    //: Is some data ready ?
    // true = yes.
    
    virtual bool IsGetEOS() const {
      if(!passEOS)
	return false;
      MutexLockC lock(access);
      RavlAssert(input.IsValid());
      return input.IsGetEOS(); 
      // Unlock here.
    }
    //: Has the End Of Stream been reached ?
    // true = yes.
    
    bool Open(const DPISPortC<DataT> &nPort, bool bPlayMode) 
    {
      MutexLockC lock(access);
      if(!DPPlayControlBodyC::Open(nPort, bPlayMode))
	return false;
      input = nPort;
      lock.Unlock();
      this->ReparentAttributeCtrl(input);
      if(pause)
	sema.Post(); // Display at least first frame.
      return true;
    }
    //: Open new video stream.
    
    virtual DListC<DPIPortBaseC> IPorts() const;
    //: Input ports.
    
    virtual DListC<DPIPlugBaseC> IPlugs() const;
    //: Input plugs.
    
    virtual AttributeCtrlC ParentCtrl() const { 
      MutexLockC lock(access);
      return AttributeCtrlC(input); 
    }
    //: Get Parent attribute control.
    
  protected:
    DPISPortC<DataT> input; // Where to get data from.
    
  public:
    inline const DPISPortC<DataT> &Input() 
    { return input; }
    // Access input port.

  private:
    DPIPlayControlBodyC(const DPIPlayControlBodyC &oth)
    { RavlAssert(0); }
    //: Disable copy constructor.
  }; 
  
  ///////////////////////////////////
  //! userlevel=Normal
  //: Stream operation handle class.
  
  template<class DataT>
  class DPIPlayControlC 
    : public DPIPortC<DataT>,
      public DPPlayControlC,
      public DPStreamOpC
  {
  public:
    DPIPlayControlC() 
      : DPEntityC(true)
    {}
    //: Default Constructor.
    
    DPIPlayControlC(const DPISPortC<DataT> &nin,bool nPassEOS = true,UIntT nstart = ((UIntT) -1),UIntT nend = ((UIntT) -1))
      : DPEntityC(*new DPIPlayControlBodyC<DataT>(nin,nPassEOS,nstart,nend))
    {}
    //: Constructor.
    
    DPIPlayControlC(const DPIPlayControlC<DataT> &oth) 
      : DPEntityC(oth)
    {}
    //: Copy Constructor.
    
    DPIPlayControlC(bool)
      : DPEntityC(*new DPIPlayControlBodyC<DataT>())
    {}
    //: Constructor.
    
  protected:
    DPIPlayControlC(const DPIPlayControlBodyC<DataT> &bod)
      : DPEntityC((DPIPortBodyC<DataT> &) bod)
    {}
    //: Body constructor.
    
    DPIPlayControlBodyC<DataT> &Body() 
    { return dynamic_cast<DPIPlayControlBodyC<DataT> &>(DPEntityC::Body()); }
    //: Access body.
    
    const DPIPlayControlBodyC<DataT> &Body() const
    { return dynamic_cast<const DPIPlayControlBodyC<DataT> &>(DPEntityC::Body()); }
    //: Access body.
    
  public:  
    DPIPlayControlC<DataT> &operator=(const DPIPlayControlC<DataT> &other) {
      // Simplify the assigment for the compiler.
      DPEntityC::operator=((DPEntityC &) other); 
      return *this;
    }
    //: Assignment 

    bool Save(ostream &out) const 
    { return DPPlayControlC::Save(out); }
    //: Save to ostream.

    bool Save(BinOStreamC &out) const 
    { return DPPlayControlC::Save(out); }
    //: Save to ostream.

    inline const DPISPortC<DataT> &Input() 
    { return Body().Input(); }
    //: Access input port.

    inline bool Open(const DPISPortC<DataT> &nPort, bool bPlayMode = true)
    { return Body().Open(nPort, bPlayMode); }
    //: Open a new input.
    
    friend class DPIPlayControlBodyC<DataT>;
  };

  template<class DataT>
  DListC<DPIPortBaseC> DPIPlayControlBodyC<DataT>::IPorts() const {
    DListC<DPIPortBaseC> ret;
    ret.InsLast(DPIPlayControlC<DataT>(const_cast<DPIPlayControlBodyC<DataT> &>(*this)));
    return ret;
  }
  //: Input ports.
  
  template<class DataT>
  DListC<DPIPlugBaseC> DPIPlayControlBodyC<DataT>::IPlugs() const {
    DListC<DPIPlugBaseC> ret;
    ret.InsLast(DPIPlugC<DataT>(input,DPEntityC(const_cast<DPIPlayControlBodyC<DataT> &>(*this))));
    return ret;
  }
  //: Input plugs.
  
}

#endif
