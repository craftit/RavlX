// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_CALLMETHODPTRS_HEADER
#define RAVL_CALLMETHODPTRS_HEADER 1
//////////////////////////////////////////////////////////
//! rcsid="$Id: CallMethods.hh 5504 2006-05-30 15:28:02Z apanman $"
//! docentry="Ravl.API.Core.Calls"
//! lib=RavlCore
//! author="Charles Galambos"
//! file="Ravl/Core/Base/CallMethodPtrs.hh"
//! date="24/08/2006"

#include "Ravl/Calls.hh"

namespace RavlN {
  
  //////////////////////////////////////////////////////
  
  //! userlevel=Develop
  //: Body class for CallMethodPtr0C
  
  template<class HandleT,class ObjT,class RetT = bool>
  class CallMethodPtr0BodyC
    : public CallFunc0BodyC<RetT>
  {
  public:
    typedef typename TraitsC<ObjT>::BaseTypeT BaseObjT; //: Basic type of object, ignore references and const's
    typedef RetT (BaseObjT::*FuncT)();    //: Function ptr type.
    
    CallMethodPtr0BodyC(const HandleT &nobj,FuncT nfunc)
      : CallFunc0BodyC<RetT>((VoidFuncPtrT) 0),
	mPtr(nfunc),
	obj(nobj)
    {}
    //: Constructor.
    
    virtual void Invoke()
    { (obj->*FuncPtr())(); }
    //: Invoke event.
    
    virtual RetT Call()
    { return (obj->*FuncPtr())(); }
    //: Call signal
    
  protected:
    virtual RCBodyVC &Copy() const
    { return *new CallMethodPtr0BodyC<HandleT,ObjT,RetT>(obj,FuncPtr()); }
    //: Copy call.
    
    FuncT mPtr;
    
    inline
    FuncT FuncPtr() const
    { return mPtr; }
    //: Function.
    
    HandleT obj;
  };
  
  //! userlevel=Advanced
  //:  Call method with no arguments
  
  template<class HandleT,class ObjT,class RetT = bool>
  class CallMethodPtr0C
    : public CallFunc0C<RetT>
  {
  public:
    typedef typename TraitsC<ObjT>::BaseTypeT BaseObjT; //: Basic type of object, ignore references and const's
    typedef RetT (BaseObjT::*FuncT)();    //: Function ptr type.
    
    CallMethodPtr0C()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    CallMethodPtr0C(const HandleT &nobj,FuncT nfunc)
      : CallFunc0C<RetT>(*new CallMethodPtr0BodyC<HandleT,ObjT,RetT>(nobj,nfunc))
    {}
    //:  Constructor.x
  };

  /////////////////////////////////////////////////////////
  
  //! userlevel=Develop
  //: Body class for CallMethodPtr1C
  
  template<class HandleT,class ObjT,class DataT,class RetT = bool>
  class CallMethodPtr1BodyC
    : public CallFunc1BodyC<DataT,RetT>
  {
  public:
    typedef typename TraitsC<ObjT>::BaseTypeT BaseObjT; //: Basic type of object, ignore references and const's
    typedef typename TraitsC<DataT>::BaseTypeT Arg1T; //: Basic type of arg, ignore references and const's
    typedef RetT (BaseObjT::*FuncT)(DataT);    //: Function ptr type.
    
    CallMethodPtr1BodyC(const HandleT &nobj,FuncT nfunc,const Arg1T &ndat)
      : CallFunc1BodyC<DataT,RetT>((VoidFuncPtrT) 0,ndat),
	mPtr(nfunc),
        obj(nobj)
    {}
    //: Constructor.
    
    virtual void Invoke()
    { (obj->*FuncPtr())(this->dat1); }
    //: Invoke event.

    virtual RetT Call()
    { return (obj->*FuncPtr())(this->dat1); }
    //: Invoke event, with parameter.
    
    virtual RetT Call(Arg1T &pd)
    { return (obj->*FuncPtr())(pd); }
    //: Invoke event, with parameter.

  protected:
    virtual RCBodyVC &Copy() const
    { return *new CallMethodPtr1BodyC<HandleT,ObjT,DataT,RetT>(obj,FuncPtr(),this->dat1); }
    //: Copy call.

    FuncT mPtr;
    
    inline
    FuncT FuncPtr() const
    { return mPtr; }
    //: Function.
    
    HandleT obj;
  };
  
  //! userlevel=Advanced
  //: Call method with 1 argument
  
  template<class HandleT,class ObjT,class DataT,class RetT = bool>
  class CallMethodPtr1C
    : public CallFunc1C<DataT,RetT>
  {
  public:
    typedef typename TraitsC<ObjT>::BaseTypeT BaseObjT; //: Basic type of object, ignore references and const's
    typedef typename TraitsC<DataT>::BaseTypeT Arg1T; //: Basic type of arg, ignore references and const's
    typedef RetT (BaseObjT::*FuncT)(DataT);    //: Function ptr type.
    
    CallMethodPtr1C()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    CallMethodPtr1C(const HandleT &nobj,FuncT nfunc, const Arg1T &dat = Arg1T())
      : CallFunc1C<DataT,RetT>(*new CallMethodPtr1BodyC<HandleT,ObjT,DataT,RetT>(nobj,nfunc,dat))
    {}
    //: Constructor.
  };
  
  /////////////////////////////////////////////////////////
  
  //! userlevel=Develop
  //: Body class for CallMethodPtr2C
  
  template<class HandleT,class ObjT,class Data1T,class Data2T,class RetT = bool>
  class CallMethodPtr2BodyC
    : public CallFunc2BodyC<Data1T,Data2T,RetT>
  {
  public:
    typedef typename TraitsC<ObjT>::BaseTypeT BaseObjT; //: Basic type of object, ignore references and const's
    typedef typename TraitsC<Data1T>::BaseTypeT Arg1T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data2T>::BaseTypeT Arg2T; //: Basic type of arg, ignore references and const's
    typedef RetT (BaseObjT::*FuncT)(Data1T,Data2T);    //: Function ptr type.
    
    CallMethodPtr2BodyC(const HandleT &nobj,FuncT nfunc,const Arg1T &ndat1,const Arg2T &ndat2)
      : CallFunc2BodyC<Data1T,Data2T,RetT>((VoidFuncPtrT) 0,ndat1,ndat2),
	mPtr(nfunc),
	obj(nobj)
    {}
    //: Constructor.
    
    virtual void Invoke()
    { (obj->*FuncPtr())(this->dat1,this->dat2); }
    //: Invoke event.
    
    virtual RetT Call()
    { return (obj->*FuncPtr())(this->dat1,this->dat2); }
    //: Invoke event, with parameter.
    
    virtual RetT Call(Arg1T &pd)
    { return (obj->*FuncPtr())(pd,this->dat2); }
    //: Invoke event, with parameter.

    virtual RetT Call(Arg1T &pd1,Arg2T &pd2)
    { return (obj->*FuncPtr())(pd1,pd2); }
    //: Invoke event, with parameter.
    
  protected:
    
    inline
    FuncT FuncPtr() const
    { return mPtr; }
    //: Function.

    virtual RCBodyVC &Copy() const
    { return *new CallMethodPtr2BodyC<HandleT,ObjT,Data1T,Data2T,RetT>(obj,FuncPtr(),this->dat1,this->dat2); }
    //: Copy call.
    
    FuncT mPtr;
    
    HandleT obj;
  };
  
  //! userlevel=Advanced
  //: Call method with 2 arguments
  
  template<class HandleT,class ObjT,class Data1T,class Data2T,class RetT = bool>
  class CallMethodPtr2C
    : public CallFunc2C<Data1T,Data2T,RetT>
  {
  public:
    typedef typename TraitsC<ObjT>::BaseTypeT BaseObjT; //: Basic type of object, ignore references and const's
    typedef typename TraitsC<Data1T>::BaseTypeT Arg1T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data2T>::BaseTypeT Arg2T; //: Basic type of arg, ignore references and const's
    typedef RetT (BaseObjT::*FuncT)(Data1T,Data2T);    //: Function ptr type.
    
    CallMethodPtr2C()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    CallMethodPtr2C(const HandleT &nobj,FuncT nfunc,
                    const Arg1T &ndat1 = Arg1T(),const Arg2T &ndat2 = Arg2T())
      : CallFunc2C<Data1T,Data2T,RetT>(*new CallMethodPtr2BodyC<HandleT,ObjT,Data1T,Data2T,RetT>(nobj,nfunc,ndat1,ndat2))
    {}
    //: Default constructor.
    // Creates an invalid handle.
  };
  
  /////////////////////////////////////////////////////////
  
  //! userlevel=Develop
  //: Body class for CallMethodPtr3C
  
  template<class HandleT,class ObjT,class Data1T,class Data2T,class Data3T,class RetT = bool>
  class CallMethodPtr3BodyC
    : public CallFunc3BodyC<Data1T,Data2T,Data3T,RetT>
  {
  public:
    typedef typename TraitsC<ObjT>::BaseTypeT BaseObjT; //: Basic type of object, ignore references and const's
    typedef typename TraitsC<Data1T>::BaseTypeT Arg1T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data2T>::BaseTypeT Arg2T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data3T>::BaseTypeT Arg3T; //: Basic type of arg, ignore references and const's
    typedef RetT (BaseObjT::*FuncT)(Data1T,Data2T,Data3T);    //: Function ptr type.
    
    CallMethodPtr3BodyC(const HandleT &nobj,FuncT nfunc,
                        const Arg1T &ndat1,const Arg2T &ndat2,const Arg3T &ndat3)
      : CallFunc3BodyC<Data1T,Data2T,Data3T,RetT>((VoidFuncPtrT) 0,ndat1,ndat2,ndat3),
	mPtr(nfunc),
        obj(nobj)
    {}
    //: Constructor.
    
    virtual void Invoke()
    { (obj->*FuncPtr())(this->dat1,this->dat2,this->dat3); }
    //: Invoke event.
    
    virtual RetT Call()
    { return (obj->*FuncPtr())(this->dat1,this->dat2,this->dat3); }
    //: Invoke event, with parameter.

    virtual RetT Call(Arg1T &pd)
    { return (obj->*FuncPtr())(pd,this->dat2,this->dat3); }
    //: Invoke event, with parameter.

    virtual RetT Call(Arg1T &pd1,Arg2T &pd2)
    { return (obj->*FuncPtr())(pd1,pd2,this->dat3); }
    //: Invoke event, with parameter.
    
    virtual RetT Call(Arg1T &pd1,Arg2T &pd2,Arg3T &pd3)
    { return (obj->*FuncPtr())(pd1,pd2,pd3); }
    //: Invoke event, with parameter.
    
  protected:
    virtual RCBodyVC &Copy() const
    { return *new CallMethodPtr3BodyC<HandleT,ObjT,Data1T,Data2T,Data3T,RetT>(obj,FuncPtr(),this->dat1,this->dat2,this->dat3); }
    //: Copy call.
    
    FuncT mPtr;
    
    inline
    FuncT FuncPtr() const
    { return mPtr; }
    //: Function.
    
    HandleT obj;
  };
  
  //! userlevel=Advanced
  //: Call method with 3 arguments
  
  template<class HandleT,class ObjT,class Data1T,class Data2T,class Data3T,class RetT = bool>
  class CallMethodPtr3C
    : public CallFunc3C<Data1T,Data2T,Data3T,RetT>
  {
  public:
    typedef typename TraitsC<ObjT>::BaseTypeT BaseObjT; //: Basic type of object, ignore references and const's
    typedef typename TraitsC<Data1T>::BaseTypeT Arg1T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data2T>::BaseTypeT Arg2T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data3T>::BaseTypeT Arg3T; //: Basic type of arg, ignore references and const's
    typedef RetT (BaseObjT::*FuncT)(Data1T,Data2T,Data3T);    //: Function ptr type.
    
    CallMethodPtr3C()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    CallMethodPtr3C(const HandleT &nobj,FuncT nfunc,
                    const Arg1T &ndat1 = Arg1T(),const Arg2T &ndat2 = Arg2T(),const Arg3T &ndat3 = Arg3T())
      : CallFunc3C<Data1T,Data2T,Data3T,RetT>(*new CallMethodPtr3BodyC<HandleT,ObjT,Data1T,Data2T,Data3T,RetT>(nobj,nfunc,ndat1,ndat2,ndat3))
    {}
    //: Constructor.
  };
  
  /////////////////////////////////////////////////////////
  
  //! userlevel=Develop
  //: Body class for CallMethodPtr4C
  
  template<class HandleT,class ObjT,class Data1T,class Data2T,class Data3T,class Data4T,class RetT = bool>
  class CallMethodPtr4BodyC
    : public CallFunc4BodyC<Data1T,Data2T,Data3T,Data4T,RetT>
  {
  public:
    typedef typename TraitsC<ObjT>::BaseTypeT BaseObjT; //: Basic type of object, ignore references and const's
    typedef typename TraitsC<Data1T>::BaseTypeT Arg1T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data2T>::BaseTypeT Arg2T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data3T>::BaseTypeT Arg3T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data4T>::BaseTypeT Arg4T; //: Basic type of arg, ignore references and const's
    typedef RetT (BaseObjT::*FuncT)(Data1T,Data2T,Data3T,Data4T);    //: Function ptr type.
    
    CallMethodPtr4BodyC(const HandleT &nobj,FuncT nfunc,const Arg1T &ndat1,const Arg2T &ndat2,const Arg3T &ndat3,const Arg4T &ndat4)
      : CallFunc4BodyC<Data1T,Data2T,Data3T,Data4T,RetT>((VoidFuncPtrT) 0,ndat1,ndat2,ndat3,ndat4),
	mPtr(nfunc),
	obj(nobj)
    {}
    //: Constructor.
    
    virtual void Invoke()
    { (obj->*FuncPtr())(this->dat1,this->dat2,this->dat3,this->dat4); }
    //: Invoke event.

    virtual RetT Call()
    { return (obj->*FuncPtr())(this->dat1,this->dat2,this->dat3,this->dat4); }
    //: Invoke event.

    virtual RetT Call(Arg1T &pd)
    { return (obj->*FuncPtr())(pd,this->dat2,this->dat3,this->dat4); }
    //: Invoke event, with parameters.

    virtual RetT Call(Arg1T &pd1,Arg2T &pd2)
    { return (obj->*FuncPtr())(pd1,pd2,this->dat3,this->dat4); }
    //: Invoke event, with parameters.
    
    virtual RetT Call(Arg1T &pd1,Arg2T &pd2,Arg3T &pd3)
    { return (obj->*FuncPtr())(pd1,pd2,pd3,this->dat4); }
    //: Invoke event, with parameters.
    
    virtual RetT Call(Arg1T &pd1,Arg2T &pd2,Arg3T &pd3,Arg4T &pd4)
    { return (obj->*FuncPtr())(pd1,pd2,pd3,pd4); }
    //: Invoke event, with parameters.
    
  protected:
    virtual RCBodyVC &Copy() const
    { return *new CallMethodPtr4BodyC<HandleT,ObjT,Data1T,Data2T,Data3T,Data4T,RetT>(obj,FuncPtr(),this->dat1,this->dat2,this->dat3,this->dat4); }
    //: Copy call.
    
    FuncT mPtr;

    inline
    FuncT FuncPtr() const
    { return mPtr; }
    //: Function.
    
    HandleT obj;
  };
  
  //! userlevel=Advanced
  //: Call method with 4 arguments
  
  template<class HandleT,class ObjT,class Data1T,class Data2T,class Data3T,class Data4T,class RetT = bool>
  class CallMethodPtr4C
    : public CallFunc4C<Data1T,Data2T,Data3T,Data4T,RetT>
  {
  public:
    typedef typename TraitsC<ObjT>::BaseTypeT BaseObjT; //: Basic type of object, ignore references and const's
    typedef typename TraitsC<Data1T>::BaseTypeT Arg1T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data2T>::BaseTypeT Arg2T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data3T>::BaseTypeT Arg3T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data4T>::BaseTypeT Arg4T; //: Basic type of arg, ignore references and const's
    typedef RetT (BaseObjT::*FuncT)(Data1T,Data2T,Data3T,Data4T);    //: Function ptr type.
    
    CallMethodPtr4C()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    CallMethodPtr4C(const HandleT &nobj,FuncT nfunc,
                    const Arg1T &ndat1 = Arg1T(),const Arg2T &ndat2 = Arg2T(),const Arg3T &ndat3 = Arg3T(),const Arg4T &ndat4 = Arg4T())
      : CallFunc4C<Data1T,Data2T,Data3T,Data4T,RetT>(*new CallMethodPtr4BodyC<HandleT,ObjT,Data1T,Data2T,Data3T,Data4T,RetT>(nobj,nfunc,ndat1,ndat2,ndat3,ndat4))
    {}
    //: Constructor.
  };

  /////////////////////////////////////////////////////////

  //! userlevel=Develop
  //: Body class for CallMethodPtr5C
  
  template<class HandleT,class ObjT,class Data1T,class Data2T,class Data3T,class Data4T,class Data5T,class RetT = bool>
  class CallMethodPtr5BodyC
    : public CallFunc5BodyC<Data1T,Data2T,Data3T,Data4T,Data5T,RetT>
  {
  public:
    typedef typename TraitsC<ObjT>::BaseTypeT BaseObjT; //: Basic type of object, ignore references and const's
    typedef typename TraitsC<Data1T>::BaseTypeT Arg1T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data2T>::BaseTypeT Arg2T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data3T>::BaseTypeT Arg3T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data4T>::BaseTypeT Arg4T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data5T>::BaseTypeT Arg5T; //: Basic type of arg, ignore references and const's
    typedef RetT (BaseObjT::*FuncT)(Data1T,Data2T,Data3T,Data4T,Data5T);    //: Function ptr type.
    
    CallMethodPtr5BodyC(const HandleT &nobj,FuncT nfunc,
                        const Arg1T &ndat1 = Arg1T(),
                        const Arg2T &ndat2 = Arg2T(),
                        const Arg3T &ndat3 = Arg3T(),
                        const Arg4T &ndat4 = Arg4T(),
                        const Arg5T &ndat5 = Arg5T())
      : CallFunc5BodyC<Data1T,Data2T,Data3T,Data4T,Data5T,RetT>((VoidFuncPtrT) 0,ndat1,ndat2,ndat3,ndat4,ndat5),
	mPtr(nfunc),
	obj(nobj)
    {}
    //: Constructor.
    
    virtual void Invoke()
    { (obj->*FuncPtr())(this->dat1,this->dat2,this->dat3,this->dat4,this->dat5); }
    //: Invoke event.

    virtual RetT Call()
    { return (obj->*FuncPtr())(this->dat1,this->dat2,this->dat3,this->dat4,this->dat5); }
    //: Invoke event.

    virtual RetT Call(Arg1T &pd)
    { return (obj->*FuncPtr())(pd,this->dat2,this->dat3,this->dat4,this->dat5); }
    //: Invoke event, with parameters.

    virtual RetT Call(Arg1T &pd1,Arg2T &pd2)
    { return (obj->*FuncPtr())(pd1,pd2,this->dat3,this->dat4,this->dat5); }
    //: Invoke event, with parameters.
    
    virtual RetT Call(Arg1T &pd1,Arg2T &pd2,Arg3T &pd3)
    { return (obj->*FuncPtr())(pd1,pd2,pd3,this->dat4,this->dat5); }
    //: Invoke event, with parameters.
    
    virtual RetT Call(Arg1T &pd1,Arg2T &pd2,Arg3T &pd3,Arg4T &pd4)
    { return (obj->*FuncPtr())(pd1,pd2,pd3,pd4,this->dat5); }
    //: Invoke event, with parameters.

    virtual RetT Call(Arg1T &pd1,Arg2T &pd2,Arg3T &pd3,Arg4T &pd4,Arg5T &pd5)
    { return (obj->*FuncPtr())(pd1,pd2,pd3,pd4,pd5); }
    //: Invoke event, with parameters.
    
  protected:
    FuncT mPtr;
    
    virtual RCBodyVC &Copy() const
    { return *new CallMethodPtr5BodyC<HandleT,ObjT,Data1T,Data2T,Data3T,Data4T,Data5T,RetT>(obj,FuncPtr(),this->dat1,this->dat2,this->dat3,this->dat4,this->dat5); }
    //: Copy call.
    
    inline
    FuncT FuncPtr() const
    { return mPtr; }
    //: Function.
    
    HandleT obj;
  };
  
  //! userlevel=Advanced
  //: Call method with 5 arguments
  
  template<class HandleT,class ObjT,class Data1T,class Data2T,class Data3T,class Data4T,class Data5T,class RetT = bool>
  class CallMethodPtr5C
    : public  CallFunc5C<Data1T,Data2T,Data3T,Data4T,Data5T,RetT>
  {
  public:
    typedef typename TraitsC<ObjT>::BaseTypeT BaseObjT; //: Basic type of object, ignore references and const's
    typedef typename TraitsC<Data1T>::BaseTypeT Arg1T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data2T>::BaseTypeT Arg2T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data3T>::BaseTypeT Arg3T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data4T>::BaseTypeT Arg4T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data5T>::BaseTypeT Arg5T; //: Basic type of arg, ignore references and const's
    typedef RetT (BaseObjT::*FuncT)(Data1T,Data2T,Data3T,Data4T,Data5T);    //: Function ptr type.
    
    CallMethodPtr5C()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    CallMethodPtr5C(const HandleT &nobj,FuncT nfunc,const Arg1T &ndat1,const Arg2T &ndat2,const Arg3T &ndat3,const Arg4T &ndat4,const Arg5T &ndat5)
      :  CallFunc5C<Data1T,Data2T,Data3T,Data4T,Data5T,RetT>(*new CallMethodPtr5BodyC<HandleT,ObjT,Data1T,Data2T,Data3T,Data4T,Data5T,RetT>(nobj,nfunc,ndat1,ndat2,ndat3,ndat4,ndat5))
    {}
    //: Constructor.
  };
  
  //! userlevel=Normal

  template<class HandleT,class ObjT,class RetT>
  inline
  CallMethodPtr0C<HandleT,ObjT,RetT> 
  TriggerPtr(const HandleT &nobj,RetT (ObjT::*nfunc)())
  { return CallMethodPtr0C<HandleT,ObjT,RetT>(nobj,nfunc); }
  //: Create a call to a method with no arguments
  // See <a href="../Tree/Ravl.API.Core.Calls.html"><b>Calls and Triggers</b></a> for details.
  
  template<class HandleT,class ObjT,class DataT,class RetT>
  inline
  CallMethodPtr1C<HandleT,ObjT,DataT,RetT> 
  TriggerPtr(const HandleT &nobj,RetT (ObjT::*nfunc)(DataT),const typename TraitsC<DataT>::BaseTypeT &defaultArg)
  { return CallMethodPtr1C<HandleT,ObjT,DataT,RetT>(nobj,nfunc,defaultArg); }
  //: Create a call to a method with 1 argument
  // See <a href="../Tree/Ravl.API.Core.Calls.html"><b>Calls and Triggers</b></a> for details.
  
  template<class HandleT,class ObjT,class Data1T,class Data2T,class RetT>
  inline
  CallMethodPtr2C<HandleT,ObjT,Data1T,Data2T,RetT> 
  TriggerPtr(const HandleT &nobj,RetT (ObjT::*nfunc)(Data1T,Data2T),
             const typename TraitsC<Data1T>::BaseTypeT &defaultArg1,
             const typename TraitsC<Data2T>::BaseTypeT &defaultArg2
             )
  { return CallMethodPtr2C<HandleT,ObjT,Data1T,Data2T,RetT>(nobj,nfunc,defaultArg1,defaultArg2); }
  //: Create a call to a method with 2 arguments
  // See <a href="../Tree/Ravl.API.Core.Calls.html"><b>Calls and Triggers</b></a> for details.
  
  template<class HandleT,class ObjT,class Data1T,class Data2T,class Data3T,class RetT>
  inline
  CallMethodPtr3C<HandleT,ObjT,Data1T,Data2T,Data3T,RetT>
  TriggerPtr(const HandleT &nobj,RetT (ObjT::*nfunc)(Data1T,Data2T,Data3T),
             const typename TraitsC<Data1T>::BaseTypeT &defaultArg1,
             const typename TraitsC<Data2T>::BaseTypeT &defaultArg2,
             const typename TraitsC<Data3T>::BaseTypeT &defaultArg3
             )
  { return CallMethodPtr3C<HandleT,ObjT,Data1T,Data2T,Data3T,RetT>(nobj,nfunc,defaultArg1,defaultArg2,defaultArg3); }
  //: Create a call to a method with 3 arguments
  // See <a href="../Tree/Ravl.API.Core.Calls.html"><b>Calls and Triggers</b></a> for details.
  
  template<class HandleT,class ObjT,class Data1T,class Data2T,class Data3T,class Data4T,class RetT>
  inline
  CallMethodPtr4C<HandleT,ObjT,Data1T,Data2T,Data3T,Data4T,RetT>
  TriggerPtr(const HandleT &nobj,RetT (ObjT::*nfunc)(Data1T,Data2T,Data3T,Data4T),
             const typename TraitsC<Data1T>::BaseTypeT &defaultArg1,
             const typename TraitsC<Data2T>::BaseTypeT &defaultArg2,
             const typename TraitsC<Data3T>::BaseTypeT &defaultArg3,
             const typename TraitsC<Data4T>::BaseTypeT &defaultArg4
             )
  { return CallMethodPtr4C<HandleT,ObjT,Data1T,Data2T,Data3T,Data4T,RetT>(nobj,nfunc,defaultArg1,defaultArg2,defaultArg3,defaultArg4); }
  //: Create a call to a method with 4 arguments
  // See <a href="../Tree/Ravl.API.Core.Calls.html"><b>Calls and Triggers</b></a> for details.

  template<class HandleT,class ObjT,class Data1T,class Data2T,class Data3T,class Data4T,class Data5T,class RetT>
  inline
  CallMethodPtr5C<HandleT,ObjT,Data1T,Data2T,Data3T,Data4T,Data5T,RetT>
  TriggerPtr(const HandleT &nobj,RetT (ObjT::*nfunc)(Data1T,Data2T,Data3T,Data4T,Data5T),
             const typename TraitsC<Data1T>::BaseTypeT &defaultArg1,
             const typename TraitsC<Data2T>::BaseTypeT &defaultArg2,
             const typename TraitsC<Data3T>::BaseTypeT &defaultArg3,
             const typename TraitsC<Data4T>::BaseTypeT &defaultArg4,
             const typename TraitsC<Data5T>::BaseTypeT &defaultArg5
             )
  { return CallMethodPtr5C<HandleT,ObjT,Data1T,Data2T,Data3T,Data4T,Data5T,RetT>(nobj,nfunc,defaultArg1,defaultArg2,defaultArg3,defaultArg4,defaultArg5); }
  //: Create a call to a method with 5 arguments
  // See <a href="../Tree/Ravl.API.Core.Calls.html"><b>Calls and Triggers</b></a> for details.
  //:-
  // Refrence methods.
  
  
}


#endif
