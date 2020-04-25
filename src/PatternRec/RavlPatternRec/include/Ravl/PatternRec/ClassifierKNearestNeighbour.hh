// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_KNEARESTNEIGHBOUR_HEADER
#define RAVL_KNEARESTNEIGHBOUR_HEADER 1
/////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! author="Charles Galambos"
//! userlevel=Normal
//! docentry="Ravl.API.Pattern Recognition.Classifier"
//! file="Ravl/PatternRec/Classify/ClassifierKNearestNeighbour.hh"
//! example=exKNearestNeighbour.cc

#include "Ravl/PatternRec/Distance.hh"
#include "Ravl/PatternRec/DataSet2.hh"
#include "Ravl/PatternRec/DistanceSqrEuclidean.hh"
#include "Ravl/Tuple2.hh"
#include "Ravl/PatternRec/SampleLabel.hh"
#include "Ravl/PatternRec/SampleVector.hh"
#include "Ravl/PatternRec/Classifier.hh"

namespace RavlN {

  //! userlevel=Develop
  //: k nearest neighbours classifier.
  // Classifier probe vector as the most common amoung its k nearest neighbours.
  // If there's a conflict the label with the smallest average distance is used.
  // This class can be used with the <a href="RavlN.DesignKNearestNeighbourC.html"> DesignKNearestNeighbourC </a> class. 

  
  class ClassifierKNearestNeighbourBodyC 
    : public ClassifierBodyC
  {
  public:
    ClassifierKNearestNeighbourBodyC(const DataSet2C<SampleVectorC,SampleLabelC> &data,
				     UIntT defaultK = 5,
				     const DistanceC &xdistMetric = DistanceSqrEuclideanC());
    //: Constructor.
    
    ClassifierKNearestNeighbourBodyC(std::istream &strm);
    //: Load from stream.
    
    ClassifierKNearestNeighbourBodyC(BinIStreamC &strm);
    //: Load from binary stream.
    
    virtual bool Save (std::ostream &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual bool Save (BinOStreamC &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual SArray1dC<Tuple2C<UIntT,RealT> > Search(const VectorC &ex,UIntT k) const;
    //: Search for the the 'k' nearest neighbours.
    // Returns the label and the distance for the 'k' nearest neighbours.
    
    virtual UIntT Classify(const VectorC &data) const;
    //: Classifier vector 'data' return the most likely label.
    
    virtual VectorC Confidence(const VectorC &data) const;
     //: Estimate the confidence for each label.
    // The meaning of the confidence assigned to each label depends
    // on the classifier used. The higher the confidence the more likely
    // it is the label is correct.
    
    UIntT K() const
    { return defaultK; }
    //: Number of samples to use when classifying.
    
  protected:
    UIntT defaultK; // Number of samples to use in classification.
    
    DistanceC distanceMetric; // Distance metric to use.
    DataSet2C<SampleVectorC,SampleLabelC> data;
  };
  
  //! userlevel=Normal
  //: k nearest neighbours classifier.
  // Classifier probe vector as the most common among its k nearest neighbours.
  // If there's a conflict the label with the smallest average distance is used.
  // This class can be used with the <a href="RavlN.DesignKNearestNeighbourC.html"> DesignKNearestNeighbourC </a> class. 

  class ClassifierKNearestNeighbourC 
    : public ClassifierC
  {
  public:
    ClassifierKNearestNeighbourC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    ClassifierKNearestNeighbourC(const DataSet2C<SampleVectorC,SampleLabelC> &data,UIntT defaultK = 5,const DistanceC &xdistMetric = DistanceSqrEuclideanC())
      : ClassifierC(*new ClassifierKNearestNeighbourBodyC(data,defaultK,xdistMetric))
    {}
    //: Constructor.
    
    ClassifierKNearestNeighbourC(std::istream &strm);
    //: Load from stream.
    
    ClassifierKNearestNeighbourC(BinIStreamC &strm);
    //: Load from binary stream.
    
  protected:
    ClassifierKNearestNeighbourC(ClassifierKNearestNeighbourBodyC &bod)
      : ClassifierC(bod)
    {}
    //: Body constructor.

    ClassifierKNearestNeighbourC(ClassifierKNearestNeighbourBodyC *bod)
      : ClassifierC(bod)
    {}
    //: Body ptr constructor.
    
    ClassifierKNearestNeighbourBodyC &Body()
    { return static_cast<ClassifierKNearestNeighbourBodyC &>(ClassifierC::Body()); }
    //: Access body.
    
    const ClassifierKNearestNeighbourBodyC &Body() const
    { return static_cast<const ClassifierKNearestNeighbourBodyC &>(ClassifierC::Body()); }
    //: Access body.
    
  public:
    SArray1dC<Tuple2C<UIntT,RealT> > Search(const VectorC &ex,UIntT k)
    { return Body().Search(ex,k); }
    //: Search for the the 'k' nearest neighbours.
    // Returns the label and the distance for the 'k' nearest neighbours.
    
    UIntT K() const
    { return Body().K(); }
    //: Number of samples to use when classifying.
    
  };
  
}
#endif
