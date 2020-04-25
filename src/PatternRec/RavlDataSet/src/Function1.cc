// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/DataSet/Function1.cc"


#include "Ravl/PatternRec/Function1.hh"
#include "Ravl/VirtualConstructor.hh"

namespace RavlN {
  
  //: XMLFactoryC constructor.
  Function1BodyC::Function1BodyC(const XMLFactoryContextC &factory)
    : FunctionBodyC(factory)
  {
    //: FIXME: Probably should just check outputSize is 1 rather
    //: tha force it!
    outputSize = 1;
    
  }

  //: Load from stream.

  Function1BodyC::Function1BodyC(std::istream &strm) 
    : FunctionBodyC(strm)
  {}
  
  //: Load from binary stream.
  
  Function1BodyC::Function1BodyC(BinIStreamC &strm) 
    : FunctionBodyC(strm)
  {}
  
  //: Writes object to stream, can be loaded using constructor
  
  bool Function1BodyC::Save (std::ostream &out) const {
    if(!FunctionBodyC::Save(out))
      return false;
    //...
    return true;
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool Function1BodyC::Save (BinOStreamC &out) const {
    if(!FunctionBodyC::Save(out))
      return false;
    //...
    return true;
  }
  
  //: Apply function to 'data'
  
  VectorC Function1BodyC::Apply(const VectorC &data) const {
    VectorC ret(1);
    ret[0] = Apply1(data);
    return ret;
  }
  
  //: Apply function to 'data'
  
  RealT Function1BodyC::Apply1(const VectorC &data) const {
    RavlAssertMsg(0,"Function1BodyC::Apply1(), Abstract method called. \n");
    return 0;
  }

  //! Apply to a vector of floats.

  float Function1BodyC::Apply1(const TVectorC<float> &data) const {
    VectorC tmp(data);
    return Apply1(tmp);
  }

  MatrixC Function1BodyC::Jacobian(const VectorC &X) const {
    VectorC g = Jacobian1(X);
    return MatrixC((UIntT) 1,(UIntT)g.Size().V(),static_cast<SArray1dC<RealT> &>(g));
  }
  //: Compute the jacobian using jacobian1

  VectorC Function1BodyC::Jacobian1(const VectorC &X) const {
    RavlAssert(0); // Should be overridden.
    return Jacobian(X).SliceRow(0);
  }

  //: Obtain the hessian of the function at X
  // The default implementation performs numerical estimation of the Jacobian using differences. The
  // default method does not provide a good approximation.
  
  MatrixC Function1BodyC::Hessian(const VectorC &X) const {
    // Compute something numerically ?
    return MatrixC();
  }
    
  //: Evaluate the value,jacobian, and hessian of the function at point X
  // Returns true if all values are provide, false if one or more is approximated.
  
  bool Function1BodyC::EvaluateValueJacobianHessian(const VectorC &X,RealT &value,VectorC &jacobian,MatrixC &hessian) const {
    value = Apply1(X);
    jacobian = Jacobian1(X);
    hessian = Hessian(X);
    return false;
  }
                                                                                                                        

  ///////////////////////////////////////////////////////////

  //: Load from stream.
  
  Function1C::Function1C(std::istream &strm)
    : FunctionC(RAVL_VIRTUALCONSTRUCTOR(strm,Function1BodyC))    
  {}
  
  //: Load from binary stream.
  
  Function1C::Function1C(BinIStreamC &strm)
    : FunctionC(RAVL_VIRTUALCONSTRUCTOR(strm,Function1BodyC))
  {}

  //: Setup virtual constructor.
  // As this is an abstract class its not really needed, but its usefull to
  // have an example of how it should work here.
  
  //RAVL_INITVIRTUALCONSTRUCTOR(Function1BodyC);
  
}

  
