// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_WEAKLINEAR_HEADER
#define RAVL_WEAKLINEAR_HEADER 1
/////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! author="Robert Crida"
//! userlevel=Normal
//! docentry="Ravl.API.Pattern Recognition.Classifier"
//! file="Ravl/PatternRec/Classify/ClassifierWeakLinear.hh"

#include "Ravl/PatternRec/Classifier.hh"

namespace RavlN {

  //! userlevel=Develop
  //: Weak linear classifier
  // A weak linear classifier which uses a single feature to classify into one
  // of two classes using a threshold.
  // This class can be used with the <a href="RavlN.DesignWeakLinearC.html"> DesignWeakLinearC </a> class. 

  class ClassifierWeakLinearBodyC 
    : public ClassifierBodyC
  {
  public:
    ClassifierWeakLinearBodyC(RealT threshold1, RealT parity1, RealT threshold2, RealT parity2);
    //: Constructor
    //!param: threshold1 - the first threshold value
    //!param: parity1    - parity (+1 or -1) used to indicate direction of inequality
    //!param: threshold2 - the second threshold value
    //!param: parity2    - parity (+1 or -1) used to indicate direction of inequality
    // Classification is label 1 if parity1*feature1 < parity1*threshold1 && 
    // parity2*feature1 < parity2*threshold2
    
    ClassifierWeakLinearBodyC(std::istream &strm);
    //: Load from stream.
    
    ClassifierWeakLinearBodyC(BinIStreamC &strm);
    //: Load from binary stream.
    
    virtual bool Save (std::ostream &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual bool Save (BinOStreamC &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual UIntT Classify(const VectorC &data) const
    { 
      RavlAssert(data.Size() == 1);
      return Classify(data,m_featureSet);
    }
    //: Classifier vector 'data' return the most likely label.

    virtual UIntT Classify(const VectorC &data,const SArray1dC<IndexC> &featureSet) const
    { 
      RavlAssert(featureSet.Size() == 1);
      RealT feature = data[featureSet[0]];
      return m_parity1 * feature < m_parityThreshold1 && m_parity2 * feature < m_parityThreshold2;
    }
    //: Classify vector using only the feature in the set
    
    virtual VectorC Confidence(const VectorC &data) const { 
      VectorC vec(2);
      vec.Fill(0.0);
      vec[Classify(data)] = 1.0;
      return vec;
    }
     //: Estimate the confidence for each label.
    // The meaning of the confidence assigned to each label depends
    // on the classifier used. The higher the confidence the more likely
    // it is the label is correct.
    // This class can be used with the <a href="RavlN.DesignWeakLinearC.html"> DesignWeakLinearC </a> class. 

  protected:
    SArray1dC<IndexC> m_featureSet;
    RealT m_parityThreshold1;
    RealT m_parity1;
    RealT m_parityThreshold2;
    RealT m_parity2;
  };
  
  //! userlevel=Normal
  //: Weak linear classifier
  // A weak linear classifier which uses a single feature to classify into one
  // of two classes using a threshold.
  // This class can be used with the <a href="RavlN.DesignWeakLinearC.html"> DesignWeakLinearC </a> class. 

  class ClassifierWeakLinearC 
    : public ClassifierC
  {
  public:
    ClassifierWeakLinearC()
    {}
    //: Default constructor
    // Creates an invalid handle.
    
    ClassifierWeakLinearC(RealT threshold1, RealT parity1, RealT threshold2, RealT parity2)
      : ClassifierC(*new ClassifierWeakLinearBodyC(threshold1,parity1,threshold2,parity2))
    {}
    //: Constructor
    //!param: threshold1 - the first threshold value
    //!param: parity1    - parity (+1 or -1) used to indicate direction of inequality
    //!param: threshold2 - the second threshold value
    //!param: parity2    - parity (+1 or -1) used to indicate direction of inequality
    // Classification is label 1 if parity1*feature1 < parity1*threshold1 && 
    // parity2*feature1 < parity2*threshold2
   
    ClassifierWeakLinearC(std::istream &strm);
    //: Load from stream.
    
    ClassifierWeakLinearC(BinIStreamC &strm);
    //: Load from binary stream.
    
  protected:
    ClassifierWeakLinearC(ClassifierWeakLinearBodyC &bod)
      : ClassifierC(bod)
    {}
    //: Body constructor.

    ClassifierWeakLinearC(ClassifierWeakLinearBodyC *bod)
      : ClassifierC(bod)
    {}
    //: Body ptr constructor.
  };
      
  void InitRavlClassifierWeakLinearIO();

}
#endif
