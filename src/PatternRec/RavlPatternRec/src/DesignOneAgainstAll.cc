// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id: DesignOneAgainstAll.cc 7590 2010-02-23 12:03:11Z kier $"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Classify/DesignOneAgainstAll.cc"

#include "Ravl/PatternRec/DesignOneAgainstAll.hh"
#include "Ravl/PatternRec/ClassifierOneAgainstAll.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/PatternRec/DataSetVectorLabel.hh"
#include "Ravl/SArray1dIter2.hh"
#include "Ravl/XMLFactoryRegister.hh"

namespace RavlN {
  
  //: Constructor.
  
  DesignOneAgainstAllBodyC::DesignOneAgainstAllBodyC(const DesignClassifierSupervisedC & design)
    : m_design(design)
  {}
  
  DesignOneAgainstAllBodyC::DesignOneAgainstAllBodyC(const XMLFactoryContextC & factory)
    : DesignClassifierSupervisedBodyC(factory)
  {
    if(!factory.UseComponent("Classifier", m_design))
      RavlIssueError("No Design Classifier in XML factory");    
  }
  
  //: Load from stream.
  
  DesignOneAgainstAllBodyC::DesignOneAgainstAllBodyC(std::istream &strm)
    : DesignClassifierSupervisedBodyC(strm)
  {
    int version;
    strm >> version;
    if(version != 0)
      throw ExceptionOutOfRangeC("DesignOneAgainstAllBodyC::DesignOneAgainstAllBodyC(std::istream &), Unrecognised version number in stream. ");
    strm >> m_design;
  }
  
  //: Load from binary stream.
  
  DesignOneAgainstAllBodyC::DesignOneAgainstAllBodyC(BinIStreamC &strm)
    : DesignClassifierSupervisedBodyC(strm)
  {
    int version;
    strm >> version;
    if(version != 0)
      throw ExceptionOutOfRangeC("DesignOneAgainstAllBodyC::DesignOneAgainstAllBodyC(BinIStreamC &), Unrecognised version number in stream. ");
    strm >> m_design;
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool DesignOneAgainstAllBodyC::Save (std::ostream &out) const {
    if(!DesignClassifierSupervisedBodyC::Save(out))
      return false;
    int version = 0;
    out << ' ' << version << ' ' << m_design;
    return true;
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool DesignOneAgainstAllBodyC::Save (BinOStreamC &out) const {
    if(!DesignClassifierSupervisedBodyC::Save(out))
      return false;
    int version = 0;
    out << version << m_design;
    return true;
  }
  
  //: Create a clasifier.
  
  ClassifierC DesignOneAgainstAllBodyC::Apply(const SampleC<VectorC> &in, const SampleC<UIntT> &out) {
    RavlAssertMsg(in.Size() == out.Size(),"DesignOneAgainstAllBodyC::Apply(), Sample of vector and labels should be the same size.");


    // OK here is where the fun starts
    RCHashC<UIntT, ClassifierC>classifiers;
    DataSetVectorLabelC dset(in,out);
    SArray1dC<SampleVectorC>samples = dset.SeperateLabels();

    for(SArray1dIterC<SampleVectorC>outIt(samples);outIt;outIt++) {

      DataSetVectorLabelC vl(in.Size() * dset.ClassNums().Sum());
      UIntT classLabel = outIt.Index().V();
      vl.Append(*outIt, 0);
      
      for(SArray1dIterC<SampleVectorC>inIt(samples);inIt;inIt++) {        
        if(outIt.Index() != inIt.Index()) {
          vl.Append(*inIt, 1);
        }        
      }
      
      std::cerr << "building classifier for label: " << classLabel << std::endl;
      // now we train our classifier for this label
      if(!m_design.Reset())
        RavlIssueError("Failed to reset our classifier");
      classifiers.Insert(classLabel, m_design.Apply(vl.Sample1(), vl.Sample2()));
    }

    return ClassifierOneAgainstAllC(classifiers);
  }
  
  //: Create a clasifier with weights for the samples.
  
  ClassifierC DesignOneAgainstAllBodyC::Apply(const SampleC<VectorC> &in,
							  const SampleC<UIntT> &out,
							  const SampleC<RealT> &weight) {
    RavlAssertMsg(0,"DesignOneAgainstAllBodyC::Apply(in,out,weight), Not implemented. Send a feature request! ");
    return ClassifierC();
  }
 
  //////////////////////////////////////////////////////////
  RavlN::XMLFactoryRegisterHandleConvertC<DesignOneAgainstAllC, DesignClassifierSupervisedC> g_registerXMLFactoryDesignOneAgainstAll("RavlN::DesignOneAgainstAllC");

  
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(DesignOneAgainstAllBodyC,DesignOneAgainstAllC,DesignClassifierSupervisedC);

  void linkDesignOneAgainstAll()
  {}
  
}
