// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DPTYPEINFO_HEADER
#define RAVL_DPTYPEINFO_HEADER 1
////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlIO
//! docentry="Ravl.API.Core.IO.Type Converter"
//! file="Ravl/Core/IO/TypeInfo.hh"
//! author="Charles Galambos"
//! date="19/08/1998"
//! userlevel=Normal

#include "Ravl/DP/Entity.hh"
#include "Ravl/DP/Pipes.hh"
#include "Ravl/DP/Func2Stream.hh"
#include "Ravl/RCWrap.hh"
#include "Ravl/String.hh"
#include "Ravl/Hash.hh"

namespace RavlN {

  template <class DataT> class DListC;
  class DPTypeInfoC;
  
  ///////////////////////////////
  //! userlevel=Develop
  //: Type information body.
  // This base class holds meta information about C++ types. <p>
  // Including methods which will do operation with abstract
  // handles such as loading and saving from a file. See RCWrapC
  // for information about how the abstract handles work.
  
  class DPTypeInfoBodyC : public DPEntityBodyC {
  public:
    DPTypeInfoBodyC(const std::type_info &typeInfo);
    //: Default constructor.
    
    StringC Name() const { return StringC(typeInfo.name()); }
    //: Get name of type.
    
    const std::type_info & TypeInfo() const { return typeInfo; }
    //: Access type information.
    
    virtual RCWrapAbstractC Create() const;
    //: Create instance of the type with the default constructor.
    
    virtual RCWrapAbstractC Create(std::istream &strm) const;
    //: Create instance of the type from stream constructor.

    virtual UIntT Move(DPIPortBaseC &iport,DPOPortBaseC &oport,UIntT count = 1) const;
    //: Move 'count' elements from input to output.
    // return number of elements actually moved.

    virtual bool Put(DPOPortBaseC &port,const RCWrapAbstractC &obj);
    //: Put generic object to port.
    // Will throw ExceptionErrorCastC if types aren't correct, or in debug mode
    // an assert may fail.
  
    virtual RCWrapAbstractC Get(DPIPortBaseC &port);
    //: Get generic object from port.
    // Will throw ExceptionErrorCastC if types aren't correct, or in debug mode
    // an assert may fail.
    
    virtual DPIPipeBaseC CreateIPipe(const DPEntityC &hold = DPEntityC(true)) const;
    //: Create an input pipe for this type.
    
    virtual DPOPipeBaseC CreateOPipe(const DPEntityC &hold = DPEntityC(true)) const;
    //: Create an output pipe for this type.
    
    virtual bool GetAndWrite(DPIPortBaseC &port,BinOStreamC &strm) const;
    //: Get an item from the port and write it to the BinOStreamC.

    virtual Int64T GetAndWriteArray(DPIPortBaseC &port,const Int64T size,BinOStreamC &strm) const;
    //: Get an array of items from the port and write it to the BinOStreamC.

    virtual bool ReadAndPut(BinIStreamC &strm,DPOPortBaseC &port) const;
    //: Read an item from the binary stream and write it to a port.

    virtual bool ReadAndPutArray(const Int64T size,BinIStreamC &strm,DPOPortBaseC &port) const;
    //: Read an array of items from the binary stream and write it to a port.

    virtual DPIPortC<RCWrapAbstractC> CreateConvToAbstract(DPIPortBaseC &port);
    //: Convert from an IPort to an stream of abstract handles.
    
    virtual DPIPortBaseC CreateConvFromAbstract(DPIPortC<RCWrapAbstractC> &port);
    //: Convert from a stream of abstract handles to an IPort  

    virtual DPOPortC<RCWrapAbstractC> CreateConvToAbstract(DPOPortBaseC &port);
    //: Convert from an IPort to an stream of abstract handles.
    
    virtual DPOPortBaseC CreateConvFromAbstract(DPOPortC<RCWrapAbstractC> &port);
    //: Convert from a stream of abstract handles to an IPort  
    
    static HashC<const char *,DPTypeInfoC> &Types();
    //: Access type list.
    
    UIntT Hash() const { return Name().Hash(); }
    //: Calculate hash value.
  protected:
    const std::type_info &typeInfo;
  };

  
  //////////////////////////////
  //! userlevel=Normal
  //: Type information handle.
  // This base class holds meta information about C++ types. <p>
  // Including methods which will do operation with abstract
  // handles such as loading and saving from a file. See RCWrapC
  // for information about how the abstract handles work.

  class DPTypeInfoC : public DPEntityC {
  public:
    DPTypeInfoC()
      : DPEntityC(true)
    {}
    //: Default constructor.
  
    DPTypeInfoC(const std::type_info &);
    //: Lookup type. 
    // Creates an invalid handle if failed.
    
    DPTypeInfoC(DPTypeInfoBodyC &bod)
      : DPEntityC(bod)
    {}
    //: Body constructor.
    
    inline DPTypeInfoBodyC &Body() 
    { return static_cast<DPTypeInfoBodyC & > (DPEntityC::Body()); }
    //: Access body.
    
    inline const DPTypeInfoBodyC &Body() const
    { return static_cast<const DPTypeInfoBodyC & > (DPEntityC::Body()); }
    //: Access body.
    
    inline StringC Name() const 
    { return Body().Name(); }
    //: Get name of type.
    
    inline const std::type_info & TypeInfo() const 
    { return Body().TypeInfo(); }
    //: Access type information.
    
    inline RCWrapAbstractC Create() const
    { return Body().Create(); }
    //: Create instance of the type with the default constructor.
    
    inline RCWrapAbstractC Create(std::istream &in) const
    { return Body().Create(in); }
    //: Create instance of the type with the default constructor.

    inline UIntT Move(DPIPortBaseC &iport,DPOPortBaseC &oport,UIntT count = 1) const
    { return Body().Move(iport,oport,count); }
    //: Move 'count' elements from input to output.
    // return number of elements actually moved.

    inline bool Put(DPOPortBaseC &port,const RCWrapAbstractC &obj)
    { return Body().Put(port,obj); }
    //: Put generic object to port.
    // Will throw ExceptionErrorCastC if types aren't correct, or in debug mode
    // an assert may fail.
    
    inline RCWrapAbstractC Get(DPIPortBaseC &port)
    { return Body().Get(port); }
    //: Get generic object from port.
    // Will throw ExceptionErrorCastC if types aren't correct, or in debug mode
    // an assert may fail.
    
    inline DPIPipeBaseC CreateIPipe(const DPEntityC &hold = DPEntityC(true)) const
    { return Body().CreateIPipe(hold); }
    //: Create an input pipe for this type.
    
    inline DPOPipeBaseC CreateOPipe(const DPEntityC &hold = DPEntityC(true)) const
    { return Body().CreateOPipe(hold); }
    //: Create an output pipe for this type.

    inline bool GetAndWrite(DPIPortBaseC &port,BinOStreamC &strm) const
    { return Body().GetAndWrite(port,strm); }
    //: Get an item from the port and write it to the BinOStreamC.

    inline Int64T GetAndWriteArray(DPIPortBaseC &port,const Int64T size,BinOStreamC &strm) const
    { return Body().GetAndWriteArray(port,size,strm); }
    //: Get an array of items from the port and write it to the BinOStreamC.

    inline bool ReadAndPut(BinIStreamC &strm,DPOPortBaseC &port) const
    { return Body().ReadAndPut(strm,port); }
    //: Read an item from the binary stream and write it to a port.

    inline bool ReadAndPutArray(const Int64T size,BinIStreamC &strm,DPOPortBaseC &port) const
    { return Body().ReadAndPutArray(size,strm,port); }
    //: Read an item from the binary stream and write it to a port.

    inline DPIPortC<RCWrapAbstractC> CreateConvToAbstract(DPIPortBaseC &port)
    { return Body().CreateConvToAbstract(port); }
    //: Convert from an IPort to an stream of abstract handles.
    
    inline DPIPortBaseC CreateConvFromAbstract(DPIPortC<RCWrapAbstractC> &port)
    { return Body().CreateConvFromAbstract(port); }
    //: Convert from a stream of abstract handles to an IPort  

    inline DPOPortC<RCWrapAbstractC> CreateConvToAbstract(DPOPortBaseC &port)
    { return Body().CreateConvToAbstract(port); }
    //: Convert from an IPort to an stream of abstract handles.
    
    inline DPOPortBaseC CreateConvFromAbstract(DPOPortC<RCWrapAbstractC> &port)
    { return Body().CreateConvFromAbstract(port); }
    //: Convert from a stream of abstract handles to an IPort  
    
    inline UIntT Hash() const 
    { return Body().Hash(); }
    //: Calculate hash value.  
  };
  
  //////////////////////////////
  //! userlevel=Develop
  //: Type information instance body.
  // This class holds meta information about C++ types. <p>
  // Including methods which will do operation with abstract
  // handles such as loading and saving from a file. See RCWrapC
  // for information about how the abstract handles work.
  
  template<class DataT>
  class DPTypeInfoInstBodyC : public DPTypeInfoBodyC {
  public:  
    DPTypeInfoInstBodyC();
    //: Default constructor.
    
    virtual RCWrapAbstractC Create() const 
    { return RCWrapC<DataT>(); }
    //: Create instance of the type with the default constructor.
    
    virtual RCWrapAbstractC Create(std::istream &strm) const
    { return RCWrapC<DataT>(strm); }
    //: Create instance of the type from stream constructor.
    
    virtual bool Put(DPOPortBaseC &port,const RCWrapAbstractC &obj)
    {
      if(!obj.IsValid() || !port.IsValid()) {
        cerr << "DPTypeInfoInstBodyC::Put(), ERROR: Invalid arguments. \n";
        RavlAssert(0);
        return false;
      }
      DPOPortC<DataT> oport(port);
      if(!oport.IsValid()) {
        RavlAssert(0);
        throw ExceptionErrorCastC("DPTypeInfoInstBodyC::Put(), 1",
                                  port.OutputType(),
                                  typeid(DataT));
      }
      RCWrapC<DataT> wo(obj,false);
      if(!wo.IsValid()) {
        RavlAssert(0);
        throw ExceptionErrorCastC("DPTypeInfoInstBodyC::Put(), 2",
                                  obj.DataType(),
                                  typeid(DataT));
      }
      return oport.Put((DataT &) wo);
    }
    //: Put generic object to port.
    // Will throw ExceptionErrorCastC if types aren't correct, or in debug mode
    // an assert may fail.
    
    virtual RCWrapAbstractC Get(DPIPortBaseC &port) {
      if(!port.IsValid()) {
        cerr << "DPTypeInfoInstBodyC::Get(), ERROR: Invalid arguments. \n";
        RavlAssert(0);
        return RCWrapAbstractC();
      }
      DPIPortC<DataT> iport(port);
      if(!iport.IsValid()) {
        RavlAssert(0);
        throw ExceptionErrorCastC("DPTypeInfoInstBodyC::Get(), ",
                                  port.InputType(),
                                  typeid(DataT));
      }
      return RCWrapC<DataT>(iport.Get());
    }
    //: Get generic object from port.
    // Will throw ExceptionErrorCastC if types aren't correct, or in debug mode
    // an assert may fail.
    
    virtual UIntT Move(DPIPortBaseC &iport,DPOPortBaseC &oport,UIntT count = 1) const
    {
      RavlAlwaysAssert(iport.IsValid());
      RavlAlwaysAssert(oport.IsValid());
      DPIPortC<DataT> tiport(iport);
      if(!tiport.IsValid()) {
        RavlAssert(0);
        throw ExceptionErrorCastC("DPTypeInfoInstBodyC::Get(), ",
                                  iport.InputType(),
                                  typeid(DataT));
      }
      DPOPortC<DataT> toport(oport);
      if(!toport.IsValid()) {
        RavlAssert(0);
        throw ExceptionErrorCastC("DPTypeInfoInstBodyC::Put(), 1",
                                  oport.OutputType(),
                                  typeid(DataT));
      }
      UIntT ret = 0;
      for(unsigned i = 0;i < count;i++) {
        DataT tmp;
        if(!tiport.Get(tmp))
          break;
        if(!toport.Put(tmp))
          break;
        ret++;
      }
      return ret;
    }
    //: move 'count' elements from the input port to the output port.
    // returns the number of elements actually moved.

    virtual DPIPipeBaseC CreateIPipe(const DPEntityC &hold = DPEntityC(true)) const
    { return DPIPipeC<DataT>(hold); }
    //: Create an input pipe
    
    virtual DPOPipeBaseC CreateOPipe(const DPEntityC &hold = DPEntityC(true)) const
    { return DPOPipeC<DataT>(hold); }
    //: Create an output pipe

    virtual bool GetAndWrite(DPIPortBaseC &port,BinOStreamC &strm) const {
      DPIPortC<DataT> iport(port);
      if(!iport.IsValid()) {
        RavlAssert(0);
        throw ExceptionErrorCastC("DPTypeInfoInstBodyC::GetAndWrite(), ",
                                  port.InputType(),
                                  typeid(DataT));
      }
      // Check stream is valid before doing anything.
      if(!strm.Stream()) return false;
      DataT tmp;
      if(!iport.Get(tmp))
        return false;
      strm << tmp;
      return true;
    }
    //: Get an item from the port and write it to the BinOStreamC.

    virtual Int64T GetAndWriteArray(DPIPortBaseC &port, const Int64T size, BinOStreamC &strm) const
    {
      DPIPortC<DataT> iport(port);
      if (!iport.IsValid())
      {
        RavlAssert(0);
        throw ExceptionErrorCastC("DPTypeInfoInstBodyC::GetAndWriteArray(), ",
                                  port.InputType(),
                                  typeid(DataT));
      }

      if (iport.IsGetEOS())
        return 0;

      if (!strm.Stream())
      {
        std::cerr << "DPTypeInfoInstBodyC<DataT>::GetAndWriteArray stream fail" << std::endl;
        return 0;
      }

      SArray1dC<DataT> data(static_cast<size_t>(size));
      Int64T dataRead = iport.GetArray(data);
      if (dataRead > 0)
      {
        SArray1dC<DataT> subData(data, (UIntT) dataRead);
        strm << subData;
      }

      return dataRead;
    }
    //: Get an array of items from the port and write it to the BinOStreamC.

    virtual bool ReadAndPut(BinIStreamC &strm,DPOPortBaseC &port) const {
      DPOPortC<DataT> oport(port);
      if(!oport.IsValid()) {
        RavlAssert(0);
        throw ExceptionErrorCastC("DPTypeInfoInstBodyC::ReadAndPut(), ",
                                  port.OutputType(),
                                  typeid(DataT));
      }
      // Check stream is valid before doing anything.
      if(!strm.Stream()) return false;
      // Copy data.
      DataT tmp;
      strm >> tmp;
      return oport.Put(tmp);
    }
    //: Read an item from the binary stream and write it to a port.

    virtual bool ReadAndPutArray(const Int64T size,BinIStreamC &strm,DPOPortBaseC &port) const
    {
      DPOPortC<DataT> oport(port);
      if (!oport.IsValid())
      {
        RavlAssert(0);
        throw ExceptionErrorCastC("DPTypeInfoInstBodyC::ReadAndPutArray(), ",
                                  port.OutputType(),
                                  typeid(DataT));
      }

      // Check stream is valid before doing anything.
      if (!strm.Stream())
        return false;

      // Copy data.
      SArray1dC<DataT> data;
      strm >> data;
      return oport.PutArray(data) == (IntT) data.Size().V();
    }
    //: Read an array of items from the binary stream and write it to a port.

  protected:
    static RCWrapAbstractC Data2Abstract(const DataT &data)
    { return RCWrapC<DataT>(data); }
    //: Function to convert data to abstract handle.
    
    static DataT Abstract2Data(const RCWrapAbstractC &adata)
    { return RCWrapC<DataT>(adata,true).Data(); }
    //: Function to convert abstract handle to data.
    
  public:
    //: Convert from an IPort to an stream of abstract handles.
    
    virtual DPIPortC<RCWrapAbstractC> CreateConvToAbstract(DPIPortBaseC &port) 
    { return DPIPortC<DataT>(port) >> (&Data2Abstract); }
    
    //: Convert from a stream of abstract handles to an IPort  
    
    virtual DPIPortBaseC CreateConvFromAbstract(DPIPortC<RCWrapAbstractC> &port)
    { return port >> (&Abstract2Data); }

    virtual DPOPortC<RCWrapAbstractC> CreateConvToAbstract(DPOPortBaseC &port)
#if RAVL_COMPILER_VISUALCPP
    // Need to be more specific for VC++
    { return DPFuncOStreamC<RCWrapAbstractC,DataT,RCWrapAbstractC>(port,&Abstract2Data); }
#else
    { return (&Abstract2Data) >> DPOPortC<DataT>(port); }
#endif
    //: Convert from an IPort to an stream of abstract handles.
    
    virtual DPOPortBaseC CreateConvFromAbstract(DPOPortC<RCWrapAbstractC> &port)
#if RAVL_COMPILER_VISUALCPP
    // Need to be more specific for VC++
    { return DPFuncOStreamC<DataT,RCWrapAbstractC,DataT>(port,&Data2Abstract); }
#else
    { return (&Data2Abstract) >> port;  }
#endif
    //: Convert from a stream of abstract handles to an IPort  
    
  };
  

  //////////////////////////////
  //! userlevel=Normal
  //: Type information instance body.
  // This class holds meta information about C++ types. <p>
  // Including methods which will do operation with abstract
  // handles such as loading and saving from a file. See RCWrapC
  // for information about how the abstract handles work.
  
  template<class DataT>
  class DPTypeInfoInstC : public DPTypeInfoC {
  protected:
    DPTypeInfoInstC(DPTypeInfoInstBodyC<DataT> &bod)
      : DPTypeInfoC(bod)
    {}
    //: Body constructor.
    
  public:
    DPTypeInfoInstC()
    {
      if(!DPTypeInfoBodyC::Types().IsElm(typeid(DataT).name()))
        (*this) = DPTypeInfoInstC(*new DPTypeInfoInstBodyC<DataT>());
    }
    //: Default constructor.
  };
  
  //////////////////////////////////////// 
}
#include "Ravl/DP/Converter.hh"

namespace RavlN {  
  template<class DataT>
  DPTypeInfoInstBodyC<DataT>::DPTypeInfoInstBodyC()
    : DPTypeInfoBodyC(typeid(DataT))
  {
    //  static DPConverterFuncC<DataT,RCWrapAbstractC,DPTypeInfoInstBodyC<DataT>::Convert2Wrapper> ConvToWrapper(1);
    // Register wrapper converter.
  }
  
  const DPTypeInfoC &TypeInfo(const std::type_info &ti);
  //: Access extra information about a type.

  const DPTypeInfoC &TypeInfo(const char *tn);
  //: Access extra information about a type.
}
#endif
