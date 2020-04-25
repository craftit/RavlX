// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlIO
//! file="Ravl/Core/IO/Converter.cc"

#include "Ravl/DP/Converter.hh"
#include "Ravl/DP/DataConv.hh"
#include "Ravl/DP/TypeConverter.hh"
#include "Ravl/GraphConnIt.hh"
#include "Ravl/HSet.hh"
#include "Ravl/String.hh"
#include "Ravl/SArray2d.hh"
#include "Ravl/Array2d.hh"

#define DPDEBUG 0

#if DPDEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  ///////////////////////
  //: Constructor.
  
  DPConverterBaseBodyC::DPConverterBaseBodyC(const std::type_info &ninput_type,const std::type_info &noutput_type,RealT ncost)
    : input_type(ninput_type),
      output_type(noutput_type),
      cost(ncost)
  { 
    ONDEBUG(cout << "Registering converter: "<<input_type.name() << " to " << output_type.name() << " with cost " << cost << endl);
    DPConverterBaseC me(*this);
    SystemTypeConverter().Insert(me);
  }
  
  ///////////////////////////
  //: Number of args for process.
  
  IntT DPConverterBaseBodyC::Args() const { 
    return 1; 
  }
  
  ////////////////////////////
  //: Type of args.
  
  const std::type_info &DPConverterBaseBodyC::ArgType(IntT i) const {
    if(i != 0)
      return typeid(void); 
    return input_type;
  }
  
  ////////////////////////////
  //: Output type.
  
  const std::type_info &DPConverterBaseBodyC::Output() const { 
    return output_type; 
  }
  
  
  ///////////////////////////
  //: Apply conversion.
  
  RCAbstractC DPConverterBaseBodyC::Apply(const RCAbstractC &dat) {
    if(!proc.IsValid())
      proc = CreateProc();
    RavlAssert(proc.IsValid());
    return Apply1(proc,dat);
  }
  
  /////////////////////////////////////////////////////////
  
  static IntT ConvByte2Int(const ByteT &v)
  { return (IntT) v; }

  static IntT ConvUByte2Int(const UByteT &v)
  { return (IntT) v; }

  static IntT ConvInt162Int(const Int16T &v)
  { return (IntT) v; }
  
  static RealT ConvInt2Real(const IntT &v)
  { return (RealT) v; }

  static Int64T ConvInt2Int64(const IntT &v)
  { return v; }

  static UInt64T ConvUInt2UInt64(const UIntT &v)
  { return v; }

  static RealT ConvUInt2Real(const UIntT &v)
  { return (RealT) v; }

  static RealT ConvFloat2Real(const float &v)
  { return (RealT) v; }

  static float ConvReal2Float(const RealT &v)
  { return v; }

  static StringC ConvInt2String(const IntT &v)
  { return StringC(v); }

  static StringC ConvReal2String(const RealT &v)
  { return StringC(v); }

  static SArray1dC<RealT> ConvSArray1d_Float2Array1dC_Real(const SArray1dC<float> &arr)
  {
    SArray1dC<RealT> ret(arr.Size());
    for(unsigned i = 0;i < ret.Size();i++)
      ret[i] = arr[i];
    return ret;
  }

  static SArray2dC<RealT> ConvSArray2d_Float2Array2dC_Real(const SArray2dC<float> &arr)
  {
    SArray2dC<RealT> ret(arr.Size1(),arr.Size2());
    for(unsigned i = 0;i < ret.Size1();i++)
      for(unsigned j = 0;j < ret.Size2();j++)
        ret[i][j] = arr[i][j];
    return ret;
  }

  static Array2dC<float> ConvSArray2d_FloatArray2dC_Float(const SArray2dC<float> &arr)
  { return arr; }

  DP_REGISTER_CONVERSION_NAMED(ConvByte2Int,1,"IntT RavlN::Convert(const ByteT &)");
  DP_REGISTER_CONVERSION_NAMED(ConvUByte2Int,1,"IntT RavlN::Convert(const UByteT &)");
  DP_REGISTER_CONVERSION_NAMED(ConvInt162Int,1,"IntT RavlN::Convert(const Int16T &)");
  DP_REGISTER_CONVERSION_NAMED(ConvInt2Int64,1,"Int64T RavlN::Convert(const IntT &)");
  DP_REGISTER_CONVERSION_NAMED(ConvUInt2UInt64,1,"UInt64T RavlN::Convert(const UIntT &)");
  DP_REGISTER_CONVERSION_NAMED(ConvInt2Real,1,"RealT RavlN::Convert(const IntT &)");
  DP_REGISTER_CONVERSION_NAMED(ConvFloat2Real,1,"RealT RavlN::Convert(const float &)");
  DP_REGISTER_CONVERSION_NAMED(ConvReal2Float,2,"float RavlN::Convert(const RealT &)");
  DP_REGISTER_CONVERSION_NAMED(ConvUInt2Real,1,"RealT RavlN::Convert(const UIntT &)");
  DP_REGISTER_CONVERSION_NAMED(ConvInt2String,1,"StringC RavlN::Convert(const IntT &)");
  DP_REGISTER_CONVERSION_NAMED(ConvReal2String,1,"StringC RavlN::Convert(const RealT &)");
  DP_REGISTER_CONVERSION(ConvSArray1d_Float2Array1dC_Real,1);
  DP_REGISTER_CONVERSION(ConvSArray2d_Float2Array2dC_Real,1);
  DP_REGISTER_CONVERSION(ConvSArray2d_FloatArray2dC_Float,1);
  

}
