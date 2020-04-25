// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Classify/DesignKNearestNeighbour.cc"

#include "Ravl/PatternRec/DesignKNearestNeighbour.hh"
#include "Ravl/PatternRec/ClassifierKNearestNeighbour.hh"
#include "Ravl/PatternRec/ClassifierAverageNearestNeighbour.hh"
#include "Ravl/PatternRec/DistanceEuclidean.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/XMLFactoryRegister.hh"

namespace RavlN {
  
  //: Constructor.
  
  DesignKNearestNeighbourBodyC::DesignKNearestNeighbourBodyC(UIntT nk,const DistanceC &distMetric,bool nuseAverageKNN)
    : k(nk),
      distanceMetric(distMetric),
      useAverageKNN(nuseAverageKNN)
  {}
  
  //: Load from XML factory
  
  DesignKNearestNeighbourBodyC::DesignKNearestNeighbourBodyC(const XMLFactoryContextC & factory)
    : DesignClassifierSupervisedBodyC(factory),
      k(factory.AttributeUInt("k", 3)),
      useAverageKNN(factory.AttributeBool("use_average_knn", false))
  {
    if(!factory.UseComponent("Distance", distanceMetric))
      RavlIssueError("Unable to initialise distance metric from XML Factory");
  }
  
  //: Load from stream.
  
  DesignKNearestNeighbourBodyC::DesignKNearestNeighbourBodyC(std::istream &strm)
    : DesignClassifierSupervisedBodyC(strm),
      k(0),
      useAverageKNN(false)
  {
    int version;
    strm >> version;
    if(version != 0)
      throw ExceptionOutOfRangeC("DesignKNearestNeighbourBodyC::DesignKNearestNeighbourBodyC(std::istream &), Unrecognised version number in stream. ");
    strm >> k >> distanceMetric >> useAverageKNN;
  }
  
  //: Load from binary stream.
  
  DesignKNearestNeighbourBodyC::DesignKNearestNeighbourBodyC(BinIStreamC &strm)
    : DesignClassifierSupervisedBodyC(strm),
      k(0),
      useAverageKNN(false)
  {
    int version;
    strm >> version;
    if(version != 0)
      throw ExceptionOutOfRangeC("DesignKNearestNeighbourBodyC::DesignKNearestNeighbourBodyC(BinIStreamC &), Unrecognised version number in stream. ");
    strm >> k >> distanceMetric >> useAverageKNN;
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool DesignKNearestNeighbourBodyC::Save (std::ostream &out) const {
    if(!DesignClassifierSupervisedBodyC::Save(out))
      return false;
    int version = 0;
    out << ' ' << version << ' ' << k << ' ' << distanceMetric << ' ' << useAverageKNN;
    return true;
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool DesignKNearestNeighbourBodyC::Save (BinOStreamC &out) const {
    if(!DesignClassifierSupervisedBodyC::Save(out))
      return false;
    int version = 0;
    out << version << k << distanceMetric << useAverageKNN;
    return true;
  }
  
  //: Create a classifier.
  
  ClassifierC DesignKNearestNeighbourBodyC::Apply(const SampleC<VectorC> &in,const SampleC<UIntT> &out) {
    RavlAssertMsg(in.Size() == out.Size(),"DesignKNearestNeighbourBodyC::Apply(), Sample of vector and labels should be the same size.");
    if(useAverageKNN)
      return ClassifierAverageNearestNeighbourC (DataSet2C<SampleVectorC,SampleLabelC>(in,out),k,distanceMetric);
    return ClassifierKNearestNeighbourC (DataSet2C<SampleVectorC,SampleLabelC>(in,out),k,distanceMetric);
  }
  
  //: Create a classifier with weights for the samples.
  
  ClassifierC DesignKNearestNeighbourBodyC::Apply(const SampleC<VectorC> &in,
						  const SampleC<UIntT> &out,
						  const SampleC<RealT> &weight) {
    RavlAssertMsg(0,"DesignKNearestNeighbourBodyC::Apply(in,out,weight), Not implemented. Send a feature request! ");
    return ClassifierC();
  }
 
  //////////////////////////////////////////////////////////
  
  RavlN::XMLFactoryRegisterHandleConvertC<DesignKNearestNeighbourC, DesignClassifierSupervisedC> g_registerXMLFactoryDesignKNearestNeighbour("RavlN::DesignKNearestNeighbourC");

  RAVL_INITVIRTUALCONSTRUCTOR_FULL(DesignKNearestNeighbourBodyC,DesignKNearestNeighbourC,DesignClassifierSupervisedC);

  void linkDesignKNearestNeighbour()
  {}

}
