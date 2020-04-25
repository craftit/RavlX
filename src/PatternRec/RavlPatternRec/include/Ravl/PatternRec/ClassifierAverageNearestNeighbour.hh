// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_CLASSIFYAVERAGENEARESTNEIGHBOUR_HEADER
#define RAVL_CLASSIFYAVERAGENEARESTNEIGHBOUR_HEADER 1
/////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! userlevel=Normal
//! docentry="Ravl.API.Pattern Recognition.Classifier"
//! author="Charles Galambos"
//! file="Ravl/PatternRec/Classify/ClassifierAverageNearestNeighbour.hh"

#include "Ravl/PatternRec/ClassifierKNearestNeighbour.hh"

namespace RavlN {

  //! userlevel=Develop
  //: ClassifierAverage k nearest neighbour classifier.
  // Among the k nearest neighbours to the probe choose the label with the smallest average distance.
  // This class can be used with the <a href="RavlN.DesignKNearestNeighbourC.html"> DesignKNearestNeighbourC </a> class. 
    
  class ClassifierAverageNearestNeighbourBodyC 
    : public ClassifierKNearestNeighbourBodyC
  {
  public:
    ClassifierAverageNearestNeighbourBodyC(const DataSet2C<SampleVectorC,SampleLabelC> &data,
					   UIntT defaultK = 5,
					   const DistanceC &xdistMetric = DistanceSqrEuclideanC());
    //: Default constructor.
    
    ClassifierAverageNearestNeighbourBodyC(std::istream &strm);
    //: Load from stream.
    
    ClassifierAverageNearestNeighbourBodyC(BinIStreamC &strm);
    //: Load from binary stream.
    
    virtual bool Save (std::ostream &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual bool Save (BinOStreamC &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual UIntT Classify(const VectorC &data) const;
    //: Classifier vector 'data' return the most likely label.
    
    virtual VectorC Confidence(const VectorC &data) const;
     //: Estimate the confidence for each label.
    // The meaning of the confidence assigned to each label depends
    // on the classifier used. The higher the confidence the more likely
    // it is the label is correct.
    
  };
  
  //! userlevel=Normal
  //: ClassifierAverage k nearest neighbour classifier.
  // Amoung the k nearest neighours to the probe choose the label with the smallest average distance.
  // This class can be used with the <a href="RavlN.DesignKNearestNeighbourC.html"> DesignKNearestNeighbourC </a> class. 

  class ClassifierAverageNearestNeighbourC 
    : public ClassifierKNearestNeighbourC
  {
  public:
    ClassifierAverageNearestNeighbourC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    ClassifierAverageNearestNeighbourC(const DataSet2C<SampleVectorC,SampleLabelC> &data,UIntT defaultK = 5,const DistanceC &xdistMetric = DistanceSqrEuclideanC())
      : ClassifierKNearestNeighbourC(*new ClassifierAverageNearestNeighbourBodyC(data,defaultK,xdistMetric))
    {}
    //: Constructor.
    
    ClassifierAverageNearestNeighbourC(std::istream &strm);
    //: Load from stream.
    
    ClassifierAverageNearestNeighbourC(BinIStreamC &strm);
    //: Load from binary stream.

  protected:
    ClassifierAverageNearestNeighbourC(ClassifierAverageNearestNeighbourBodyC &bod)
      : ClassifierKNearestNeighbourC(bod)
    {}
    //: Body constructor.

    ClassifierAverageNearestNeighbourC(ClassifierAverageNearestNeighbourBodyC *bod)
      : ClassifierKNearestNeighbourC(bod)
    {}
    //: Body ptr constructor.
    
    ClassifierAverageNearestNeighbourBodyC &Body()
    { return static_cast<ClassifierAverageNearestNeighbourBodyC &>(ClassifierC::Body()); }
    //: Access body.
    
    const ClassifierAverageNearestNeighbourBodyC &Body() const
    { return static_cast<const ClassifierAverageNearestNeighbourBodyC &>(ClassifierC::Body()); }
    //: Access body.
    
  public:
    
  };
  
}
#endif
