// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlPatternRec
//! author="Charles Galambos"
//! file="Ravl/PatternRec/Modeling/Basic/FunctionCascade.cc"

#include "Ravl/PatternRec/FunctionCascade.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/DP/FileFormatStream.hh"
#include "Ravl/DP/FileFormatBinStream.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/config.h"
#include "Ravl/SArray1dIter.hh"
#include "Ravl/XMLFactoryRegister.hh"

namespace RavlN {

  //: Construct from XML factory
  FunctionCascadeBodyC::FunctionCascadeBodyC(const XMLFactoryContextC &factory)
    : FunctionBodyC(factory)
  {
    factory.UseComponentGroup("Functions",m_functions);
    if(m_functions.Size() > 0) {
      UIntT newISize = m_functions[0].InputSize();
      UIntT newOSize = m_functions[m_functions.Size()-1].OutputSize();
      if(inputSize != 0 && newISize != inputSize) {
        RavlError("Unexpected input size. ");
        throw RavlN::ExceptionBadConfigC("Unexpected input size");
      }
      if(outputSize != 0 && newOSize != outputSize) {
        RavlError("Unexpected output size. ");
        throw RavlN::ExceptionBadConfigC("Unexpected output size");
      }
      inputSize = newISize;
      outputSize = newOSize;
    }
  }

  //: Default constructor.
  
  FunctionCascadeBodyC::FunctionCascadeBodyC()
  {}
  
  //: Default constructor.
  
  FunctionCascadeBodyC::FunctionCascadeBodyC(const SArray1dC<FunctionC> & functions)
    : m_functions(functions)
  { 
    RavlAssert(functions.Size() > 0);
    
    UIntT numberOfFunctions = functions.Size();

    // FIXME Should check all input/output sizes marry
    
    InputSize(functions[0].InputSize());
    OutputSize(functions[numberOfFunctions-1].OutputSize());
  }

  FunctionCascadeBodyC::FunctionCascadeBodyC(const FunctionC & function1, const FunctionC & function2)
    : m_functions(2)
  {    
    m_functions[0] = function1;
    m_functions[1] = function2;
    InputSize(m_functions[0].InputSize());
    OutputSize(m_functions[1].OutputSize());
  }

  FunctionCascadeBodyC::FunctionCascadeBodyC(const FunctionC & function1, const FunctionC & function2, const FunctionC &function3)
    : m_functions(3)
  {
    m_functions[0] = function1;
    m_functions[1] = function2;
    m_functions[2] = function3;
    InputSize(m_functions[0].InputSize());
    OutputSize(m_functions[2].OutputSize());
  }

  
  //: Load from stream.
  
  FunctionCascadeBodyC::FunctionCascadeBodyC(std::istream &strm) 
    : FunctionBodyC(strm)
  { strm >> m_functions; }
  
  //: Load from binary stream.
  
  FunctionCascadeBodyC::FunctionCascadeBodyC(BinIStreamC &strm) 
    : FunctionBodyC(strm)
  { strm >> m_functions; }
  
  //: Writes object to stream.
  
  bool FunctionCascadeBodyC::Save (std::ostream &out) const {
    if(!FunctionBodyC::Save(out))
      return false;
    out << ' ' << m_functions;
    return true;    
  }
  
  //: Writes object to binary stream.
  
  bool FunctionCascadeBodyC::Save (BinOStreamC &out) const {
    if(!FunctionBodyC::Save(out))
      return false;
    out << m_functions;
    return true;
  }
  

  //: Apply function in place to 'data', overwrite values in out if its of the correct size.
  void FunctionCascadeBodyC::ApplyInPlace(const VectorC &data,VectorC &out) const {
    VectorC worki;
    VectorC worko = data.Copy();
    // This is at least efficient if the input and output vectors are the same length.
    for(SArray1dIterC<FunctionC> it(m_functions);it;it++) {
      RavlN::Swap(worki,worko);
      it.Data().Apply(worki,worko);
    }
    out = worko;
  }

  //: Apply function to 'data'
  
  VectorC FunctionCascadeBodyC::Apply(const VectorC &data) const {
    VectorC outputVec = data.Copy();
    for(SArray1dIterC<FunctionC> it(m_functions);it;it++) {
      outputVec= it.Data().Apply(outputVec);
    }            
    return outputVec;
  }
  //: Apply function to 'data'

  void FunctionCascadeBodyC::ApplyInPlace(const TVectorC<float> &data,TVectorC<float> &out) const {
    TVectorC<float> worki;
    TVectorC<float> worko = data.Copy();
    // This is at least efficient if the input and output vectors are the same length.
    for(SArray1dIterC<FunctionC> it(m_functions);it;it++) {
      RavlN::Swap(worki,worko);
      it.Data().Apply(worki,worko);
    }
    out = worko;
  }
  //: Apply function in place to 'data', overwrite values in out if its of the correct size.

  TVectorC<float> FunctionCascadeBodyC::Apply(const TVectorC<float> &data) const {
    TVectorC<float> outputVec = data;
    for(SArray1dIterC<FunctionC> it(m_functions);it;it++) {
      outputVec= it.Data().Apply(outputVec);
    }
    return outputVec;
  }
  //: Apply function with float vectors to 'data'

  ///////////////////////////////////////////////////////////
  
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(FunctionCascadeBodyC,FunctionCascadeC,FunctionC);

  static RavlN::XMLFactoryRegisterHandleC<RavlN::FunctionCascadeC> g_registerXMLFactoryFunctionCascade("RavlN::FunctionCascadeC");

  void LinkFunctionCascade()
  {}
}
