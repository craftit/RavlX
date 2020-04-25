// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_CALLS_HEADER
#define RAVL_CALLS_HEADER 1
//////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Core/Base/Calls.hh"
//! lib=RavlCore
//! docentry="Ravl.API.Core.Calls"
//! userlevel=Advanced
//! author="Charles Galambos"
//! date="24/01/2001"

#include "Ravl/Trigger.hh"
#include "Ravl/Assert.hh"
#include "Ravl/Traits.hh"

namespace RavlN {
  
  //////////////////////////////////////////////////////
  
  //! userlevel=Develop
  //: Body for CallFunc0C
  
  template<class RetT = bool>
  class CallFunc0BodyC
    : public TriggerBodyC
  {
  public:
    
    CallFunc0BodyC(VoidFuncPtrT nfunc)
      : func(nfunc)
    {}
    //: Constructor.
    
    CallFunc0BodyC(bool issueErrorOnCall) {
      if(issueErrorOnCall)
	func = (VoidFuncPtrT) &CallFunc0BodyC<RetT>::IssueError;
      else
	func = (VoidFuncPtrT) &CallFunc0BodyC<RetT>::NoOp;
    }
    //: Constructor.
    // if issueErrorOnCall a function which causes the
    // equivelent of a assertion failure is called. Otherwise
    // the call returns silently.
        
    virtual void Invoke()
    { (*FuncPtr())(); }
    //: Invoke event.

    virtual RetT Call()
    { return (*FuncPtr())(); }
    //: Call signal
    
    RetT operator()()
    { return Call(); }
    
    virtual RCBodyVC &Copy() const
    { return *new CallFunc0BodyC<RetT>(func); }
    //: Copy call.
    
  protected:
    VoidFuncPtrT func; // Function ptr.
    
    typedef RetT (*FuncT)();
    //: Function ptr type.
    
    static RetT NoOp() { 
      return RetT(); 
    }
    //: NoOp function.
    
    static RetT IssueError() { 
      RavlN::IssueError(__FILE__,__LINE__,"NoOp CallFunc0 Called. ");
      return RetT(); 
    }
    //: Error function.
    
    inline
    FuncT FuncPtr() const
    { return (FuncT)(func); }
    //: Function.
    
  };
  
  //! userlevel=Advanced
  //: Call function with no arguments
  
  template<class RetT = bool>
  class CallFunc0C
    : public TriggerC
  {
  public:
    CallFunc0C()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    CallFunc0C(const TriggerC &trig,bool invalidOk = false)
      : TriggerC(dynamic_cast<const CallFunc0BodyC<RetT> *>(RCHandleC<TriggerBodyC>::BodyPtr(trig)))
    { RavlAlwaysAssertMsg(invalidOk || RCHandleC<TriggerBodyC>::IsValid(), "Casting to incorrect function type. "); }
    //: Convert from a base handle.
    // If the base handle is of another type, the invaldOk flag is true, 
    // an invalid handle is generated, otherwise an assertion failure is 
    // generated.
    
    CallFunc0C(bool issueErrorOnCall)
      : TriggerC(*new CallFunc0BodyC<RetT>(issueErrorOnCall))
    {}
    //: NoOp constructor.
    // if issueErrorOnCall a function which causes the
    // equivelent of a assertion failure is called. Otherwise
    // the call returns silently.
    
    CallFunc0C(RetT (*nfunc)())
      : TriggerC(*new CallFunc0BodyC<RetT>((VoidFuncPtrT) nfunc))
    {}
    //:  Construct from a function pointer
    // Creates an invalid handle.
    
  protected:
    CallFunc0C(CallFunc0BodyC<RetT> &bod)
      : TriggerC(bod)
    {}
    // Body constructor.

    CallFunc0C(const CallFunc0BodyC<RetT> *bod)
      : TriggerC(bod)
    {}
    // Body constructor.
    
    CallFunc0BodyC<RetT> &Body()
    { return static_cast<CallFunc0BodyC<RetT> &>(TriggerC::Body()); }
    //: Access body.

    const CallFunc0BodyC<RetT> &Body() const
    { return static_cast<const CallFunc0BodyC<RetT> &>(TriggerC::Body()); }
    //: Constant access to body.

    CallFunc0C<RetT> Copy() const
    { return CallFunc0C<RetT>(static_cast<CallFunc0BodyC<RetT> &>(Body().Copy())); }
    //: Copy func call.

  public:
    RetT Call()
    { return Body().Call(); }
    //: Call function.
    
    RetT operator()()
    { return Body().Call(); }
    //: Call function.
    
  };

  //////////////////////////////////////////////////////
  
  //! userlevel=Develop
  //: Body for CallFunc1C
  
  template<typename DataT,class RetT = bool>
  class CallFunc1BodyC
    : public CallFunc0BodyC<RetT>
  {
  public:
    typedef typename TraitsC<DataT>::BaseTypeT Arg1T; //: Basic type of arg, ignore references and const's
    
    CallFunc1BodyC() 
      : CallFunc0BodyC<RetT>((VoidFuncPtrT)&CallFunc1BodyC<DataT,RetT>::IssueError)
    {}
    //: Default constructor.
    
    CallFunc1BodyC(RetT (*nfunc)(DataT),const Arg1T &ndat)
      : CallFunc0BodyC<RetT>((VoidFuncPtrT) nfunc),
	dat1(ndat)
    {}
    //: Constructor.
    
    CallFunc1BodyC(VoidFuncPtrT voidFuncPtr)
      : CallFunc0BodyC<RetT>(voidFuncPtr)
    {}
    //: void function ptr constructor.

    CallFunc1BodyC(bool issueErrorOnCall) 
      : CallFunc0BodyC<RetT>(issueErrorOnCall 
			     ? ((VoidFuncPtrT) &CallFunc1BodyC<DataT,RetT>::IssueError) 
			     : ((VoidFuncPtrT) &CallFunc1BodyC<DataT,RetT>::NoOp))
    {}
    //: Constructor.
    // if issueErrorOnCall a function which causes the
    // equivelent of a assertion failure is called. Otherwise
    // the call returns silently.
    
    virtual void Invoke()
    { (*FuncPtr())(dat1); }
    //: Invoke event.
    
    virtual RetT Call()
    { return (*FuncPtr())(dat1); }
    //: Call an function.
    
    virtual RetT Call(Arg1T &pd)
    { return (*FuncPtr())(pd); }
    //: Call event, with parameter.
    
    Arg1T &Data1()
    { return dat1; }
    //: Access data.
    
    const Arg1T &Data1() const
    { return dat1; }
    //: Access data.

    virtual RCBodyVC &Copy() const
    { return *new CallFunc1BodyC<DataT,RetT>(this->func,this->dat1); }
    //: Copy call.
    
    static RetT NoOp(DataT) 
    { return RetT(); }
    //: NoOp function.
    
    static RetT IssueError(DataT) { 
      RavlN::IssueError(__FILE__,__LINE__,"NoOp CallFunc1 Called. ");
      return RetT(); 
    }
    //: Error function.
    
  protected:
    CallFunc1BodyC(void (*nfunc)(),const Arg1T &ndat)
      : CallFunc0BodyC<RetT>(nfunc),
	dat1(ndat)
    {}
    //: Constructor.
    
    typedef RetT (*FuncT)(DataT);
    //: Function ptr type.    
    
    inline
    FuncT FuncPtr() const
    { return (FuncT)(this->func); }
    //: Function.
    
    Arg1T dat1;
  };
  
  //! userlevel=Advanced
  //: Call function with 1 argument
  
  template<class DataT,class RetT = bool>
  class CallFunc1C
    : public CallFunc0C<RetT>
  {
  public:
    typedef typename TraitsC<DataT>::BaseTypeT Arg1T;
    //: Basic type of arg, ignore references and const's
    
    CallFunc1C()
    {}
    //: Default constructor.
    // Creates an invalid handle.

    CallFunc1C(const TriggerC &trig,bool invalidOk = false)
      : CallFunc0C<RetT>(dynamic_cast<const CallFunc1BodyC<DataT,RetT> *>(RCHandleC<TriggerBodyC>::BodyPtr(trig)))
    { RavlAlwaysAssertMsg(invalidOk || RCHandleC<TriggerBodyC>::IsValid(),"Casting to incorrect function type. "); }
    //: Convert from a base handle.
    // If the base handle is of another type, the invaldOk flag is true, 
    // an invalid handle is generated, otherwise an assertion failure is 
    // generated.
    
    CallFunc1C(bool issueErrorOnCall)
      : CallFunc0C<RetT>(*new CallFunc1BodyC<DataT,RetT>(issueErrorOnCall))
    {}
    //: NoOp constructor.
    // if issueErrorOnCall a function which causes the
    // equivalent of a assertion failure is called. Otherwise
    // the call returns silently.
    
    CallFunc1C(RetT (*nfunc)(DataT),const Arg1T &dat = Arg1T())
      : CallFunc0C<RetT>(*new CallFunc1BodyC<DataT,RetT>(nfunc,dat))
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    void Invalidate()
    { RCHandleC<TriggerBodyC>::Invalidate(); }
    //: Invalidate handle
    
  protected:
    CallFunc1C(CallFunc1BodyC<DataT,RetT> &body)
      : CallFunc0C<RetT>(body)
    {}
    //: Body constructor.

    CallFunc1C(const CallFunc1BodyC<DataT,RetT> *body)
      : CallFunc0C<RetT>(body)
    {}
    //: Body constructor.
    
    CallFunc1BodyC<DataT,RetT> &Body()
    { return static_cast<CallFunc1BodyC<DataT,RetT> &>(TriggerC::Body()); }
    //: Access body.
    
    const CallFunc1BodyC<DataT,RetT> &Body() const
    { return static_cast<const CallFunc1BodyC<DataT,RetT> &>(TriggerC::Body()); }
    //: Constant access to body.
    
  public:
    RetT Call()
    { return Body().Call(); }
    //: Invoke event, with parameter.
    
    RetT Call(Arg1T &pd)
    { return Body().Call(pd); }
    //: Invoke event, with parameter.
    
    RetT operator()()
    { return Body().Call(); }
    //: Call function.
    
    RetT operator()(Arg1T &pd)
    { return Body().Call(pd); }
    //: Call function with parameter
    
    CallFunc1C<DataT,RetT> Copy() const
    { return CallFunc1C<DataT,RetT>(static_cast<CallFunc1BodyC<DataT,RetT> &>(Body().Copy())); }
    //: Copy func call.
    
    Arg1T &Data1()
    { return Body().Data1(); }
    //: Access data.
    
    const Arg1T &Data1() const
    { return Body().Data1(); }
    //: Access data.
    
  };

  //////////////////////////////////////////////////////
  
  //! userlevel=Develop
  //: Body for CallFunc2C
  
  template<class Data1T,class Data2T,class RetT = bool>
  class CallFunc2BodyC
    : public CallFunc1BodyC<Data1T,RetT>
  {
  public:
    typedef typename TraitsC<Data1T>::BaseTypeT Arg1T;  //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data2T>::BaseTypeT Arg2T;  //: Basic type of arg, ignore references and const's
    
    CallFunc2BodyC() 
      : CallFunc1BodyC<Data1T,RetT>((VoidFuncPtrT)&CallFunc2BodyC<Data1T,Data2T,RetT>::IssueError)
    {}
    //: Default constructor.
    
    CallFunc2BodyC(RetT (*nfunc)(Data1T,Data2T),const Arg1T &ndat1,const Arg2T &ndat2)
      : CallFunc1BodyC<Data1T,RetT>((VoidFuncPtrT)nfunc,ndat1),
	dat2(ndat2)
    {}
    //: Constructor.
    
    CallFunc2BodyC(VoidFuncPtrT voidFuncPtr)
      : CallFunc1BodyC<Data1T,RetT>(voidFuncPtr)
    {}
    //: void function ptr constructor.
    
    CallFunc2BodyC(bool issueErrorOnCall) {
      if(issueErrorOnCall)
	this->func = (VoidFuncPtrT) &CallFunc2BodyC<Data1T,Data2T,RetT>::IssueError;
      else
	this->func = (VoidFuncPtrT) &CallFunc2BodyC<Data1T,Data2T,RetT>::NoOp;
    }
    //: Constructor.
    // if issueErrorOnCall a function which causes the
    // equivelent of a assertion failure is called. Otherwise
    // the call returns silently.
    
    virtual void Invoke()
    { (*FuncPtr())(this->dat1,dat2); }
    //: Invoke event.

    virtual RetT Call()
    { return (*FuncPtr())(this->dat1,dat2); }
    //: Invoke event.
    
    virtual RetT Call(Arg1T &pd)
    { return (*FuncPtr())(pd,dat2); }
    //: Invoke event, with parameter.
    
    virtual RetT Call(Arg1T &pd1,Arg2T &pd2)
    { return (*FuncPtr())(pd1,pd2); }
    //: Invoke event, with parameters.
    
    Arg2T &Data2()
    { return dat2; }
    //: Access data.
    
    const Arg2T &Data2() const
    { return dat2; }
    //: Access data.

    virtual RCBodyVC &Copy() const
    { return *new CallFunc2BodyC<Data1T,Data2T,RetT>(this->func,this->dat1,dat2); }
    //: Copy call.
    
  protected:
    CallFunc2BodyC(void (*nfunc)(),const Arg1T &ndat1,const Arg2T &ndat2)
      : CallFunc1BodyC<Data1T,RetT>(nfunc,ndat1),
	dat2(ndat2)
    {}
    //: Constructor.
    
    Arg2T dat2;
    
    static RetT NoOp(Data1T,Data2T) { 
      return RetT(); 
    }
    //: NoOp function.
    
    static RetT IssueError(Data1T,Data2T) { 
      RavlN::IssueError(__FILE__,__LINE__,"NoOp CallFunc2 Called. ");
      return RetT(); 
    }
    //: Error function.
    
    typedef RetT (*FuncT)(Data1T,Data2T);
    //: Function ptr type.    
    
    inline
    FuncT FuncPtr() const
    { return (FuncT)(this->func); }
    //: Function.
    
  };
  
  //! userlevel=Advanced
  //: Call function with 2 arguments
  
  template<class Data1T,class Data2T,class RetT = bool>
  class CallFunc2C
    : public CallFunc1C<Data1T,RetT>
  {
  public:
    typedef typename TraitsC<Data1T>::BaseTypeT Arg1T;
    //: Basic type of arg, ignore references and const's
    
    typedef typename TraitsC<Data2T>::BaseTypeT Arg2T;
    //: Basic type of arg, ignore references and const's
    
    CallFunc2C()
    {}
    //: Default constructor.
    // Creates an invalid handle.
   
    CallFunc2C(const TriggerC &trig,bool invalidOk = false)
      : CallFunc1C<Data1T,RetT>(dynamic_cast<const CallFunc2BodyC<Data1T,Data2T,RetT> *>(RCHandleC<TriggerBodyC>::BodyPtr(trig)))
    { RavlAlwaysAssertMsg(invalidOk || RCHandleC<TriggerBodyC>::IsValid(),"Casting to incorrect function type. "); }
    //: Convert from a base handle.
    // If the base handle is of another type, the invaldOk flag is true, 
    // an invalid handle is generated, otherwise an assertion failure is 
    // generated.
    
    CallFunc2C(bool issueErrorOnCall)
      : CallFunc1C<Data1T,RetT>(*new CallFunc2BodyC<Data1T,Data2T,RetT>(issueErrorOnCall))
    {}
    //: NoOp constructor.
    // if issueErrorOnCall a function which causes the
    // equivelent of a assertion failure is called. Otherwise
    // the call returns silently.
    
    CallFunc2C(RetT (*nfunc)(Data1T,Data2T),const Arg1T &dat1 = Arg1T(),const Arg2T &dat2 = Arg2T())
      : CallFunc1C<Data1T,RetT>(*new CallFunc2BodyC<Data1T,Data2T,RetT>(nfunc,dat1,dat2))
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    void Invalidate()
    { RCHandleC<TriggerBodyC>::Invalidate(); }
    //: Invalidate handle
  protected:
    CallFunc2C(CallFunc2BodyC<Data1T,Data2T,RetT> &bod)
      : CallFunc1C<Data1T,RetT>(bod)
    {}
    //: Body constructor.
    
    CallFunc2C(const CallFunc2BodyC<Data1T,Data2T,RetT> *bod)
      : CallFunc1C<Data1T,RetT>(bod)
    {}
    //: Body constructor.
    
    CallFunc2BodyC<Data1T,Data2T,RetT> &Body()
    { return static_cast<CallFunc2BodyC<Data1T,Data2T,RetT> &>(TriggerC::Body()); }
    //: Access body.
    
    const CallFunc2BodyC<Data1T,Data2T,RetT> &Body() const
    { return static_cast<const CallFunc2BodyC<Data1T,Data2T,RetT> &>(TriggerC::Body()); }
    //: Constant access to body.
    
  public:
    RetT Call()
    { return Body().Call(); }
    //: Call function.
    
    RetT Call(Arg1T &pd1)
    { return Body().Call(pd1); }
    //: Invoke event, with parameters.
    
    RetT Call(Arg1T &pd1,Arg2T &pd2)
    { return Body().Call(pd1,pd2); }
    //: Invoke event, with parameters.

    RetT operator()()
    { return Call(); }
    //: Call function.
    
    RetT operator()(Arg1T &pd1)
    { return Call(pd1); }
    //: Call function.
    
    RetT operator()(Arg1T &pd1,Arg2T &pd2)
    { return Body().Call(pd1,pd2); }
    //: Call function with parameters
    
    CallFunc2C<Data1T,Data2T,RetT> Copy() const
    { return CallFunc2C<Data1T,Data2T,RetT>(static_cast<CallFunc2BodyC<Data1T,Data2T,RetT> &>(Body().Copy())); }
    //: Copy func call.
    
    Arg2T &Data2()
    { return Body().Data2(); }
    //: Access data.
   
    const Arg2T &Data2() const
    { return Body().Data2(); }
    //: Access data.
  };
  
  //////////////////////////////////////////////////////
  
  //! userlevel=Develop
  //: Body for CallFunc3C
  
  template<class Data1T,class Data2T,class Data3T,class RetT = bool>
  class CallFunc3BodyC
    : public CallFunc2BodyC<Data1T,Data2T,RetT>
  {
  public:
    typedef typename TraitsC<Data1T>::BaseTypeT Arg1T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data2T>::BaseTypeT Arg2T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data3T>::BaseTypeT Arg3T; //: Basic type of arg, ignore references and const's
    
    CallFunc3BodyC(bool issueErrorOnCall) {
      if(issueErrorOnCall)
	this->func = (VoidFuncPtrT) &CallFunc3BodyC<Data1T,Data2T,RetT>::IssueError;
      else
	this->func = (VoidFuncPtrT) &CallFunc3BodyC<Data1T,Data2T,RetT>::NoOp;
    }
    //: Constructor.
    // if issueErrorOnCall a function which causes the
    // equivelent of a assertion failure is called. Otherwise
    // the call returns silently.
    
    CallFunc3BodyC() 
      : CallFunc2BodyC<Data1T,Data2T,RetT>((VoidFuncPtrT) &CallFunc3BodyC<Data1T,Data2T,Data3T,RetT>::IssueError)
    {}
    //: Default constructor.
    
    CallFunc3BodyC(RetT (*nfunc)(Data1T,Data2T,Data3T),const Arg1T &ndat1 = Arg1T(),const Arg2T &ndat2 = Arg2T(),const Arg3T &ndat3 = Arg3T())
      : CallFunc2BodyC<Data1T,Data2T,RetT>((VoidFuncPtrT) nfunc,ndat1,ndat2),
	dat3(ndat3)
    {}
    //: Constructor.
    
    CallFunc3BodyC(VoidFuncPtrT voidFuncPtr)
      : CallFunc2BodyC<Data1T,Data2T,RetT>(voidFuncPtr)
    {}
    //: void function ptr constructor.
    
    virtual void Invoke()
    { (*FuncPtr())(this->dat1,this->dat2,dat3); }
    //: Invoke event.

    virtual RetT Call()
    { return (*FuncPtr())(this->dat1,this->dat2,dat3); }
    //: Invoke event.
    
    virtual RetT Call(Arg1T &pd)
    { return (*FuncPtr())(pd,this->dat2,dat3); }
    //: Invoke event, with parameter.
    
    virtual RetT Call(Arg1T &pd1,Arg2T &pd2)
    { return (*FuncPtr())(pd1,pd2,dat3); }
    //: Invoke event, with parameters.
    
    virtual RetT Call(Arg1T &pd1,Arg2T &pd2,Arg3T &pd3)
    { return (*FuncPtr())(pd1,pd2,pd3); }
    //: Invoke event, with parameters.
    
    Arg3T &Data3()
    { return dat3; }
    //: Access data.
    
    const Arg3T &Data3() const
    { return dat3; }
    //: Access data.

    virtual RCBodyVC &Copy() const
    { return *new CallFunc3BodyC<Data1T,Data2T,Data3T,RetT>(this->func,this->dat1,this->dat2,dat3); }
    //: Copy call.

    static RetT NoOp(Data1T,Data2T,Data3T) { 
      return RetT(); 
    }
    //: NoOp function.
    
    static RetT IssueError(Data1T,Data2T,Data3T) { 
      RavlN::IssueError(__FILE__,__LINE__,"NoOp CallFunc3 Called. ");
      return RetT(); 
    }
    //: Error function.
    
  protected:
    CallFunc3BodyC(void (*nfunc)(),const Arg1T &ndat1,const Arg2T &ndat2,const Arg3T &ndat3)
      : CallFunc2BodyC<Data1T,Data2T,RetT>((VoidFuncPtrT) nfunc,ndat1,ndat2),
	dat3(ndat3)
    {}
    //: Constructor.
    
    Arg3T dat3;
    
    typedef RetT (*FuncT)(Data1T,Data2T,Data3T);
    //: Function ptr type.
    
    inline
    FuncT FuncPtr() const
    { return (FuncT)(this->func); }
    //: Function.
  };
  
  //! userlevel=Advanced
  //: Call function with 3 arguments
  
  template<class Data1T,class Data2T,class Data3T,class RetT = bool>
  class CallFunc3C
    : public CallFunc2C<Data1T,Data2T,RetT>
  {
  public:
    typedef typename TraitsC<Data1T>::BaseTypeT Arg1T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data2T>::BaseTypeT Arg2T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data3T>::BaseTypeT Arg3T; //: Basic type of arg, ignore references and const's
    
    CallFunc3C()
    {}
    //: Default constructor.
    // Creates an invalid handle.

    CallFunc3C(const TriggerC &trig,bool invalidOk = false)
      : CallFunc2C<Data1T,Data2T,RetT>(dynamic_cast<const CallFunc3BodyC<Data1T,Data2T,Data3T,RetT> *>(RCHandleC<TriggerBodyC>::BodyPtr(trig)))
    { RavlAlwaysAssertMsg(invalidOk || RCHandleC<TriggerBodyC>::IsValid(),"Casting to incorrect function type. "); }
    //: Convert from a base handle.
    // If the base handle is of another type, the invaldOk flag is true, 
    // an invalid handle is generated, otherwise an assertion failure is 
    // generated.
    
    CallFunc3C(bool issueErrorOnCall)
      : CallFunc2C<Data1T,Data2T,RetT>(*new CallFunc3BodyC<Data1T,Data2T,Data3T,RetT>(issueErrorOnCall))
    {}
    //: NoOp constructor.
    // if issueErrorOnCall a function which causes the
    // equivelent of a assertion failure is called. Otherwise
    // the call returns silently.
    
    CallFunc3C(RetT (*nfunc)(Data1T,Data2T,Data3T),const Arg1T &dat1 = Arg1T(),const Arg2T &dat2 = Arg2T(),const Arg3T &dat3 = Arg3T())
      : CallFunc2C<Data1T,Data2T,RetT>(*new CallFunc3BodyC<Data1T,Data2T,Data3T,RetT>(nfunc,dat1,dat2,dat3))
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    void Invalidate()
    { RCHandleC<TriggerBodyC>::Invalidate(); }
    //: Invalidate handle

  protected:
    CallFunc3C(CallFunc3BodyC<Data1T,Data2T,Data3T,RetT> &bod)
      : CallFunc2C<Data1T,Data2T,RetT>(bod)
    {}
    //: Body constructor.
    
    CallFunc3C(const CallFunc3BodyC<Data1T,Data2T,Data3T,RetT> *bod)
      : CallFunc2C<Data1T,Data2T,RetT>(bod)
    {}
    //: Body constructor.
    
    CallFunc3BodyC<Data1T,Data2T,Data3T,RetT> &Body()
    { return static_cast<CallFunc3BodyC<Data1T,Data2T,Data3T,RetT> &>(TriggerC::Body()); }
    //: Access body.
    
    const CallFunc3BodyC<Data1T,Data2T,Data3T,RetT> &Body() const
    { return static_cast<const CallFunc3BodyC<Data1T,Data2T,Data3T,RetT> &>(TriggerC::Body()); }
    //: Constant access to body.
    
  public:
    RetT Call()
    { return Body().Call(); }
    //: Call function.
    
    RetT Call(Arg1T &pd1)
    { return Body().Call(pd1); }
    //: Invoke event, with parameters.
    
    RetT Call(Arg1T &pd1,Arg2T &pd2)
    { return Body().Call(pd1,pd2); }
    //: Invoke event, with parameters.
    
    RetT Call(Arg1T &pd1,Arg2T &pd2,Arg3T &pd3)
    { return Body().Call(pd1,pd2,pd3); }
    //: Invoke event, with parameters.

    RetT operator()(Arg1T &pd1)
    { return Body().Call(pd1); }
    //: Call function with parameters
    
    RetT operator()(Arg1T &pd1,Arg2T &pd2)
    { return Body().Call(pd1,pd2); }
    //: Call function with parameters
    
    RetT operator()(Arg1T &pd1,Arg2T &pd2,Arg3T &pd3)
    { return Body().Call(pd1,pd2,pd3); }
    //: Call function with parameters

    CallFunc3C<Data1T,Data2T,Data3T,RetT> Copy() const
    { return CallFunc3C<Data1T,Data2T,Data3T,RetT>(static_cast<CallFunc3BodyC<Data1T,Data2T,Data3T,RetT> &>(Body().Copy())); }
    //: Copy func call.

    Arg3T &Data3()
    { return Body().Data3(); }
    //: Access data.
   
    const Arg3T &Data3() const
    { return Body().Data3(); }
    //: Access data.
    
  };

  //////////////////////////////////////////////////////
  
  //! userlevel=Develop
  //: Body for CallFunc4C
  
  template<class Data1T,class Data2T,class Data3T,class Data4T,class RetT = bool>
  class CallFunc4BodyC
    : public CallFunc3BodyC<Data1T,Data2T,Data3T,RetT>
  {
  public:
    typedef typename TraitsC<Data1T>::BaseTypeT Arg1T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data2T>::BaseTypeT Arg2T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data3T>::BaseTypeT Arg3T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data4T>::BaseTypeT Arg4T; //: Basic type of arg, ignore references and const's
    
    CallFunc4BodyC() 
      : CallFunc3BodyC<Data1T,Data2T,Data3T,RetT>((VoidFuncPtrT) &CallFunc4BodyC<Data1T,Data2T,Data3T,Data4T,RetT>::IssueError)
    {}
    //: Default constructor.
    
    CallFunc4BodyC(VoidFuncPtrT voidFuncPtr)
      : CallFunc3BodyC<Data1T,Data2T,Data3T,RetT>(voidFuncPtr)
    {}
    //: void function ptr constructor.
    
    CallFunc4BodyC(bool issueErrorOnCall) {
      if(issueErrorOnCall)
	this->func = (VoidFuncPtrT) &CallFunc4BodyC<Data1T,Data2T,Data3T,Data4T,RetT>::IssueError;
      else
	this->func = (VoidFuncPtrT) &CallFunc4BodyC<Data1T,Data2T,Data3T,Data4T,RetT>::NoOp;
    }
    //: Constructor.
    // if issueErrorOnCall a function which causes the
    // equivelent of a assertion failure is called. Otherwise
    // the call returns silently.
    
    CallFunc4BodyC(RetT (*nfunc)(Data1T,Data2T,Data3T,Data4T),const Arg1T &ndat1,const Arg2T &ndat2,const Arg3T &ndat3,const Arg4T &ndat4)
      : CallFunc3BodyC<Data1T,Data2T,Data3T,RetT>((VoidFuncPtrT) nfunc,ndat1,ndat2,ndat3),
	dat4(ndat4)
    {}
    //: Constructor.
    
    virtual void Invoke()
    { (*FuncPtr())(this->dat1,this->dat2,this->dat3,dat4); }
    //: Invoke event.

    virtual RetT Call()
    { return (*FuncPtr())(this->dat1,this->dat2,this->dat3,dat4); }
    //: Invoke event.
    
    virtual RetT Call(Arg1T &pd)
    { return (*FuncPtr())(pd,this->dat2,this->dat3,dat4); }
    //: Invoke event, with parameter.
    
    virtual RetT Call(Arg1T &pd1,Arg2T &pd2)
    { return (*FuncPtr())(pd1,pd2,this->dat3,dat4); }
    //: Invoke event, with parameters.
    
    virtual RetT Call(Arg1T &pd1,Arg2T &pd2,Arg3T &pd3)
    { return (*FuncPtr())(pd1,pd2,pd3,dat4); }
    //: Invoke event, with parameters.

    virtual RetT Call(Arg1T &pd1,Arg2T &pd2,Arg3T &pd3,Arg4T &pd4)
    { return (*FuncPtr())(pd1,pd2,pd3,pd4); }
    //: Invoke event, with parameters.

    Arg4T &Data4()
    { return dat4; }
    //: Access data.
    
    const Arg4T &Data4() const
    { return dat4; }
    //: Access data.

    virtual RCBodyVC &Copy() const
    { return *new CallFunc4BodyC<Data1T,Data2T,Data3T,Data4T,RetT>(this->func,this->dat1,this->dat2,this->dat3,dat4); }
    //: Copy call.
    
  protected:
    CallFunc4BodyC(void (*nfunc)(),const Arg1T &ndat1,const Arg2T &ndat2,const Arg3T &ndat3,const Arg4T &ndat4)
      : CallFunc3BodyC<Data1T,Data2T,Data3T,RetT>((VoidFuncPtrT) nfunc,ndat1,ndat2,ndat3),
	dat4(ndat4)
    {}
    //: Constructor.
    
    Arg4T dat4;

    static RetT NoOp(Data1T,Data2T,Data3T,Data4T) { 
      return RetT();
    }
    //: NoOp function.
    
    static RetT IssueError(Data1T,Data2T,Data3T,Data4T) { 
      RavlN::IssueError(__FILE__,__LINE__,"NoOp CallFunc4 Called. ");
      return RetT(); 
    }
    //: Error function.

    typedef RetT (*FuncT)(Data1T,Data2T,Data3T,Data4T);
    //: Function ptr type. 
    
    inline
    FuncT FuncPtr() const
    { return (FuncT)(this->func); }
    //: Function.
  };
  
  //! userlevel=Advanced
  //: Call function with 4 arguments
  
  template<class Data1T,class Data2T,class Data3T,class Data4T,class RetT = bool>
  class CallFunc4C
    : public CallFunc3C<Data1T,Data2T,Data3T,RetT>
  {
  public:
    typedef typename TraitsC<Data1T>::BaseTypeT Arg1T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data2T>::BaseTypeT Arg2T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data3T>::BaseTypeT Arg3T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data4T>::BaseTypeT Arg4T; //: Basic type of arg, ignore references and const's
    
    CallFunc4C()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    CallFunc4C(const TriggerC &trig,bool invalidOk = false)
      : CallFunc3C<Data1T,Data2T,Data3T,RetT>(dynamic_cast<const CallFunc4BodyC<Data1T,Data2T,Data3T,Data4T,RetT> *>(RCHandleC<TriggerBodyC>::BodyPtr(trig)))
    { RavlAlwaysAssertMsg(invalidOk || RCHandleC<TriggerBodyC>::IsValid(),"Casting to incorrect function type. "); }
    //: Convert from a base handle.
    // If the base handle is of another type, the invaldOk flag is true, 
    // an invalid handle is generated, otherwise an assertion failure is 
    // generated.

    CallFunc4C(bool issueErrorOnCall)
      : CallFunc3C<Data1T,Data2T,Data3T,RetT>(*new CallFunc4BodyC<Data1T,Data2T,Data3T,Data4T,RetT>(issueErrorOnCall))
    {}
    //: NoOp constructor.
    // if issueErrorOnCall a function which causes the
    // equivelent of a assertion failure is called. Otherwise
    // the call returns silently.
    
    CallFunc4C(RetT (*nfunc)(Data1T,Data2T,Data3T,Data4T),
	       const Arg1T &dat1 = Arg1T(),const Arg2T &dat2 = Arg2T(),const Arg3T &dat3 = Arg3T(),const Arg4T &dat4 = Arg4T())
      : CallFunc3C<Data1T,Data2T,Data3T,RetT>(*new CallFunc4BodyC<Data1T,Data2T,Data3T,Data4T,RetT>(nfunc,dat1,dat2,dat3,dat4))
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    void Invalidate()
    { RCHandleC<TriggerBodyC>::Invalidate(); }
    //: Invalidate handle
    
  protected:
    CallFunc4C(CallFunc4BodyC<Data1T,Data2T,Data3T,Data4T,RetT> &bod)
      : CallFunc3C<Data1T,Data2T,Data3T,RetT>(bod)
    {}
    //: Body constructor.
    
    CallFunc4C(const CallFunc4BodyC<Data1T,Data2T,Data3T,Data4T,RetT> *bod)
      : CallFunc3C<Data1T,Data2T,Data3T,RetT>(bod)
    {}
    //: Body constructor.
    
    CallFunc4BodyC<Data1T,Data2T,Data3T,Data4T,RetT> &Body()
    { return static_cast<CallFunc4BodyC<Data1T,Data2T,Data3T,Data4T,RetT> &>(TriggerC::Body()); }
    //: Access body.
    
    const CallFunc4BodyC<Data1T,Data2T,Data3T,Data4T,RetT> &Body() const
    { return static_cast<const CallFunc4BodyC<Data1T,Data2T,Data3T,Data4T,RetT> &>(TriggerC::Body()); }
    //: Constant access to body.
    
  public:
    RetT Call()
    { return Body().Call(); }
    //: Call function.
    
    RetT Call(Arg1T &pd1)
    { return Body().Call(pd1); }
    //: Invoke event, with parameters.
    
    RetT Call(Arg1T &pd1,Arg2T &pd2)
    { return Body().Call(pd1,pd2); }
    //: Invoke event, with parameters.

    RetT Call(Arg1T &pd1,Arg2T &pd2,Arg3T &pd3)
    { return Body().Call(pd1,pd2,pd3); }
    //: Invoke event, with parameters.
    
    RetT Call(Arg1T &pd1,Arg2T &pd2,Arg3T &pd3,Arg4T &pd4)
    { return Body().Call(pd1,pd2,pd3,pd4); }
    //: Invoke event, with parameters.
    
    RetT operator()()
    { return Body().Call(); }
    //: Call function with parameters
    
    RetT operator()(Arg1T &pd1)
    { return Body().Call(pd1); }
    //: Call function with parameters
    
    RetT operator()(Arg1T &pd1,Arg2T &pd2)
    { return Body().Call(pd1,pd2); }
    //: Call function with parameters

    RetT operator()(Arg1T &pd1,Arg2T &pd2,Arg3T &pd3)
    { return Body().Call(pd1,pd2,pd3); }
    //: Call function with parameters

    RetT operator()(Arg1T &pd1,Arg2T &pd2,Arg3T &pd3,Arg4T &pd4)
    { return Body().Call(pd1,pd2,pd3,pd4); }
    //: Call function with parameters
    
    CallFunc4C<Data1T,Data2T,Data3T,Data4T,RetT> Copy() const
    { return CallFunc4C<Data1T,Data2T,Data3T,Data4T,RetT>(static_cast<CallFunc4BodyC<Data1T,Data2T,Data3T,Data4T,RetT> &>(Body().Copy())); }
    //: Copy func call.
    
    Arg4T &Data4()
    { return Body().Data4(); }
    //: Access data.
   
    const Arg4T &Data4() const
    { return Body().Data4(); }
    //: Access data.
    
  };

  //////////////////////////////////////////////////////
  
  //! userlevel=Develop
  //: Body for CallFunc5C
  
  template<class Data1T,class Data2T,class Data3T,class Data4T,class Data5T,class RetT = bool>
  class CallFunc5BodyC
    : public CallFunc4BodyC<Data1T,Data2T,Data3T,Data4T,RetT>
  {
  public:
    typedef typename TraitsC<Data1T>::BaseTypeT Arg1T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data2T>::BaseTypeT Arg2T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data3T>::BaseTypeT Arg3T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data4T>::BaseTypeT Arg4T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data5T>::BaseTypeT Arg5T; //: Basic type of arg, ignore references and const's
    
    CallFunc5BodyC(bool issueErrorOnCall) {
      if(issueErrorOnCall)
	this->func = (VoidFuncPtrT) &CallFunc5BodyC<Data1T,Data2T,Data3T,Data4T,Data5T,RetT>::IssueError;
      else
	this->func = (VoidFuncPtrT) &CallFunc5BodyC<Data1T,Data2T,Data3T,Data4T,Data5T,RetT>::NoOp;
    }
    //: Constructor.
    // if issueErrorOnCall a function which causes the
    // equivelent of a assertion failure is called. Otherwise
    // the call returns silently.

    CallFunc5BodyC() 
      : CallFunc4BodyC<Data1T,Data2T,Data3T,Data4T,RetT>((VoidFuncPtrT) &CallFunc5BodyC<Data1T,Data2T,Data3T,Data4T,Data5T,RetT>::IssueError)
    {}
    //: Default constructor.
    
    
    CallFunc5BodyC(RetT (*nfunc)(Data1T,Data2T,Data3T,Data4T,Data5T),
		   const Arg1T &ndat1,
		   const Arg2T &ndat2,
		   const Arg3T &ndat3,
		   const Arg4T &ndat4,
		   const Arg5T &ndat5)
      : CallFunc4BodyC<Data1T,Data2T,Data3T,Data4T,RetT>((VoidFuncPtrT) nfunc,ndat1,ndat2,ndat3,ndat4),
	dat5(ndat5)
    {}
    //: Constructor.
    
    virtual void Invoke()
    { (*FuncPtr())(this->dat1,this->dat2,this->dat3,this->dat4,dat5); }
    //: Invoke event.

    virtual RetT Call()
    { return (*FuncPtr())(this->dat1,this->dat2,this->dat3,this->dat4,dat5); }
    //: Invoke event.
    
    virtual RetT Call(Arg1T &pd)
    { return (*FuncPtr())(pd,this->dat2,this->dat3,this->dat4,dat5); }
    //: Invoke event, with parameter.
    
    virtual RetT Call(Arg1T &pd1,Arg2T &pd2)
    { return (*FuncPtr())(pd1,pd2,this->dat3,this->dat4,dat5); }
    //: Invoke event, with parameters.
    
    virtual RetT Call(Arg1T &pd1,Arg2T &pd2,Arg3T &pd3)
    { return (*FuncPtr())(pd1,pd2,pd3,this->dat4,dat5); }
    //: Invoke event, with parameters.

    virtual RetT Call(Arg1T &pd1,Arg2T &pd2,Arg3T &pd3,Arg4T &pd4)
    { return (*FuncPtr())(pd1,pd2,pd3,pd4,dat5); }
    //: Invoke event, with parameters.

    virtual RetT Call(Arg1T &pd1,Arg2T &pd2,Arg3T &pd3,Arg4T &pd4,Arg5T &pd5)
    { return (*FuncPtr())(pd1,pd2,pd3,pd4,pd5); }
    //: Invoke event, with parameters.
    
    Arg5T &Data5()
    { return dat5; }
    //: Access data.
    
    const Arg5T &Data5() const
    { return dat5; }
    //: Access data.
    
    virtual RCBodyVC &Copy() const
    { return *new CallFunc5BodyC<Data1T,Data2T,Data3T,Data4T,Data5T,RetT>(this->func,this->dat1,this->dat2,this->dat3,this->dat4,dat5); }
    //: Copy call.
    
  protected:
    CallFunc5BodyC(void (*nfunc)(),
		   const Arg1T &ndat1,
		   const Arg2T &ndat2,
		   const Arg3T &ndat3,
		   const Arg4T &ndat4,
		   const Arg5T &ndat5)
      : CallFunc4BodyC<Data1T,Data2T,Data3T,Data4T,RetT>(nfunc,ndat1,ndat2,ndat3,ndat4),
	dat5(ndat5)
    {}
    //: Constructor.
    
    Arg5T dat5;
    
    static RetT NoOp(Data1T,Data2T,Data3T,Data4T,Data5T) { 
      return RetT();
    }
    //: NoOp function.
    
    static RetT IssueError(Data1T,Data2T,Data3T,Data4T,Data5T) { 
      RavlN::IssueError(__FILE__,__LINE__,"NoOp CallFunc5 Called. ");
      return RetT(); 
    }
    //: Error function.
    
    typedef RetT (*FuncT)(Data1T,Data2T,Data3T,Data4T,Data5T);
    //: Function ptr type.    
    
    inline
    FuncT FuncPtr() const
    { return (FuncT)(this->func); }
    //: Function.
  };
  
  //! userlevel=Advanced
  //: Call function with 5 arguments
  
  template<class Data1T,class Data2T,class Data3T,class Data4T,class Data5T,class RetT = bool>
  class CallFunc5C
    : public CallFunc4C<Data1T,Data2T,Data3T,Data4T,RetT>
  {
  public:
    typedef typename TraitsC<Data1T>::BaseTypeT Arg1T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data2T>::BaseTypeT Arg2T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data3T>::BaseTypeT Arg3T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data4T>::BaseTypeT Arg4T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data5T>::BaseTypeT Arg5T; //: Basic type of arg, ignore references and const's
    
    CallFunc5C()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    CallFunc5C(bool issueErrorOnCall)
      : CallFunc4C<Data1T,Data2T,Data3T,Data4T,RetT>(*new CallFunc5BodyC<Data1T,Data2T,Data3T,Data4T,Data5T,RetT>(issueErrorOnCall))
    {}
    //: NoOp constructor.
    // if issueErrorOnCall a function which causes the
    // equivelent of a assertion failure is called. Otherwise
    // the call returns silently.

    CallFunc5C(const TriggerC &trig,bool invalidOk = false)
      : CallFunc4C<Data1T,Data2T,Data3T,Data4T,RetT>(dynamic_cast<const CallFunc5BodyC<Data1T,Data2T,Data3T,Data4T,Data5T,RetT> *>(RCHandleC<TriggerBodyC>::BodyPtr(trig)))
    { RavlAlwaysAssertMsg(invalidOk || RCHandleC<TriggerBodyC>::IsValid(),"Casting to incorrect function type. "); }
    //: Convert from a base handle.
    // If the base handle is of another type, the invaldOk flag is true, 
    // an invalid handle is generated, otherwise an assertion failure is 
    // generated.
    
    CallFunc5C(RetT (*nfunc)(Data1T,Data2T,Data3T,Data4T,Data5T),
	       const Arg1T &dat1 = Arg1T(),
	       const Arg2T &dat2 = Arg2T(),
	       const Arg3T &dat3 = Arg3T(),
	       const Arg4T &dat4 = Arg4T(),
	       const Arg5T &dat5 = Arg5T())
      : CallFunc4C<Data1T,Data2T,Data3T,Data4T,RetT>(*new CallFunc5BodyC<Data1T,Data2T,Data3T,Data4T,Data5T,RetT>(nfunc,dat1,dat2,dat3,dat4,dat5))
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    void Invalidate()
    { RCHandleC<TriggerBodyC>::Invalidate(); }
    //: Invalidate handle
    
  protected:
    CallFunc5C(CallFunc5BodyC<Data1T,Data2T,Data3T,Data4T,Data5T,RetT> &bod)
      : CallFunc4C<Data1T,Data2T,Data3T,Data4T,RetT>(bod)
    {}
    //: Body constructor.
    
    CallFunc5C(const CallFunc5BodyC<Data1T,Data2T,Data3T,Data4T,Data5T,RetT> *bod)
      : CallFunc4C<Data1T,Data2T,Data3T,Data4T,RetT>(bod)
    {}
    //: Body constructor.
    
    CallFunc5BodyC<Data1T,Data2T,Data3T,Data4T,Data5T,RetT> &Body()
    { return static_cast<CallFunc5BodyC<Data1T,Data2T,Data3T,Data4T,Data5T,RetT> &>(TriggerC::Body()); }
    //: Access body.
    
    const CallFunc5BodyC<Data1T,Data2T,Data3T,Data4T,Data5T,RetT> &Body() const
    { return static_cast<const CallFunc5BodyC<Data1T,Data2T,Data3T,Data4T,Data5T,RetT> &>(TriggerC::Body()); }
    //: Constant access to body.
    
  public:
    RetT Call()
    { return Body().Call(); }
    //: Call function.
    
    RetT Call(Arg1T &pd1)
    { return Body().Call(pd1); }
    //: Invoke event, with parameters.
    
    RetT Call(Arg1T &pd1,Arg2T &pd2)
    { return Body().Call(pd1,pd2); }
    //: Invoke event, with parameters.
    
    RetT Call(Arg1T &pd1,Arg2T &pd2,Arg3T &pd3)
    { return Body().Call(pd1,pd2,pd3); }
    //: Invoke event, with parameters.
    
    RetT Call(Arg1T &pd1,Arg2T &pd2,Arg3T &pd3,Arg4T &pd4)
    { return Body().Call(pd1,pd2,pd3,pd4); }
    //: Invoke event, with parameters.
    
    RetT Call(Arg1T &pd1,Arg2T &pd2,Arg3T &pd3,Arg4T &pd4,Arg5T &pd5)
    { return Body().Call(pd1,pd2,pd3,pd4,pd5); }
    //: Invoke event, with parameters.

    RetT operator()()
    { return Body().Call(); }
    //: Call function with parameters
    
    RetT operator()(Arg1T &pd1)
    { return Body().Call(pd1); }
    //: Call function with parameters

    RetT operator()(Arg1T &pd1,Arg2T &pd2)
    { return Body().Call(pd1,pd2); }
    //: Call function with parameters
    
    RetT operator()(Arg1T &pd1,Arg2T &pd2,Arg3T &pd3)
    { return Body().Call(pd1,pd2,pd3); }
    //: Call function with parameters
    
    RetT operator()(Arg1T &pd1,Arg2T &pd2,Arg3T &pd3,Arg4T &pd4)
    { return Body().Call(pd1,pd2,pd3,pd4); }
    //: Call function with parameters
    
    RetT operator()(Arg1T &pd1,Arg2T &pd2,Arg3T &pd3,Arg4T &pd4,Arg5T &pd5)
    { return Body().Call(pd1,pd2,pd3,pd4,pd5); }
    //: Call function with parameters
    
    CallFunc5C<Data1T,Data2T,Data3T,Data4T,Data5T,RetT> Copy() const
    { return CallFunc5C<Data1T,Data2T,Data3T,Data4T,Data5T,RetT>(static_cast<CallFunc5BodyC<Data1T,Data2T,Data3T,Data4T,Data5T,RetT> &>(Body().Copy())); }
    //: Copy func call.
    
    Arg5T &Data5()
    { return Body().Data5(); }
    //: Access data.
   
    const Arg5T &Data5() const
    { return Body().Data5(); }
    //: Access data.

  };


  /////////////////////////////////////////////////////////
  //! userlevel=Normal

//#if !defined(RAVL_COMPILER_VISUALCPP)
#if (!RAVL_COMPILER_VISUALCPP || RAVL_COMPILER_VISUALCPPNET)
  
  template<class RetT>
  inline
  CallFunc0C<RetT> Trigger(RetT (*nfunc)())
  { return CallFunc0C<RetT>(nfunc); }
  //: Create a function call with no arguments
  // See <a href="../Tree/Ravl.API.Core.Calls.html"><b>Calls and Triggers</b></a> for details.
  
  template<class DataT,class RetT>
  inline
  CallFunc1C<DataT,RetT> 
  Trigger(RetT (*nfunc)(DataT dat),const typename TraitsC<DataT>::BaseTypeT &defaultArg)
  { return CallFunc1C<DataT,RetT>(nfunc,defaultArg); }
  //: Create a function call with 1 argument
  // See <a href="../Tree/Ravl.API.Core.Calls.html"><b>Calls and Triggers</b></a> for details.
  
  template<class Data1T,class Data2T,class RetT>
  inline
  CallFunc2C<Data1T,Data2T,RetT> 
  Trigger(RetT (*nfunc)(Data1T ,Data2T ),
	  const typename TraitsC<Data1T>::BaseTypeT &defaultArg1,
	  const typename TraitsC<Data2T>::BaseTypeT &defaultArg2)
  { return CallFunc2C<Data1T,Data2T,RetT>(nfunc,defaultArg1,defaultArg2); }
  //: Create a function call with 2 arguments
  // See <a href="../Tree/Ravl.API.Core.Calls.html"><b>Calls and Triggers</b></a> for details.
  
  template<class Data1T,class Data2T,class Data3T,class RetT>
  inline
  CallFunc3C<Data1T,Data2T,Data3T,RetT> 
  Trigger(RetT (*nfunc)(Data1T,Data2T,Data3T),
	  const typename TraitsC<Data1T>::BaseTypeT &defaultArg1,
	  const typename TraitsC<Data2T>::BaseTypeT &defaultArg2,
	  const typename TraitsC<Data3T>::BaseTypeT &defaultArg3)
  { return CallFunc3C<Data1T,Data2T,Data3T,RetT>(nfunc,defaultArg1,defaultArg2,defaultArg3); }
  //: Create a function call with 3 arguments
  // See <a href="../Tree/Ravl.API.Core.Calls.html"><b>Calls and Triggers</b></a> for details.
  
  template<class Data1T,class Data2T,class Data3T,class Data4T,class RetT>
  inline
  CallFunc4C<Data1T,Data2T,Data3T,Data4T,RetT> 
  Trigger(RetT (*nfunc)(Data1T,Data2T,Data3T,Data4T),
	  const typename TraitsC<Data1T>::BaseTypeT &defaultArg1,
	  const typename TraitsC<Data2T>::BaseTypeT &defaultArg2,
	  const typename TraitsC<Data3T>::BaseTypeT &defaultArg3,
	  const typename TraitsC<Data4T>::BaseTypeT &defaultArg4)
  { return CallFunc4C<Data1T,Data2T,Data3T,Data4T,RetT>(nfunc,defaultArg1,defaultArg2,defaultArg3,defaultArg4); }
  //: Create a function call with 4 arguments
  // See <a href="../Tree/Ravl.API.Core.Calls.html"><b>Calls and Triggers</b></a> for details.
  
  template<class Data1T,class Data2T,class Data3T,class Data4T,class Data5T,class RetT>
  inline
  CallFunc5C<Data1T,Data2T,Data3T,Data4T,Data5T,RetT> 
  Trigger(RetT (*nfunc)(Data1T,Data2T,Data3T,Data4T,Data5T),
	  const typename TraitsC<Data1T>::BaseTypeT &defaultArg1,
	  const typename TraitsC<Data2T>::BaseTypeT &defaultArg2,
	  const typename TraitsC<Data3T>::BaseTypeT &defaultArg3,
	  const typename TraitsC<Data4T>::BaseTypeT &defaultArg4,
	  const typename TraitsC<Data5T>::BaseTypeT &defaultArg5)
  { return CallFunc5C<Data1T,Data2T,Data3T,Data4T,Data5T,RetT>(nfunc,defaultArg1,defaultArg2,defaultArg3,defaultArg4,defaultArg5); }
  //: Create a function call with 5 arguments
  // See <a href="../Tree/Ravl.API.Core.Calls.html"><b>Calls and Triggers</b></a> for details.
  
#else
  // Lobotimized version for Visual C++.
  // Doesn't template on return types.
  
  inline
  CallFunc0C<bool> Trigger(bool (*nfunc)())
  { return CallFunc0C<bool>(nfunc); }
  //: Create a call 
  // See <a href="../Tree/Ravl.API.Core.Calls.html"><b>Calls and Triggers</b></a> for details.
  //! docentry="Ravl.API.Core.Calls.VisualC++"
  
  template<class DataT>
  inline
  CallFunc1C<DataT,bool> 
  Trigger(bool (*nfunc)(DataT dat),const typename TraitsC<DataT>::BaseTypeT &defaultArg)
  { return CallFunc1C<DataT,bool>(nfunc,defaultArg); }
  //: Create a call 
  // See <a href="../Tree/Ravl.API.Core.Calls.html"><b>Calls and Triggers</b></a> for details.
  
  template<class Data1T,class Data2T>
  inline
  CallFunc2C<Data1T,Data2T,bool> 
  Trigger(bool (*nfunc)(Data1T ,Data2T ),
	  const typename TraitsC<Data1T>::BaseTypeT &defaultArg1,
	  const typename TraitsC<Data2T>::BaseTypeT &defaultArg2)
  { return CallFunc2C<Data1T,Data2T,bool>(nfunc,defaultArg1,defaultArg2); }
  //: Create a call 
  // See <a href="../Tree/Ravl.API.Core.Calls.html"><b>Calls and Triggers</b></a> for details.
  
  template<class Data1T,class Data2T,class Data3T>
  inline
  CallFunc3C<Data1T,Data2T,Data3T,bool> 
  Trigger(bool (*nfunc)(Data1T,Data2T,Data3T),
	  const typename TraitsC<Data1T>::BaseTypeT &defaultArg1,
	  const typename TraitsC<Data2T>::BaseTypeT &defaultArg2,
	  const typename TraitsC<Data3T>::BaseTypeT &defaultArg3)
  { return CallFunc3C<Data1T,Data2T,Data3T,bool>(nfunc,defaultArg1,defaultArg2,defaultArg3); }
  //: Create a call 
  // See <a href="../Tree/Ravl.API.Core.Calls.html"><b>Calls and Triggers</b></a> for details.
  
  template<class Data1T,class Data2T,class Data3T,class Data4T>
  inline
  CallFunc4C<Data1T,Data2T,Data3T,Data4T,bool> 
  Trigger(bool (*nfunc)(Data1T,Data2T,Data3T,Data4T),
	  const typename TraitsC<Data1T>::BaseTypeT &defaultArg1,
	  const typename TraitsC<Data2T>::BaseTypeT &defaultArg2,
	  const typename TraitsC<Data3T>::BaseTypeT &defaultArg3,
	  const typename TraitsC<Data4T>::BaseTypeT &defaultArg4)
  { return CallFunc4C<Data1T,Data2T,Data3T,Data4T,bool>(nfunc,defaultArg1,defaultArg2,defaultArg3,defaultArg4); }
  //: Create a call 
  // See <a href="../Tree/Ravl.API.Core.Calls.html"><b>Calls and Triggers</b></a> for details.
  
  template<class Data1T,class Data2T,class Data3T,class Data4T,class Data5T>
  inline
  CallFunc5C<Data1T,Data2T,Data3T,Data4T,Data5T,bool> 
  Trigger(bool (*nfunc)(Data1T,Data2T,Data3T,Data4T,Data5T),
	  const typename TraitsC<Data1T>::BaseTypeT &defaultArg1,
	  const typename TraitsC<Data2T>::BaseTypeT &defaultArg2,
	  const typename TraitsC<Data3T>::BaseTypeT &defaultArg3,
	  const typename TraitsC<Data4T>::BaseTypeT &defaultArg4,
	  const typename TraitsC<Data5T>::BaseTypeT &defaultArg5)
  { return CallFunc5C<Data1T,Data2T,Data3T,Data4T,Data5T,bool>(nfunc,defaultArg1,defaultArg2,defaultArg3,defaultArg4,defaultArg5); }
  //: Create a call 
  // See <a href="../Tree/Ravl.API.Core.Calls.html"><b>Calls and Triggers</b></a> for details.
  
  
  
#endif
  
}

#endif
