// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DPCONVERTER_HEADER
#define RAVL_DPCONVERTER_HEADER 1
/////////////////////////////////////////////////////////
//! docentry="Ravl.API.Core.IO.Type Converter"
//! lib=RavlIO
//! file="Ravl/Core/IO/Converter.hh"
//! author="Charles Galambos"
//! date="17/08/1998"
//! userlevel=Advanced

// This header contains the classes need to define a single type
// conversion.  The classes for handling a type conversion
// graph can be found in TypeConverterC.

#include "Ravl/Types.hh"
#include "Ravl/String.hh"
#include "Ravl/DP/Port.hh"
#include "Ravl/DP/Process.hh"
#include "Ravl/DP/ProcInfo.hh"
#include "Ravl/DP/FuncP2Proc.hh"
#include "Ravl/DP/ProcIStream.hh"
#include "Ravl/DP/ProcOStream.hh"
#include "Ravl/DP/TypeInfo.hh"
#include "Ravl/FunctionRegister.hh"

namespace RavlN {
  class DPConverterBaseC;  
  class TypeConverterBodyC;
  
  extern void DPIncludeStdConverters();
  
  ////////////////////////////////////
  //! userlevel=Develop
  //: Converted base body class
  // This base body class provides an abstract interface
  // to a particular type conversion.
  
  class DPConverterBaseBodyC 
    : public DPProcInfoBodyC 
  {
  public:
    DPConverterBaseBodyC(const std::type_info &ninput_type,const std::type_info &noutput_type,RealT ncost = 1);
    //: Constructor.
    
    virtual IntT Args() const;
    //: Number of args for process.
    virtual const std::type_info &ArgType(IntT i) const;
    //: Type of args.
    
    virtual const std::type_info &Output() const;
    //: Output type.
    
    using DPProcInfoBodyC::Apply;

    RCAbstractC Apply(const RCAbstractC &dat);
    //: Apply conversion.

    RealT Cost() const
    { return cost; }
    //: Cost of conversion.
    
  protected:
    
    const std::type_info &input_type;
    const std::type_info &output_type;
    RealT cost; // Conversion cost.
    
    DPProcessBaseC proc; 
    // Instance of processes.
    // Since all conversion processes should be stateless.
  };
  
  ////////////////////////////
  //! userlevel=Normal
  //: Converter base class
  // This base class provides an abstract interface
  // to a particular type conversion.
  
  class DPConverterBaseC 
    : public DPProcInfoC 
  {
  public:
    DPConverterBaseC() 
    {}
    //: Default Constructor.
    
    template<class InT,class OutT>
    DPConverterBaseC(OutT (*func)(const InT &in),RealT ncost = 1);
    //: Function Constructor.
    
    static bool IsA(const DPEntityC &obj)
    { return dynamic_cast<const DPConverterBaseBodyC *>(&(obj.Body())) != 0; }
    //: Is widget a menu ?
    
  protected:
    DPConverterBaseC(DPConverterBaseBodyC &bod) 
      : DPProcInfoC(bod)
    {}
    //: Body Constructor.
    
    inline DPConverterBaseBodyC &Body() 
    { return static_cast<DPConverterBaseBodyC & > (DPEntityC::Body()); }
    //: Access body.
    
    inline const DPConverterBaseBodyC &Body() const
    { return static_cast<const DPConverterBaseBodyC & > (DPEntityC::Body()); }
    //: Access body.
    
  public:
    inline RealT Cost() const { return Body().Cost(); }
    //: Get cost of conversion.
    
    inline RCAbstractC Apply(const RCAbstractC &dat)
    { return Body().Apply(dat); }
    //: Apply conversion.
    
    friend class DPConverterBaseBodyC;
    friend class TypeConverterBodyC;
  };
  
  ///////////////////////////////////
  //! userlevel=Develop
  //: Data conversion body.
  // The actualy implements the conversion between two types.
  
  template<class ProcT>
  class DPConverterBodyC 
    : public DPConverterBaseBodyC 
  {
  public:
    typedef typename ProcT::ProcInputT InT;
    typedef typename ProcT::ProcOutputT OutT;
    
    DPConverterBodyC(const ProcT &inst,RealT ncost = 1);
    //: Constructor.
    
    virtual const std::type_info &ProcType() const
    { return typeid(ProcT); }
    //: Get type of process
    
    virtual DPProcessBaseC CreateProc() const { 
      if(conv.IsStateless()) // Do we need to make a copy.
	return conv; 
      return conv.Copy();
    }
    //: Create an instance of this converter.
    
    virtual DPIPortBaseC CreateIStream(const DPIPortBaseC &inp) const
    { return DPProcIStreamC<InT,OutT>(DPProcessC<InT,OutT>(CreateProc()),DPIPortC<InT>(inp)); }
    //: Create instance of process.
    
    virtual DPOPortBaseC CreateOStream(const DPOPortBaseC &outp) const
    { return DPProcOStreamC<InT,OutT>(DPProcessC<InT,OutT>(CreateProc()),DPOPortC<OutT>(outp)); }
    //: Create instance of process.
    
    virtual RCAbstractC Apply(const DPProcessBaseC &proc,SArray1dC<RCAbstractC> dat) {
      DPProcessC<InT,OutT> p(proc);
      if(!p.IsValid())
        throw ExceptionC("Apply(), Bad process handle. ");
      if(dat.Size() != 1)
        throw ExceptionC("Apply(), Wrong number of args. ");
      return RCWrapC<OutT>(p.Apply(RCWrapC<InT>(dat[0],true).Data())).Abstract();
    }
    //: Apply process using abstract data handle.
    // These should be RCWrap's of the actual data.
    
    virtual RCAbstractC Apply1(const DPProcessBaseC &proc,const RCAbstractC &arg) {
      DPProcessC<InT,OutT> p(proc);
      if(!p.IsValid())
	throw ExceptionC("Apply(), Bad process handle. ");
      return RCWrapC<OutT>(p.Apply(RCWrapC<InT>(arg,true).Data())).Abstract();
    }
    //: Apply process using abstract data handle.
    // Use only is you know the process uses 1 arg ! 
    
  protected:
    ProcT conv; // Instance of converter.
  };
  
  ///////////////////////////////////////////////////////////////
  
  //! userlevel=Normal
  //: Type conversion handle.
  // Handle for the class that actually implements the 
  // conversion between two types.
  
  template<class ProcT>
  class DPConverterC : public DPConverterBaseC {
  public:
    DPConverterC() 
    {}
    //: Default constructor.
    
    DPConverterC(DPConverterBaseBodyC &bod)
      : DPConverterBaseC(bod)
    {}
    //: Body constructor.
  };
  
  /////////////////////////////////////////////////
  
  //! userlevel=Normal
  //: Setup data conversion function.
  // This uses a pointer to a function to implement a type converter.
  
  template<class InT,class OutT>
  class DPConverterFuncC 
    : public DPConverterC<DPProcessC<InT,OutT> >
  {
  public:
    DPConverterFuncC(OutT (*func)(const InT &in),RealT ncost = 1,const char *funcName = 0)
      : DPConverterC<DPProcessC<InT,OutT> > (*new DPConverterBodyC<DPProcessC<InT,OutT> > (DPFuncP2ProcC<InT,OutT>(func)
											   ,ncost))
    {
      if(funcName != 0)
	RegisterFunction(funcName,func);
    }
    //: Default constructor. 
  };

  
  /////////////////////////////////////////////////
  
  template<class ProcT>
  DPConverterBodyC<ProcT>::DPConverterBodyC(const ProcT &inst,RealT ncost)
    : DPConverterBaseBodyC(typeid(InT),typeid(OutT),ncost),
      conv(inst)
  {
    RavlAssertMsg(ncost >= 1.0,"Costs are multiplicative and should always be greater than or equal to 1.");

    static DPTypeInfoInstC<InT> inType;
    static DPTypeInfoInstC<OutT> outType;
    //static DPConverterFuncC<OutT,RCWrapBaseC,DPConverterBodyC<ProcT>::Convert2Wrapper> ConvToWrapper(1);
  }
  
  //: Function Constructor.
  
  template<class InT,class OutT>
  DPConverterBaseC::DPConverterBaseC(OutT (*func)(const InT &in),RealT ncost)
    : DPProcInfoC(DPConverterFuncC<InT,OutT>(func,ncost))
  {}
  
  template<class InT,class OutT>
  DPConverterFuncC<InT,OutT> RegisterConversion(OutT (*func)(const InT &in),RealT ncost = 1,const char *funcName = 0) { 
    if(funcName != 0)
      RegisterFunction(funcName,func);
    return DPConverterFuncC<InT,OutT>(func,ncost); 
  }
  //: Register a conversion function.

#if !RAVL_HAVE_TEMPLATEDFUNCPTR_BUG
#define DP_REGISTER_CONVERSION(func,cost) \
  static RavlN::DPConverterBaseC DPConv_ ## func(RavlN::RegisterConversion(func,cost));
#define DP_REGISTER_CONVERSION_NAMED(func,cost,fname) \
  static RavlN::DPConverterBaseC DPConv_ ## func(RavlN::RegisterConversion(func,cost,fname));
#else
  // Labotomise automatic type conversion.
  // Where the conversion is required on use DP_REGISTER_CONVERSION_FT
#define DP_REGISTER_CONVERSION(func,cost)
#define DP_REGISTER_CONVERSION_NAMED(func,cost,fname)
#endif

  // Fixed type conversion macro
#define DP_REGISTER_CONVERSION_FT(InT,OutT,func,cost) \
  RavlN::DPConverterFuncC<InT,OutT > DPConv_ ## func(func,cost);

#define DP_REGISTER_CONVERSION_FT_NAMED(InT,OutT,func,cost,fname) \
  RavlN::DPConverterFuncC<InT,OutT > DPConv_ ## func(func,cost,fname);

}
#endif
