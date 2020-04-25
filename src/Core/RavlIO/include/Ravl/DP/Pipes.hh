// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DPPIPE_HEADER
#define RAVL_DPPIPE_HEADER 1
////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Core/IO/Pipes.hh"
//! lib=RavlIO
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Data Processing.Extras" 
//! date="07/01/1999"
//! userlevel=Default

#include "Ravl/DP/Port.hh"
#include "Ravl/DP/StreamOp.hh"

namespace RavlN {
  class DPIPipeBaseC;
  class DPOPipeBaseC;
  
  //////////////////////////////////////
  //! userlevel=Develop
  //: Pipe base body.
  
  class DPIPipeBaseBodyC 
    : public virtual DPEntityBodyC
  {
  public:
    typedef bool (*FuncT)(DPIPipeBaseC &,DPEntityC &);
    //: Function ptr.
    
    DPIPipeBaseBodyC(const DPEntityC &nhold = DPEntityC(true),FuncT nAuxOp = 0)
      : forceReady(false),
	forceNotEOS(false),
	hold(nhold),
	auxOp(nAuxOp)
    {}
    //: Default constructor.
    
    virtual bool SetInput(const DPIPortBaseC &in);
    //: Setup input.
    
    void SetAuxOperation(FuncT nAuxOp) { auxOp = nAuxOp; }
    //: Set auxilary operation.
    // This function is called just before any 'Get' operation
    // is passed on.
    
    void SetStreamStatus(bool fGR,bool fNEOS)
    { forceReady = fGR; forceNotEOS = fNEOS; }
    //: Set stream status.
    
    virtual DPIPortBaseC Port();
    //: Access input port.
    
  protected:
    bool forceReady,forceNotEOS;
    DPEntityC hold; // Hold aux info.
    FuncT auxOp;    // Auxilary operation. 0 == None.
  };
  
  //////////////////////////////////////
  //! userlevel=Develop
  //: Pipe base body.
  
  class DPOPipeBaseBodyC 
    : public virtual DPEntityBodyC 
  {
  public:
    typedef bool (*FuncT)(DPOPipeBaseC &,DPEntityC &);
    //: Function ptr.
  
    DPOPipeBaseBodyC(const DPEntityC &nhold = DPEntityC(true),FuncT nAuxOp = 0)
      : forceReady(false),
        forceNotEOS(false),
        hold(nhold),
	auxOp(nAuxOp)
    {}
    //: Default constructor.
  
    virtual bool SetOutput(const DPOPortBaseC &out);
    //: Setup output.
  
    void SetAuxOperation(FuncT nAuxOp) { auxOp = nAuxOp; }
    //: Set auxilary operation.
    // This function is called just before any 'Put' operation
    // is passed on.
  
    virtual DPOPortBaseC Port();
    //: Access input port.
  
    void SetStreamStatus(bool fGR,bool fNEOS)
    { forceReady = fGR; forceNotEOS = fNEOS; }
    //: Set stream status.
  
  protected:
    bool forceReady,forceNotEOS;
    DPEntityC hold; // Hold aux info.    
    FuncT auxOp;    // Auxilary operation. 0 == None.
  };
  
  //////////////////////////////////////
  //! userlevel=Normal
  //: Pipe base.
  
  class DPIPipeBaseC 
    : public virtual DPEntityC 
  {
  public:
    DPIPipeBaseC()
      : DPEntityC(true)
    {}
    //: Default constructor.
    
    inline bool SetInput(const DPIPortBaseC &in);
    //: Setup input.
    
    DPIPipeBaseC(DPIPipeBaseBodyC &bod)
      : DPEntityC(bod)
    {}
    //: Body constructor.
  protected:
    
    DPIPipeBaseBodyC &Body() 
    { return  dynamic_cast<DPIPipeBaseBodyC &>(DPEntityC::Body()); }
    //: Access body.
    
    const DPIPipeBaseBodyC &Body() const 
    { return  dynamic_cast<const DPIPipeBaseBodyC &>(DPEntityC::Body()); }
    //: Access body.
    
  public:
    void SetAuxOperation(DPIPipeBaseBodyC::FuncT nAuxOp) 
    { Body().SetAuxOperation(nAuxOp); }
    //: Set auxilary operation.
    // This function is called just before any get operation
    // is passed on.
    
    void SetStreamStatus(bool fGR,bool fNEOS)
    { Body().SetStreamStatus(fGR,fNEOS); }
    //: Set stream status.
    
    inline DPIPortBaseC Port() 
    { return Body().Port(); }
    //: Access input port.
    
  };
  
  //////////////////////////////////////
  //! userlevel=Normal
  //: Pipe base body.
  
  class DPOPipeBaseC 
    : public virtual DPEntityC 
  {
  public:
    DPOPipeBaseC()
      : DPEntityC(true)
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    inline bool SetOutput(const DPOPortBaseC &in);
    //: Setup input.
    
    DPOPipeBaseC(DPOPipeBaseBodyC &bod)
      : DPEntityC(bod)
    {}
    //: Body constructor.
    
  protected:
    DPOPipeBaseBodyC &Body() 
    { return  dynamic_cast<DPOPipeBaseBodyC &>(DPEntityC::Body()); }
    //: Access body.
    
    const DPOPipeBaseBodyC &Body() const 
    { return  dynamic_cast<const DPOPipeBaseBodyC &>(DPEntityC::Body()); }
    //: Access body.
    
  public:
    void SetAuxOperation(DPOPipeBaseBodyC::FuncT nAuxOp) 
    { Body().SetAuxOperation(nAuxOp); }
    //: Set auxilary operation.
    // This function is called just before any get operation
    // is passed on.
    
    void SetStreamStatus(bool fGR,bool fNEOS)
    { Body().SetStreamStatus(fGR,fNEOS); }
    //: Set stream status.
    
    inline DPOPortBaseC Port() 
    { return Body().Port(); }
    //: Access ouput port.
  };
  
  //////////////////////////////////////
  //! userlevel=Develop
  //: IPipe body.
  
  template<class DataT>
  class DPIPipeBodyC : 
    public DPIPipeBaseBodyC,
    public DPIStreamOpBodyC<DataT,DataT>
  {
  public:
    DPIPipeBodyC(const DPEntityC &nhold = DPEntityC(true),FuncT nAuxOp = 0) 
      : DPIPipeBaseBodyC(nhold,nAuxOp)
    {}
    //: Default constructor.
   
    DPIPipeBodyC(const DPIPortC<DataT> &port,const DPEntityC &nhold = DPEntityC(true),FuncT nAuxOp = 0)
      : DPIPipeBaseBodyC(nhold,nAuxOp),
	DPIStreamOpBodyC<DataT,DataT>(port)
    {}
   
    //: Constructor.
   
    virtual bool SetInput(const DPIPortBaseC &in) {
      DPIPortC<DataT> inport(in);
      this->input = inport;
      return true;
    }
    //: Setup input.
   
    virtual bool IsGetReady() const  {
      if(forceReady)
	return true;
      if(!this->input.IsValid())
	return false;
      return this->input.IsGetReady(); 
    }
    //: Is some data ready ?
    // true = yes.
   
    virtual bool IsGetEOS() const {
      if(forceNotEOS)
	return false;
      if(!this->input.IsValid())
	return true;
      return this->input.IsGetEOS(); 
    }
    //: Has the End Of Stream been reached ?
    // true = yes.
   
    virtual DataT Get()  { 
      if(auxOp != 0) {
	DPIPipeBaseC IPB(*this);
	if(!auxOp(IPB,hold))
	  throw DataNotReadyC("DPIPipeBodyC<>::Get(), Auxilary function returned false. ");
      }
      if(!this->input.IsValid())
	throw DataNotReadyC("DPIPipeBodyC<>::Get(), No input source. ");
      return this->input.Get();
    }
    //: Get next piece of data.
    // May block if not ready.
   
    virtual bool Get(DataT &buff)  {
      if(auxOp != 0) {
	DPIPipeBaseC IPB(*this);
	if(!auxOp(IPB,hold))
	  return false;
      }
      if(!this->input.IsValid())
	return false;
      return this->input.Get(buff);
    }
    //: Get next piece of data.
    // May block if not ready.
   
    virtual IntT GetArray(SArray1dC<DataT> &data) {
      for(SArray1dIterC<DataT> it(data);it;it++) {
	if(auxOp != 0) {
	  DPIPipeBaseC IPB(*this);
	  if(!auxOp(IPB,hold))
	    return it.Index().V();
	}
	if(!this->input.IsValid())
	  return it.Index().V();
	if(!this->input.Get(*it))
	  return it.Index().V();
      }
      return data.Size();
    }
    //: Get an array of data from stream.
    // returns the number of elements processed
   
    virtual DPIPortBaseC Port()
    { return DPIPortBaseC(*this); }
    //: Access input port.
    
    virtual bool Save(std::ostream &out) const 
    { return DPIStreamOpBodyC<DataT,DataT>::Save(out); }
    //: Save to std::ostream.

  };
  
  //////////////////////////////////////
  //! userlevel=Develop
  //: OPipe body.
  
  template<class DataT>
  class DPOPipeBodyC : 
    public DPOPipeBaseBodyC,
    public DPOStreamOpBodyC<DataT,DataT>
  {
  public:
    DPOPipeBodyC(const DPEntityC &nhold = DPEntityC(true),FuncT nAuxOp = 0) 
      : DPOPipeBaseBodyC(nhold,nAuxOp)
    {}
    //: Default constructor.
    
    DPOPipeBodyC(const DPOPortC<DataT> &port,const DPEntityC &nhold = DPEntityC(true),FuncT nAuxOp = 0)
      : DPOPipeBaseBodyC(nhold,nAuxOp),
	DPOStreamOpBodyC<DataT,DataT>(port)
    {}
    //: Constructor.
    
    virtual bool SetOutput(const DPOPortBaseC &out) {
      DPOPortC<DataT> outport(out);
      this->output = outport;
      return true;
    }
    //: Setup output.
    
    virtual bool Put(const DataT &buff) {
      if(auxOp != 0) {
	DPOPipeBaseC OPB(*this);
	if(!auxOp(OPB,hold))
	  return false;
      }
      if(!this->output.IsValid())
	return false;
      return this->output.Put(buff);
    }
    //: Put an item of data.
    
    virtual IntT PutArray(const SArray1dC<DataT> &data) {
      for(SArray1dIterC<DataT> it(data);it;it++) {
	if(auxOp != 0) {
	  DPOPipeBaseC OPB(*this);
	  if(!auxOp(OPB,hold))
	    return it.Index().V();
	}
	if(!this->output.IsValid())
	  return it.Index().V();
	if(!this->output.Put(*it))
	  return it.Index().V();
      }
      return data.Size();
    }
    //: Put an array of data to stream.
    // returns the number of elements processed.
    
    virtual bool IsPutReady() const  {
      if(forceReady)
	return true;
      if(!this->output.IsValid())
	return false;
      return this->output.IsPutReady(); 
    }
    //: Is some data ready ?
    // true = yes.
    
    virtual void PutEOS() {
      if(forceNotEOS)
	return ;
      if(this->output.IsValid())
	this->output.PutEOS();
    }
    //: Put End Of Stream marker.
    
    virtual DPOPortBaseC Port()
    { return DPOPortBaseC(*this); }
    //: Access ouput port.
    
    virtual bool Save(std::ostream &out) const 
    { return DPOStreamOpBodyC<DataT,DataT>::Save(out); }
    //: Save to std::ostream.
    
  };
  
  
  //////////////////////////////////////
  //! userlevel=Develop
  //: IPipe body.
  
  template<class DataT>
  class DPIPipeC : 
    public DPIPipeBaseC,
    public DPIStreamOpC<DataT,DataT>
  {
  public:
    DPIPipeC() 
      : DPEntityC(true)
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    DPIPipeC(const DPEntityC &hold,DPIPipeBaseBodyC::FuncT nAuxOp = 0) 
      : DPEntityC(*new DPIPipeBodyC<DataT>(hold,nAuxOp))
    {}
    //: Constructor.
    
    DPIPipeC(const DPIPortC<DataT> &port,const DPEntityC &nhold = DPEntityC(true),DPIPipeBaseBodyC::FuncT nAuxOp = 0) 
      : DPEntityC(*new DPIPipeBodyC<DataT>(port,nhold,nAuxOp))
    {}
    //: Constructor.
  };
  
  //////////////////////////////////////
  //! userlevel=Develop
  //: OPipe body.
  
  template<class DataT>
  class DPOPipeC : 
    public DPOPipeBaseC,
    public DPOStreamOpC<DataT,DataT>
  { 
  public:
    DPOPipeC() 
      : DPEntityC(true)
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    DPOPipeC(const DPEntityC &hold,DPOPipeBaseBodyC::FuncT nAuxOp = 0) 
      : DPEntityC(*new DPOPipeBodyC<DataT>(hold,nAuxOp))
    {}
    //: Constructor.
    
    DPOPipeC(const DPOPortC<DataT> &port,const DPEntityC &nhold = DPEntityC(true),DPOPipeBaseBodyC::FuncT nAuxOp = 0)
      : DPEntityC(*new DPOPipeBodyC<DataT>(port,nhold,nAuxOp))
    {}
    //: Constructor.
  };
  
  ///////////////////////////////////////////
  
  inline 
  bool DPIPipeBaseC::SetInput(const DPIPortBaseC &in)
  { return Body().SetInput(in); }
  
  
  ///////////////////////////////////////////
  
  inline 
  bool DPOPipeBaseC::SetOutput(const DPOPortBaseC &out)
  { return Body().SetOutput(out); }
  
  ///////////////////////////////////////////
  
}

#endif
