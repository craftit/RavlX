// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_CLASSIFIERFUZZYCMEANS_HEADER
#define RAVL_CLASSIFIERFUZZYCMEANS_HEADER 1
/////////////////////////////////////////////////////////////
//! lib=RavlPatternRec
//! author="Charles Galambos"
//! userlevel=Normal
//! docentry="Ravl.API.Pattern Recognition.Classifier"
//! file="Ravl/PatternRec/Cluster/ClassifierFuzzyCMeans.hh"
//! example="exFuzzyCMeansCluster.cc"

#include "Ravl/PatternRec/Distance.hh"
#include "Ravl/PatternRec/DataSet2.hh"
#include "Ravl/PatternRec/DistanceSqrEuclidean.hh"
#include "Ravl/Tuple2.hh"
#include "Ravl/PatternRec/SampleLabel.hh"
#include "Ravl/PatternRec/SampleVector.hh"
#include "Ravl/PatternRec/Classifier.hh"

namespace RavlN {

  //! userlevel=Develop
  //: Fuzzy C-Means classifier
  // This class can be used with the <a href="RavlN.DesignFuzzyCMeansClusterC.html"> DesignFuzzyCMeansClusterC </a> class. 

  
  class ClassifierFuzzyCMeansBodyC 
    : public ClassifierBodyC
  {
  public:
    ClassifierFuzzyCMeansBodyC(const SampleVectorC &vectors,
			       RealT m,
			       const DistanceC &xdistMetric = DistanceSqrEuclideanC());
    //: Constructor.
    
    ClassifierFuzzyCMeansBodyC(istream &strm);
    //: Load from stream.
    
    ClassifierFuzzyCMeansBodyC(BinIStreamC &strm);
    //: Load from binary stream.
    
    virtual bool Save (ostream &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual bool Save (BinOStreamC &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual VectorC Apply(const VectorC &data) const;
    //: Classify vector.
    
    virtual UIntT Classify(const VectorC &data) const;
    //: Classifier vector 'data' return the most likely label.
    
  protected:    
    DistanceC distanceMetric; // Distance metric to use.
    SampleVectorC vectors;
    RealT _m; //: Class membership weighting exponent
  };
  
  //! userlevel=Normal
  //: Fuzzy C-Means classifier
   // This class can be used with the <a href="RavlN.DesignFuzzyCMeansClusterC.html"> DesignFuzzyCMeansClusterC </a> class. 

  
  class ClassifierFuzzyCMeansC 
    : public ClassifierC
  {
  public:
    ClassifierFuzzyCMeansC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    ClassifierFuzzyCMeansC(const SampleVectorC &vectors,
			   RealT m,
			   const DistanceC &xdistMetric = DistanceSqrEuclideanC())
      : ClassifierC(*new ClassifierFuzzyCMeansBodyC(vectors,m,xdistMetric))
    {}
    //: Constructor.
    
    ClassifierFuzzyCMeansC(istream &strm);
    //: Load from stream.
    
    ClassifierFuzzyCMeansC(BinIStreamC &strm);
    //: Load from binary stream.

  protected:
    ClassifierFuzzyCMeansC(ClassifierFuzzyCMeansBodyC &bod)
      : ClassifierC(bod)
    {}
    //: Body constructor.
    
    ClassifierFuzzyCMeansBodyC &Body()
    { return static_cast<ClassifierFuzzyCMeansBodyC &>(ClassifierC::Body()); }
    //: Access body.
    
    const ClassifierFuzzyCMeansBodyC &Body() const
    { return static_cast<const ClassifierFuzzyCMeansBodyC &>(ClassifierC::Body()); }
    //: Access body.
    
  public:
    
  };
  
  
  inline istream &operator>>(istream &strm,ClassifierFuzzyCMeansC &obj) {
    obj = ClassifierFuzzyCMeansC(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.
  
  inline ostream &operator<<(ostream &out,const ClassifierFuzzyCMeansC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
  inline BinIStreamC &operator>>(BinIStreamC &strm,ClassifierFuzzyCMeansC &obj) {
    obj = ClassifierFuzzyCMeansC(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.
  
  inline BinOStreamC &operator<<(BinOStreamC &out,const ClassifierFuzzyCMeansC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
}
#endif
