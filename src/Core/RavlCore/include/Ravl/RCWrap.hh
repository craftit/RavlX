// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_WRAP_HEADER
#define RAVL_WRAP_HEADER 1
//////////////////////////////////////////////////////////
//! docentry="Ravl.API.Core.Reference Counting"
//! file="Ravl/Core/Base/RCWrap.hh"
//! lib=RavlCore
//! userlevel=Normal
//! author="Charles Galambos"
//! date="06/05/1998"

#include "Ravl/RCHandleV.hh"
#include "Ravl/RCAbstract.hh"
#include "Ravl/DeepCopy.hh"
#include "Ravl/Types.hh"
#include "Ravl/BinStream.hh"

#if RAVL_HAVE_RTTI
#if RAVL_HAVE_ANSICPPHEADERS
#include <typeinfo>
#else
#include <typeinfo.h>
#endif
#endif

//: Ravl library namespace.

namespace RavlN {

  class BinOStreamC;

  //! userlevel=Develop
  //: RCWrapped object base class.

  class RCWrapBaseBodyC
    : public RCBodyVC
  {
  public:
    RCWrapBaseBodyC()
    {}
    //: Default constructor.

    RCWrapBaseBodyC(BinIStreamC &strm)
      : RCBodyVC(strm)
    {}
    //: Stream constructor.

    RCWrapBaseBodyC(std::istream &strm)
      : RCBodyVC(strm)
    {}
    //: Stream constructor.

    virtual bool Save(std::ostream &strm) const;
    //: Save to text stream.

    virtual bool Save(BinOStreamC &strm) const;
    //: Save to binary stream.

    virtual void *DataVoidPtr()
    { return 0; }
    //: Access void pointer to data member.

    virtual const void *DataVoidPtr() const
    { return 0; }
    //: Access void pointer to data member.

#if RAVL_HAVE_RTTI
    virtual const std::type_info &DataType() const;
    //: Get type of wrapped object.
#endif
  };

  //! userlevel=Advanced
  //: Abstract wrapped object handle.

  class RCWrapAbstractC
    : public RCHandleVC<RCWrapBaseBodyC>
  {
  public:
    RCWrapAbstractC()
    {}
    //: Default constructor.
    // Creates an invalid handle.

    RCWrapAbstractC(const RCAbstractC &val)
      : RCHandleVC<RCWrapBaseBodyC>(val)
    {}
    //:  Constructor from an abstract.

  protected:
    RCWrapAbstractC(RCWrapBaseBodyC &bod)
      : RCHandleVC<RCWrapBaseBodyC>(bod)
    {}
    //: Body constructor.

    RCWrapAbstractC(const RCWrapBaseBodyC *bod)
      : RCHandleVC<RCWrapBaseBodyC>(bod)
    {}
    //: Body constructor.

    RCWrapBaseBodyC &Body()
    { return RCHandleC<RCWrapBaseBodyC>::Body(); }
    //: Access body of object.

    const RCWrapBaseBodyC &Body() const
    { return RCHandleC<RCWrapBaseBodyC>::Body(); }
    //: Access body of object.

  public:

#if RAVL_HAVE_RTTI
    const std::type_info &DataType() const
    { return Body().DataType(); }
    //: Get type of wrapped object.
#endif
    void *DataVoidPtr()
    { return Body().DataVoidPtr(); }
    //: Access void pointer to data member.

    const void *DataVoidPtr() const
    { return Body().DataVoidPtr(); }
    //: Access void pointer to data member.

    template<class DataT>
    bool GetPtr(DataT *&value);
    //: Get pointer to value if type matches.
    // Returns true if the value has been retrieved
  };

  //! userlevel=Develop
  //: RCWrapper body.

  template<class DataT>
  class RCWrapBodyC
    : public RCWrapBaseBodyC
  {
  public:
    RCWrapBodyC()
    {}
    //: Default constructor.

    explicit RCWrapBodyC(const DataT &val)
      : data(val)
    {}
    //: Constructor.

    RCWrapBodyC(std::istream &in)
      : RCWrapBaseBodyC(in)
    { in >> data; }
    //: Construct from a stream.
    // See RCWrapIO for implementation with full IO.

    RCWrapBodyC(BinIStreamC &in)
      : RCWrapBaseBodyC(in)
    { in >> data; }
    //: Construct from a stream.
    // See RCWrapIO for implementation with full IO.

    virtual bool Save(std::ostream &strm) const {
      if(!RCWrapBaseBodyC::Save(strm))
        return false;
      strm << data;
      //RavlAssertMsg(0,"Not implemented, use RCWrapIOC for IO support. ");
      return true;
    }
    //: Save to text stream.
    // To avoid wrapped classes having to support a
    // IO implementation this method does nothing.
    // See RCWrapIO for implementation with full IO.

    virtual bool Save(BinOStreamC &strm) const {
      if(!RCWrapBaseBodyC::Save(strm))
        return false;
      strm << data;
      //RavlAssertMsg(0,"Not implemented, use RCWrapIOC for IO support. ");
      return true;
    }
    //: Save to binary stream.
    // To avoid wrapped classes having to support a
    // IO implementation this method does nothing.
    // See RCWrapIO for implementation with full IO.

#if 0
    virtual RCBodyVC &Copy() const
    { return *new RCWrapBodyC<DataT>(data); }
    //: Make copy of body.
    // DISABLED.

    virtual RCBodyC &DeepCopy(UIntT levels = ((UIntT) -1)) const {
      switch(levels) {
      case 0: return const_cast<RCWrapBodyC<DataT> &>(*this);
      case 1: return Copy();
      case 2: return *new RCWrapBodyC<DataT>(StdCopy(data));
      default: break;
      }
      return *new RCWrapBodyC<DataT>(StdDeepCopy(data,levels-1));
    }
    //: Make a deep copy of body.
    // DISABLED. The use of a virtual function would force
    // all wrapped classes to implement a DeepCopy() operator of
    // some form.  This would cause undue overhead when using this
    // class, as providing a DeepCopy() may not always be appropriate.

#endif

    DataT &Data()
    { return data; }
    //: Access data.

    const DataT &Data() const
    { return data; }
    //: Access data.

    virtual void *DataVoidPtr()
    { return &data; }
    //: Access void pointer to data member.
    
    virtual const void *DataVoidPtr() const
    { return &data; }
    //: Access void pointer to data member.

#if RAVL_HAVE_RTTI
    virtual const std::type_info &DataType() const
    { return typeid(DataT); }
    //: Get type of wrapped object.
#endif

  protected:
    DataT data;
  };


  //! userlevel=Advanced
  //: RCWrapper handle.

  template<class DataT>
  class RCWrapC
    : public RCWrapAbstractC
  {
  public:
    RCWrapC()
    {}
    //: Default constructor.
    // Creates an invalid handle.

    RCWrapC(bool makebod,bool){
      if(makebod)
        *this = RCWrapC(DataT());
    }
    //: Default constructor.
    // Creates an invalid handle.

    explicit RCWrapC(const DataT &dat)
      : RCWrapAbstractC(*new RCWrapBodyC<DataT>(dat))
    {}
    //: Construct from an instance.
    // Uses the copy constructor to create a reference
    // counted copy of 'dat.

    RCWrapC(const RCWrapAbstractC &val,bool v)
      : RCWrapAbstractC(dynamic_cast<const RCWrapBodyC<DataT> *>(BodyPtr(val)))
    {}
    //: Construct from an abstract handle.
    // if the object types do not match, an invalid handle
    // is created.

    RCWrapC(const RCAbstractC &val,bool v)
      : RCWrapAbstractC(dynamic_cast<const RCWrapBodyC<DataT> *>(val.BodyPtr()))
    {}
    //: Construct from an abstract handle.

    RCWrapC(std::istream &in)
      : RCWrapAbstractC(*new RCWrapBodyC<DataT>(in))
    {}
    //: Construct from a stream.

    RCWrapC(BinIStreamC &in)
      : RCWrapAbstractC(*new RCWrapBodyC<DataT>(in))
    {}
    //: Construct from a stream.

  protected:
    RCWrapC(RCWrapBodyC<DataT> &bod)
      : RCWrapAbstractC(bod)
    {}
    //: Body constructor.

    RCWrapBodyC<DataT> &Body()
    { return static_cast<RCWrapBodyC<DataT> &>(RCWrapAbstractC::Body()); }
    //: Body access.

    const RCWrapBodyC<DataT> &Body() const
    { return static_cast<const RCWrapBodyC<DataT> &>(RCWrapAbstractC::Body()); }
    //: Constant body access.

  public:
    RCWrapC<DataT> Copy() const
    { return RCWrapC<DataT>(Body().Data()); }
    //: Make a copy of this handle.
    // NB. This assumes the wrapped object is SMALL, and so
    // just using the copy constructor is sufficient.

    RCWrapC<DataT> DeepCopy(UIntT levels = ((UIntT) -1)) const {
      switch(levels) {
      case 0: return *this;
      case 1: return Copy();
      case 2: return RCWrapC<DataT>(StdCopy(Body().Data()));
      default: break;
      }
      levels--;
      return RCWrapC<DataT>(StdDeepCopy(Body().Data(),levels));
    }
    //: Make a copy of this handle.

    DataT &Data()
    { return Body().Data(); }
    //: Access data.

    const DataT &Data() const
    { return Body().Data(); }
    //: Access data.

    operator DataT &()
    { return Body().Data(); }
    //: Default conversion to data type.

    operator const DataT &() const
    { return Body().Data(); }
    //: Default conversion to data type.
  };

  template<typename DataT>
  RCWrapAbstractC RCWrap(const DataT &val)
  { return RCWrapC<DataT>(val); }
  //: Helper function to Wrap a value.


  //! Convert to a RCAbstract handle
  template<typename DataT>
  inline RCAbstractC ToRCAbstract(const DataT &value) {
    RavlN::RCWrapC<DataT> wrap(value);
    return wrap.Abstract();
  }

  //! Convert from a RCAbstract handle
  template<typename DataT>
  inline void FromRCAbstract(const RavlN::RCAbstractC &val,DataT &value) {
    RavlN::RCWrapC<DataT> wrap(val,true);
    RavlAssert(wrap.IsValid());
    value = wrap.Data();
  }

  // ----------------------------------------------------------

  //: Get pointer to value if type matches.
  template<class DataT>
  bool RCWrapAbstractC::GetPtr(DataT *&value) {
    RCWrapBodyC<DataT> *theBody = dynamic_cast<RCWrapBodyC<DataT> *>(BodyPtr());
    if(theBody == 0)
      return false;
    value = &theBody->Data();
    return true;
  }

}



#endif
