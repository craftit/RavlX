// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DesignCascade_HEADER
#define RAVL_DesignCascade_HEADER 1
//! rcsid="$Id: DesignCascade.hh 7585 2010-02-23 09:36:10Z kier $"
//! lib=RavlPatternRec
//! author="Kieron Messer"
//! docentry="Ravl.API.Pattern Recognition.Classifier.DesignClassifier"
//! file="Ravl/PatternRec/Classify/DesignCascade.hh"

#include "Ravl/PatternRec/DesignClassifierSupervisedWithValidation.hh"
#include "Ravl/PatternRec/DataSetVectorLabel.hh"
#include "Ravl/XMLFactory.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Design a classifier cascade
  
  class DesignCascadeBodyC : public DesignClassifierSupervisedWithValidationBodyC
  {
  public:
    DesignCascadeBodyC(const DesignClassifierSupervisedC & classifier);
    //: Constructor.

    DesignCascadeBodyC(const XMLFactoryContextC & factory);
    //: Constructor.

    DesignCascadeBodyC(std::istream &strm);
    //: Load from stream.
    
    DesignCascadeBodyC(BinIStreamC &strm);
    //: Load from binary stream.
    
    virtual bool Save(std::ostream &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual bool Save(BinOStreamC &out) const;
    //: Writes object to stream, can be loaded using constructor

    virtual ClassifierC Apply(const DataSetVectorLabelC & trainingSet,
        const DataSetVectorLabelC & validationSet);
    //: Create a classifier from training and validation set
    
  protected:
    DesignClassifierSupervisedC m_design;

  };
  
  //! userlevel=Normal
  //: Design a one against all classifier

  class DesignCascadeC : public DesignClassifierSupervisedWithValidationC
  {
  public:
    DesignCascadeC()
    {
    }
    //: Default constructor.
    // Creates an invalid handle.
    
    DesignCascadeC(const DesignClassifierSupervisedC & classifier) :
        DesignClassifierSupervisedWithValidationC(*new DesignCascadeBodyC(classifier))
    {
    }
    //: Create a new designer.

    DesignCascadeC(const XMLFactoryContextC & factory) :
        DesignClassifierSupervisedWithValidationC(*new DesignCascadeBodyC(factory))
    {
    }
    //: Create a new designer.
    
    DesignCascadeC(std::istream &strm);
    //: Load from stream.
    
    DesignCascadeC(BinIStreamC &strm);
    //: Load from binary stream.

  protected:
    DesignCascadeC(DesignCascadeBodyC &bod) :
        DesignClassifierSupervisedWithValidationC(bod)
    {
    }
    //: Body constructor.

    DesignCascadeC(DesignCascadeBodyC *bod) :
        DesignClassifierSupervisedWithValidationC(bod)
    {
    }
    //: Body ptr constructor.
    
    DesignCascadeBodyC &Body()
    {
      return static_cast<DesignCascadeBodyC &>(DesignClassifierSupervisedWithValidationC::Body());
    }
    //: Access body.
    
    const DesignCascadeBodyC &Body() const
    {
      return static_cast<const DesignCascadeBodyC &>(DesignClassifierSupervisedWithValidationC::Body());
    }
    //: Access body.
    
  public:
    
  };

  inline std::istream &operator>>(std::istream &strm, DesignCascadeC &obj)
  {
    obj = DesignCascadeC(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.
  
  inline std::ostream &operator<<(std::ostream &out, const DesignCascadeC &obj)
  {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
  inline BinIStreamC &operator>>(BinIStreamC &strm, DesignCascadeC &obj)
  {
    obj = DesignCascadeC(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.
  
  inline BinOStreamC &operator<<(BinOStreamC &out, const DesignCascadeC &obj)
  {
    obj.Save(out);
    return out;
  }
//: Save to a stream.
// Uses virtual constructor.

}

#endif
