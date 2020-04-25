// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_SIGNAL2_HEADER
#define RAVL_SIGNAL2_HEADER 1
/////////////////////////////////////////////////////
//! docentry="Ravl.API.OS.Signals"
//! rcsid="$Id$"
//! file="Ravl/OS/Threads/Tools/Signal2.hh"
//! lib=RavlThreads
//! author="Charles Galambos"
//! date="23/09/1999"
//! example=exSignal1.cc

#include "Ravl/Threads/Signal1.hh"
#include "Ravl/IntrDLIter.hh"
#include "Ravl/Threads/RWLock.hh"

// Fix for Visual C++ which doesn't like default values for
// some templated arguments. 
#if  !RAVL_COMPILER_VISUALCPP && !RAVL_COMPILER_VISUALCPPNET
#define VCPPARGFIX(x) x
#else
#define VCPPARGFIX(x)
#endif

namespace RavlN {
  
  template<class Data1T,class Data2T> class Signal2C;
  
  ////////////////////////////////////////////////////
  //! userlevel=Develop
  //: Signal 1 connector.
  
  template<class Data1T,class Data2T>
  class SignalConnector2BodyC
    : virtual public SignalConnector1BodyC<Data1T>
  {
  public:    
    inline SignalConnector2BodyC(const Data1T &def1,const Data2T &def2)
      : SignalConnector1BodyC<Data1T>(def1),
	defaultVal2(def2)
      {}
    //: Constructor.
    
    inline SignalConnector2BodyC()
    {}
    //: Constructor.
    
    virtual bool Invoke(Data1T &,Data2T &) = 0;
    //: Pass signal on.
    
    virtual bool Invoke(Data1T &d1)
    { return Invoke(d1,defaultVal2); }
    //: Pass signal on, use default value.
    
    virtual bool Invoke()
    { return Invoke(this->defaultVal,defaultVal2); }
    //: Pass signal on, use default value.
    
  protected:
    Data2T defaultVal2;
  };
  
  //! userlevel=Develop
  //: Signal 2 inter connector.
  
  template<class Data1T,class Data2T>
  class SignalInterConnect2BodyC
    : virtual public SignalConnector2BodyC<Data1T,Data2T>,
      virtual public SignalInterConnect0BodyC
  {
  public:
    SignalInterConnect2BodyC()
    {}
    //: Constructor.
    
    inline virtual bool Invoke(Data1T &,Data2T &);
    //: Invoke signal, with value.
    
    inline virtual bool Invoke(Data1T &);
    //: Invoke signal, with value.
    
    inline virtual bool Invoke();
    //: Invoke signal, with default value.
    
    virtual void Disconnect(bool waitThreadsExit)
    { SignalInterConnect0BodyC::Disconnect(waitThreadsExit); }
    //: Disconnect from input list.
    
  };
  
  ///////////////////////////
  //! userlevel=Advanced
  //: class for signal interconnector with 1 arg.
  
  template<class Data1T,class Data2T>
  class SignalInterConnect2C
    : public SignalInterConnect0C
  {
  public:
    SignalInterConnect2C(Signal0C &from,Signal2C<Data1T,Data2T> &targ)
      : SignalInterConnect0C(*new SignalInterConnect2BodyC<Data1T,Data2T>())
    { Body().Connect(from,targ); }
    //: Constructor.
    
  protected:
    SignalInterConnect2BodyC<Data1T,Data2T> &Body()
    { return dynamic_cast<SignalInterConnect2BodyC<Data1T,Data2T> &>(RCHandleC<SignalConnector0BodyC>::Body()); }
    //: Access body.

  };
  
  ///////////////////////////////////////////////////
  //! userlevel=Develop
  //: Signal 1 function connector.
  
  template<class Data1T,class Data2T>
  class Signal2FuncBodyC
    : public SignalConnector2BodyC<typename TraitsC<Data1T>::BaseTypeT,typename TraitsC<Data2T>::BaseTypeT>
  {
  public:
    typedef typename TraitsC<Data1T>::BaseTypeT Arg1T; //: Type of arguments without const's and refs.
    typedef typename TraitsC<Data2T>::BaseTypeT Arg2T; //: Type of arguments without const's and refs.
    typedef bool (*Func2T)(Data1T,Data2T);
    
    Signal2FuncBodyC(Func2T nFunc,const Arg1T &def1,const Arg2T &def2)
      : SignalConnector1BodyC<typename TraitsC<Data1T>::BaseTypeT>(def1),
	SignalConnector2BodyC<typename TraitsC<Data1T>::BaseTypeT,typename TraitsC<Data2T>::BaseTypeT>(def1,def2),
	func(nFunc)
    {}
    //: Constructor.
    
    virtual bool Invoke()
    { return func(this->defaultVal,this->defaultVal2); }
    //: Call function.
    // Use default value.
    
    virtual bool Invoke(Arg1T &val)
    { return func(val,this->defaultVal2); }
    //: Call function.
    
    virtual bool Invoke(Arg1T &val1,Arg2T &val2)
    { return func(val1,val2); }
    //: Call function.
    
  protected:
    Func2T func;
  };
  
  //! userlevel=Advanced
  //: Signal a function
  
  template<class Data1T,class Data2T>
  class Signal2FuncC 
    : public SignalConnectorC
  {
  public:
    typedef typename TraitsC<Data1T>::BaseTypeT Arg1T; //: Type of arguments without const's and refs.
    typedef typename TraitsC<Data2T>::BaseTypeT Arg2T; //: Type of arguments without const's and refs.
    
    Signal2FuncC(Signal0C &from,typename Signal2FuncBodyC<Data1T,Data2T>::Func2T nFunc,const Arg1T &def1 = Arg1T(),const Arg2T &def2 = Arg2T())
      : SignalConnectorC(*new Signal2FuncBodyC<Data1T,Data2T>(nFunc,def1,def2))
    { Body().Connect(from); }
    //: Constructor.
    
  };
  
  ///////////////////////////////////////////////////
  //! userlevel=Develop
  //: Signal 2 method connector.
  
  template<class Data1T,class Data2T,class ObjT>
  class Signal2MethodBodyC
    : public SignalConnector2BodyC<typename TraitsC<Data1T>::BaseTypeT,typename TraitsC<Data2T>::BaseTypeT>
  {
  public:
    typedef typename TraitsC<ObjT>::BaseTypeT BaseObjT; //: Type of object without const's and refs.
    typedef typename TraitsC<Data1T>::BaseTypeT Arg1T; //: Type of arguments without const's and refs.
    typedef typename TraitsC<Data2T>::BaseTypeT Arg2T; //: Type of arguments without const's and refs.
#if !RAVL_COMPILER_VISUALCPP 
    typedef bool (BaseObjT::*Func2T)(Data1T,Data2T);
#else
    typedef bool (ObjT::*Func2T)(Data1T,Data2T);
#endif
    
    Signal2MethodBodyC(BaseObjT &nobj,
		       typename Signal2MethodBodyC<Data1T,Data2T,ObjT>::Func2T nFunc,
		       const Arg1T &dat1 = Arg1T(),
		       const Arg2T &dat2 = Arg2T())
      : SignalConnector1BodyC<typename TraitsC<Data1T>::BaseTypeT>(dat1),
	SignalConnector2BodyC<typename TraitsC<Data1T>::BaseTypeT,typename TraitsC<Data2T>::BaseTypeT>(dat1,dat2),
	obj(nobj),
	func(nFunc)
    {}
    //: Constructor.
    
    virtual bool Invoke()
    { return (obj.*func)(this->defaultVal,this->defaultVal2); }
    //: Call function.
    
    virtual bool Invoke(Arg1T &val)
    { return (obj.*func)(val,this->defaultVal2); }
    //: Call function.
    
    virtual bool Invoke(Arg1T &val1,Arg2T &val2)
    { return (obj.*func)(val1,val2); }
    //: Call function.
    
  protected:
    ObjT obj;
    Func2T func;
  };
  
  //! userlevel=Advanced
  //: Signal a method.
  
  template<class Data1T,class Data2T,class ObjT>
  class Signal2MethodC
    : public SignalConnectorC
  {
  public:
    typedef typename TraitsC<ObjT>::BaseTypeT BaseObjT; //: Type of object without const's and refs.
    typedef typename TraitsC<Data1T>::BaseTypeT Arg1T; //: Type of arguments without const's and refs.
    typedef typename TraitsC<Data2T>::BaseTypeT Arg2T; //: Type of arguments without const's and refs.
    
    Signal2MethodC(Signal0C &from,
		   BaseObjT &nobj,
		   typename Signal2MethodBodyC<Data1T,Data2T,ObjT>::Func2T nFunc,
		   const Arg1T &dat1 = Arg1T(),
		   const Arg2T &dat2 = Arg2T())
      : SignalConnectorC(*new Signal2MethodBodyC<Data1T,Data2T,ObjT>(nobj,nFunc,dat1,dat2))
    { Body().Connect(from); }
    //: Constructor.
  };







  ///////////////////////////////////////////////////
  //! userlevel=Develop
  //: Signal 2 method connector.
  
  template<class Data1T,class Data2T,class HandleT,class ObjT>
  class Signal2MethodPtrBodyC
    : public SignalConnector2BodyC<typename TraitsC<Data1T>::BaseTypeT,typename TraitsC<Data2T>::BaseTypeT>
  {
  public:
    typedef typename TraitsC<ObjT>::BaseTypeT BaseObjT; //: Type of object without const's and refs.
    typedef typename TraitsC<Data1T>::BaseTypeT Arg1T; //: Type of arguments without const's and refs.
    typedef typename TraitsC<Data2T>::BaseTypeT Arg2T; //: Type of arguments without const's and refs.
#if !RAVL_COMPILER_VISUALCPP 
    typedef bool (BaseObjT::*Func2T)(Data1T,Data2T);
#else
    typedef bool (ObjT::*Func2T)(Data1T,Data2T);
#endif
    
    Signal2MethodPtrBodyC(const HandleT &nobj,
			  typename Signal2MethodPtrBodyC<Data1T,Data2T,HandleT,ObjT>::Func2T nFunc,
			  const Arg1T &dat1 = Arg1T(),
			  const Arg2T &dat2 = Arg2T())
      : SignalConnector1BodyC<typename TraitsC<Data1T>::BaseTypeT>(dat1),
	SignalConnector2BodyC<typename TraitsC<Data1T>::BaseTypeT,typename TraitsC<Data2T>::BaseTypeT>(dat1,dat2),
	obj(nobj),
	func(nFunc)
    {}
    //: Constructor.
    
    virtual bool Invoke()
    { return (obj->*func)(this->defaultVal,this->defaultVal2); }
    //: Call function.
    
    virtual bool Invoke(Arg1T &val)
    { return (obj->*func)(val,this->defaultVal2); }
    //: Call function.
    
    virtual bool Invoke(Arg1T &val1,Arg2T &val2)
    { return (obj->*func)(val1,val2); }
    //: Call function.
    
  protected:
    HandleT obj;
    Func2T func;
  };
  
  //! userlevel=Advanced
  //: Signal a method.
  
  template<class Data1T,class Data2T,class HandleT,class ObjT>
  class Signal2MethodPtrC
    : public SignalConnectorC
  {
  public:
    typedef typename TraitsC<ObjT>::BaseTypeT BaseObjT; //: Type of object without const's and refs.
    typedef typename TraitsC<Data1T>::BaseTypeT Arg1T; //: Type of arguments without const's and refs.
    typedef typename TraitsC<Data2T>::BaseTypeT Arg2T; //: Type of arguments without const's and refs.
    
    Signal2MethodPtrC(Signal0C &from,
		      const HandleT &nobj,
		      typename Signal2MethodPtrBodyC<Data1T,Data2T,HandleT,ObjT>::Func2T nFunc,
		      const Arg1T &dat1 = Arg1T(),
		      const Arg2T &dat2 = Arg2T())
      : SignalConnectorC(*new Signal2MethodPtrBodyC<Data1T,Data2T,HandleT,ObjT>(nobj,nFunc,dat1,dat2))
    { Body().Connect(from); }
    //: Constructor.
  };
  
  
#if RAVL_COMPILER_VISUALCPP 
  
  ///////////////////////////////////////////////////
  //! userlevel=Develop
  //: Signal 2 method connector.
  
  template<class Data1T,class Data2T,class ObjT>
  class Signal2MethodRefBodyC
    : public SignalConnector2BodyC<typename TraitsC<Data1T>::BaseTypeT,typename TraitsC<Data2T>::BaseTypeT>
  {
  public:
    typedef typename TraitsC<ObjT>::BaseTypeT BaseObjT; //: Type of object without const's and refs.
    typedef typename TraitsC<Data1T>::BaseTypeT Arg1T; //: Type of arguments without const's and refs.
    typedef typename TraitsC<Data2T>::BaseTypeT Arg2T; //: Type of arguments without const's and refs.
    typedef bool (ObjT::*Func2T)(Data1T,Data2T);
    
    Signal2MethodRefBodyC(BaseObjT &nobj,
			  typename Signal2MethodBodyC<Data1T,Data2T,ObjT>::Func2T nFunc,
			  const Arg1T &dat1 = Arg1T(),
			  const Arg2T &dat2 = Arg2T())
      : SignalConnector1BodyC<typename TraitsC<Data1T>::BaseTypeT>(dat1),
	SignalConnector2BodyC<typename TraitsC<Data1T>::BaseTypeT,typename TraitsC<Data2T>::BaseTypeT>(dat1,dat2),
	obj(nobj),
	func(nFunc)
    {}
    //: Constructor.
    
    virtual bool Invoke()
    { return (obj.*func)(defaultVal,defaultVal2); }
    //: Call function.
    
    virtual bool Invoke(Arg1T &val)
    { return (obj.*func)(val,defaultVal2); }
    //: Call function.
    
    virtual bool Invoke(Arg1T &val1,Arg2T &val2)
    { return (obj.*func)(val1,val2); }
    //: Call function.
    
  protected:
    ObjT &obj;
    Func2T func;
  };
  
  //! userlevel=Advanced
  //: Signal a method.
  
  template<class Data1T,class Data2T,class ObjT>
  class Signal2MethodRefC
    : public SignalConnectorC
  {
  public:
    typedef typename TraitsC<ObjT>::BaseTypeT BaseObjT; //: Type of object without const's and refs.
    typedef typename TraitsC<Data1T>::BaseTypeT Arg1T; //: Type of arguments without const's and refs.
    typedef typename TraitsC<Data2T>::BaseTypeT Arg2T; //: Type of arguments without const's and refs.
    
    Signal2MethodRefC(Signal0C &from,
		      BaseObjT &nobj,
		      typename Signal2MethodBodyC<Data1T,Data2T,ObjT>::Func2T nFunc,
		      const Arg1T &dat1 = Arg1T(),
		      const Arg2T &dat2 = Arg2T())
      : SignalConnectorC(*new Signal2MethodRefBodyC<Data1T,Data2T,ObjT>(nobj,nFunc,dat1,dat2))
    { Body().Connect(from); }
    //: Constructor.
  };
  
#endif

  
  ////////////////////////////////////////////////////////////////
  
  //! userlevel=Develop
  //: Signal 1 body.
  
  template<class Data1T,class Data2T>
  class Signal2BodyC
    : public Signal1BodyC<Data1T>
  {
  public:
    Signal2BodyC(const Data1T &def1,const Data2T &def2)
      : Signal1BodyC<Data1T>(def1), 
	defaultVal2(def2)     
      {}
    //: Constructor.
    
    virtual bool Invoke(Data1T &v1,Data2T &v2) {
      RWLockHoldC hold(this->access,RWLOCK_READONLY);
      SArray1dIterC<SignalConnectorC> it(this->outputs);
      RCRWLockC refExecLock = this->execLock;
      hold.Unlock();
      // Flag that we're executing signal code.
      // This is used to ensure all threads have left the signal handlers
      // before they are disconnected.
      RWLockHoldC holdExec(refExecLock,RWLOCK_READONLY);
      bool ret = true;
      for(;it;it++) {
	SignalConnector2BodyC<Data1T,Data2T> *sc2 = dynamic_cast<SignalConnector2BodyC<Data1T,Data2T> *>(&it.Data().Body());
	if(sc2 != 0) {
	  ret &= sc2->Invoke(v1,v2);
	  continue;
	}
	SignalConnector1BodyC<Data1T> *sc1 = dynamic_cast<SignalConnector1BodyC<Data1T> *>(&it.Data().Body());
	if(sc1 != 0) {
	  ret &= sc1->Invoke(v1);
	  continue;
	}
	// Must be a 0.
	ret &= it.Data().Invoke();
      }
      return ret;
    }
    //: Send signal with value.
    
    virtual bool Invoke(Data1T &v1)
    { return Signal2BodyC<Data1T,Data2T>::Invoke(v1,defaultVal2); }
    //: Send signal with value.
    
    virtual bool Invoke()
    { return Signal2BodyC<Data1T,Data2T>::Invoke(this->defaultVal,defaultVal2); }
    //: Send signal with default value where needed.
    
    Data2T &DefaultValue2()
    { return defaultVal2; }
    //: Access default value 2.
    
    const Data2T &DefaultValue2() const
    { return defaultVal2; }
    //: Access default value 2.
    
  protected:
    Data2T defaultVal2; // Default data value.
  };
  
  //! userlevel=Normal
  //: Signal with 2 arguments
  
  template<class Data1T,class Data2T>
  class Signal2C
    : public Signal1C<Data1T>
  {
  public:
    typedef bool (*Func2T)(Data1T &dat1,Data2T &dat2);
    
    Signal2C()
      {}
    //: Default constructor.
    // Creates an invalid handle.
    
    Signal2C(const Data1T &def1,const Data2T &def2 = Data2T())
      : Signal1C<Data1T>(*new Signal2BodyC<Data1T,Data2T>(def1,def2))
      {}
    //: Default constructor.
    // Creates an invalid handle.
    
    Signal2C(const Signal0C &base)
		: Signal1C<Data1T>(dynamic_cast<const Signal2BodyC<Data1T,Data2T> *>(RCLayerC<Signal0BodyC>::BodyPtr(base)))
    {}
    //: Base constructor.
    // Creates an invalid handle if body type
    // is correct.
    
  protected:
    Signal2C(Signal2BodyC<Data1T,Data2T> &sig)
      : Signal1C<Data1T>(sig)
    {}
    //: Body constructor.
    
    explicit Signal2C(const Signal2BodyC<Data1T,Data2T> *sig)
      : Signal1C<Data1T>(sig)
    {}
    //: Body constructor.
    
    inline 
    Signal2BodyC<Data1T,Data2T> &Body() 
    { return static_cast<Signal2BodyC<Data1T,Data2T> &>(Signal0C::Body()); }
    //: Access body.
    
    inline 
    const Signal2BodyC<Data1T,Data2T> &Body() const 
    { return static_cast<const Signal2BodyC<Data1T,Data2T> &>(Signal0C::Body()); }
    //: Access body.
    
  public:
    inline bool Invoke(Data1T &dat1,Data2T &dat2)
    { return Body().Invoke(dat1,dat2); }
    //: Send default signal.
    
    inline bool operator()(Data1T &dat1,Data2T &dat2)
    { return Body().Invoke(dat1,dat2); }
    //: Simple invokation.
    
    inline bool operator()(const Data1T &dat1,const Data2T &dat2) { 
      Data1T tmp1(dat1);
      Data2T tmp2(dat2);
      return Body().Invoke(tmp1,tmp2); 
    }
    //: Simple invokation.  
    
    Data2T &DefaultValue2()
    { return Body().DefaultValue2(); }
    //: Access default value 2.
    
    const Data2T &DefaultValue2() const
    { return Body().DefaultValue2(); }
    //: Access default value 2.

  };
  
  template<class Data1T,class Data2T>
  std::ostream &operator<<(std::ostream &out,const Signal2C<Data1T,Data2T> &) { 
    RavlAssert(0); // Not implemented.
    return out;
  }
  //! userlevel=Normal
  //: IO Operator.
  // Not implemented
  
  ////////////////////////////
  
  template<class Data1T,class Data2T>
  inline 
  bool SignalInterConnect2BodyC<Data1T,Data2T>::Invoke(Data1T &dat1,Data2T &dat2) {
    RWLockHoldC hold(sigTargetAccess,RWLOCK_READONLY);
    
    // Has connection been terminated?
    if(target == 0) return false;
    
    // Copy pointer in case its zero'd
    Signal0BodyC *tmp = &Target();
    
    // Make handle to target signal to ensure its not deleted before we're finished.
    RCLayerC<RCLayerBodyC> localHandle = targetHandle;
    
    // Don't hold a lock while we're working.
    hold.Unlock();
    
    // Do actual call.
    return dynamic_cast<Signal2BodyC<Data1T,Data2T> &>(*tmp).Invoke(dat1,dat2);
  }
  
  template<class Data1T,class Data2T>
  inline
  bool SignalInterConnect2BodyC<Data1T,Data2T>::Invoke(Data1T &dat1) 
  { return Invoke(dat1,this->defaultVal2); }
  
  template<class Data1T,class Data2T>
  inline
  bool SignalInterConnect2BodyC<Data1T,Data2T>::Invoke()
  { return Invoke(this->defaultVal,this->defaultVal2); }
  
  ///////////////////////
  
  template<class Data1T,class Data2T>
  inline SignalConnectorC Connect(Signal0C &from,Signal2C<Data1T,Data2T> &oth) { 
    RavlAssertMsg(from.IsValid(),"Source signal not valid.");
    RavlAssertMsg(oth.IsValid(),"Destination signal not valid.");
    return SignalInterConnect2C<Data1T,Data2T>(from,oth);
  }
  //! userlevel=Normal
  //: Connect a signal to another signal.
  
  template<class Data1T,class Data2T>  
  inline 
  SignalConnectorC Connect(Signal0C &from,bool (*func)(Data1T,Data2T),
			   const typename TraitsC<Data1T>::BaseTypeT &def1 = VCPPARGFIX(typename) TraitsC<Data1T>::BaseTypeT(),
			   const typename TraitsC<Data2T>::BaseTypeT &def2 = VCPPARGFIX(typename) TraitsC<Data2T>::BaseTypeT()
                           ) { 
    RavlAssertMsg(from.IsValid(),"Source signal not valid.");
    return Signal2FuncC<Data1T,Data2T>(from,func,def1,def2);  
  }
  //! userlevel=Normal
  //: Connect a signal to a function.

  template<class Data1T,class Data2T,class ObjT>
  inline
  SignalConnectorC Connect(Signal0C &from,const ObjT &obj,bool (ObjT::* func)(Data1T,Data2T),
			   const typename TraitsC<Data1T>::BaseTypeT &def1 = VCPPARGFIX(typename) TraitsC<Data1T>::BaseTypeT(),
			   const typename TraitsC<Data2T>::BaseTypeT &def2 = VCPPARGFIX(typename) TraitsC<Data2T>::BaseTypeT()) {
    RavlAssertMsg(from.IsValid(),"Source signal not valid.");
    return Signal2MethodC<Data1T,Data2T,ObjT>(from,const_cast<typename TraitsC<ObjT>::BaseTypeT &>(obj),func,def1,def2); 
  }
  //! userlevel=Normal
  //: Connect a signal to a method.

  template<class Data1T,class Data2T,class HandleT,class ObjT>
  inline
  SignalConnectorC ConnectPtr(Signal0C &from,const HandleT &obj,bool (ObjT::* func)(Data1T,Data2T),
			      const typename TraitsC<Data1T>::BaseTypeT &def1 = VCPPARGFIX(typename) TraitsC<Data1T>::BaseTypeT(),
			      const typename TraitsC<Data2T>::BaseTypeT &def2 = VCPPARGFIX(typename) TraitsC<Data2T>::BaseTypeT()) {
    RavlAssertMsg(from.IsValid(),"Source signal not valid.");
    return Signal2MethodPtrC<Data1T,Data2T,HandleT,ObjT>(from,obj,func,def1,def2); 
  }
  //! userlevel=Normal
  //: Connect a signal to a method.
  
  template<class Data1T,class Data2T,class ObjT>
  inline
  SignalConnectorC ConnectRef(Signal0C &from,ObjT &obj,bool (ObjT::* func)(Data1T,Data2T),
			      const typename TraitsC<Data1T>::BaseTypeT &def1 = VCPPARGFIX(typename) TraitsC<Data1T>::BaseTypeT(),
			      const typename TraitsC<Data2T>::BaseTypeT &def2 = VCPPARGFIX(typename) TraitsC<Data2T>::BaseTypeT()) {
    RavlAssertMsg(from.IsValid(),"Source signal not valid.");
#if RAVL_COMPILER_VISUALCPP 
    return Signal2MethodRefC<Data1T,Data2T,ObjT>(from,obj,func,def1,def2); 
#else
    return Signal2MethodC<Data1T,Data2T,ObjT &>(from,obj,func,def1,def2); 
#endif
  }
  //! userlevel=Normal
  //: Connect to a method.
  // Uses reference to object not instance. <p>
  // NB. It is the users responsibility to ensure the object
  // remains valid while being used.

  template<class Data1T,class Data2T,class ObjT>
  inline
  SignalConnectorC ConnectR(Signal0C &from,ObjT &obj,bool (ObjT::* func)(Data1T,Data2T),
			    const typename TraitsC<Data1T>::BaseTypeT &def1 = VCPPARGFIX(typename) TraitsC<Data1T>::BaseTypeT(),
			    const typename TraitsC<Data2T>::BaseTypeT &def2 = VCPPARGFIX(typename) TraitsC<Data2T>::BaseTypeT()) {
    RavlAssertMsg(from.IsValid(),"Source signal not valid.");
#if RAVL_COMPILER_VISUALCPP 
    return Signal2MethodRefC<Data1T,Data2T,ObjT>(from,obj,func,def1,def2); 
#else
    return Signal2MethodC<Data1T,Data2T,ObjT &>(from,obj,func,def1,def2); 
#endif
  }
  //! userlevel=Normal
  //: Connect to a method.
  // Uses reference to object not instance. <p>
  // NB. It is the users responsibility to ensure the object
  // remains valid while being used.

};

#undef VCPPARGFIX
#endif
