// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DPMULTIPLEX_HEADER
#define RAVL_DPMULTIPLEX_HEADER 1
////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! example=exDPMultiplex.cc
//! docentry="Ravl.API.Core.Data Processing.Multiplexer" 
//! file="Ravl/Core/IO/Multiplex.hh"
//! lib=RavlIO
//! author="Charles Galambos"
//! date="04/07/1998"

#include "Ravl/DP/Port.hh"
#include "Ravl/DP/IOPort.hh"
#include "Ravl/SArray1d.hh"

#include "Ravl/Stream.hh"
#include "Ravl/BinStream.hh"

#define DPMDEBUG 0
#if DPMDEBUG 
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  ////////////////////////////////
  //! userlevel=Develop
  //: Multiplex an operation body.
  // This is indended to be used with DPThreadC to
  // do multiprocessing. <p>
  // This will preserve the order of the data in the 
  // stream.
  
  template<class InT,class OutT>
  class DPMultiplexBodyC 
    : public DPIOPortBodyC<InT,OutT>
  {
  public:
    DPMultiplexBodyC(IntT num,const DPIOPortC<InT,OutT> & nproc);
    //: Constructor.
    // NB. This only uses nproc as a template, it is not actually
    // used for processing. This avoids possible problems of copying
    // data out of a running thread. <p>
    // ** nproc MUST not have a running thread **.
    
    DPMultiplexBodyC(const SArray1dC<DPIOPortC<InT,OutT> > &nprocs)
      : in(0),
	out(0),
	procs(nprocs)
    {}
    //: Constructor.
    
    DPMultiplexBodyC(std::istream &ins)
      : DPIOPortBodyC<InT,OutT>(ins)
    { ins >> in >> out >> procs; }
    //: Stream constructor.
    
    DPMultiplexBodyC(const DPMultiplexBodyC &oth);
    //: Copy constructor.
    // Makes a deep copy of procs array.
    
    virtual bool IsPutReady() const
    { return procs[in].IsPutReady(); }
    //: Is some data ready ?
    // true = yes.
    
    virtual bool IsGetReady() const
    { return procs[out].IsGetReady(); }
    //: Is some data ready ?
    // true = yes.
    
    virtual void PutEOS();
    //: Put End Of Stream marker.
    
    virtual bool Put(const OutT &dat);
    //: Put data.
    
    virtual IntT PutArray(const SArray1dC<OutT> &data);
    //: Put an array of data to stream.
    // returns the number of elements processed.
    
    virtual InT Get();
    //: Get next piece of data.
    // May block if not ready, or it will return a constructed
    // with the default constructor.
    
    inline IntT GetArray(SArray1dC<InT> &data);
    //: Get an array of data from stream.
    // returns the number of elements processed.
    
    virtual bool Get(InT &buff);
    //: Get next piece of data.
    // May block if not ready, or it will return a constructed
    // with the default constructor.
    
    virtual bool IsGetEOS() const { 
      // No point in advancing the stream ptr.
      return procs[out].IsGetEOS(); 
    }
    //: Is get EOS ?
    
    virtual RCBodyVC &Copy() const
    { return *new DPMultiplexBodyC<InT,OutT>(*this); }
    //: Make a deep copy of object.
    
    virtual bool Save(std::ostream &out) const;
    //: Save to std::ostream.
    
  private:
    IntT in,out;
    SArray1dC<DPIOPortC<InT,OutT> > procs;
  };
  
  //////////////////////////////////
  //! userlevel=Normal
  //: Multiplex an operation.
  // This is indended to be used with DPThreadC to
  // do multiprocessing.
  
  template<class InT,class OutT>
  class DPMultiplexC : public DPIOPortC<InT,OutT> 
  {
  public:
    DPMultiplexC()
    {}
    //: Default constructor.
    
    DPMultiplexC(IntT num,const DPIOPortC<InT,OutT> & nproc)
      : DPEntityC(*new DPMultiplexBodyC<InT,OutT>(num,nproc))
    {}
    //: Constructor.
    
    DPMultiplexC(const SArray1dC<DPIOPortC<InT,OutT> > &procs)
      : DPEntityC(*new DPMultiplexBodyC<InT,OutT>(procs))
    {}
    //: Constructor.
    
  };
  
  ///////////////////////////////
  
  template<class InT,class OutT>
  DPMultiplexC<InT,OutT> DPMultiplex(IntT num,const DPIOPortC<InT,OutT> & nproc)
  { return DPMultiplexC<InT,OutT>(num,nproc); }
  //: Create a multiplexer.
  // Helper function.
  
  template<class InT,class OutT>
  DPMultiplexC<InT,OutT> DPMultiplex(const SArray1dC<DPIOPortC<InT,OutT> > &procs)
  { return DPMultiplexC<InT,OutT>(procs); }
  //: Create a multiplexer.
  // Helper function.
  
  ////////////////////////////////////
  
  template<class InT,class OutT>
  DPMultiplexBodyC<InT,OutT>::DPMultiplexBodyC(IntT num,const DPIOPortC<InT,OutT> & nproc)
    : in(0),
      out(0),
      procs(num)
  {
    RavlAssert(nproc.IsValid());
    for(IntT i = 0;i < num;i++)
      procs[i] = nproc.Copy();
  }
  
  template<class InT,class OutT>
  DPMultiplexBodyC<InT,OutT>::DPMultiplexBodyC(const DPMultiplexBodyC &oth) 
    : in(oth.in),
      out(oth.out),
      procs(oth.procs.Size())
  {
    for(IntT i = 0;i < (IntT) oth.procs.Size();i++)
      procs[i] = oth.procs[i].Copy();
  }
  
  template<class InT,class OutT>
  void DPMultiplexBodyC<InT,OutT>::PutEOS() { 
    IntT i;
    // Put EOS's in proper sequence. 
    for(i = in;i < (IntT) procs.Size();i++)
      procs[i].PutEOS();
    for(i = 0;i < in;i++)
      procs[i].PutEOS();
  }
  
  template<class InT,class OutT>
  bool DPMultiplexBodyC<InT,OutT>::Put(const OutT &dat) { 
    ONDEBUG(cerr << "DPMultiplexBodyC<InT,OutT>::Put(),  in:" << in << " Started\n");
    if(!procs[in].Put(dat)) {
      cerr << "WARNING: DPMultiplexBodyC<InT,OutT>::Put(), Failed. \n";
      return false;
    }
    ONDEBUG(cerr << "DPMultiplexBodyC<InT,OutT>::Put(),  in:" << in << " Done\n");
    in++;
    if(in >= (IntT) procs.Size())
      in = 0;
    return true; 
  }

  template<class InT,class OutT>
  IntT DPMultiplexBodyC<InT,OutT>::PutArray(const SArray1dC<OutT> &data) {
    for(SArray1dIterC<OutT> it(data);it;it++) {
      if(!procs[in].Put(*it)) {
	cerr << "WARNING: DPMultiplexBodyC<InT,OutT>::PutArray(), Failed. \n";
	return it.Index().V();
      }
      in++;
      if(in >= (IntT) procs.Size())
	in = 0;
    }
    return data.Size();
  }
  
  template<class InT,class OutT>
  InT DPMultiplexBodyC<InT,OutT>::Get() { 
    ONDEBUG(cerr << "DPMultiplexBodyC<InT,OutT>::Get(),  out:" << out << " Started\n");
    InT ret = procs[out].Get(); 
    ONDEBUG(cerr << "DPMultiplexBodyC<InT,OutT>::Get(),  out:" << out << " Done.\n");
    out++;
    if(out >= (IntT) procs.Size())
      out = 0;
    return ret;
  }
  
  template<class InT,class OutT>
  bool DPMultiplexBodyC<InT,OutT>::Get(InT &buff) {
    ONDEBUG(cerr << "DPMultiplexBodyC<InT,OutT>::Get(),  out:" << out << " Started\n");
    if(!procs[out].Get(buff)) {
      cerr << "WARNING: DPMultiplexBodyC<InT,OutT>::Get(), Failed. \n";
      return false;
    }
    ONDEBUG(cerr << "DPMultiplexBodyC<InT,OutT>::Get(),  out:" << out << " Done.\n");
    out++;
    if(out >= (IntT) procs.Size())
      out = 0;
    return true;
  }
  
  template<class InT,class OutT>
  IntT DPMultiplexBodyC<InT,OutT>::GetArray(SArray1dC<InT> &data) { 
    for(SArray1dIterC<InT> it(data);it;it++) {
      if(!procs[out].Get(*it)) {
	cerr << "WARNING: DPMultiplexBodyC<InT,OutT>::PutArray(), Failed. \n";
	return it.Index().V();
      }
      out++;
      if(out >= (IntT) procs.Size())
	out = 0;
    }
    return data.Size();
  }
  
  template<class InT,class OutT>
  bool DPMultiplexBodyC<InT,OutT>::Save(std::ostream &sout) const {
    DPIOPortBodyC<InT,OutT>::Save(sout);
    sout << in << " " << out << " " << procs;
    return true;
  }
  
}
#ifdef ONDEBUG
#undef ONDEBUG
#endif

#endif
