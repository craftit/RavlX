// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_CALLMETHODS_HEADER
#define RAVL_CALLMETHODS_HEADER 1
//////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Core.Calls"
//! lib=RavlCore
//! author="Charles Galambos"
//! file="Ravl/Core/Base/CallMethods.hh"
//! date="24/01/2001"

#include "Ravl/Calls.hh"

namespace RavlN {
  
  //////////////////////////////////////////////////////
  
  //! userlevel=Develop
  //: Body class for CallMethod0C
  
  template<class ObjT,class RetT = bool>
  class CallMethod0BodyC
    : public CallFunc0BodyC<RetT>
  {
  public:
    typedef typename TraitsC<ObjT>::BaseTypeT BaseObjT; //: Basic type of object, ignore references and const's
#if !RAVL_COMPILER_VISUALCPP || RAVL_COMPILER_VISUALCPPNET_2005
    typedef RetT (BaseObjT::*FuncT)();    //: Function ptr type.
#else // Lobotomize for Visual C++
    typedef RetT (ObjT::*FuncT)();    //: Function ptr type.
#endif
    
    CallMethod0BodyC(const BaseObjT &nobj,FuncT nfunc)
      : CallFunc0BodyC<RetT>((VoidFuncPtrT) 0),
	mPtr(nfunc),
	obj(const_cast<BaseObjT &>(nobj))
    {}
    //: Constructor.
    
    virtual void Invoke()
    { (obj.*FuncPtr())(); }
    //: Invoke event.
    
    virtual RetT Call()
    { return (obj.*FuncPtr())(); }
    //: Call signal
    
  protected:
    virtual RCBodyVC &Copy() const
    { return *new CallMethod0BodyC<ObjT,RetT>(const_cast<BaseObjT &>(obj),FuncPtr()); }
    //: Copy call.
    
    FuncT mPtr;
    
    inline
    FuncT FuncPtr() const
    { return mPtr; }
    //: Function.
    
    ObjT obj;
  };
  
  //! userlevel=Advanced
  //:  Call method with no arguments
  
  template<class ObjT,class RetT = bool>
  class CallMethod0C
    : public CallFunc0C<RetT>
  {
  public:
    typedef typename TraitsC<ObjT>::BaseTypeT BaseObjT; //: Basic type of object, ignore references and const's
#if !RAVL_COMPILER_VISUALCPP || RAVL_COMPILER_VISUALCPPNET_2005
    typedef RetT (BaseObjT::*FuncT)();    //: Function ptr type.
#else // Lobotomize for Visual C++
    typedef RetT (ObjT::*FuncT)();    //: Function ptr type.
#endif
    
    CallMethod0C()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    CallMethod0C(const BaseObjT &nobj,FuncT nfunc)
      : CallFunc0C<RetT>(*new CallMethod0BodyC<ObjT,RetT>(nobj,nfunc))
    {}
    //:  Constructor.x
  };

  /////////////////////////////////////////////////////////
  
  //! userlevel=Develop
  //: Body class for CallMethod1C
  
  template<class ObjT,class DataT,class RetT = bool>
  class CallMethod1BodyC
    : public CallFunc1BodyC<DataT,RetT>
  {
  public:
    typedef typename TraitsC<ObjT>::BaseTypeT BaseObjT; //: Basic type of object, ignore references and const's
    typedef typename TraitsC<DataT>::BaseTypeT Arg1T; //: Basic type of arg, ignore references and const's
#if !RAVL_COMPILER_VISUALCPP || RAVL_COMPILER_VISUALCPPNET_2005
    typedef RetT (BaseObjT::*FuncT)(DataT);    //: Function ptr type.
#else // Lobotomize for Visual C++
    typedef RetT (ObjT::*FuncT)(DataT);    //: Function ptr type.
#endif
    
    CallMethod1BodyC(const BaseObjT &nobj,FuncT nfunc,const Arg1T &ndat)
      : CallFunc1BodyC<DataT,RetT>((VoidFuncPtrT) 0,ndat),
	mPtr(nfunc),
        obj(const_cast<BaseObjT &>(nobj))
    {}
    //: Constructor.
    
    virtual void Invoke()
    { (obj.*FuncPtr())(this->dat1); }
    //: Invoke event.

    virtual RetT Call()
    { return (obj.*FuncPtr())(this->dat1); }
    //: Invoke event, with paramiter.
    
    virtual RetT Call(Arg1T &pd)
    { return (obj.*FuncPtr())(pd); }
    //: Invoke event, with paramiter.

  protected:
    virtual RCBodyVC &Copy() const
    { return *new CallMethod1BodyC<ObjT,DataT,RetT>(const_cast<BaseObjT &>(obj),FuncPtr(),this->dat1); }
    //: Copy call.

    FuncT mPtr;
    
    inline
    FuncT FuncPtr() const
    { return mPtr; }
    //: Function.
    
    ObjT obj;
  };
  
  //! userlevel=Advanced
  //: Call method with 1 argument
  
  template<class ObjT,class DataT,class RetT = bool>
  class CallMethod1C
    : public CallFunc1C<DataT,RetT>
  {
  public:
    typedef typename TraitsC<ObjT>::BaseTypeT BaseObjT; //: Basic type of object, ignore references and const's
    typedef typename TraitsC<DataT>::BaseTypeT Arg1T; //: Basic type of arg, ignore references and const's
#if !RAVL_COMPILER_VISUALCPP || RAVL_COMPILER_VISUALCPPNET_2005
    typedef RetT (BaseObjT::*FuncT)(DataT);    //: Function ptr type.
#else // Lobotomize for Visual C++
    typedef RetT (ObjT::*FuncT)(DataT);    //: Function ptr type.
#endif
    
    CallMethod1C()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    CallMethod1C(const BaseObjT &nobj,FuncT nfunc, const Arg1T &dat = Arg1T())
      : CallFunc1C<DataT,RetT>(*new CallMethod1BodyC<ObjT,DataT,RetT>(nobj,nfunc,dat))
    {}
    //: Constructor.
  };

  /////////////////////////////////////////////////////////
  
  //! userlevel=Develop
  //: Body class for CallMethod2C
  
  template<class ObjT,class Data1T,class Data2T,class RetT = bool>
  class CallMethod2BodyC
    : public CallFunc2BodyC<Data1T,Data2T,RetT>
  {
  public:
    typedef typename TraitsC<ObjT>::BaseTypeT BaseObjT; //: Basic type of object, ignore references and const's
    typedef typename TraitsC<Data1T>::BaseTypeT Arg1T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data2T>::BaseTypeT Arg2T; //: Basic type of arg, ignore references and const's
#if !RAVL_COMPILER_VISUALCPP || RAVL_COMPILER_VISUALCPPNET_2005
    typedef RetT (BaseObjT::*FuncT)(Data1T,Data2T);    //: Function ptr type.
#else // Lobotomize for Visual C++
    typedef RetT (ObjT::*FuncT)(Data1T,Data2T);    //: Function ptr type.
#endif
    
    CallMethod2BodyC(const BaseObjT &nobj,FuncT nfunc,const Arg1T &ndat1,const Arg2T &ndat2)
      : CallFunc2BodyC<Data1T,Data2T,RetT>((VoidFuncPtrT) 0,ndat1,ndat2),
	mPtr(nfunc),
	obj(const_cast<BaseObjT &>(nobj))
    {}
    //: Constructor.
    
    virtual void Invoke()
    { (obj.*FuncPtr())(this->dat1,this->dat2); }
    //: Invoke event.
    
    virtual RetT Call()
    { return (obj.*FuncPtr())(this->dat1,this->dat2); }
    //: Invoke event, with paramiter.
    
    virtual RetT Call(Arg1T &pd)
    { return (obj.*FuncPtr())(pd,this->dat2); }
    //: Invoke event, with paramiter.

    virtual RetT Call(Arg1T &pd1,Arg2T &pd2)
    { return (obj.*FuncPtr())(pd1,pd2); }
    //: Invoke event, with paramiter.
    
  protected:
    
    inline
    FuncT FuncPtr() const
    { return mPtr; }
    //: Function.

    virtual RCBodyVC &Copy() const
    { return *new CallMethod2BodyC<ObjT,Data1T,Data2T,RetT>(const_cast<BaseObjT &>(obj),FuncPtr(),this->dat1,this->dat2); }
    //: Copy call.
    
    FuncT mPtr;
    
    ObjT obj;
  };
  
  //! userlevel=Advanced
  //: Call method with 2 arguments
  
  template<class ObjT,class Data1T,class Data2T,class RetT = bool>
  class CallMethod2C
    : public CallFunc2C<Data1T,Data2T,RetT>
  {
  public:
    typedef typename TraitsC<ObjT>::BaseTypeT BaseObjT; //: Basic type of object, ignore references and const's
    typedef typename TraitsC<Data1T>::BaseTypeT Arg1T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data2T>::BaseTypeT Arg2T; //: Basic type of arg, ignore references and const's
#if !RAVL_COMPILER_VISUALCPP || RAVL_COMPILER_VISUALCPPNET_2005
    typedef RetT (BaseObjT::*FuncT)(Data1T,Data2T);    //: Function ptr type.
#else // Lobotomize for Visual C++
    typedef RetT (ObjT::*FuncT)(Data1T,Data2T);    //: Function ptr type.
#endif
    
    CallMethod2C()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    CallMethod2C(const BaseObjT &nobj,FuncT nfunc,
		 const Arg1T &ndat1 = Arg1T(),const Arg2T &ndat2 = Arg2T())
      : CallFunc2C<Data1T,Data2T,RetT>(*new CallMethod2BodyC<ObjT,Data1T,Data2T,RetT>(nobj,nfunc,ndat1,ndat2))
    {}
    //: Default constructor.
    // Creates an invalid handle.
  };
  
  /////////////////////////////////////////////////////////
  
  //! userlevel=Develop
  //: Body class for CallMethod3C
  
  template<class ObjT,class Data1T,class Data2T,class Data3T,class RetT = bool>
  class CallMethod3BodyC
    : public CallFunc3BodyC<Data1T,Data2T,Data3T,RetT>
  {
  public:
    typedef typename TraitsC<ObjT>::BaseTypeT BaseObjT; //: Basic type of object, ignore references and const's
    typedef typename TraitsC<Data1T>::BaseTypeT Arg1T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data2T>::BaseTypeT Arg2T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data3T>::BaseTypeT Arg3T; //: Basic type of arg, ignore references and const's
#if !RAVL_COMPILER_VISUALCPP || RAVL_COMPILER_VISUALCPPNET_2005
    typedef RetT (BaseObjT::*FuncT)(Data1T,Data2T,Data3T);    //: Function ptr type.
#else // Lobotomize for Visual C++
    typedef RetT (ObjT::*FuncT)(Data1T,Data2T,Data3T);    //: Function ptr type.
#endif
    
    CallMethod3BodyC(const BaseObjT &nobj,FuncT nfunc,
		     const Arg1T &ndat1,const Arg2T &ndat2,const Arg3T &ndat3)
      : CallFunc3BodyC<Data1T,Data2T,Data3T,RetT>((VoidFuncPtrT) 0,ndat1,ndat2,ndat3),
	mPtr(nfunc),
        obj(const_cast<BaseObjT &>(nobj))
    {}
    //: Constructor.
    
    virtual void Invoke()
    { (obj.*FuncPtr())(this->dat1,this->dat2,this->dat3); }
    //: Invoke event.
    
    virtual RetT Call()
    { return (obj.*FuncPtr())(this->dat1,this->dat2,this->dat3); }
    //: Invoke event, with parameter.

    virtual RetT Call(Arg1T &pd)
    { return (obj.*FuncPtr())(pd,this->dat2,this->dat3); }
    //: Invoke event, with parameter.

    virtual RetT Call(Arg1T &pd1,Arg2T &pd2)
    { return (obj.*FuncPtr())(pd1,pd2,this->dat3); }
    //: Invoke event, with parameter.
    
    virtual RetT Call(Arg1T &pd1,Arg2T &pd2,Arg3T &pd3)
    { return (obj.*FuncPtr())(pd1,pd2,pd3); }
    //: Invoke event, with parameter.
    
  protected:
    virtual RCBodyVC &Copy() const
    { return *new CallMethod3BodyC<ObjT,Data1T,Data2T,Data3T,RetT>(const_cast<BaseObjT &>(obj),FuncPtr(),this->dat1,this->dat2,this->dat3); }
    //: Copy call.
    
    FuncT mPtr;
    
    inline
    FuncT FuncPtr() const
    { return mPtr; }
    //: Function.
    
    ObjT obj;
  };
  
  //! userlevel=Advanced
  //: Call method with 3 arguments
  
  template<class ObjT,class Data1T,class Data2T,class Data3T,class RetT = bool>
  class CallMethod3C
    : public CallFunc3C<Data1T,Data2T,Data3T,RetT>
  {
  public:
    typedef typename TraitsC<ObjT>::BaseTypeT BaseObjT; //: Basic type of object, ignore references and const's
    typedef typename TraitsC<Data1T>::BaseTypeT Arg1T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data2T>::BaseTypeT Arg2T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data3T>::BaseTypeT Arg3T; //: Basic type of arg, ignore references and const's
#if !RAVL_COMPILER_VISUALCPP || RAVL_COMPILER_VISUALCPPNET_2005
    typedef RetT (BaseObjT::*FuncT)(Data1T,Data2T,Data3T);    //: Function ptr type.
#else // Lobotomize for Visual C++
    typedef RetT (ObjT::*FuncT)(Data1T,Data2T,Data3T);    //: Function ptr type.
#endif
    
    CallMethod3C()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    CallMethod3C(const BaseObjT &nobj,FuncT nfunc,
		 const Arg1T &ndat1 = Arg1T(),const Arg2T &ndat2 = Arg2T(),const Arg3T &ndat3 = Arg3T())
      : CallFunc3C<Data1T,Data2T,Data3T,RetT>(*new CallMethod3BodyC<ObjT,Data1T,Data2T,Data3T,RetT>(nobj,nfunc,ndat1,ndat2,ndat3))
    {}
    //: Constructor.
  };
  
  /////////////////////////////////////////////////////////
  
  //! userlevel=Develop
  //: Body class for CallMethod4C
  
  template<class ObjT,class Data1T,class Data2T,class Data3T,class Data4T,class RetT = bool>
  class CallMethod4BodyC
    : public CallFunc4BodyC<Data1T,Data2T,Data3T,Data4T,RetT>
  {
  public:
    typedef typename TraitsC<ObjT>::BaseTypeT BaseObjT; //: Basic type of object, ignore references and const's
    typedef typename TraitsC<Data1T>::BaseTypeT Arg1T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data2T>::BaseTypeT Arg2T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data3T>::BaseTypeT Arg3T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data4T>::BaseTypeT Arg4T; //: Basic type of arg, ignore references and const's
#if !RAVL_COMPILER_VISUALCPP || RAVL_COMPILER_VISUALCPPNET_2005
    typedef RetT (BaseObjT::*FuncT)(Data1T,Data2T,Data3T,Data4T);    //: Function ptr type.
#else // Lobotomize for Visual C++
    typedef RetT (ObjT::*FuncT)(Data1T,Data2T,Data3T,Data4T);    //: Function ptr type.
#endif
    
    CallMethod4BodyC(const BaseObjT &nobj,FuncT nfunc,const Arg1T &ndat1,const Arg2T &ndat2,const Arg3T &ndat3,const Arg4T &ndat4)
      : CallFunc4BodyC<Data1T,Data2T,Data3T,Data4T,RetT>((VoidFuncPtrT) 0,ndat1,ndat2,ndat3,ndat4),
	mPtr(nfunc),
	obj(const_cast<BaseObjT &>(nobj))
    {}
    //: Constructor.
    
    virtual void Invoke()
    { (obj.*FuncPtr())(this->dat1,this->dat2,this->dat3,this->dat4); }
    //: Invoke event.

    virtual RetT Call()
    { return (obj.*FuncPtr())(this->dat1,this->dat2,this->dat3,this->dat4); }
    //: Invoke event.

    virtual RetT Call(Arg1T &pd)
    { return (obj.*FuncPtr())(pd,this->dat2,this->dat3,this->dat4); }
    //: Invoke event, with parameters.

    virtual RetT Call(Arg1T &pd1,Arg2T &pd2)
    { return (obj.*FuncPtr())(pd1,pd2,this->dat3,this->dat4); }
    //: Invoke event, with parameters.
    
    virtual RetT Call(Arg1T &pd1,Arg2T &pd2,Arg3T &pd3)
    { return (obj.*FuncPtr())(pd1,pd2,pd3,this->dat4); }
    //: Invoke event, with parameters.
    
    virtual RetT Call(Arg1T &pd1,Arg2T &pd2,Arg3T &pd3,Arg4T &pd4)
    { return (obj.*FuncPtr())(pd1,pd2,pd3,pd4); }
    //: Invoke event, with parameters.
    
  protected:
    virtual RCBodyVC &Copy() const
    { return *new CallMethod4BodyC<ObjT,Data1T,Data2T,Data3T,Data4T,RetT>(const_cast<BaseObjT &>(obj),FuncPtr(),this->dat1,this->dat2,this->dat3,this->dat4); }
    //: Copy call.
    
    FuncT mPtr;

    inline
    FuncT FuncPtr() const
    { return mPtr; }
    //: Function.
    
    ObjT obj;
  };
  
  //! userlevel=Advanced
  //: Call method with 4 arguments
  
  template<class ObjT,class Data1T,class Data2T,class Data3T,class Data4T,class RetT = bool>
  class CallMethod4C
    : public CallFunc4C<Data1T,Data2T,Data3T,Data4T,RetT>
  {
  public:
    typedef typename TraitsC<ObjT>::BaseTypeT BaseObjT; //: Basic type of object, ignore references and const's
    typedef typename TraitsC<Data1T>::BaseTypeT Arg1T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data2T>::BaseTypeT Arg2T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data3T>::BaseTypeT Arg3T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data4T>::BaseTypeT Arg4T; //: Basic type of arg, ignore references and const's
#if !RAVL_COMPILER_VISUALCPP || RAVL_COMPILER_VISUALCPPNET_2005
    typedef RetT (BaseObjT::*FuncT)(Data1T,Data2T,Data3T,Data4T);    //: Function ptr type.
#else // Lobotomize for Visual C++
    typedef RetT (ObjT::*FuncT)(Data1T,Data2T,Data3T,Data4T);    //: Function ptr type.
#endif
    
    CallMethod4C()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    CallMethod4C(const BaseObjT &nobj,FuncT nfunc,
		 const Arg1T &ndat1 = Arg1T(),const Arg2T &ndat2 = Arg2T(),const Arg3T &ndat3 = Arg3T(),const Arg4T &ndat4 = Arg4T())
      : CallFunc4C<Data1T,Data2T,Data3T,Data4T,RetT>(*new CallMethod4BodyC<ObjT,Data1T,Data2T,Data3T,Data4T,RetT>(nobj,nfunc,ndat1,ndat2,ndat3,ndat4))
    {}
    //: Constructor.
  };

  /////////////////////////////////////////////////////////

  //! userlevel=Develop
  //: Body class for CallMethod5C
  
  template<class ObjT,class Data1T,class Data2T,class Data3T,class Data4T,class Data5T,class RetT = bool>
  class CallMethod5BodyC
    : public CallFunc5BodyC<Data1T,Data2T,Data3T,Data4T,Data5T,RetT>
  {
  public:
    typedef typename TraitsC<ObjT>::BaseTypeT BaseObjT; //: Basic type of object, ignore references and const's
    typedef typename TraitsC<Data1T>::BaseTypeT Arg1T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data2T>::BaseTypeT Arg2T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data3T>::BaseTypeT Arg3T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data4T>::BaseTypeT Arg4T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data5T>::BaseTypeT Arg5T; //: Basic type of arg, ignore references and const's
#if !RAVL_COMPILER_VISUALCPP || RAVL_COMPILER_VISUALCPPNET_2005
    typedef RetT (BaseObjT::*FuncT)(Data1T,Data2T,Data3T,Data4T,Data5T);    //: Function ptr type.
#else // Lobotomize for Visual C++
    typedef RetT (ObjT::*FuncT)(Data1T,Data2T,Data3T,Data4T,Data5T);    //: Function ptr type.
#endif
    
    CallMethod5BodyC(const BaseObjT &nobj,FuncT nfunc,
		     const Arg1T &ndat1 = Arg1T(),
		     const Arg2T &ndat2 = Arg2T(),
		     const Arg3T &ndat3 = Arg3T(),
		     const Arg4T &ndat4 = Arg4T(),
		     const Arg5T &ndat5 = Arg5T())
      : CallFunc5BodyC<Data1T,Data2T,Data3T,Data4T,Data5T,RetT>((VoidFuncPtrT) 0,ndat1,ndat2,ndat3,ndat4,ndat5),
	mPtr(nfunc),
	obj(const_cast<BaseObjT &>(nobj))
    {}
    //: Constructor.
    
    virtual void Invoke()
    { (obj.*FuncPtr())(this->dat1,this->dat2,this->dat3,this->dat4,this->dat5); }
    //: Invoke event.

    virtual RetT Call()
    { return (obj.*FuncPtr())(this->dat1,this->dat2,this->dat3,this->dat4,this->dat5); }
    //: Invoke event.

    virtual RetT Call(Arg1T &pd)
    { return (obj.*FuncPtr())(pd,this->dat2,this->dat3,this->dat4,this->dat5); }
    //: Invoke event, with parameters.

    virtual RetT Call(Arg1T &pd1,Arg2T &pd2)
    { return (obj.*FuncPtr())(pd1,pd2,this->dat3,this->dat4,this->dat5); }
    //: Invoke event, with parameters.
    
    virtual RetT Call(Arg1T &pd1,Arg2T &pd2,Arg3T &pd3)
    { return (obj.*FuncPtr())(pd1,pd2,pd3,this->dat4,this->dat5); }
    //: Invoke event, with parameters.
    
    virtual RetT Call(Arg1T &pd1,Arg2T &pd2,Arg3T &pd3,Arg4T &pd4)
    { return (obj.*FuncPtr())(pd1,pd2,pd3,pd4,this->dat5); }
    //: Invoke event, with parameters.

    virtual RetT Call(Arg1T &pd1,Arg2T &pd2,Arg3T &pd3,Arg4T &pd4,Arg5T &pd5)
    { return (obj.*FuncPtr())(pd1,pd2,pd3,pd4,pd5); }
    //: Invoke event, with parameters.
    
  protected:
    FuncT mPtr;
    
    virtual RCBodyVC &Copy() const
    { return *new CallMethod5BodyC<ObjT,Data1T,Data2T,Data3T,Data4T,Data5T,RetT>(const_cast<BaseObjT &>(obj),FuncPtr(),this->dat1,this->dat2,this->dat3,this->dat4,this->dat5); }
    //: Copy call.
    
    inline
    FuncT FuncPtr() const
    { return mPtr; }
    //: Function.
    
    ObjT obj;
  };
  
  //! userlevel=Advanced
  //: Call method with 5 arguments
  
  template<class ObjT,class Data1T,class Data2T,class Data3T,class Data4T,class Data5T,class RetT = bool>
  class CallMethod5C
    : public  CallFunc5C<Data1T,Data2T,Data3T,Data4T,Data5T,RetT>
  {
  public:
    typedef typename TraitsC<ObjT>::BaseTypeT BaseObjT; //: Basic type of object, ignore references and const's
    typedef typename TraitsC<Data1T>::BaseTypeT Arg1T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data2T>::BaseTypeT Arg2T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data3T>::BaseTypeT Arg3T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data4T>::BaseTypeT Arg4T; //: Basic type of arg, ignore references and const's
    typedef typename TraitsC<Data5T>::BaseTypeT Arg5T; //: Basic type of arg, ignore references and const's
#if !RAVL_COMPILER_VISUALCPP || RAVL_COMPILER_VISUALCPPNET_2005
    typedef RetT (BaseObjT::*FuncT)(Data1T,Data2T,Data3T,Data4T,Data5T);    //: Function ptr type.
#else // Lobotomize for Visual C++
    typedef RetT (ObjT::*FuncT)(Data1T,Data2T,Data3T,Data4T,Data5T);    //: Function ptr type.
#endif
    
    CallMethod5C()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    CallMethod5C(const BaseObjT &nobj,FuncT nfunc,const Arg1T &ndat1,const Arg2T &ndat2,const Arg3T &ndat3,const Arg4T &ndat4,const Arg5T &ndat5)
      :  CallFunc5C<Data1T,Data2T,Data3T,Data4T,Data5T,RetT>(*new CallMethod5BodyC<ObjT,Data1T,Data2T,Data3T,Data4T,Data5T,RetT>(nobj,nfunc,ndat1,ndat2,ndat3,ndat4,ndat5))
    {}
    //: Constructor.
  };

  //! userlevel=Normal

  template<class ObjT,class RetT>
  inline
  CallMethod0C<ObjT,RetT> 
  Trigger(const ObjT &nobj,RetT (ObjT::*nfunc)())
  { return CallMethod0C<ObjT,RetT>(const_cast<ObjT &>(nobj),nfunc); }
  //: Create a call to a method with no arguments
  // See <a href="../Tree/Ravl.API.Core.Calls.html"><b>Calls and Triggers</b></a> for details.
  
  template<class ObjT,class DataT,class RetT>
  inline
  CallMethod1C<ObjT,DataT,RetT> 
  Trigger(const ObjT &nobj,RetT (ObjT::*nfunc)(DataT),const typename TraitsC<DataT>::BaseTypeT &defaultArg)
  { return CallMethod1C<ObjT,DataT,RetT>(const_cast<ObjT &>(nobj),nfunc,defaultArg); }
  //: Create a call to a method with 1 argument
  // See <a href="../Tree/Ravl.API.Core.Calls.html"><b>Calls and Triggers</b></a> for details.
  
  template<class ObjT,class Data1T,class Data2T,class RetT>
  inline
  CallMethod2C<ObjT,Data1T,Data2T,RetT> 
  Trigger(const ObjT &nobj,RetT (ObjT::*nfunc)(Data1T,Data2T),
	  const typename TraitsC<Data1T>::BaseTypeT &defaultArg1,
	  const typename TraitsC<Data2T>::BaseTypeT &defaultArg2
	  )
  { return CallMethod2C<ObjT,Data1T,Data2T,RetT>(nobj,nfunc,defaultArg1,defaultArg2); }
  //: Create a call to a method with 2 arguments
  // See <a href="../Tree/Ravl.API.Core.Calls.html"><b>Calls and Triggers</b></a> for details.
  
  template<class ObjT,class Data1T,class Data2T,class Data3T,class RetT>
  inline
  CallMethod3C<ObjT,Data1T,Data2T,Data3T,RetT>
  Trigger(const ObjT &nobj,RetT (ObjT::*nfunc)(Data1T,Data2T,Data3T),
	  const typename TraitsC<Data1T>::BaseTypeT &defaultArg1,
	  const typename TraitsC<Data2T>::BaseTypeT &defaultArg2,
	  const typename TraitsC<Data3T>::BaseTypeT &defaultArg3
	  )
  { return CallMethod3C<ObjT,Data1T,Data2T,Data3T,RetT>(nobj,nfunc,defaultArg1,defaultArg2,defaultArg3); }
  //: Create a call to a method with 3 arguments
  // See <a href="../Tree/Ravl.API.Core.Calls.html"><b>Calls and Triggers</b></a> for details.
  
  template<class ObjT,class Data1T,class Data2T,class Data3T,class Data4T,class RetT>
  inline
  CallMethod4C<ObjT,Data1T,Data2T,Data3T,Data4T,RetT>
  Trigger(const ObjT &nobj,RetT (ObjT::*nfunc)(Data1T,Data2T,Data3T,Data4T),
	  const typename TraitsC<Data1T>::BaseTypeT &defaultArg1,
	  const typename TraitsC<Data2T>::BaseTypeT &defaultArg2,
	  const typename TraitsC<Data3T>::BaseTypeT &defaultArg3,
	  const typename TraitsC<Data4T>::BaseTypeT &defaultArg4
	  )
  { return CallMethod4C<ObjT,Data1T,Data2T,Data3T,Data4T,RetT>(nobj,nfunc,defaultArg1,defaultArg2,defaultArg3,defaultArg4); }
  //: Create a call to a method with 4 arguments
  // See <a href="../Tree/Ravl.API.Core.Calls.html"><b>Calls and Triggers</b></a> for details.

  template<class ObjT,class Data1T,class Data2T,class Data3T,class Data4T,class Data5T,class RetT>
  inline
  CallMethod5C<ObjT,Data1T,Data2T,Data3T,Data4T,Data5T,RetT>
  Trigger(const ObjT &nobj,RetT (ObjT::*nfunc)(Data1T,Data2T,Data3T,Data4T,Data5T),
	  const typename TraitsC<Data1T>::BaseTypeT &defaultArg1,
	  const typename TraitsC<Data2T>::BaseTypeT &defaultArg2,
	  const typename TraitsC<Data3T>::BaseTypeT &defaultArg3,
	  const typename TraitsC<Data4T>::BaseTypeT &defaultArg4,
	  const typename TraitsC<Data5T>::BaseTypeT &defaultArg5
	  )
  { return CallMethod5C<ObjT,Data1T,Data2T,Data3T,Data4T,Data5T,RetT>(nobj,nfunc,defaultArg1,defaultArg2,defaultArg3,defaultArg4,defaultArg5); }
  //: Create a call to a method with 5 arguments
  // See <a href="../Tree/Ravl.API.Core.Calls.html"><b>Calls and Triggers</b></a> for details.
  //:-
  // Refrence methods.
  
#if !RAVL_COMPILER_VISUALCPP

  template<class ObjT,class RetT>
  inline
  CallMethod0C<ObjT &,RetT> 
  TriggerR(const ObjT &nobj,RetT (ObjT::*nfunc)())
  { return CallMethod0C<ObjT &,RetT>(nobj,nfunc); }
  //: Create a call to a method (of a referenced object) with no arguments
  // See <a href="../Tree/Ravl.API.Core.Calls.html"><b>Calls and Triggers</b></a> for details.
  
  template<class ObjT,class DataT,class RetT>
  inline
  CallMethod1C<ObjT &,DataT,RetT> 
  TriggerR(const ObjT &nobj,RetT (ObjT::*nfunc)(DataT),const typename TraitsC<DataT>::BaseTypeT &defaultArg)
  { return CallMethod1C<ObjT &,DataT,RetT>(nobj,nfunc,defaultArg); }
  //: Create a call to a method (of a referenced object) with 1 argument
  // See <a href="../Tree/Ravl.API.Core.Calls.html"><b>Calls and Triggers</b></a> for details.
  
  template<class ObjT,class Data1T,class Data2T,class RetT>
  inline
  CallMethod2C<ObjT &,Data1T,Data2T,RetT> 
  TriggerR(const ObjT &nobj,RetT (ObjT::*nfunc)(Data1T,Data2T),
	  const typename TraitsC<Data1T>::BaseTypeT &defaultArg1,
	  const typename TraitsC<Data2T>::BaseTypeT &defaultArg2
	  )
  { return CallMethod2C<ObjT &,Data1T,Data2T,RetT>(nobj,nfunc,defaultArg1,defaultArg2); }
  //: Create a call to a method (of a referenced object) with 2 arguments
  // See <a href="../Tree/Ravl.API.Core.Calls.html"><b>Calls and Triggers</b></a> for details.
  
  template<class ObjT,class Data1T,class Data2T,class Data3T,class RetT>
  inline
  CallMethod3C<ObjT &,Data1T,Data2T,Data3T,RetT>
  TriggerR(const ObjT &nobj,RetT (ObjT::*nfunc)(Data1T,Data2T,Data3T),
	  const typename TraitsC<Data1T>::BaseTypeT &defaultArg1,
	  const typename TraitsC<Data2T>::BaseTypeT &defaultArg2,
	  const typename TraitsC<Data3T>::BaseTypeT &defaultArg3
	  )
  { return CallMethod3C<ObjT &,Data1T,Data2T,Data3T,RetT>(nobj,nfunc,defaultArg1,defaultArg2,defaultArg3); }
  //: Create a call to a method (of a referenced object) with 3 arguments
  // See <a href="../Tree/Ravl.API.Core.Calls.html"><b>Calls and Triggers</b></a> for details.
  
  template<class ObjT,class Data1T,class Data2T,class Data3T,class Data4T,class RetT>
  inline
  CallMethod4C<ObjT &,Data1T,Data2T,Data3T,Data4T,RetT>
  TriggerR(const ObjT &nobj,RetT (ObjT::*nfunc)(Data1T,Data2T,Data3T,Data4T),
	  const typename TraitsC<Data1T>::BaseTypeT &defaultArg1,
	  const typename TraitsC<Data2T>::BaseTypeT &defaultArg2,
	  const typename TraitsC<Data3T>::BaseTypeT &defaultArg3,
	  const typename TraitsC<Data4T>::BaseTypeT &defaultArg4
	  )
  { return CallMethod4C<ObjT &,Data1T,Data2T,Data3T,Data4T,RetT>(nobj,nfunc,defaultArg1,defaultArg2,defaultArg3,defaultArg4); }
  //: Create a call to a method (of a referenced object) with 4 arguments
  // See <a href="../Tree/Ravl.API.Core.Calls.html"><b>Calls and Triggers</b></a> for details.

  template<class ObjT,class Data1T,class Data2T,class Data3T,class Data4T,class Data5T,class RetT>
  inline
  CallMethod5C<ObjT &,Data1T,Data2T,Data3T,Data4T,Data5T,RetT>
  TriggerR(const ObjT &nobj,RetT (ObjT::*nfunc)(Data1T,Data2T,Data3T,Data4T,Data5T),
	  const typename TraitsC<Data1T>::BaseTypeT &defaultArg1,
	  const typename TraitsC<Data2T>::BaseTypeT &defaultArg2,
	  const typename TraitsC<Data3T>::BaseTypeT &defaultArg3,
	  const typename TraitsC<Data4T>::BaseTypeT &defaultArg4,
	  const typename TraitsC<Data5T>::BaseTypeT &defaultArg5
	  )
  { return CallMethod5C<ObjT &,Data1T,Data2T,Data3T,Data4T,Data5T,RetT>(nobj,nfunc,defaultArg1,defaultArg2,defaultArg3,defaultArg4,defaultArg5); }
  //: Create a call to a method (of a referenced object) with 5 arguments
  // See <a href="../Tree/Ravl.API.Core.Calls.html"><b>Calls and Triggers</b></a> for details.
  
}

#else
  
}
// Lobotomize for Visual C++
// VC++ does not interpret BaseObjT as a class and so does not allow BaseObjT::
// The lobotomized version of FuncT useing ObjT:: does not then allow 
// templating on references so we have to deal with this separately.

#include "Ravl/CallMethodRefs.hh"

#endif


#endif
