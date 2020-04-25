// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_NEARESTNEIGHBOUR_HEADER
#define RAVL_NEARESTNEIGHBOUR_HEADER 1
/////////////////////////////////////////////////////////////
//! lib=RavlPatternRec
//! author="Charles Galambos"
//! userlevel=Normal
//! docentry="Ravl.API.Pattern Recognition.Classifier"
//! file="Ravl/PatternRec/Cluster/ClassifierNearestNeighbour.hh"

#include "Ravl/PatternRec/Distance.hh"
#include "Ravl/PatternRec/DataSet2.hh"
#include "Ravl/PatternRec/DistanceSqrEuclidean.hh"
#include "Ravl/Tuple2.hh"
#include "Ravl/PatternRec/SampleLabel.hh"
#include "Ravl/PatternRec/SampleVector.hh"
#include "Ravl/PatternRec/Classifier.hh"

namespace RavlN {

  //! userlevel=Develop
  //: Nearest neighbours classifier.
  // Classifier probe vector as the closest neighbour.
  // This class can be used with the <a href="RavlN.DesignKNearestNeighbourC.html"> DesignKNearestNeighbourC </a> class. 

  class ClassifierNearestNeighbourBodyC 
    : public ClassifierBodyC
  {
  public:
    ClassifierNearestNeighbourBodyC(const SampleVectorC &vectors,
				    const SampleLabelC &labels,
				    const DistanceC &xdistMetric = DistanceSqrEuclideanC());
    //: Constructor.

    ClassifierNearestNeighbourBodyC(const SampleVectorC &vectors,
				    const DistanceC &xdistMetric = DistanceSqrEuclideanC());
    //: Constructor.
    
    ClassifierNearestNeighbourBodyC(istream &strm);
    //: Load from stream.
    
    ClassifierNearestNeighbourBodyC(BinIStreamC &strm);
    //: Load from binary stream.
    
    virtual bool Save (ostream &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual bool Save (BinOStreamC &out) const;
    //: Writes object to stream, can be loaded using constructor

    virtual UIntT Classify(const VectorC &data) const;
    //: Classifier vector 'data' return the most likely label.

    const DistanceC &DistanceMetric() const
    { return distanceMetric; }
    //: Distance metric to use.
    
    const SampleVectorC &Vectors() const
    { return vectors; }
    //: Access vectors

    const SampleLabelC &Labels() const
    { return vlabels; }
    //: Access labels

    SampleVectorC &Vectors()
    { return vectors; }
    //: Access vectors

    SampleLabelC &Labels()
    { return vlabels; }
    //: Access labels

  protected:    
    DistanceC distanceMetric; // Distance metric to use.
    SampleVectorC vectors;
    SampleLabelC vlabels;
  };
  
  //! userlevel=Normal
  //: Nearest neighbours classifier.
  // Classifier probe vector as the closest neighbour.
    // This class can be used with the <a href="RavlN.DesignKNearestNeighbourC.html"> DesignKNearestNeighbourC </a> class. 

  class ClassifierNearestNeighbourC 
    : public ClassifierC
  {
  public:
    ClassifierNearestNeighbourC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    ClassifierNearestNeighbourC(const SampleVectorC &vectors,
				const SampleLabelC &labels,
				const DistanceC &xdistMetric = DistanceSqrEuclideanC())
      : ClassifierC(*new ClassifierNearestNeighbourBodyC(vectors,labels,xdistMetric))
    {}
    //: Constructor.
    
    ClassifierNearestNeighbourC(const SampleVectorC &vectors,
				const DistanceC &xdistMetric = DistanceSqrEuclideanC())
      : ClassifierC(*new ClassifierNearestNeighbourBodyC(vectors,xdistMetric))
    {}
    //: Constructor.
    
    ClassifierNearestNeighbourC(istream &strm);
    //: Load from stream.
    
    ClassifierNearestNeighbourC(BinIStreamC &strm);
    //: Load from binary stream.

    ClassifierNearestNeighbourC(const ClassifierC &classifier)
     : ClassifierC(dynamic_cast<const ClassifierNearestNeighbourBodyC *>(classifier.BodyPtr()))
    {}
    //: Base class constructor.
    // Will create an invalid handle if classes don't match

  protected:
    ClassifierNearestNeighbourC(ClassifierNearestNeighbourBodyC &bod)
      : ClassifierC(bod)
    {}
    //: Body constructor.
    
    ClassifierNearestNeighbourC(const ClassifierNearestNeighbourBodyC *bod)
      : ClassifierC(bod)
    {}
    //: Body constructor.
    
    ClassifierNearestNeighbourBodyC &Body()
    { return static_cast<ClassifierNearestNeighbourBodyC &>(ClassifierC::Body()); }
    //: Access body.
    
    const ClassifierNearestNeighbourBodyC &Body() const
    { return static_cast<const ClassifierNearestNeighbourBodyC &>(ClassifierC::Body()); }
    //: Access body.
    
  public:

    const DistanceC &DistanceMetric() const
    { return Body().DistanceMetric(); }
    //: Distance metric used.
    
    const SampleVectorC &Vectors() const
    { return Body().Vectors(); }
    //: Access vectors

    SampleVectorC &Vectors()
    { return Body().Vectors(); }
    //: Access vectors

    const SampleLabelC &Labels() const
    { return Body().Labels(); }
    //: Access labels

    SampleLabelC &Labels()
    { return Body().Labels(); }
    //: Access labels

  };
  
  
  inline istream &operator>>(istream &strm,ClassifierNearestNeighbourC &obj) {
    obj = ClassifierNearestNeighbourC(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.
  
  inline ostream &operator<<(ostream &out,const ClassifierNearestNeighbourC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
  inline BinIStreamC &operator>>(BinIStreamC &strm,ClassifierNearestNeighbourC &obj) {
    obj = ClassifierNearestNeighbourC(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.
  
  inline BinOStreamC &operator<<(BinOStreamC &out,const ClassifierNearestNeighbourC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
}
#endif
