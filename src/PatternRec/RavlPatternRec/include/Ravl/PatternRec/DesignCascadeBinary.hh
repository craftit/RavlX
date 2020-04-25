// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DesignCascadeBinary_HEADER
#define RAVL_DesignCascadeBinary_HEADER 1
//! rcsid="$Id: DesignCascadeBinary.hh 7585 2010-02-23 09:36:10Z kier $"
//! lib=RavlPatternRec
//! author="Kieron Messer"
//! docentry="Ravl.API.Pattern Recognition.Classifier.DesignClassifier"
//! file="Ravl/PatternRec/Classify/DesignCascadeBinary.hh"

#include "Ravl/PatternRec/DesignCascade.hh"
#include "Ravl/PatternRec/FeatureSelector.hh"
#include "Ravl/XMLFactory.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Design a classifier cascade
  
  class DesignCascadeBinaryBodyC : public DesignCascadeBodyC
  {
  public:
    DesignCascadeBinaryBodyC(const DesignClassifierSupervisedC & classifier,
        const FeatureSelectorC & featureSelector,
        UIntT maxStages,
        RealT targetErrorRate);
    //: Constructor.

    DesignCascadeBinaryBodyC(const XMLFactoryContextC & factory);
    //: Constructor.

    DesignCascadeBinaryBodyC(std::istream &strm);
    //: Load from stream.
    
    DesignCascadeBinaryBodyC(BinIStreamC &strm);
    //: Load from binary stream.
    
    virtual bool Save(std::ostream &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual bool Save(BinOStreamC &out) const;
    //: Writes object to stream, can be loaded using constructor

    virtual ClassifierC Apply(const DataSetVectorLabelC & trainingSet, const DataSetVectorLabelC & validationSet);
    //: Create a classifier from training and validation set
    
  protected:
    FeatureSelectorC m_featureSelector;
    UIntT m_maxStages;
    RealT m_targetErrorRate;
  };
  
  //! userlevel=Normal
  //: Design a one against all classifier

  class DesignCascadeBinaryC : public DesignCascadeC
  {
  public:
    DesignCascadeBinaryC()
    {
    }
    //: Default constructor.
    // Creates an invalid handle.
    
    DesignCascadeBinaryC(const DesignClassifierSupervisedC & classifier,
        const FeatureSelectorC & featureSelector,
        UIntT maxStages = 100,
        RealT targetErrorRate = 0.01) :
        DesignCascadeC(*new DesignCascadeBinaryBodyC(classifier, featureSelector, maxStages, targetErrorRate))
    {
    }
    //: Create a new designer.

    DesignCascadeBinaryC(const XMLFactoryContextC & factory) :
        DesignCascadeC(*new DesignCascadeBinaryBodyC(factory))
    {
    }
    //: Create a new designer.
    
    DesignCascadeBinaryC(std::istream &strm);
    //: Load from stream.
    
    DesignCascadeBinaryC(BinIStreamC &strm);
    //: Load from binary stream.

  protected:
    DesignCascadeBinaryC(DesignCascadeBinaryBodyC &bod) :
        DesignCascadeC(bod)
    {
    }
    //: Body constructor.

    DesignCascadeBinaryC(DesignCascadeBinaryBodyC *bod) :
        DesignCascadeC(bod)
    {
    }
    //: Body ptr constructor.
    
    DesignCascadeBinaryBodyC &Body()
    {
      return static_cast<DesignCascadeBinaryBodyC &>(DesignCascadeC::Body());
    }
    //: Access body.
    
    const DesignCascadeBinaryBodyC &Body() const
    {
      return static_cast<const DesignCascadeBinaryBodyC &>(DesignCascadeC::Body());
    }
    //: Access body.
    
  public:
    
  };
  
  inline std::istream &operator>>(std::istream &strm, DesignCascadeBinaryC &obj)
  {
    obj = DesignCascadeBinaryC(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.
  
  inline std::ostream &operator<<(std::ostream &out, const DesignCascadeBinaryC &obj)
  {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
  inline BinIStreamC &operator>>(BinIStreamC &strm, DesignCascadeBinaryC &obj)
  {
    obj = DesignCascadeBinaryC(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.
  
  inline BinOStreamC &operator<<(BinOStreamC &out, const DesignCascadeBinaryC &obj)
  {
    obj.Save(out);
    return out;
  }
//: Save to a stream.
// Uses virtual constructor.

}

#endif
