// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_SIGNAL_HEADER
#define RAVL_SIGNAL_HEADER 1
/////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlThreads
//! author="Charles Galambos"
//! date="23/09/1999"
//! docentry="Ravl.API.OS.Signals"
//! file="Ravl/OS/Threads/Tools/Signal.hh"
//! example=exSignal.cc

#include "Ravl/Threads/RCRWLock.hh"
#include "Ravl/RefCounter.hh"
#include "Ravl/DLink.hh"
#include "Ravl/SArray1d.hh"
#include "Ravl/SArray1dIter.hh"
#include "Ravl/Assert.hh"
#include "Ravl/IntrDList.hh"
#include "Ravl/Types.hh"
#include "Ravl/RCLayer.hh"

namespace RavlN {

  class Signal0BodyC;
  class Signal0C;
  
  //! userlevel=Develop
  //: 0 signal base connector.
  
  class SignalConnector0BodyC
    : public RCBodyVC
  {
  public:
    inline SignalConnector0BodyC()
      : sigbod(0),
	ind(-1)
    {}
    //: Constructor.
    
    inline void Connect(Signal0BodyC &from);
    //: Constructor.
    
    inline void Connect(Signal0C &from);
    //: Constructor.
    
    virtual ~SignalConnector0BodyC();
    //: Default constructor.
    
    virtual void Disconnect(bool waitThreadsExit = false);
    //: Disconnect from input list.
    
    virtual bool Invoke();
    //: Pass signal on.
    
    bool IsConnected() const 
    { return ind >= 0; }
    //: Test if connection is made
    
  protected:
    Signal0BodyC *sigbod;
    IntT ind; // Index of entry in Signal0BodyC. -1 = Disconnected.
    RCRWLockC sigAccess; // RWLock for source signal, used to ensure the lock is not deleted before its finished with.
    
    friend class Signal0BodyC;
  };
  
  //! userlevel=Advanced
  //: Signal connector handle.
  
  class SignalConnectorC
    : public RCHandleC<SignalConnector0BodyC>
  {
  public:
    SignalConnectorC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    void Disconnect(bool waitThreadsExit = false)
    { Body().Disconnect(waitThreadsExit); }
    //: Disconnect handle.
    
    bool Invoke()
    { return Body().Invoke(); }
    //: Pass signal on.
    
    bool IsConnected() const 
    { return Body().IsConnected(); }
    //: Test if connection is made
    
    bool operator==(const SignalConnectorC &oth) const
    { return &Body() == &oth.Body(); }
    //: Are to connectors the same ?
    
    bool operator!=(const SignalConnectorC &oth) const
    { return &Body() != &oth.Body(); }
    //: Are to connectors different ?

    SignalConnector0BodyC  &Body()
    { return RCHandleC<SignalConnector0BodyC>::Body(); }
    //: Access body.

    const SignalConnector0BodyC  &Body() const
    { return RCHandleC<SignalConnector0BodyC>::Body(); }
    //: Access body.
    
  protected:
    SignalConnectorC(SignalConnector0BodyC &bod)
      : RCHandleC<SignalConnector0BodyC>(bod)
    {}
    //: Body constructor.  
    
    SignalConnectorC(const SignalConnector0BodyC *bod)
      : RCHandleC<SignalConnector0BodyC>(bod)
    {}
    //: Body constructor.  
    
    friend class Signal0BodyC;
    friend class SignalConnector0BodyC;
  };
  
  std::ostream &operator<<(std::ostream &os,const SignalConnectorC &sc);
  //! userlevel=Normal
  //: Output to a stream.
  //Only usefull for debugging.
  
  std::istream &operator>>(std::istream &os,SignalConnectorC &sc);
  //! userlevel=Normal
  //: Output to a stream.
  // Not implemented.
  
  BinOStreamC &operator<<(BinOStreamC &strm,const SignalConnectorC  &vertex);
  //: Binary stream output.
  // Not implemented
  
  BinIStreamC &operator>>(BinIStreamC &strm,SignalConnectorC  &vertex);
  //: Binary stream input.
  // Not implemented
  
  ////////////////////////////////////////////////////////////////
  //! userlevel=Develop
  //: class for signal interconnector with 0 args.
  
  class SignalInterConnect0BodyC
    : public DLinkC,
      virtual public SignalConnector0BodyC
  {
  public:
    SignalInterConnect0BodyC()
      : target(0)
    {}
    //: Constructor.
    
    ~SignalInterConnect0BodyC();
    //: Destructor.
    
    void Connect(Signal0BodyC &from,Signal0BodyC &targ);
    //: Constructor.
    
    void Connect(Signal0C &from,Signal0C &targ);
    //: Constructor.
    
    virtual void Disconnect(bool waitThreadsExit);
    //: Disconnect from input list.
    
    virtual bool Invoke();
    //: Invoke signal.
    
    Signal0BodyC &Target() {
      RavlAssert(target != 0);
      return *target; 
    }
    //: Get target for connection.
    
    const Signal0BodyC &Target() const {
      RavlAssert(target != 0);
      return *target; 
    }
    //: Get target for connection.
    
  protected:
    RCLayerC<RCLayerBodyC> targetHandle; // Reference to taret handle to ensure its not deleted before all signals are processed.
    RCRWLockC sigTargetAccess; // RWLock for target signal, used to ensure the lock is not deleted before its finished with.    
    Signal0BodyC *target; // Target to send signal to.
    
    friend class Signal0BodyC;
  };
  
  //! userlevel=Advanced
  //: class for signal interconnector with 0 args.
  
  class SignalInterConnect0C
    : public SignalConnectorC
  {
  public:
    SignalInterConnect0C(Signal0C &from,Signal0C &targ);
    //: Constructor.  
    
    SignalInterConnect0C()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
  protected:
    SignalInterConnect0C(SignalInterConnect0BodyC &bod)
      : SignalConnectorC(bod)
    {}
    //: Body constructor.
    
    const SignalInterConnect0BodyC &Body() const
    { return dynamic_cast<const SignalInterConnect0BodyC &>(RCHandleC<SignalConnector0BodyC>::Body()); }
    //: Access body.
    
    SignalInterConnect0BodyC &Body() 
    { return dynamic_cast<SignalInterConnect0BodyC &>(RCHandleC<SignalConnector0BodyC>::Body()); }
    //: Access body.
    
    friend class Signal0BodyC;
  };

  ////////////////////////////////////////////////////////////////
  
  //! userlevel=Develop
  //: Signal base body.
  
  class Signal0BodyC
    : public RCLayerBodyC
  {
  public:
    Signal0BodyC()
      : access(true),
	execLock(true)
    {}
    //: Default constructor.
    
    ~Signal0BodyC();
    //: Destructor.
    
    virtual bool Invoke();
    //: Send default signal.
    
    bool Disconnect(Signal0C &targ);
    //: Disconnect other signal from this one.
    // 'targ' must be the target of the signal.
    
    SignalInterConnect0C FindInterConnect(const Signal0C &targ);
    //: Find interconnection between this and 'targ'.
    
    void DisconnectAll(bool waitThreadsExit = false);
    //: Disconnect all signals.
    
    void DisconnectInputs(bool waitThreadsExit = false);
    //: Disconnect all inputs to this signal.
    
    void DisconnectOutputs(bool waitThreadsExit = false);
    //: Disconnect all outputs from this signal.
    
    IntT NumOutputs() const;
    //: Access the number of connections from the signal.
    
    IntT NumInputs() const;
    //: Access the number of inputs to the signal.
    
  protected:
    virtual void ZeroOwners();
    //: Called when owner handles drop to zero.
    
    void Connect(SignalConnector0BodyC &con);
    //: Connect a new output.
    
    void Disconnect(SignalConnector0BodyC &con,bool waitThreadsExit,RCRWLockC &signalLock);
    //: Disconnect an output.
    //!param: con - Connector to remove
    //!param: waitThreadsExist - Wait for all threads to exit?
    //!param: signalLock - Handle to the lock for THIS signal. Needed to avoid problems if signal has been destroyed before the connector has been removed.

    void ConnectInput(SignalInterConnect0BodyC &in);
    //: Connect an input.
    
    void DisconnectInput(SignalInterConnect0BodyC &in);
    //: Disconnect an input.
    
    RCRWLockC access; // Access control for structure.
    RCRWLockC execLock; // Execution control, used to manage disconnection.
    SArray1dC<SignalConnectorC> outputs; // List of outputs.
    IntrDListC<SignalInterConnect0BodyC > inputs; // List of inputs.
    
    friend class SignalConnector0BodyC;
    friend class SignalInterConnect0BodyC;
  };
  
  //! userlevel=Normal
  //: Signal 0 handle.
  
  class Signal0C
    : public RCLayerC<Signal0BodyC>
  {
  public:
    typedef bool (*FuncT)(void);
    
    Signal0C()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    Signal0C(bool makeBod)
      : RCLayerC<Signal0BodyC>(*new Signal0BodyC())
    {}
    //: Constructor.
    
  protected:
    Signal0C(Signal0BodyC &bod,RCLayerHandleT handleType = RCLH_OWNER)
      : RCLayerC<Signal0BodyC>(bod,handleType)
    {}
    //: Body constructor.
    
    Signal0C(const Signal0BodyC *bod,RCLayerHandleT handleType = RCLH_OWNER)
      : RCLayerC<Signal0BodyC>(bod,handleType)
    {}
    //: Body constructor.
    
    Signal0C(const Signal0C &other,RCLayerHandleT handleType)
      : RCLayerC<Signal0BodyC>(other,handleType)
    {}
    //: Change handle type.
    
    inline 
    Signal0BodyC &Body() 
    { return RCLayerC<Signal0BodyC>::Body(); }
    //: Access body.
    
    inline 
    const Signal0BodyC &Body() const 
    { return RCLayerC<Signal0BodyC>::Body(); }
    //: Access body.
    
  public:
    SignalInterConnect0C FindInterConnect(const Signal0C &targ)
    { return Body().FindInterConnect(targ); }
    //: Find interconnection between this and 'targ'.
    // 'targ' must be the target of the signal.
    
    inline bool Disconnect(Signal0C &targ)
    { return Body().Disconnect(targ); }
    //: Disconnect other signal from this one.
    // 'targ' must be the target of the signal.
    
    inline void DisconnectAll(bool waitThreadsExit = false)
    { Body().DisconnectAll(waitThreadsExit); }
    //: Disconnect all signals from this one.
    
    void DisconnectInputs(bool waitThreadsExit = false)
    { Body().DisconnectInputs(waitThreadsExit); }
    //: Disconnect all inputs to this signal.
    
    void DisconnectOutputs(bool waitThreadsExit = false)
    { Body().DisconnectOutputs(waitThreadsExit); }
    //: Disconnect all outputs from this signal.
    
    inline bool Invoke()
    { return Body().Invoke(); }
    //: Send default signal.
    
    inline bool operator()()
    { return Invoke(); }
    //: Simple invokation.
    
    IntT NumOutputs() const
    { return Body().NumOutputs(); }
    //: Access the number of connections from the signal.
    
    IntT NumInputs() const
    { return Body().NumInputs(); }
    //: Access the number of inputs to the signal.
    
    friend class SignalConnector0BodyC;
    friend class SignalInterConnect0C;
    friend class SignalInterConnect0BodyC;
    friend class Signal0BodyC;
  };
  
  
  inline std::ostream &operator<<(std::ostream &out,const Signal0C &) { 
    RavlAssert(0); // Not implemented.
    return out;
  }
  //! userlevel=Normal
  //: Output to a stream.
  // Not implemented, only here to keep some templates that 
  // require an output operator happy.
  
  ////////////////////////////////////////////////////////////////
  
  //! userlevel=Develop
  //: Signal a function.
  
  class Signal0FuncBodyC 
    : public SignalConnector0BodyC
  {
  public:
    typedef bool (*FuncT)(void);
    
    Signal0FuncBodyC(FuncT nFunc)
      : func(nFunc)
    { 
      // We can't connect to the signal here
      // as there's just a chance it'll construct a reference and delete it again
      // before we've returned.
    }
    //: Constructor.
    
    virtual bool Invoke()
    { return func(); }
    //: Call function.
    
  protected:
    FuncT func;
  };
  
  //! userlevel=Advanced
  //: Signal a function
  
  class Signal0FuncC 
    : public SignalConnectorC
  {
  public:
    Signal0FuncC(Signal0C &from,Signal0FuncBodyC::FuncT nFunc)
      : SignalConnectorC(*new Signal0FuncBodyC(nFunc))
    { Body().Connect(from); }
    //: Constructor.
  };
  
  ////////////////////////////////////////////////////////////////
  
  //! userlevel=Develop
  //: Signal a method
  
  template<class DataT>
  class Signal0MethodBodyC 
    : public SignalConnector0BodyC
  {
  public:
    typedef bool (DataT::*FuncT)();
    
    Signal0MethodBodyC(const DataT &ndata,
		       FuncT nFunc)
      : data(ndata),
	func(nFunc)
    {}
    //: Constructor.
    
    virtual bool Invoke()
    { return (data.*func)(); }
    //: Call function.
    
  protected:
    DataT data;
    FuncT func;
  };
  
  //! userlevel=Advanced
  //: Signal a method.

  template<class DataT>
  class Signal0MethodC 
    : public SignalConnectorC
  {
  public:
    Signal0MethodC(Signal0C &from,
		   const DataT &ndata,
		   bool (DataT::*nFunc)())
      : SignalConnectorC(*new Signal0MethodBodyC<DataT>(ndata,nFunc))
    { Body().Connect(from); }
    //: Constructor.
  };

  
  ////////////////////////////////////////////////////////////////
  
  //! userlevel=Develop
  //: Signal a method
  
  template<class HandleT,class DataT>
  class Signal0MethodPtrBodyC 
    : public SignalConnector0BodyC
  {
  public:
    typedef bool (DataT::*FuncT)();
    
    Signal0MethodPtrBodyC(const HandleT &ndata,
		       FuncT nFunc)
      : data(ndata),
	func(nFunc)
    {}
    //: Constructor.
    
    virtual bool Invoke()
    { return (data->*func)(); }
    //: Call function.
    
  protected:
    HandleT data;
    FuncT func;
  };
  
  //! userlevel=Advanced
  //: Signal a method.

  template<class HandleT,class DataT>
  class Signal0MethodPtrC 
    : public SignalConnectorC
  {
  public:
    Signal0MethodPtrC(Signal0C &from,
		      const HandleT &ndata,
		      bool (DataT::*nFunc)())
      : SignalConnectorC(*new Signal0MethodPtrBodyC<HandleT,DataT>(ndata,nFunc))
    { Body().Connect(from); }
    //: Constructor.
  };
  
  ////////////////////////////////////////////////////////////////
  
  //! userlevel=Develop
  //: Signal a method
  // Uses reference to object not instance. <p>
  // NB. It is the users responsibility to ensure the object
  // remains valid while being used.
  
  template<class DataT>
  class Signal0MethodRefBodyC 
    : public SignalConnector0BodyC
  {
  public:
    typedef bool (DataT::*FuncT)();
    
    Signal0MethodRefBodyC(DataT &ndata,
			  FuncT nFunc)
      : data(ndata),
	func(nFunc)
    {}
    //: Constructor.
    
    virtual bool Invoke()
    { return (data.*func)(); }
    //: Call function.
    
  protected:
    DataT &data;
    FuncT func;
  };
  
  //! userlevel=Advanced
  //: Signal a method.
  // Uses reference to object not instance. <p>
  // NB. It is the users responsibility to ensure the object
  // remains valid while being used.
  
  template<class DataT>
  class Signal0MethodRefC 
    : public SignalConnectorC
  {
  public:
    Signal0MethodRefC(Signal0C &from,
		      DataT &ndata,
		      bool (DataT::*nFunc)())
      : SignalConnectorC(*new Signal0MethodRefBodyC<DataT>(ndata,nFunc))
    { Body().Connect(from); }
    //: Constructor.
  };
  
  ////////////////////////////////////////////
  
  inline
  void SignalConnector0BodyC::Connect(Signal0BodyC &from) {
    RavlAssert(ind == -1); // Signal can't already be connected
    sigbod = &from;
    from.Connect(*this); 
  }
  
  inline 
  void SignalConnector0BodyC::Connect(Signal0C &from) { 
    RavlAssert(ind == -1); // Signal can't already be connected
    sigbod = &(from.Body());
    sigbod->Connect(*this); 
  }
  
  ////////////////////////////////////////////////////////////////
  
  inline 
  SignalConnectorC Connect(Signal0C &from,Signal0C &to) { 
    RavlAssert(from.IsValid());
    RavlAssert(to.IsValid());
    return SignalInterConnect0C(from,to); 
  }
  //! userlevel=Normal
  //: Connect signal to another signal.
  
  inline 
  SignalConnectorC Connect(Signal0C &from,Signal0FuncBodyC::FuncT func) { 
    RavlAssert(from.IsValid());
    return Signal0FuncC(from,func); 
  }
  //! userlevel=Normal
  //: Connect signal to a function with 0 args.
  
  template<class DataT>
  inline 
  SignalConnectorC Connect(Signal0C &from,const DataT &obj,bool (DataT::* func)()) { 
    RavlAssert(from.IsValid());
    return Signal0MethodC<DataT>(from,obj,func); 
  }
  //! userlevel=Normal
  //: Connect signal to a method with 0 args.
  // This holds a handle to the class to be called.
  
  template<class HandleT,class DataT>
  inline 
  SignalConnectorC ConnectPtr(Signal0C &from,const HandleT &obj,bool (DataT::* func)()) { 
    RavlAssert(from.IsValid());
    return Signal0MethodPtrC<HandleT,DataT>(from,obj,func); 
  }
  //! userlevel=Normal
  //: Connect signal to a method with 0 args.
  // This holds a handle to the class to be called. Uses the -> operator
  // to invoke the method.
  
  template<class DataT>
  inline 
  SignalConnectorC ConnectRef(Signal0C &from,DataT &obj,bool (DataT::* func)()) { 
    RavlAssert(from.IsValid());
    return Signal0MethodRefC<DataT>(from,obj,func); 
  }
  //! userlevel=Normal
  //: Connect signal to a method with 0 args.
  // This holds a REFRENCE to the class to be called.
  // <p>
  // Uses reference to object not instance. <p>
  // NB. It is the users responsibility to ensure the object
  // remains valid while being used.

  template<class DataT>
  inline 
  SignalConnectorC ConnectR(Signal0C &from,DataT &obj,bool (DataT::* func)()) { 
    RavlAssert(from.IsValid());
    return Signal0MethodRefC<DataT>(from,obj,func); 
  }
  //! userlevel=Normal
  //: Connect signal to a method with 0 args.
  // This holds a REFRENCE to the class to be called.
  // <p>
  // Uses reference to object not instance. <p>
  // NB. It is the users responsibility to ensure the object
  // remains valid while being used.

};

#endif
