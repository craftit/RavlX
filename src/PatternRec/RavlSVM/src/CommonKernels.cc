// This file is used in conjunction with RAVL, Recognition And Vision Library
// Copyright (C) 2004, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU
// General Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
//! lib=RavlSVM

//! author="Alexey Kostin"

#include <math.h>

#include "Ravl/PatternRec/CommonKernels.hh"
#include "Ravl/VectorUtils.hh"
#include "Ravl/XMLFactoryRegister.hh"

namespace RavlN
{

//-- Linear kernel ------------------------------------------------------------


  //: XMLFactoryC constructor.  
  LinearKernelBodyC::LinearKernelBodyC(const XMLFactoryContextC &factory)
    : KernelFunctionBodyC(factory),
      scale(factory.AttributeReal("scale", 1.0))
  {
  }

  //: Load from stream.
  LinearKernelBodyC::LinearKernelBodyC(istream &strm)
    :KernelFunctionBodyC(strm)
  {
    IntT version;
    strm >> version;
    if(version != 0)
      throw ExceptionOperationFailedC("LinearKernelBodyC::LinearKernelBodyC(istream &strm), Unrecognised version number in stream.");
    strm >> scale;
  }

  //---------------------------------------------------------------------------
  //: Load from binary stream.
  LinearKernelBodyC::LinearKernelBodyC(BinIStreamC &strm)
    :KernelFunctionBodyC(strm)
  {
    IntT version;
    strm >> version;
    if(version != 0)
      throw ExceptionOperationFailedC("LinearKernelBodyC::LinearKernelBodyC(BinIStreamC &strm), Unrecognised version number in stream.");
    strm >> scale;
  }
  //---------------------------------------------------------------------------

  // Writes object to stream
  bool LinearKernelBodyC::Save(ostream &out) const
  {
    if(!RCBodyVC::Save(out))
      return false;
    const IntT version = 0;
    out << ' ' << version << ' ' << scale;
    return true;
  }

  //---------------------------------------------------------------------------
  // Writes object to stream
  bool LinearKernelBodyC::Save(BinOStreamC &out) const
  {
    if(!RCBodyVC::Save(out))
      return false;
    const IntT version = 0;
    out << version << scale;
    return true;
  }

  //---------------------------------------------------------------------------
  // Apply function to 'data' vectors supposed to be of the same size
  RealT LinearKernelBodyC::Apply(int Size, const RealT *X1, const RealT *X2) const
  {
    return scale * RavlBaseVectorN::DotProduct(X1, X2, Size);
  }
  //---------------------------------------------------------------------------

  //-- Quadratic kernel ------------------------------------------------------------

  //: XMLFactoryC constructor.  
  QuadraticKernelBodyC::QuadraticKernelBodyC(const XMLFactoryContextC &factory)
    : KernelFunctionBodyC(factory),
      scale(factory.AttributeReal("scale", 1.0))
  {
  }


  //: Load from stream.
  QuadraticKernelBodyC::QuadraticKernelBodyC(istream &strm)
    : KernelFunctionBodyC(strm)
  {
    IntT version;
    strm >> version;
    if(version != 0)
      throw ExceptionOperationFailedC("QuadraticKernelBodyC::QuadraticKernelBodyC(istream &strm), Unrecognised version number in stream.");
    strm >> scale;
  }

  //---------------------------------------------------------------------------
  //: Load from binary stream.
  QuadraticKernelBodyC::QuadraticKernelBodyC(BinIStreamC &strm)
    : KernelFunctionBodyC(strm)
  {
    IntT version;
    strm >> version;
    if(version != 0)
      throw ExceptionOperationFailedC("QuadraticKernelBodyC::QuadraticKernelBodyC(BinIStreamC &strm), Unrecognised version number in stream.");
    strm >> scale;
  }

  //---------------------------------------------------------------------------
  // Writes object to stream
  bool QuadraticKernelBodyC::Save(ostream &out) const
  {
    if(!RCBodyVC::Save(out))
      return false;
    const IntT version = 0;
    out << ' ' << version << ' ' << scale;
    return true;
  }

  //---------------------------------------------------------------------------
  // Writes object to stream
  bool QuadraticKernelBodyC::Save(BinOStreamC &out) const
  {
    if(!RCBodyVC::Save(out))
      return false;
    const IntT version = 0;
    out << version << scale;
    return true;
  }

  //---------------------------------------------------------------------------
  // Apply function to 'data' vectors supposed to be of the same size
  RealT QuadraticKernelBodyC::Apply(int Size, const RealT *X1, const RealT *X2) const
  {
    const RealT *p1 = X1;
    const RealT *p2 = X2;
    const RealT *p1end = p1 + Size;
    long double retVal = 0;
    while(p1 != p1end) {
      const RealT v1 = *p1++;
      const RealT v2 = *p2++;
      retVal += v1 * (v2 + v1 * v2 * v2);
    }
    return scale * retVal;
  }
  //---------------------------------------------------------------------------


  //-- Polynomial kernel ------------------------------------------------------------

  //: XMLFactoryC constructor.  
  PolynomialKernelBodyC::PolynomialKernelBodyC(const XMLFactoryContextC &factory)
    : KernelFunctionBodyC(factory),
      power(factory.AttributeReal("power", 2.0)),
      scale(factory.AttributeReal("scale", 1.0)),
      b(factory.AttributeReal("b", 1.0))
  {
  }
  
  //: Load from stream.
  PolynomialKernelBodyC::PolynomialKernelBodyC(istream &strm)
    :KernelFunctionBodyC(strm)
  {
    IntT version;
    strm >> version;
    if(version != 0)
      throw ExceptionOperationFailedC("PolynomialKernelBodyC::PolynomialKernelBodyC(istream &strm), Unrecognised version number in stream.");
    strm >> power;
    strm >> scale;
    strm >> b;
  }
  //---------------------------------------------------------------------------

  //: Load from binary stream.
  PolynomialKernelBodyC::PolynomialKernelBodyC(BinIStreamC &strm)
    :KernelFunctionBodyC(strm)
  {
    IntT version;
    strm >> version;
    if(version != 0)
      throw ExceptionOperationFailedC("PolynomialKernelBodyC::PolynomialKernelBodyC(BinIStreamC &strm), Unrecognised version number in stream.");
    strm >> power;
    strm >> scale;
    strm >> b;
  }

  //---------------------------------------------------------------------------
  // Writes object to stream
  bool PolynomialKernelBodyC::Save(ostream &out) const
  {
    if(!RCBodyVC::Save(out))
      return false;
    const IntT version = 0;
    out << ' ' << version << ' ' << power << ' ' << scale << ' ' << b;
    return true;
  }

  //---------------------------------------------------------------------------
  // Writes object to stream
  bool PolynomialKernelBodyC::Save(BinOStreamC &out) const
  {
    if(!RCBodyVC::Save(out))
      return false;
    const IntT version = 0;
    out << version << power << scale << b;
    return true;
  }

  //---------------------------------------------------------------------------
  // Apply function to 'data' vectors supposed to be of the same size
  RealT PolynomialKernelBodyC::Apply(int Size, const RealT *X1, const RealT *X2) const
  {
    long double sum = 0.0;
    const RealT* endPtr = X1 + Size;
    while(X1 < endPtr)
      sum += *X1++ * *X2++;
    //if(sum < 0.)
    //  printf("sum<0:%g\n", float(sum));
    return pow(scale * static_cast<double>(sum) + b, power);
  }
  //---------------------------------------------------------------------------

  //-- Radial basis kernel ------------------------------------------------------

  //: XMLFactoryC constructor.  
  RBFKernelBodyC::RBFKernelBodyC(const XMLFactoryContextC &factory)
    : KernelFunctionBodyC(factory),
      gamma(factory.AttributeReal("gamma", 1.0))
  {
  }

  //: Load from stream.
  RBFKernelBodyC::RBFKernelBodyC(istream &strm)
    :KernelFunctionBodyC(strm)
  {
    IntT version;
    strm >> version;
    if(version != 0)
      throw ExceptionOperationFailedC("RBFKernelBodyC::RBFKernelBodyC(istream &strm), Unrecognised version number in stream.");
    strm >> gamma;
  }

  //---------------------------------------------------------------------------
  //: Load from binary stream.
  RBFKernelBodyC::RBFKernelBodyC(BinIStreamC &strm)
    :KernelFunctionBodyC(strm)
  {
    IntT version;
    strm >> version;
    if(version != 0)
      throw ExceptionOperationFailedC("RBFKernelBodyC::RBFKernelBodyC(BinIStreamC &strm), Unrecognised version number in stream.");
    strm >> gamma;
  }

  //---------------------------------------------------------------------------
  // Writes object to stream
  bool RBFKernelBodyC::Save(ostream &out) const
  {
    if(!RCBodyVC::Save(out))
      return false;
    const IntT version = 0;
    out << ' ' << version << ' ' << gamma;
    return true;
  }
  //---------------------------------------------------------------------------

  // Writes object to stream
  bool RBFKernelBodyC::Save(BinOStreamC &out) const
  {
    if(!RCBodyVC::Save(out))
      return false;
    const IntT version = 0;
    out << version << gamma;
    return true;
  }

  //---------------------------------------------------------------------------
  // Apply function to 'data' vectors supposed to be of the same size
  RealT RBFKernelBodyC::Apply(int Size, const RealT *X1, const RealT *X2) const
  {
    long double sum = 0.;
    const RealT* endPtr = X1 + Size;
    while(X1 < endPtr)
      {
        RealT t = *X1++ - *X2++;
        sum -= t * t;
      }    
    return exp(sum / gamma);
  }
  //---------------------------------------------------------------------------

  //-- Chi^2 kernel ----------------------------------------------------------
  //: XMLFactoryC constructor.  
  Chi2KernelBodyC::Chi2KernelBodyC(const XMLFactoryContextC &factory)
    : KernelFunctionBodyC(factory),
      gamma(factory.AttributeReal("gamma", 1.0))
  {
  }

  //: Load from stream.
  Chi2KernelBodyC::Chi2KernelBodyC(istream &strm)
    :KernelFunctionBodyC(strm)
  {
    IntT version;
    strm >> version;
    if(version != 0)
      throw ExceptionOperationFailedC("Chi2KernelBodyC::Chi2KernelBodyC(istream &strm), Unrecognised version number in stream.");
    strm >> gamma;
  }

  //---------------------------------------------------------------------------
  //: Load from binary stream.
  Chi2KernelBodyC::Chi2KernelBodyC(BinIStreamC &strm)
    :KernelFunctionBodyC(strm)
  {
    IntT version;
    strm >> version;
    if(version != 0)
      throw ExceptionOperationFailedC("Chi2KernelBodyC::Chi2KernelBodyC(BinIStreamC &strm), Unrecognised version number in stream.");
    strm >> gamma;
  }

  //---------------------------------------------------------------------------
  // Writes object to stream
  bool Chi2KernelBodyC::Save(ostream &out) const
  {
    if(!RCBodyVC::Save(out))
      return false;

    const IntT version = 0;
    out << ' ' << version << ' ' << gamma;
    return true;
  }
  //---------------------------------------------------------------------------

  // Writes object to stream
  bool Chi2KernelBodyC::Save(BinOStreamC &out) const
  {
    if(!RCBodyVC::Save(out))
      return false;
    const IntT version = 0;
    out << version << gamma;
    return true;
  }

  //---------------------------------------------------------------------------
  // Apply function to 'data' vectors supposed to be of the same size
  RealT Chi2KernelBodyC::Apply(int Size, const RealT *X1, const RealT *X2) const
  {
    long double sum = 0.;
    const RealT* endPtr = X1 + Size;
    while(X1 < endPtr)
      {
	RealT bottom = (*X1 + *X2);
        if (bottom <= 0.0) return 0.0;
        sum += Sqr(*X1 - *X2) / bottom;
	X1++;
	X2++;
      }
    return exp(-0.5*sum / gamma);
  }
  //---------------------------------------------------------------------------



  //---------------------------------------------------------------------------


  RavlN::XMLFactoryRegisterHandleConvertC<LinearKernelC, KernelFunctionC> g_registerXMLFactoryLinearKernel("RavlN::LinearKernelC");
  RavlN::XMLFactoryRegisterHandleConvertC<QuadraticKernelC, KernelFunctionC> g_registerXMLFactoryQuadraticKernel("RavlN::QuadraticKernelC");
  RavlN::XMLFactoryRegisterHandleConvertC<PolynomialKernelC, KernelFunctionC> g_registerXMLFactoryPolynomialKernel("RavlN::PolynomialKernelC");
  RavlN::XMLFactoryRegisterHandleConvertC<RBFKernelC, KernelFunctionC> g_registerXMLFactoryRBFKernel("RavlN::RBFKernelC");
  RavlN::XMLFactoryRegisterHandleConvertC<Chi2KernelC, KernelFunctionC> g_registerXMLFactoryChi2Kernel("RavlN::Chi2KernelC");  
  extern void linkCommonKernels()
  {}

}

