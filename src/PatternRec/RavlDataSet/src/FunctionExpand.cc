// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlPatternRec
//! author="Charles Galambos"
//! file="Ravl/PatternRec/Modeling/Basic/FunctionExpand.cc"

#include "Ravl/PatternRec/FunctionExpand.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/DP/FileFormatStream.hh"
#include "Ravl/DP/FileFormatBinStream.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/config.h"
#include "Ravl/SArray1dIter.hh"
#include "Ravl/XMLFactoryRegister.hh"

namespace RavlN {

  //: Default constructor.
  
  FunctionExpandBodyC::FunctionExpandBodyC()
  {}

  //: Construct from XML factory

  FunctionExpandBodyC::FunctionExpandBodyC(const XMLFactoryContextC &factory)
  {
    factory.UseComponentGroup("Functions",m_functions);
    ComputeSizes();
  }

  //: Default constructor.
  
  FunctionExpandBodyC::FunctionExpandBodyC(const SArray1dC<FunctionC> & functions)
    : m_functions(functions)
  { 
    ComputeSizes();
  }

  FunctionExpandBodyC::FunctionExpandBodyC(const FunctionC & function1, const FunctionC & function2)
    : m_functions(2)
  {
    m_functions[0] = function1;
    m_functions[1] = function2;
    ComputeSizes();
  }

  FunctionExpandBodyC::FunctionExpandBodyC(const FunctionC & function1, const FunctionC & function2, const FunctionC &function3)
    : m_functions(3)
  {
    m_functions[0] = function1;
    m_functions[1] = function2;
    m_functions[2] = function3;
    ComputeSizes();
  }
  
  //! Compute input and output sizes.
  void FunctionExpandBodyC::ComputeSizes() {
    UIntT ninputSize = 0;
    UIntT noutputSize = 0;

    if(m_functions.Size() > 0) {
      ninputSize = m_functions[0].InputSize();
    }
    // work out the input and output size
    for(SArray1dIterC<FunctionC> it(m_functions);it;it++) {
      if(ninputSize != it.Data().InputSize()) {
        RavlError("Mismatch in input size. ");
        throw RavlN::ExceptionBadConfigC("Mismatch in input size.");
      }
      noutputSize += it.Data().OutputSize();
    }

    InputSize(ninputSize);
    OutputSize(noutputSize);
  }

  
  //: Load from stream.
  
  FunctionExpandBodyC::FunctionExpandBodyC(std::istream &strm)
    : FunctionBodyC(strm)
  { strm >> m_functions; }
  
  //: Load from binary stream.
  
  FunctionExpandBodyC::FunctionExpandBodyC(BinIStreamC &strm)
    : FunctionBodyC(strm)
  { strm >> m_functions; }
  
  //: Writes object to stream.
  
  bool FunctionExpandBodyC::Save (std::ostream &out) const {
    if(!FunctionBodyC::Save(out))
      return false;
    out << ' ' << m_functions;
    return true;    
  }
  
  //: Writes object to binary stream.
  
  bool FunctionExpandBodyC::Save (BinOStreamC &out) const {
    if(!FunctionBodyC::Save(out))
      return false;
    out << m_functions;
    return true;
  }
  
  //: Apply function to 'data'
  
  VectorC FunctionExpandBodyC::Apply(const VectorC &data) const {
    // create space for level 2 vec
    VectorC vec(OutputSize());
    
    UIntT outputIndex = 0;
    for(SArray1dIterC<FunctionC> it(m_functions);it;it++) {
      VectorC outputVec = it.Data().Apply(data);
      
      // copy to our output vector
      for(SArray1dIterC<RealT>vecIt(outputVec);vecIt;vecIt++) {
        vec[outputIndex] = *vecIt;
        outputIndex++;
      }
    }
        
    return vec;
  }
  
  ///////////////////////////////////////////////////////////
  
  
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(FunctionExpandBodyC,FunctionExpandC,FunctionC);

  void LinkFunctionExpand()
  {}

  static RavlN::XMLFactoryRegisterHandleC<RavlN::FunctionExpandC> g_registerXMLFactoryFunctionCascade("RavlN::FunctionExpandC");

}
