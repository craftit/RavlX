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
  
  void InitRavlSVMIO2() {
  }
  
    // -- DesignSvmC -------------------------------------------------------
  DesignClassifierSupervisedC DesignSvm2DesignClassifierSupervised(const DesignSvmC &func)
    { return func; }

  DP_REGISTER_CONVERSION_NAMED(DesignSvm2DesignClassifierSupervised, 1,
                               "RavlN::DesignClassifierSupervisedC RavlN::"
                               "Convert(const RavlN::DesignSvm2DesignCla"
                               "ssifierSupervised &)");

  static TypeNameC TypeDesignSvm(typeid(DesignSvmC), "RavlN::DesignSvmC");

  FileFormatStreamC<DesignSvmC> FileFormatStream_DesignSvm;
  FileFormatBinStreamC<DesignSvmC> FileFormatBinStream_DesignSvm;

  // -- DesignSvmSmoC -------------------------------------------------------
  DesignSvmC DesignSvmSmo2DesignSvm(const DesignSvmSmoC &func)
    { return func; }

  DP_REGISTER_CONVERSION_NAMED(DesignSvmSmo2DesignSvm, 1,
                               "RavlN::DesignSvmC RavlN::"
                               "Convert(const RavlN::DesignSvmSmo2DesignSvm &)");

  static TypeNameC TypeDesignSvmSmo(typeid(DesignSvmSmoC),
                                    "RavlN::DesignSvmSmoC");

  FileFormatStreamC<DesignSvmSmoC> FileFormatStream_DesignSvmSmo;
  FileFormatBinStreamC<DesignSvmSmoC> FileFormatBinStream_DesignSvmSmo;

  // -- DesignOneClassC -------------------------------------------------------
  DesignSvmC DesignOneClass2DesignSvm(const DesignOneClassC &func)
    { return func; }

  DP_REGISTER_CONVERSION_NAMED(DesignOneClass2DesignSvm, 1,
                               "RavlN::DesignSvmC RavlN::"
                               "Convert(const RavlN::DesignOneClass2DesignSvm &)");

  static TypeNameC TypeDesignOneClass(typeid(DesignOneClassC),
                                      "RavlN::DesignOneClassC");

  FileFormatStreamC<DesignOneClassC> FileFormatStream_DesignOneClass;
  FileFormatBinStreamC<DesignOneClassC> FileFormatBinStream_DesignOneClass;

  // -- SvmClassifierC -------------------------------------------------------
  Classifier2C SvmClassifier2Classifier2(const SvmClassifierC &func)
  { return func; }

  DP_REGISTER_CONVERSION_NAMED(SvmClassifier2Classifier2 ,1,
                               "RavlN::Classifier2C RavlN::Convert(const "
                               "RavlN::SvmClassifier2Classifier2 &)");

  static TypeNameC TypeSvmClassifier(typeid(SvmClassifierC),
                                     "RavlN::SvmClassifierC");

  FileFormatStreamC<SvmClassifierC> FileFormatStream_SvmClassifier;
  FileFormatBinStreamC<SvmClassifierC> FileFormatBinStream_SvmClassifier;

  // -- SvmLinearClassifierC -------------------------------------------------
  Classifier2C SvmLinearClassifier2Classifier2(const SvmLinearClassifierC &func)
  { return func; }

  DP_REGISTER_CONVERSION_NAMED(SvmLinearClassifier2Classifier2 ,1,
                               "RavlN::Classifier2C RavlN::Convert(const "
                               "RavlN::SvmLinearClassifier2Classifier2 &)");

  static TypeNameC TypeSvmLinearClassifier(typeid(SvmLinearClassifierC),
                                           "RavlN::SvmLinearClassifierC");

  FileFormatStreamC<SvmLinearClassifierC> FileFormatStream_SvmLinearClassifier;
  FileFormatBinStreamC<SvmLinearClassifierC> FileFormatBinStream_SvmLinearClassifier;

  // -- SvmQuadraticClassifierC -------------------------------------------------
  Classifier2C SvmQuadraticClassifier2Classifier2(const SvmQuadraticClassifierC &func)
    { return func; }

  DP_REGISTER_CONVERSION_NAMED(SvmQuadraticClassifier2Classifier2 ,1,
                               "RavlN::Classifier2C RavlN::Convert(const "
                               "RavlN::SvmQuadraticClassifier2Classifier2 &)");

  static TypeNameC TypeSvmQuadraticClassifier(typeid(SvmQuadraticClassifierC),
                                              "RavlN::SvmQuadraticClassifierC");

  FileFormatStreamC<SvmQuadraticClassifierC> FileFormatStream_SvmQuadraticClassifier;
  FileFormatBinStreamC<SvmQuadraticClassifierC> FileFormatBinStream_SvmQuadraticClassifier;

  
  

}

