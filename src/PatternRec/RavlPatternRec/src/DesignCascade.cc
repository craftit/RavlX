// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id: DesignCascade.cc 7590 2010-02-23 12:03:11Z kier $"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Classify/DesignCascade.cc"

#include "Ravl/PatternRec/DesignCascade.hh"
#include "Ravl/PatternRec/ClassifierOneAgainstAll.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/PatternRec/DataSetVectorLabel.hh"
#include "Ravl/SArray1dIter2.hh"
#include "Ravl/XMLFactoryRegister.hh"
#include "Ravl/Collection.hh"
#include "Ravl/PatternRec/DataSet2Iter.hh"
#include "Ravl/PatternRec/DataSet3Iter.hh"
#include "Ravl/PatternRec/FuncSubset.hh"
#include "Ravl/PatternRec/ClassifierCascade.hh"
#include "Ravl/PatternRec/Error.hh"
#include "Ravl/PatternRec/FeatureSelectPlusLMinusR.hh"
#include "Ravl/PatternRec/FeatureSelectAsymmetricAdaBoost.hh"
#include "Ravl/PatternRec/ErrorBinaryClassifier.hh"

namespace RavlN {
  
  //: Constructor.
  
  DesignCascadeBodyC::DesignCascadeBodyC(const DesignClassifierSupervisedC & design) :
      m_design(design)
  {
  }
  
  DesignCascadeBodyC::DesignCascadeBodyC(const XMLFactoryContextC & factory) :
      DesignClassifierSupervisedWithValidationBodyC(factory)
  {
    if (!factory.UseComponent("Design", m_design)) {
      RavlIssueError("No Design Classifier in XML factory");
    }
  }
  
  //: Load from stream.
  
  DesignCascadeBodyC::DesignCascadeBodyC(std::istream &strm) :
      DesignClassifierSupervisedWithValidationBodyC(strm)
  {
    int version;
    strm >> version;
    if (version != 0)
      throw ExceptionOutOfRangeC("DesignCascadeBodyC::DesignCascadeBodyC(std::istream &), Unrecognised version number in stream. ");
    strm >> m_design;
  }
  
  //: Load from binary stream.
  
  DesignCascadeBodyC::DesignCascadeBodyC(BinIStreamC &strm) :
      DesignClassifierSupervisedWithValidationBodyC(strm)
  {
    int version;
    strm >> version;
    if (version != 0)
      throw ExceptionOutOfRangeC("DesignCascadeBodyC::DesignCascadeBodyC(BinIStreamC &), Unrecognised version number in stream. ");
    strm >> m_design;
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool DesignCascadeBodyC::Save(std::ostream &out) const
  {
    if (!DesignClassifierSupervisedBodyC::Save(out))
      return false;
    int version = 0;
    out << ' ' << version << ' ' << m_design;
    return true;
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool DesignCascadeBodyC::Save(BinOStreamC &out) const
  {
    if (!DesignClassifierSupervisedBodyC::Save(out))
      return false;
    int version = 0;
    out << version << m_design;
    return true;
  }
  
  ClassifierC DesignCascadeBodyC::Apply(const DataSetVectorLabelC & trainingSet,
      const DataSetVectorLabelC & validationSet)
  {
    RavlAssertMsg(0, "Abstract method called!");
    return ClassifierC();
  }
  //: Create a classifier from training and validation set
  
  //////////////////////////////////////////////////////////
  RavlN::XMLFactoryRegisterHandleConvertC<DesignCascadeC, DesignClassifierSupervisedWithValidationC> g_registerXMLFactoryDesignCascade("RavlN::DesignCascadeC");

  RAVL_INITVIRTUALCONSTRUCTOR_FULL(DesignCascadeBodyC, DesignCascadeC, DesignClassifierSupervisedWithValidationC);

  void linkDesignCascade()
  {
  }

}
