// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlPatternRecIO
//! file="Ravl/PatternRec/IO/Classifier2IO.cc"

#include "Ravl/Vector.hh"
#include "Ravl/PatternRec/Classifier.hh"
#include "Ravl/DP/FileFormatStream.hh"
#include "Ravl/DP/FileFormatBinStream.hh"
#include "Ravl/DP/Converter.hh"

#include "Ravl/PatternRec/ClassifierGaussianMixture.hh"
#include "Ravl/PatternRec/ClassifierFunc1Threshold.hh"
#include "Ravl/PatternRec/ClassifierWeakLinear.hh"
#include "Ravl/PatternRec/ClassifierLinearCombination.hh"
#include "Ravl/Types.hh"

namespace RavlN {

  void InitRavlClassifier2IO()
  {
  }
  
  //:- ClassifierGaussianMixtureC ------------------------------------------------------------------

  ClassifierC ClassifierGaussianMixture2Classifier(const ClassifierGaussianMixtureC &func)
  { return func; }
  
  DP_REGISTER_CONVERSION_NAMED(ClassifierGaussianMixture2Classifier ,1,
			       "RavlN::ClassifierC RavlN::Convert(const RavlN::ClassifierGaussianMixtureC &)");
  
  static TypeNameC type7(typeid(ClassifierGaussianMixtureC),"RavlN::ClassifierGaussianMixtureC");
  
  FileFormatStreamC<ClassifierGaussianMixtureC> FileFormatStream_ClassifierGaussianMixture;
  FileFormatBinStreamC<ClassifierGaussianMixtureC> FileFormatBinStream_ClassifierGaussianMixture;
  
  //:- ClassifierFunc1ThresholdC ------------------------------------------------------------------

  ClassifierC ClassifierFunc1Threshold2Classifier(const ClassifierFunc1ThresholdC &func)
  { return func; }
  
  DP_REGISTER_CONVERSION_NAMED(ClassifierFunc1Threshold2Classifier ,1,
			       "RavlN::ClassifierC RavlN::Convert(const RavlN::ClassifierFunc1Threshold2Classifier &)");
  
  static TypeNameC type8(typeid(ClassifierFunc1ThresholdC),"RavlN::ClassifierFunc1ThresholdC");
  
  FileFormatStreamC<ClassifierFunc1ThresholdC> FileFormatStream_ClassifierFunc1Threshold;
  FileFormatBinStreamC<ClassifierFunc1ThresholdC> FileFormatBinStream_ClassifierFunc1Threshold;

  //:- ClassifierWeakLinearC ------------------------------------------------------------------

  ClassifierC ClassifierWeakLinear2Classifier(const ClassifierWeakLinearC &func)
  { return func; }
  
  DP_REGISTER_CONVERSION_NAMED(ClassifierWeakLinear2Classifier ,1,
			       "RavlN::ClassifierC RavlN::Convert(const RavlN::ClassifierWeakLinear2Classifier &)");
  
  static TypeNameC type9(typeid(ClassifierWeakLinearC),"RavlN::ClassifierWeakLinearC");
  
  FileFormatStreamC<ClassifierWeakLinearC> FileFormatStream_ClassifierWeakLinear;
  FileFormatBinStreamC<ClassifierWeakLinearC> FileFormatBinStream_ClassifierWeakLinear;

  //:- ClassifierLinearCombinationC ------------------------------------------------------------------

  ClassifierC ClassifierLinearCombination2Classifier(const ClassifierLinearCombinationC &func)
  { return func; }
  
  DP_REGISTER_CONVERSION_NAMED(ClassifierLinearCombination2Classifier ,1,
			       "RavlN::ClassifierC RavlN::Convert(const RavlN::ClassifierLinearCombination2Classifier &)");
  
  static TypeNameC type10(typeid(ClassifierLinearCombinationC),"RavlN::ClassifierLinearCombinationC");
  
  FileFormatStreamC<ClassifierLinearCombinationC> FileFormatStream_ClassifierLinearCombination;
  FileFormatBinStreamC<ClassifierLinearCombinationC> FileFormatBinStream_ClassifierLinearCombination;
  
  
}
