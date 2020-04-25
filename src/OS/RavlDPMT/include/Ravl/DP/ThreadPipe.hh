// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DPTHREAD_HEADER
#define RAVL_DPTHREAD_HEADER 1
////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlDPMT
//! docentry="Ravl.API.Core.Data Processing.Threads"
//! file="Ravl/OS/DataProc/ThreadPipe.hh"
//! author="Charles Galambos"
//! date="02/10/1998"

#include "Ravl/DP/IOPort.hh"
#include "Ravl/DP/Process.hh"
#include "Ravl/DP/StreamOp.hh"
#include "Ravl/Threads/MessageQueue.hh"
#include "Ravl/Threads/LaunchThread.hh"

#define DPTPDEBUG 0
#if DPTPDEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {

  template<class InT,class OutT> class DPThreadPipeC;
  
  ///////////////////////////////
  //! userlevel=Develop
  //: MT safe queue body
  
  template<class InT,class OutT>
  class DPThreadPipeBodyC 
    : public DPIOPortBodyC<InT,OutT>
  {
  public:
    DPThreadPipeBodyC(const DPProcessC<OutT,InT> & nproc,IntT qsize = 5,bool makeLive = false);
    //: Default constructor.
    
    virtual StringC OpName() const
    { return StringC("threadpipe"); }
    //: Op type name.
    
    virtual bool IsPutReady() const
    { return !gotEOS; }
    //: Ready to accept data ?
    // true = yes.
    
    virtual bool Put(const OutT &dat);
    //: Put data.
    
    virtual IntT PutArray(const SArray1dC<OutT> &data);
    //: Put an array of data to stream.
    
    virtual void PutEOS()
    { gotEOS = true; }
    //: Put End Of Stream marker.
    
    virtual bool IsGetReady() const;
    //: Is some data ready ?
    // true = yes.
    
    virtual bool IsGetEOS() const;
    //: Has the End Of Stream been reached ?
    // true = yes.
    
    virtual InT Get() { 
      ONDEBUG(cerr << "DPThreadPipeBodyC<InT,OutT>::Get(), From ThreadPipe:" << ((void *) this) << "\n");
      return outq.Get(); 
    }
    //: Get next piece of data.
    // May block if not ready, or it will return a constructed
    // with the default constructor.
    
    virtual bool Get(InT &buf);
    //: Get next piece of data.
    // May block if not ready, or it will return a constructed
    // with the default constructor.
    
    virtual IntT GetArray(SArray1dC<InT> &data);
    //: Read results into an array.
    // returns the number of elements processed.
    
    bool Start();
    //: Run pipe process.
    
    virtual RCBodyVC &Copy() const; 
    //: Make a deep copy.
    // NB. All copies are made live automaticly .
    
    void ThreadLaunch();
    //: Make pipe live.
    // This may only be called ONCE per ThreadPipe<>
    
  private:
    bool gotEOS;
    DPProcessC<OutT,InT> proc;
    MessageQueueC<InT> outq;
    MessageQueueC<OutT> inq;
    IntT qsize;
    bool running;
    friend class DPThreadPipeC<InT,OutT>;
  };
  
  //////////////////////////////////
  //! userlevel=Normal
  //: MT safe queue handle.
  
  template<class InT,class OutT>
  class DPThreadPipeC 
    : public DPIOPortC<InT,OutT>
  {
  public:
    DPThreadPipeC(const DPProcessC<OutT,InT> & nproc,IntT qsize = 5)
      : DPEntityC(*new DPThreadPipeBodyC<InT,OutT>(nproc,qsize))
    {}
    //: Constructor.
    
    DPThreadPipeC()
      : DPEntityC(true)
    {}
    //: Default constructor.
    
  protected:
    DPThreadPipeC(DPThreadPipeBodyC<InT,OutT> &oth)
      : DPEntityC(oth)
    {}
    //: Body constructor.
    
    inline DPThreadPipeBodyC<InT,OutT> &Body() 
    { return dynamic_cast<DPThreadPipeBodyC<InT,OutT> &>(DPEntityC::Body()); }
    //: Access body.
    
    inline const DPThreadPipeBodyC<InT,OutT> &Body() const
    { return dynamic_cast<const DPThreadPipeBodyC<InT,OutT> &>(DPEntityC::Body()); }
    //: Const access body.
    
    bool Start()
    { return Body().Start(); }
    //: Start processing.
    
  public:
    void ThreadLaunch()
    { Body().ThreadLaunch(); }
    //: Make pipe live.
    
    friend class DPThreadPipeBodyC<InT,OutT>;
  };
  
  //////////////////////////////
  
  //: Shortcut for creating threads.
  
  template<class InT,class OutT>
  DPThreadPipeC<InT,OutT> DPThread(const DPProcessC<OutT,InT> & nproc,IntT qsize = 5)
  { return DPThreadPipeC<InT,OutT>(nproc,qsize); }
  
  ///////////////////////////////////////////
  
  template<class InT,class OutT>
  DPThreadPipeBodyC<InT,OutT>::DPThreadPipeBodyC(const DPProcessC<OutT,InT> & nproc,IntT nqsize,bool makeLive)
    : gotEOS(false),
      proc(nproc),
      outq(nqsize),
      inq(nqsize),
      qsize(nqsize),
      running(false)
  {
    if(makeLive)
      ThreadLaunch();
  }
  
  template<class InT,class OutT>
  bool DPThreadPipeBodyC<InT,OutT>::Put(const OutT &dat) { 
    if(gotEOS) {
#if RAVL_CHECK
      cerr << "DPThreadPipeBodyC<InT,OutT>::Put(), ERROR: Called after EOS ! \n";
#endif
      return false; 
    }
    inq.Put(dat); 
    return true; 
  }
  
  template<class InT,class OutT>
  IntT DPThreadPipeBodyC<InT,OutT>::PutArray(const SArray1dC<OutT> &data) {
    if(gotEOS) {
#if RAVL_CHECK
      cerr << "DPThreadPipeBodyC<InT,OutT>::PutArray(), ERROR: Called after EOS ! \n";
#endif
      return 0;
    }
    for(SArray1dIterC<OutT> it(data);it;it++)
      inq.Put(*it); 
    return data.Size();
  }
  
  template<class InT,class OutT>
  bool DPThreadPipeBodyC<InT,OutT>::IsGetReady() const {
    if(!gotEOS)
      return true;
    if((const_cast<MessageQueueC<InT> &>(outq)).Size() != 0)
      return true;
    return const_cast<MessageQueueC<OutT> &>(inq).Size() != 0;
  }
  
  template<class InT,class OutT>
  bool DPThreadPipeBodyC<InT,OutT>::IsGetEOS() const {
    if(!gotEOS)
      return false;
    if((const_cast<MessageQueueC<InT> &>(outq)).Size() != 0)
      return false;
    return const_cast<MessageQueueC<OutT> &>(inq).Size() == 0;  
  }
  
  template<class InT,class OutT>
  bool DPThreadPipeBodyC<InT,OutT>::Get(InT &buf) {
    ONDEBUG(cerr << "DPThreadPipeBodyC<InT,OutT>::Get(InT&), From ThreadPipe:" << ((void *) this) << "\n");
    if(gotEOS) {
      if((const_cast<MessageQueueC<InT> &>(outq)).Size() == 0 && 
	 const_cast<MessageQueueC<OutT> &>(inq).Size() == 0)
	return false;
    }
    buf = outq.Get();
    return true;
  }
  
  //: Read results into an array.
  // returns the number of elements processed.
  
  template<class InT,class OutT>
  IntT DPThreadPipeBodyC<InT,OutT>::GetArray(SArray1dC<InT> &data) {
    if(gotEOS) {
      if((const_cast<MessageQueueC<InT> &>(outq)).Size() == 0 && 
	 const_cast<MessageQueueC<OutT> &>(inq).Size() == 0)
	return 0;
    }
    for(SArray1dIterC<InT> it(data);it;it++) {
      *it = outq.Get();
      if(gotEOS) {
	if((const_cast<MessageQueueC<InT> &>(outq)).Size() == 0 && 
	   const_cast<MessageQueueC<OutT> &>(inq).Size() == 0)
	  return it.Index().V();
      }
    }
    return data.Size();
  }
  
  
  template<class InT,class OutT>
  bool DPThreadPipeBodyC<InT,OutT>::Start() {
    try {
      while(1) { 
	if(gotEOS) {
	  if(inq.Size() == 0)
	    break;
	}
	ONDEBUG(cerr << "ThreadPipe " << ((void *) this) << "  Started processing.\n");
	outq.Put(proc.Apply(inq.Get()));
	ONDEBUG(cerr << "ThreadPipe " << ((void *) this) << "  Queued output.\n");
      }
    } catch(...) {
      cerr << "An exception occured in: " << typeid(*this).name() << endl;
      //cerr << " Process:" << typeid(proc.Body()).name()  << endl;
      cerr << "Halting thread. \n" << flush;
    }
    return true;
  }
  
  template<class InT,class OutT>
  RCBodyVC &DPThreadPipeBodyC<InT,OutT>::Copy() const {
    DPThreadPipeBodyC<InT,OutT> *x;
    if(proc.IsStateless())
      x = new DPThreadPipeBodyC<InT,OutT>(proc,qsize); // No state use the same process.
    else
      x = new DPThreadPipeBodyC<InT,OutT>(proc.Copy(),qsize); // There's state so make a full copy.
    x->ThreadLaunch(); // Always make live copies.
    return *x;
  }
  
  template<class InT,class OutT>
  void DPThreadPipeBodyC<InT,OutT>::ThreadLaunch() {
    if(running) {
      cerr << "DPThreadPipeBodyC<InT,OutT>::ThreadThreadThreadThreadLaunch(), ERROR:Pipe already live. \n";
      return ;
    }
    running = true;
    ONDEBUG(cerr << "DPThreadPipeBodyC<InT,OutT>::ThreadThreadThreadThreadLaunch(), Starting ThreadPipe:" << ((void *) this) << "\n");
    LaunchThread(DPThreadPipeC<InT,OutT>(*this),&DPThreadPipeC<InT,OutT>::Start);
  }  
  
}

#undef ONDEBUG
 
#endif
