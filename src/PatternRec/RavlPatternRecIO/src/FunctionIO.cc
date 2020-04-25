// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlPatternRecIO
//! file="Ravl/PatternRec/IO/FunctionIO.cc"

#include "Ravl/Vector.hh"
#include "Ravl/PatternRec/Classifier.hh"
#include "Ravl/DP/FileFormatStream.hh"
#include "Ravl/DP/FileFormatBinStream.hh"
#include "Ravl/DP/Converter.hh"
#include "Ravl/Types.hh"
#include "Ravl/PatternRec/Function1.hh"
#include "Ravl/PatternRec/FuncMeanProjection.hh"
#include "Ravl/PatternRec/FuncLinear.hh"
#include "Ravl/PatternRec/FuncQuadratic.hh"
#include "Ravl/PatternRec/FuncOrthPolynomial.hh"
#include "Ravl/PatternRec/FuncLinearCoeff.hh"
#include "Ravl/PatternRec/FuncPDFNormal.hh"
#include "Ravl/PatternRec/FunctionCascade.hh"
#include "Ravl/PatternRec/FunctionConcatenate.hh"
#include "Ravl/DP/Converter.hh"

namespace RavlN {
  void InitRavlFunctionIO()
  {}

  //:- FunctionC -------------------------------------------------------------------------
  
  static TypeNameC type1(typeid(FunctionC),"RavlN::FunctionC");
  FileFormatStreamC <FunctionC> FileFormatStream_Function;
  FileFormatBinStreamC <FunctionC> FileFormatBinStream_Function;
  
  //:- Function1C -------------------------------------------------------------------------
  
  static TypeNameC type2(typeid(Function1C),"RavlN::Function1C");
  
  FunctionC Function1C2Function(const Function1C &func)
  { return func; }
  
  DP_REGISTER_CONVERSION_NAMED(Function1C2Function ,1,
			       "RavlN::FunctionC RavlN::Convert(const RavlN::Function1C &)");

  FileFormatStreamC <Function1C> FileFormatStream_Function1;
  FileFormatBinStreamC <Function1C> FileFormatBinStream_Function1;
  
  //:-  FuncMeanProjectionC --------------------------------------------------------------

  FunctionC FuncMeanProjection2Function(const FuncMeanProjectionC &func)
  { return func; }
  
  DP_REGISTER_CONVERSION_NAMED(FuncMeanProjection2Function ,1,
			       "RavlN::FunctionC RavlN::Convert(const RavlN::FuncMeanProjectionC &)");
  
  static TypeNameC type3(typeid(FuncMeanProjectionC),"RavlN::FuncMeanProjectionC");
  FileFormatStreamC <FuncMeanProjectionC> FileFormatStream_FuncMeanProjection;
  FileFormatBinStreamC <FuncMeanProjectionC> FileFormatBinStream_FuncMeanProjection;
  
  //:- FuncLinearC ------------------------------------------------------------------------
  
  FunctionC FuncLinear2Function(const FuncLinearC &func)
  { return func; }
  
  DP_REGISTER_CONVERSION_NAMED(FuncLinear2Function ,1,
			       "RavlN::FunctionC RavlN::Convert(const RavlN::FuncLinearC &)");
  
  static TypeNameC type4(typeid(FuncLinearC),"RavlN::FuncLinearC");
  FileFormatStreamC <FuncLinearC> FileFormatStream_FuncLinear;
  FileFormatBinStreamC <FuncLinearC> FileFormatBinStream_FuncLinear;
  
  //:- FuncQuadraticC ------------------------------------------------------------------------
  
  FunctionC FuncQuadratic2Function(const FuncQuadraticC &func)
  { return func; }
  
  DP_REGISTER_CONVERSION_NAMED(FuncQuadratic2Function ,1,
			       "RavlN::FunctionC RavlN::Convert(const RavlN::FuncQuadraticC &)");
  
  static TypeNameC type5(typeid(FuncQuadraticC),"RavlN::FuncQuadraticC");
  FileFormatStreamC <FuncQuadraticC> FileFormatStream_FuncQuadratic;
  FileFormatBinStreamC <FuncQuadraticC> FileFormatBinStream_FuncQuadratic;

  //:- FuncOrthPolynomialC ------------------------------------------------------------------------
  
  FunctionC FuncOrthPolynomial2Function(const FuncOrthPolynomialC &func)
  { return func; }
  
  DP_REGISTER_CONVERSION_NAMED(FuncOrthPolynomial2Function ,1,
			       "RavlN::FunctionC RavlN::Convert(const RavlN::FuncOrthPolynomialC &)");
  
  static TypeNameC type6(typeid(FuncOrthPolynomialC),"RavlN::FuncOrthPolynomialC");
  FileFormatStreamC <FuncOrthPolynomialC> FileFormatStream_FuncOrthPolynomial;
  FileFormatBinStreamC <FuncOrthPolynomialC> FileFormatBinStream_FuncOrthPolynomial;

  //:- FuncLinearCoeffC ------------------------------------------------------------------------
  
  FunctionC FuncLinearCoeff2Function(const FuncLinearCoeffC &func)
  { return func; }
  
  DP_REGISTER_CONVERSION_NAMED(FuncLinearCoeff2Function ,1,
			       "RavlN::FunctionC RavlN::Convert(const RavlN::FuncLinearCoeffC &)");
  
  static TypeNameC type7(typeid(FuncLinearCoeffC),"RavlN::FuncLinearCoeffC");
  FileFormatStreamC <FuncLinearCoeffC> FileFormatStream_FuncLinearCoeff;
  FileFormatBinStreamC <FuncLinearCoeffC> FileFormatBinStream_FuncLinearCoeff;

  //:- FuncPDFNormalC ------------------------------------------------------------------------

  FunctionC FuncPDFNormal2Function(const FuncPDFNormalC &func)
  { return func; }
  
  DP_REGISTER_CONVERSION_NAMED(FuncPDFNormal2Function ,1,
			       "RavlN::FunctionC RavlN::Convert(const RavlN::FuncPDFNormalC &)");
  
  static TypeNameC type8(typeid(FuncPDFNormalC),"RavlN::FuncPDFNormalC");
  FileFormatStreamC <FuncPDFNormalC> FileFormatStream_FuncPDFNormal;
  FileFormatBinStreamC <FuncPDFNormalC> FileFormatBinStream_FuncPDFNormal;

  //:-  FunctionCascadeC --------------------------------------------------------------

  FunctionC FunctionCascade2Function(const FunctionCascadeC &func)
  {
    return func;
  }

  DP_REGISTER_CONVERSION_NAMED(FunctionCascade2Function, 1,
      "RavlN::FunctionC RavlN::Convert(const RavlN::FunctionCascadeC &)");

  static TypeNameC type9(typeid(FunctionCascadeC), "RavlN::FunctionCascadeC");
  FileFormatStreamC<FunctionCascadeC> FileFormatStream_FunctionCascade;
  FileFormatBinStreamC<FunctionCascadeC> FileFormatBinStream_FunctionCascade;

  //:-  FunctionConcatenateC --------------------------------------------------------------

  FunctionC FunctionConcatenate2Function(const FunctionConcatenateC &func)
  {
    return func;
  }

  DP_REGISTER_CONVERSION_NAMED(FunctionConcatenate2Function, 1,
      "RavlN::FunctionC RavlN::Convert(const RavlN::FunctionConcatenateC &)");

  static TypeNameC type10(typeid(FunctionConcatenateC), "RavlN::FunctionConcatenateC");
  FileFormatStreamC<FunctionConcatenateC> FileFormatStream_FunctionConcatenate;
  FileFormatBinStreamC<FunctionConcatenateC> FileFormatBinStream_FunctionConcatenate;


}
