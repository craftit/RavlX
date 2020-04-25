// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlPatternRec
//! author="Charles Galambos"
//! file="Ravl/PatternRec/Modeling/Basic/FunctionConcatenate.cc"

#include "Ravl/PatternRec/FunctionConcatenate.hh"
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
  
  FunctionConcatenateBodyC::FunctionConcatenateBodyC()
  {}

  //: Construct from XML factory

  FunctionConcatenateBodyC::FunctionConcatenateBodyC(const XMLFactoryContextC &factory)
  {
    factory.UseComponentGroup("Functions",m_functions);
    ComputeSizes();
  }

  //: Default constructor.
  
  FunctionConcatenateBodyC::FunctionConcatenateBodyC(const SArray1dC<FunctionC> & functions)
    : m_functions(functions)
  { 
    ComputeSizes();
  }

  FunctionConcatenateBodyC::FunctionConcatenateBodyC(const FunctionC & function1, const FunctionC & function2)
    : m_functions(2)
  {
    m_functions[0] = function1;
    m_functions[1] = function2;
    ComputeSizes();
  }

  FunctionConcatenateBodyC::FunctionConcatenateBodyC(const FunctionC & function1, const FunctionC & function2, const FunctionC &function3)
    : m_functions(3)
  {
    m_functions[0] = function1;
    m_functions[1] = function2;
    m_functions[2] = function3;
    ComputeSizes();
  }
  
  //! Compute input and output sizes.
  void FunctionConcatenateBodyC::ComputeSizes() {
    UIntT inputSize = 0;
    UIntT outputSize = 0;

    // work out the input and output size
    for(SArray1dIterC<FunctionC> it(m_functions);it;it++) {
      inputSize += it.Data().InputSize();
      outputSize += it.Data().OutputSize();
    }

    InputSize(inputSize);
    OutputSize(outputSize);
  }

  
  //: Load from stream.
  
  FunctionConcatenateBodyC::FunctionConcatenateBodyC(std::istream &strm) 
    : FunctionBodyC(strm)
  { strm >> m_functions; }
  
  //: Load from binary stream.
  
  FunctionConcatenateBodyC::FunctionConcatenateBodyC(BinIStreamC &strm) 
    : FunctionBodyC(strm)
  { strm >> m_functions; }
  
  //: Writes object to stream.
  
  bool FunctionConcatenateBodyC::Save (std::ostream &out) const {
    if(!FunctionBodyC::Save(out))
      return false;
    out << ' ' << m_functions;
    return true;    
  }
  
  //: Writes object to binary stream.
  
  bool FunctionConcatenateBodyC::Save (BinOStreamC &out) const {
    if(!FunctionBodyC::Save(out))
      return false;
    out << m_functions;
    return true;
  }
  
  //: Apply function to 'data'
  
  VectorC FunctionConcatenateBodyC::Apply(const VectorC &data) const {


    // create space for level 2 vec
    VectorC vec(OutputSize());
    
    UIntT inputIndex = 0;
    UIntT outputIndex = 0;

    for(SArray1dIterC<FunctionC> it(m_functions);it;it++) {

      // do first level projection
      UIntT vecSize = it.Data().InputSize();      

      // FIXME: Can do this lots more efficiently
      // copy across input data
      VectorC inputVec(vecSize);
      for(SArray1dIterC<RealT>vecIt(inputVec);vecIt;vecIt++) {
        *vecIt = data[inputIndex];
        inputIndex++;
      }
      
      VectorC outputVec = it.Data().Apply(inputVec);                                    
      
      // copy to our output vector
      for(SArray1dIterC<RealT>vecIt(outputVec);vecIt;vecIt++) {
        vec[outputIndex] = *vecIt;
        outputIndex++;
      }

    }
        
    return vec;
  }
  
  ///////////////////////////////////////////////////////////
  
  
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(FunctionConcatenateBodyC,FunctionConcatenateC,FunctionC);

  void LinkConcatenate()
  {}

  static RavlN::XMLFactoryRegisterHandleC<RavlN::FunctionConcatenateC> g_registerXMLFactoryFunctionCascade("RavlN::FunctionConcatenateC");

  //FileFormatStreamC <FunctionConcatenateC> FileFormatStream_FunctionConcatenateC;
  //FileFormatBinStreamC <FunctionConcatenateC> FileFormatBinStream_FunctionConcatenateC;
  //static TypeNameC typenameFunctionConcatenate(typeid(FunctionConcatenateC),"FunctionConcatenateC");

}
