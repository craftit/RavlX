// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_LINEARCOMBINATION_HEADER
#define RAVL_LINEARCOMBINATION_HEADER 1
/////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! author="Robert Crida"
//! userlevel=Normal
//! docentry="Ravl.API.Pattern Recognition.Classifier"
//! file="Ravl/PatternRec/Classify/ClassifierLinearCombination.hh"

#include "Ravl/PatternRec/Classifier.hh"
#include "Ravl/SArray1dIter2.hh"

namespace RavlN {

  //! userlevel=Develop
  //: Linear combination classifier
  // Classifier which uses a weighted set of weak classifiers to classify into
  // one of two classes.
  
  class ClassifierLinearCombinationBodyC 
    : public ClassifierBodyC
  {
  public:
    ClassifierLinearCombinationBodyC(SArray1dC<ClassifierC> weakClassifiers, SArray1dC<RealT> weights, RealT threshold);
    //: Constructor.
    //!param: weakClassifiers - a set of classifiers to be combined
    //!param: weights         - relative weights for each classifier
    // Classification is label 1 if sum(weight[i]*classify[i](x)) >= 0.5sum(weight[i])
    
    ClassifierLinearCombinationBodyC(std::istream &strm);
    //: Load from stream.
    
    ClassifierLinearCombinationBodyC(BinIStreamC &strm);
    //: Load from binary stream.
    
    virtual bool Save (std::ostream &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual bool Save (BinOStreamC &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual UIntT Classify(const VectorC &data) const;
    //: Classifier vector 'data' return the most likely label.

    virtual UIntT Classify(const VectorC &data,const SArray1dC<IndexC> &featureSet) const;
    //: Classify vector 'data' using only the given subset of features
    
    virtual VectorC Apply(const VectorC & vec) const;
    //: Classify vector

    SArray1dC<ClassifierC> WeakClassifiers()
    { return m_weakClassifiers; }
    //: Access a list of classifiers used
    
    void SetWeakClassifiers(const SArray1dC<ClassifierC> &weakClassifiers)
    { m_weakClassifiers = weakClassifiers; }
    //: Set classifiers
    
    SArray1dC<RealT> WeakClassifierWeights()
    { return m_weights; }
    //: Access array of weights, one for each classifier.
    
    void SetWeakClassifierWeights(const SArray1dC<RealT> &weights);
    //: Set the array of weights, one for each classifier.
    // Note: The array must be of the same size as the number of weak classifiers.
    
  protected:
    SArray1dC<ClassifierC> m_weakClassifiers;
    SArray1dC<RealT> m_weights;
    RealT m_sumWeights;
    SArray1dC<IndexC> m_featureSet;
    RealT m_threshold;
  };
  
  //! userlevel=Normal
  //: Linear combination classifier
  // Classifier which uses a weighted set of weak classifiers to classify into
  // one of two classes.

  class ClassifierLinearCombinationC 
    : public ClassifierC
  {
  public:
    ClassifierLinearCombinationC()
    {}
    //: Default constructor 
    // Creates an invalid handle.

    ClassifierLinearCombinationC(SArray1dC<ClassifierC> weakClassifiers, SArray1dC<RealT> weights, RealT threshold)
      : ClassifierC(*new ClassifierLinearCombinationBodyC(weakClassifiers,weights,threshold))
    {}
    //: Constructor.
    
    ClassifierLinearCombinationC(std::istream &strm);
    //: Load from stream.
    
    ClassifierLinearCombinationC(BinIStreamC &strm);
    //: Load from binary stream.
    
    SArray1dC<ClassifierC> WeakClassifiers()
    { return Body().WeakClassifiers(); }
    //: Access a list of classifiers used
    
    void SetWeakClassifiers(const SArray1dC<ClassifierC> &weakClassifiers)
    { Body().SetWeakClassifiers(weakClassifiers); }
    //: Set classifiers
    
    SArray1dC<RealT> WeakClassifierWeights()
    { return Body().WeakClassifierWeights(); }
    //: Access array of weights, one for each classifier.
    
    void SetWeakClassifierWeights(const SArray1dC<RealT> &weights)
    { Body().SetWeakClassifierWeights(weights); }
    //: Set the array of weights, one for each classifier.
    // Note: The array must be of the same size as the number of weak classifiers.
    
  protected:
    ClassifierLinearCombinationC(ClassifierLinearCombinationBodyC &bod)
      : ClassifierC(bod)
    {}
    //: Body constructor.

    ClassifierLinearCombinationC(ClassifierLinearCombinationBodyC *bod)
      : ClassifierC(bod)
    {}
    //: Body ptr constructor.
    
    ClassifierLinearCombinationBodyC &Body()
    { return static_cast<ClassifierLinearCombinationBodyC &>(FunctionC::Body()); }
    //: Access body.
    
    const ClassifierLinearCombinationBodyC &Body() const
    { return static_cast<const ClassifierLinearCombinationBodyC &>(FunctionC::Body()); }
    //: Access body.

  };

  void InitRavlClassifierLinearCombinationIO();
      
}
#endif
