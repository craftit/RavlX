// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_SIGNAL3_HEADER
#define RAVL_SIGNAL3_HEADER 1
/////////////////////////////////////////////////////
//! docentry="Ravl.API.OS.Signals"
//! rcsid="$Id$"
//! file="Ravl/OS/Threads/Tools/Signal3.hh"
//! lib=RavlThreads
//! author="Charles Galambos"
//! date="23/09/1999"
//! example=exSignal2.cc

// FIXME:- Sort out some proper examples for Signal3C

#include "Ravl/Threads/Signal2.hh"
#include "Ravl/IntrDLIter.hh"

// Fix for Visual C++ which doesn't like default values for
// some templated arguments. 
#if !RAVL_COMPILER_VISUALCPP && !RAVL_COMPILER_VISUALCPPNET
#define VCPPARGFIX(x) x
#else
#define VCPPARGFIX(x)
#endif

namespace RavlN {

  template<class Data1T,class Data2T,class Data3T> class Signal3C;
  
  ////////////////////////////////////////////////////
  //! userlevel=Develop
  //: Signal 1 connector.
  
  template<class Data1T,class Data2T,class Data3T>
  class SignalConnector3BodyC
    : virtual public SignalConnector2BodyC<Data1T,Data2T>
  {
  public:
    inline SignalConnector3BodyC(const Data1T &def1,const Data2T &def2,const Data3T &def3)
      : SignalConnector1BodyC<Data1T>(def1),
	SignalConnector2BodyC<Data1T,Data2T>(def1,def2),
	defaultVal3(def3)
    {}
    //: Default constructor.
    
    inline SignalConnector3BodyC()
    {}
    //: Constructor.
    
    virtual bool Invoke(Data1T &,Data2T &,Data3T &) = 0;
    //: Pass signal on.
    
    virtual bool Invoke(Data1T &d1,Data2T &d2) 
    { return Invoke(d1,d2,defaultVal3); }
    //: Pass signal on.
    
    virtual bool Invoke(Data1T &d1)
    { return Invoke(d1,this->defaultVal2,defaultVal3); }
    //: Pass signal on, use default value.
    
    virtual bool Invoke()
    { return Invoke(this->defaultVal,this->defaultVal2,defaultVal3); }
    //: Pass signal on, use default value.
    
  protected:
    Data3T defaultVal3;
  };
  
  //! userlevel=Develop
  //: Signal 3 inter connector.
  
  template<class Data1T,class Data2T,class Data3T>
  class SignalInterConnect3BodyC
    : virtual public SignalConnector3BodyC<Data1T,Data2T,Data3T>,
      virtual public SignalInterConnect0BodyC
  {
  public:
    SignalInterConnect3BodyC()
    {}
    //: Constructor.
    
    inline virtual bool Invoke(Data1T &,Data2T &,Data3T &);
    //: Invoke signal, with value.
    
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
  
  template<class Data1T,class Data2T,class Data3T>
  class SignalInterConnect3C
    : public SignalInterConnect0C
  {
  public:
    SignalInterConnect3C(Signal0C &from,Signal3C<Data1T,Data2T,Data3T> &targ)
      : SignalInterConnect0C(*new SignalInterConnect3BodyC<Data1T,Data2T,Data3T>())
    { Body().Connect(from,targ); }
    //: Constructor.
  };
  
  ///////////////////////////////////////////////////
  //! userlevel=Develop
  //: Signal 1 function connector.

  template<class Data1T,class Data2T,class Data3T>
  class Signal3FuncBodyC
  : public SignalConnector3BodyC<typename TraitsC<Data1T>::BaseTypeT,typename TraitsC<Data2T>::BaseTypeT,typename TraitsC<Data3T>::BaseTypeT>
  {
  public:
    typedef typename TraitsC<Data1T>::BaseTypeT Arg1T; //: Type of arguments without const's and refs.
    typedef typename TraitsC<Data2T>::BaseTypeT Arg2T; //: Type of arguments without const's and refs.
    typedef typename TraitsC<Data3T>::BaseTypeT Arg3T; //: Type of arguments without const's and refs.
    typedef bool (*Func3T)(Data1T,Data2T,Data3T);
    
    Signal3FuncBodyC(Func3T nFunc,const Arg1T &def1,const Arg2T &def2,const Arg3T &def3)
      : SignalConnector1BodyC<typename TraitsC<Data1T>::BaseTypeT>(def1),
	SignalConnector2BodyC<typename TraitsC<Data1T>::BaseTypeT,typename TraitsC<Data2T>::BaseTypeT>(def1,def2),
	SignalConnector3BodyC<typename TraitsC<Data1T>::BaseTypeT,typename TraitsC<Data2T>::BaseTypeT,typename TraitsC<Data3T>::BaseTypeT>(def1,def2,def3),
	func(nFunc)
    {}
    //: Constructor.
    
    virtual bool Invoke()
    { return func(this->defaultVal,this->defaultVal2,this->defaultVal3); }
    //: Call function.
    // Use default value.
    
    virtual bool Invoke(Arg1T &val)
    { return func(val,this->defaultVal2,this->defaultVal3); }
    //: Call function.
    
    virtual bool Invoke(Arg1T &val1,Arg2T &val2)
    { return func(val1,val2,this->defaultVal3); }
    //: Call function.
    
    virtual bool Invoke(Arg1T &val1,Arg2T &val2,Arg3T &val3)
    { return func(val1,val2,val3); }
    //: Call function.
    
  protected:
    Func3T func;
  };
  
  //! userlevel=Advanced
  //: Signal a function
  
  template<class Data1T,class Data2T,class Data3T>
  class Signal3FuncC 
    : public SignalConnectorC
  {
  public:
    typedef typename TraitsC<Data1T>::BaseTypeT Arg1T; //: Type of arguments without const's and refs.
    typedef typename TraitsC<Data2T>::BaseTypeT Arg2T; //: Type of arguments without const's and refs.
    typedef typename TraitsC<Data3T>::BaseTypeT Arg3T; //: Type of arguments without const's and refs.
    
    Signal3FuncC(Signal0C &from,
		 typename Signal3FuncBodyC<Data1T,Data2T,Data3T>::Func3T nFunc,
		 const Arg1T &def1 = Arg1T(),
		 const Arg2T &def2 = Arg2T(),
		 const Arg3T &def3 = Arg3T())
      : SignalConnectorC(*new Signal3FuncBodyC<Data1T,Data2T,Data3T>(nFunc,def1,def2,def3))
    { Body().Connect(from); }
    //: Constructor.
  };

  ///////////////////////////////////////////////////
  //! userlevel=Develop
  //: Signal 3 method connector.
  
  template<class Data1T,class Data2T,class Data3T,class ObjT>
  class Signal3MethodBodyC
    : public SignalConnector3BodyC<typename TraitsC<Data1T>::BaseTypeT,typename TraitsC<Data2T>::BaseTypeT,typename TraitsC<Data3T>::BaseTypeT>
  {
  public:
    typedef typename TraitsC<ObjT>::BaseTypeT BaseObjT; //: Type of object without const's and refs.
    typedef typename TraitsC<Data1T>::BaseTypeT Arg1T; //: Type of arguments without const's and refs.
    typedef typename TraitsC<Data2T>::BaseTypeT Arg2T; //: Type of arguments without const's and refs.
    typedef typename TraitsC<Data3T>::BaseTypeT Arg3T; //: Type of arguments without const's and refs.
#if !RAVL_COMPILER_VISUALCPP 
    typedef bool (BaseObjT::*Func3T)(Data1T,Data2T,Data3T);
#else
    typedef bool (ObjT::*Func3T)(Data1T,Data2T,Data3T);
#endif
    
    Signal3MethodBodyC(BaseObjT &nobj,
		       Func3T nFunc,
		       const Arg1T &dat1 = Arg1T(),
		       const Arg2T &dat2 = Arg2T(),
		       const Arg3T &dat3 = Arg3T())
      : SignalConnector1BodyC<typename TraitsC<Data1T>::BaseTypeT>(dat1),
	SignalConnector2BodyC<typename TraitsC<Data1T>::BaseTypeT,typename TraitsC<Data2T>::BaseTypeT>(dat1,dat2),
	SignalConnector3BodyC<typename TraitsC<Data1T>::BaseTypeT,typename TraitsC<Data2T>::BaseTypeT,typename TraitsC<Data3T>::BaseTypeT>(dat1,dat2,dat3),
	obj(nobj),
	func(nFunc)
    {}
    //: Constructor.
    
    virtual bool Invoke()
    { return (obj.*func)(this->defaultVal,this->defaultVal2,this->defaultVal3); }
    //: Call function.
    
    virtual bool Invoke(Arg1T &val)
    { return (obj.*func)(val,this->defaultVal2,this->defaultVal3); }
    //: Call function.
    
    virtual bool Invoke(Arg1T &val1,Arg2T &val2)
    { return (obj.*func)(val1,val2,this->defaultVal3); }
    //: Call function.
    
    virtual bool Invoke(Arg1T &val1,Arg2T &val2,Arg3T &val3)
    { return (obj.*func)(val1,val2,val3); }
    //: Call function.
    
  protected:
    ObjT obj;
    Func3T func;
  };
  
  //! userlevel=Advanced
  //: Signal a method.
  
  template<class Data1T,class Data2T,class Data3T,class ObjT>
  class Signal3MethodC
    : public SignalConnectorC
  {
  public:
    typedef typename TraitsC<ObjT>::BaseTypeT BaseObjT; //: Type of object without const's and refs.
    typedef typename TraitsC<Data1T>::BaseTypeT Arg1T; //: Type of arguments without const's and refs.
    typedef typename TraitsC<Data2T>::BaseTypeT Arg2T; //: Type of arguments without const's and refs.
    typedef typename TraitsC<Data3T>::BaseTypeT Arg3T; //: Type of arguments without const's and refs.
    
    Signal3MethodC(Signal0C &from,
		   BaseObjT &nobj,
		   typename Signal3MethodBodyC<Data1T,Data2T,Data3T,ObjT>::Func3T nFunc,
		   const Arg1T &dat1 = Arg1T(),
		   const Arg2T &dat2 = Arg2T(),
		   const Arg3T &dat3 = Arg3T())
      : SignalConnectorC(*new Signal3MethodBodyC<Data1T,Data2T,Data3T,ObjT>(nobj,nFunc,dat1,dat2,dat3))
    { Body().Connect(from); }
    //: Constructor.
  };







  ///////////////////////////////////////////////////
  //! userlevel=Develop
  //: Signal 3 method ptr connector.
  
  template<class Data1T,class Data2T,class Data3T,class HandleT,class ObjT>
  class Signal3MethodPtrBodyC
    : public SignalConnector3BodyC<typename TraitsC<Data1T>::BaseTypeT,typename TraitsC<Data2T>::BaseTypeT,typename TraitsC<Data3T>::BaseTypeT>
  {
  public:
    typedef typename TraitsC<ObjT>::BaseTypeT BaseObjT; //: Type of object without const's and refs.
    typedef typename TraitsC<Data1T>::BaseTypeT Arg1T; //: Type of arguments without const's and refs.
    typedef typename TraitsC<Data2T>::BaseTypeT Arg2T; //: Type of arguments without const's and refs.
    typedef typename TraitsC<Data3T>::BaseTypeT Arg3T; //: Type of arguments without const's and refs.
#if !RAVL_COMPILER_VISUALCPP 
    typedef bool (BaseObjT::*Func3T)(Data1T,Data2T,Data3T);
#else
    typedef bool (ObjT::*Func3T)(Data1T,Data2T,Data3T);
#endif
    
    Signal3MethodPtrBodyC(const HandleT &nobj,
			  Func3T nFunc,
			  const Arg1T &dat1 = Arg1T(),
			  const Arg2T &dat2 = Arg2T(),
			  const Arg3T &dat3 = Arg3T())
      : SignalConnector1BodyC<typename TraitsC<Data1T>::BaseTypeT>(dat1),
	SignalConnector2BodyC<typename TraitsC<Data1T>::BaseTypeT,typename TraitsC<Data2T>::BaseTypeT>(dat1,dat2),
	SignalConnector3BodyC<typename TraitsC<Data1T>::BaseTypeT,typename TraitsC<Data2T>::BaseTypeT,typename TraitsC<Data3T>::BaseTypeT>(dat1,dat2,dat3),
	obj(nobj),
	func(nFunc)
    {}
    //: Constructor.
    
    virtual bool Invoke()
    { return (obj->*func)(this->defaultVal,this->defaultVal2,this->defaultVal3); }
    //: Call function.
    
    virtual bool Invoke(Arg1T &val)
    { return (obj->*func)(val,this->defaultVal2,this->defaultVal3); }
    //: Call function.
    
    virtual bool Invoke(Arg1T &val1,Arg2T &val2)
    { return (obj->*func)(val1,val2,this->defaultVal3); }
    //: Call function.
    
    virtual bool Invoke(Arg1T &val1,Arg2T &val2,Arg3T &val3)
    { return (obj->*func)(val1,val2,val3); }
    //: Call function.
    
  protected:
    HandleT obj;
    Func3T func;
  };
  
  //! userlevel=Advanced
  //: Signal a method.
  
  template<class Data1T,class Data2T,class Data3T,class HandleT,class ObjT>
  class Signal3MethodPtrC
    : public SignalConnectorC
  {
  public:
    typedef typename TraitsC<ObjT>::BaseTypeT BaseObjT; //: Type of object without const's and refs.
    typedef typename TraitsC<Data1T>::BaseTypeT Arg1T; //: Type of arguments without const's and refs.
    typedef typename TraitsC<Data2T>::BaseTypeT Arg2T; //: Type of arguments without const's and refs.
    typedef typename TraitsC<Data3T>::BaseTypeT Arg3T; //: Type of arguments without const's and refs.
    
    Signal3MethodPtrC(Signal0C &from,
		      const HandleT &nobj,
		      typename Signal3MethodPtrBodyC<Data1T,Data2T,Data3T,HandleT,ObjT>::Func3T nFunc,
		      const Arg1T &dat1 = Arg1T(),
		      const Arg2T &dat2 = Arg2T(),
		      const Arg3T &dat3 = Arg3T())
      : SignalConnectorC(*new Signal3MethodPtrBodyC<Data1T,Data2T,Data3T,HandleT,ObjT>(nobj,nFunc,dat1,dat2,dat3))
    { Body().Connect(from); }
    //: Constructor.
  };
  
  
#if RAVL_COMPILER_VISUALCPP 
  
  ///////////////////////////////////////////////////
  //! userlevel=Develop
  //: Signal 1 method connector.
  
  template<class Data1T,class Data2T,class Data3T,class ObjT>
  class Signal3MethodRefBodyC
    : public SignalConnector3BodyC<typename TraitsC<Data1T>::BaseTypeT,typename TraitsC<Data2T>::BaseTypeT,typename TraitsC<Data3T>::BaseTypeT>
  {
  public:
    typedef typename TraitsC<ObjT>::BaseTypeT BaseObjT; //: Type of object without const's and refs.
    typedef typename TraitsC<Data1T>::BaseTypeT Arg1T; //: Type of arguments without const's and refs.
    typedef typename TraitsC<Data2T>::BaseTypeT Arg2T; //: Type of arguments without const's and refs.
    typedef typename TraitsC<Data3T>::BaseTypeT Arg3T; //: Type of arguments without const's and refs.
    typedef bool (ObjT::*Func3T)(Data1T,Data2T,Data3T);
    
    Signal3MethodRefBodyC(BaseObjT &nobj,
			  Func3T nFunc,
			  const Arg1T &dat1 = Arg1T(),
			  const Arg2T &dat2 = Arg2T(),
			  const Arg3T &dat3 = Arg3T())
      : SignalConnector1BodyC<typename TraitsC<Data1T>::BaseTypeT>(dat1),
	SignalConnector2BodyC<typename TraitsC<Data1T>::BaseTypeT,typename TraitsC<Data2T>::BaseTypeT>(dat1,dat2),
	SignalConnector3BodyC<typename TraitsC<Data1T>::BaseTypeT,typename TraitsC<Data2T>::BaseTypeT,typename TraitsC<Data3T>::BaseTypeT>(dat1,dat2,dat3),
	obj(nobj),
	func(nFunc)
    {}
    //: Constructor.
    
    virtual bool Invoke()
    { return (obj.*func)(defaultVal,defaultVal2,defaultVal3); }
    //: Call function.
    
    virtual bool Invoke(Arg1T &val)
    { return (obj.*func)(val,defaultVal2,defaultVal3); }
    //: Call function.
    
    virtual bool Invoke(Arg1T &val1,Arg2T &val2)
    { return (obj.*func)(val1,val2,defaultVal3); }
    //: Call function.
    
    virtual bool Invoke(Arg1T &val1,Arg2T &val2,Arg3T &val3)
    { return (obj.*func)(val1,val2,val3); }
    //: Call function.
    
  protected:
    ObjT &obj;
    Func3T func;
  };



  
  //! userlevel=Advanced
  //: Signal a method.
  
  template<class Data1T,class Data2T,class Data3T,class ObjT>
  class Signal3MethodRefC
    : public SignalConnectorC
  {
  public:
    typedef typename TraitsC<ObjT>::BaseTypeT BaseObjT; //: Type of object without const's and refs.
    typedef typename TraitsC<Data1T>::BaseTypeT Arg1T; //: Type of arguments without const's and refs.
    typedef typename TraitsC<Data2T>::BaseTypeT Arg2T; //: Type of arguments without const's and refs.
    typedef typename TraitsC<Data3T>::BaseTypeT Arg3T; //: Type of arguments without const's and refs.
    
    Signal3MethodRefC(Signal0C &from,
		      BaseObjT &nobj,
		      typename Signal3MethodBodyC<Data1T,Data2T,Data3T,ObjT>::Func3T nFunc,
		      const Arg1T &dat1 = Arg1T(),
		      const Arg2T &dat2 = Arg2T(),
		      const Arg3T &dat3 = Arg3T())
      : SignalConnectorC(*new Signal3MethodRefBodyC<Data1T,Data2T,Data3T,ObjT>(nobj,nFunc,dat1,dat2,dat3))
    { Body().Connect(from); }
    //: Constructor.
  };
  
#endif
  ////////////////////////////////////////////////////////////////
  
  //! userlevel=Develop
  //: Signal 1 body.
  
  template<class Data1T,class Data2T,class Data3T>
  class Signal3BodyC
    : public Signal2BodyC<Data1T,Data2T>
  {
  public:
    Signal3BodyC(const Data1T &def1,const Data2T &def2,const Data3T &def3)
      : Signal2BodyC<Data1T,Data2T>(def1,def2), 
	defaultVal3(def3)
    {}
    //: Constructor.
    
    inline 
    virtual bool Invoke(Data1T &v1,Data2T &v2,Data3T &v3) {
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
	SignalConnector3BodyC<Data1T,Data2T,Data3T> *sc3 = dynamic_cast<SignalConnector3BodyC<Data1T,Data2T,Data3T> *>(&it.Data().Body());
	if(sc3 != 0) {
	  ret &= sc3->Invoke(v1,v2,v3);
	  continue;
	}
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
    
    inline 
    virtual bool Invoke(Data1T &v1,Data2T &v2)
    { return Signal3BodyC<Data1T,Data2T,Data3T>::Invoke(v1,v2,this->defaultVal3); }
    //: Send signal with value.
    
    inline 
    virtual bool Invoke(Data1T &v1)
    { return Signal3BodyC<Data1T,Data2T,Data3T>::Invoke(v1,this->defaultVal2,this->defaultVal3); }
    //: Send signal with value.
    
    virtual bool Invoke()
    { return Signal3BodyC<Data1T,Data2T,Data3T>::Invoke(this->defaultVal,this->defaultVal2,this->defaultVal3); }
    //: Send signal with default value where needed.
    
    Data3T &DefaultValue3()
    { return defaultVal3; }
    //: Access default value for argument 3.
    
    const Data3T &DefaultValue3() const
    { return defaultVal3; }
    //: Access default value for argument 3.
    
  protected:
    Data3T defaultVal3; // Default data value.
  };
  
  //! userlevel=Normal
  //: Signal with 3 arguments.
  
  template<class Data1T,class Data2T,class Data3T>
  class Signal3C
    : public Signal2C<Data1T,Data2T>
  {
  public:
    typedef void (*Func3T)(Data1T &dat1,Data2T &dat2,Data3T &dat3);
    
    Signal3C()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    Signal3C(const Data1T &def1,const Data2T &def2 = Data2T(),const Data3T &def3 = Data3T())
      : Signal2C<Data1T,Data2T>(*new Signal3BodyC<Data1T,Data2T,Data3T>(def1,def2,def3))
    {}
    //: Default constructor.
    // Creates an invalid handle.

    Signal3C(const Signal0C &base)
		: Signal2C<Data1T,Data2T>(dynamic_cast<const Signal3BodyC<Data1T,Data2T,Data3T> *>(RCLayerC<Signal0BodyC>::BodyPtr(base)))
    {}
    //: Base constructor.
    // Creates an invalid handle if body type
    // is correct.
    
  protected:
    Signal3C(Signal3BodyC<Data1T,Data2T,Data3T> &sig)
      : Signal2C<Data1T,Data2T>(sig)
    {}
    //: Body constructor.
    
    explicit Signal3C(const Signal3BodyC<Data1T,Data2T,Data3T> *sig)
      : Signal2C<Data1T,Data2T>(sig)
    {}
    //: Body constructor.
    
    inline 
    Signal3BodyC<Data1T,Data2T,Data3T> &Body() 
    { return static_cast<Signal3BodyC<Data1T,Data2T,Data3T> &>(Signal0C::Body()); }
    //: Access body.
    
    inline 
    const Signal3BodyC<Data1T,Data2T,Data3T> &Body() const 
    { return static_cast<const Signal3BodyC<Data1T,Data2T,Data3T> &>(Signal0C::Body()); }
    //: Access body.
    
  public:
    inline bool Invoke(Data1T &dat1,Data2T &dat2,Data3T &dat3)
    { return Body().Invoke(dat1,dat2,dat3); }
    //: Send default signal.
    
    inline bool operator()(Data1T &dat1,Data2T &dat2,Data3T &dat3)
    { return Body().Invoke(dat1,dat2,dat3); }
    //: Simple invokation.
    
    inline bool operator()(const Data1T &dat1,const Data2T &dat2,const Data3T &dat3) { 
      Data1T tmp1(dat1);
      Data2T tmp2(dat2);
      Data3T tmp3(dat3);
      return Body().Invoke(tmp1,tmp2,tmp3); 
    }
    //: Simple invocation. 
    
    Data3T &DefaultValue3()
    { return Body().DefaultValue3(); }
    //: Access default value for argument 3.
    
    const Data3T &DefaultValue3() const
    { return Body().DefaultValue3(); }
    //: Access default value for argument 3.
  };
  
  template<class Data1T,class Data2T,class Data3T>
  std::ostream &operator<<(std::ostream &out,const Signal3C<Data1T,Data2T,Data3T> &) { 
    RavlAssert(0); // Not implemented.
    return out;
  }
  //! userlevel=Normal
  //: IO Operator.
  // Not implemented
  
  ////////////////////////////
  
  template<class Data1T,class Data2T,class Data3T>
  inline 
  bool SignalInterConnect3BodyC<Data1T,Data2T,Data3T>::Invoke(Data1T &dat1,Data2T &dat2,Data3T &dat3)
  { 
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
    return dynamic_cast<Signal3BodyC<Data1T,Data2T,Data3T> &>(*tmp).Invoke(dat1,dat2,dat3); 
  }
  
  template<class Data1T,class Data2T,class Data3T>
  inline 
  bool SignalInterConnect3BodyC<Data1T,Data2T,Data3T>::Invoke(Data1T &dat1,Data2T &dat2)
  { return Invoke(dat1,dat2,this->defaultVal3); }
  
  template<class Data1T,class Data2T,class Data3T>
  inline
  bool SignalInterConnect3BodyC<Data1T,Data2T,Data3T>::Invoke(Data1T &dat1)
  { return Invoke(dat1,this->defaultVal2,this->defaultVal3); }
  
  template<class Data1T,class Data2T,class Data3T>
  inline
  bool SignalInterConnect3BodyC<Data1T,Data2T,Data3T>::Invoke()
  { return Invoke(this->defaultVal,this->defaultVal2,this->defaultVal3); }
  
  ///////////////////////
  
  template<class Data1T,class Data2T,class Data3T>
  inline 
  SignalConnectorC Connect(Signal0C &from,Signal3C<Data1T,Data2T,Data3T> &oth) { 
    RavlAssert(from.IsValid());
    RavlAssert(oth.IsValid());
    return SignalInterConnect3C<Data1T,Data2T,Data3T>(from,oth);
  }
  //! userlevel=Normal
  //: Connect to a signal
  
  template<class Data1T,class Data2T,class Data3T>  
  inline 
  SignalConnectorC Connect(Signal0C &from,bool (*func)(Data1T,Data2T,Data3T),
			   const typename TraitsC<Data1T>::BaseTypeT &def1 = VCPPARGFIX(typename) TraitsC<Data1T>::BaseTypeT(),
			   const typename TraitsC<Data2T>::BaseTypeT &def2 = VCPPARGFIX(typename) TraitsC<Data2T>::BaseTypeT(),
			   const typename TraitsC<Data3T>::BaseTypeT &def3 = VCPPARGFIX(typename) TraitsC<Data3T>::BaseTypeT()
			   ) { 
    RavlAssert(from.IsValid());
    return Signal3FuncC<Data1T,Data2T,Data3T>(from,func,def1,def2,def3);
  }
  //! userlevel=Normal
  //: Connect a signal to a function.

  template<class Data1T,class Data2T,class Data3T,class ObjT>
  inline
  SignalConnectorC Connect(Signal0C &from,const ObjT &obj,bool (ObjT::* func)(Data1T,Data2T,Data3T),
			   const typename TraitsC<Data1T>::BaseTypeT &def1 = VCPPARGFIX(typename) TraitsC<Data1T>::BaseTypeT(),
			   const typename TraitsC<Data2T>::BaseTypeT &def2 = VCPPARGFIX(typename) TraitsC<Data2T>::BaseTypeT(),
			   const typename TraitsC<Data3T>::BaseTypeT &def3 = VCPPARGFIX(typename) TraitsC<Data3T>::BaseTypeT()
			   ) {
    RavlAssert(from.IsValid());
    return Signal3MethodC<Data1T,Data2T,Data3T,ObjT>(from,const_cast<ObjT &>(obj),func,def1,def2,def3);
  }
  //! userlevel=Normal
  //: Connect a signal to a method.

  template<class Data1T,class Data2T,class Data3T,class HandleT,class ObjT>
  inline
  SignalConnectorC ConnectPtr(Signal0C &from,const HandleT &obj,bool (ObjT::* func)(Data1T,Data2T,Data3T),
			      const typename TraitsC<Data1T>::BaseTypeT &def1 = VCPPARGFIX(typename) TraitsC<Data1T>::BaseTypeT(),
			      const typename TraitsC<Data2T>::BaseTypeT &def2 = VCPPARGFIX(typename) TraitsC<Data2T>::BaseTypeT(),
			      const typename TraitsC<Data3T>::BaseTypeT &def3 = VCPPARGFIX(typename) TraitsC<Data3T>::BaseTypeT()
			      ) {
    RavlAssert(from.IsValid());
    return Signal3MethodPtrC<Data1T,Data2T,Data3T,HandleT,ObjT>(from,obj,func,def1,def2,def3);
  }
  //! userlevel=Normal
  //: Connect a signal to a method.

  template<class Data1T,class Data2T,class Data3T,class ObjT>
  inline
  SignalConnectorC ConnectRef(Signal0C &from,ObjT &obj,bool (ObjT::* func)(Data1T,Data2T,Data3T),
			      const typename TraitsC<Data1T>::BaseTypeT &def1 = VCPPARGFIX(typename) TraitsC<Data1T>::BaseTypeT(),
			      const typename TraitsC<Data2T>::BaseTypeT &def2 = VCPPARGFIX(typename) TraitsC<Data2T>::BaseTypeT(),
			      const typename TraitsC<Data3T>::BaseTypeT &def3 = VCPPARGFIX(typename) TraitsC<Data3T>::BaseTypeT()
			      ) {
    RavlAssert(from.IsValid());
#if RAVL_COMPILER_VISUALCPP 
    return Signal3MethodRefC<Data1T,Data2T,Data3T,ObjT>(from,obj,func,def1,def2,def3);
#else
    return Signal3MethodC<Data1T,Data2T,Data3T,ObjT &>(from,obj,func,def1,def2,def3);
#endif
  }
  //! userlevel=Normal
  //: Connect a signal to a method.
  // Uses reference to object not instance. <p>
  // NB. It is the users responsibility to ensure the object
  // remains valid while being used.

  template<class Data1T,class Data2T,class Data3T,class ObjT>
  inline
  SignalConnectorC ConnectR(Signal0C &from,ObjT &obj,bool (ObjT::* func)(Data1T,Data2T,Data3T),
			    const typename TraitsC<Data1T>::BaseTypeT &def1 = VCPPARGFIX(typename) TraitsC<Data1T>::BaseTypeT(),
			    const typename TraitsC<Data2T>::BaseTypeT &def2 = VCPPARGFIX(typename) TraitsC<Data2T>::BaseTypeT(),
			    const typename TraitsC<Data3T>::BaseTypeT &def3 = VCPPARGFIX(typename) TraitsC<Data3T>::BaseTypeT()
			    ) {
    RavlAssert(from.IsValid());
#if RAVL_COMPILER_VISUALCPP 
    return Signal3MethodRefC<Data1T,Data2T,Data3T,ObjT>(from,obj,func,def1,def2,def3);
#else
    return Signal3MethodC<Data1T,Data2T,Data3T,ObjT &>(from,obj,func,def1,def2,def3);
#endif
  }
  //! userlevel=Normal
  //: Connect a signal to a method.
  // Uses reference to object not instance. <p>
  // NB. It is the users responsibility to ensure the object
  // remains valid while being used.

};

#undef VCPPARGFIX
#endif
