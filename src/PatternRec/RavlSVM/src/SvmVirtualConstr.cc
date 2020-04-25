// This file is used in conjunction with RAVL, Recognition And Vision Library
// Copyright (C) 2004, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU
// General Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
// $Id: FaceGraph.cc 8674 2006-05-24 12:19:29Z alex $
//! lib=RavlSVM

//! author="Alexey Kostin"

#include "Ravl/PatternRec/SvmClassifier.hh"
#include "Ravl/PatternRec/CommonKernels.hh"
#include "Ravl/PatternRec/DesignSvm.hh"
#include "Ravl/PatternRec/DesignClassifierSvmSmo.hh"
#include "Ravl/PatternRec/SvmLinearClassifier.hh"
#include "Ravl/PatternRec/SvmQuadraticClassifier.hh"
#include "Ravl/PatternRec/DesignOneClass.hh"
#include "Ravl/PatternRec/OneClassLinear.hh"

//#include "Ravl/BinStream.hh"
#include "Ravl/VirtualConstructor.hh"

//need for typename definition
#include "Ravl/DP/FileFormatStream.hh"
#include "Ravl/DP/FileFormatBinStream.hh"
#include "Ravl/DP/Converter.hh"
#include "Ravl/Types.hh"

namespace RavlN
{

  void InitRavlSVMIO() {
  }
  // -- OneClassC -------------------------------------------------------
  Classifier2C OneClass2Classifier2(const OneClassC &func)
    { return func; }

  DP_REGISTER_CONVERSION_NAMED(OneClass2Classifier2 ,1,
                               "RavlN::Classifier2C RavlN::Convert(const "
                               "RavlN::OneClass2Classifier2 &)");

  static TypeNameC TypeOneClass(typeid(OneClassC), "RavlN::OneClassC");

  FileFormatStreamC<OneClassC> FileFormatStream_OneClass;
  FileFormatBinStreamC<OneClassC> FileFormatBinStream_OneClass;

  // -- OneClassLinearC -------------------------------------------------------
  Classifier2C OneClassLinear2Classifier2(const OneClassLinearC &func)
  { return func; }

  DP_REGISTER_CONVERSION_NAMED(OneClassLinear2Classifier2 ,1,
                               "RavlN::Classifier2C RavlN::Convert(const "
                               "RavlN::OneClassLinear2Classifier2 &)");

  static TypeNameC TypeOneClassLinear(typeid(OneClassLinearC), "RavlN::OneClassLinearC");

  FileFormatStreamC<OneClassLinearC> FileFormatStream_OneClassLinear;
  FileFormatBinStreamC<OneClassLinearC> FileFormatBinStream_OneClassLinear;  
  
  // -- Linear kernel -------------------------------------------------------
  KernelFunctionC LinearKernel2KernelFunction(const LinearKernelC &func)
  { return func; }

  DP_REGISTER_CONVERSION_NAMED(LinearKernel2KernelFunction, 1,
                               "RavlN::KernelFunctionC RavlN::Convert(const "
                               "RavlN::LinearKernel2KernelFunction &)");

  static TypeNameC TypeLinearKernel(typeid(LinearKernelC),
                                    "RavlN::LinearKernelC");

  FileFormatStreamC<LinearKernelC> FileFormatStream_LinearKernel;
  FileFormatBinStreamC<LinearKernelC> FileFormatBinStream_LinearKernel;

  // -- Quadratic kernel -------------------------------------------------------
  KernelFunctionC QuadraticKernel2KernelFunction(const QuadraticKernelC &func)
    { return func; }

  DP_REGISTER_CONVERSION_NAMED(QuadraticKernel2KernelFunction, 1,
                               "RavlN::KernelFunctionC RavlN::Convert(const "
                               "RavlN::QuadraticKernel2KernelFunction &)");

  static TypeNameC TypeQuadraticKernel(typeid(QuadraticKernelC),
                                       "RavlN::QuadraticKernelC");

  FileFormatStreamC<QuadraticKernelC> FileFormatStream_QuadraticKernel;
  FileFormatBinStreamC<QuadraticKernelC> FileFormatBinStream_QuadraticKernel;

  // -- Polynomial kernel ---------------------------------------------------
  KernelFunctionC PolynomialKernel2KernelFunction(const PolynomialKernelC &f)
  { return f; }

  DP_REGISTER_CONVERSION_NAMED(PolynomialKernel2KernelFunction, 1,
                               "RavlN::KernelFunctionC RavlN::Convert(const "
                               "RavlN::PolynomialKernel2KernelFunction &)");

  static TypeNameC TypePolynomialKernel(typeid(PolynomialKernelC),
                                        "RavlN::PolynomialKernelC");

  FileFormatStreamC<PolynomialKernelC> FileFormatStream_PolynomialKernel;
  FileFormatBinStreamC<PolynomialKernelC> FileFormatBinStream_PolynomialKernel;

  // -- RBF kernel ---------------------------------------------------
  KernelFunctionC RBFKernel2KernelFunction(const RBFKernelC &func)
  { return func; }

  DP_REGISTER_CONVERSION_NAMED(RBFKernel2KernelFunction, 1,
                               "RavlN::KernelFunctionC RavlN::Convert(const "
                               "RavlN::RBFKernel2KernelFunction &)");

  static TypeNameC TypeRBFKernel(typeid(RBFKernelC), "RavlN::RBFKernelC");

  FileFormatStreamC<RBFKernelC> FileFormatStream_RBFKernel;
  FileFormatBinStreamC<RBFKernelC> FileFormatBinStream_RBFKernel;

  // -- Chi^2 kernel ---------------------------------------------------
  KernelFunctionC Chi2Kernel2KernelFunction(const Chi2KernelC &func)
  { return func; }

  DP_REGISTER_CONVERSION_NAMED(Chi2Kernel2KernelFunction, 1,
                               "RavlN::KernelFunctionC RavlN::Convert(const "
                               "RavlN::Chi2Kernel2KernelFunction &)");

  static TypeNameC TypeChi2Kernel(typeid(Chi2KernelC), "RavlN::Chi2KernelC");

  FileFormatStreamC<Chi2KernelC> FileFormatStream_Chi2Kernel;
  FileFormatBinStreamC<Chi2KernelC> FileFormatBinStream_Chi2Kernel;

  //---------------------------------------------------------------------------
  // Stream load operators defined in RAVL_INITVIRTUALCONSTRUCTOR_FULL macro
  // Implementation of 'load from stream' constructors defined there as well
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(LinearKernelBodyC,        LinearKernelC,        KernelFunctionC);
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(QuadraticKernelBodyC,     QuadraticKernelC,     KernelFunctionC);
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(PolynomialKernelBodyC,    PolynomialKernelC,    KernelFunctionC);
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(RBFKernelBodyC,           RBFKernelC,           KernelFunctionC);
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(Chi2KernelBodyC,          Chi2KernelC,          KernelFunctionC);
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(DesignSvmBodyC,           DesignSvmC,           DesignClassifierSupervisedC);
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(DesignSvmSmoBodyC,        DesignSvmSmoC,        DesignSvmC);
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(SvmClassifierBodyC,       SvmClassifierC,       Classifier2C);
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(SvmLinearClassifierBodyC, SvmLinearClassifierC, Classifier2C);
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(SvmQuadraticClassifierBodyC, SvmQuadraticClassifierC, Classifier2C);
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(DesignOneClassBodyC,      DesignOneClassC,      DesignSvmC);
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(OneClassBodyC,            OneClassC,            Classifier2C);
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(OneClassLinearBodyC,      OneClassLinearC,      Classifier2C);
}
