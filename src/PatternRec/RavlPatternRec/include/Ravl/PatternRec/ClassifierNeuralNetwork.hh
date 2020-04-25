// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_CLASSIFIERNEURALNETWORK_HEADER
#define RAVL_CLASSIFIERNEURALNETWORK_HEADER 1
///////////////////////////////////////////////////////////////////////
//! rcsid="$Id: ClassifierNeuralNetwork.hh,v 1.8 2005/12/06 17:16:49 plugger Exp $"
//! author="Charles Galambos"
//! docentry="Ravl.API.Pattern Recognition.Classifier"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Classify/ClassifierNeuralNetwork.hh"

#include "Ravl/PatternRec/Classifier.hh"
#include "Ravl/DList.hh"
#include "Ravl/String.hh"
#include "Ravl/fann/fann.h"

namespace RavlN {
  
  //! userlevel=Develop
  //: A classifier based on fann neural network library.
  //  This classifier returns the label with the highest value.
  // This class can be used with the <a href="RavlN.DesignClassifierNeuralNetworkC.html"> DesignClassifierNeuralNetworkC </a> class. 

  class ClassifierNeuralNetworkBodyC 
    : public ClassifierBodyC
  {
  public:
    ClassifierNeuralNetworkBodyC(UIntT nLayers, UIntT nInputs, UIntT nHidden, UIntT nOutputs, const DListC<StringC> & nn);
    //: Create classifier from a file
    
    ClassifierNeuralNetworkBodyC(std::istream &strm);
    //: Load from stream.
    
    ClassifierNeuralNetworkBodyC(BinIStreamC &strm);
    //: Load from binary stream.
    
    virtual bool Save (std::ostream &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual bool Save (BinOStreamC &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual UIntT Classify(const VectorC &data) const;
    //: Classifier vector 'data' return the most likely label.
    
    using ClassifierBodyC::Apply;

    virtual VectorC Apply(const VectorC &data) const;
    //: Estimate the confidence for each label.
    // The meaning of the confidence assigned to each label depends
    // on the function used. The higher the confidence the more likely
    // it is the label is correct. The sum of the vector has been normalised
    // to 1.
    
  protected:
    bool init();
    // init the network

    //: The neural network
    UIntT m_nLayers;
    UIntT m_nInputs;
    UIntT m_nHidden;
    UIntT m_nOutputs;
    DListC<StringC> m_data; 
    //: Workspace
    fann * m_ann;
    mutable fann_type *m_output;
  };
  
  //! userlevel=Normal
  //: A classifier based on fann neural network library.
  //  This classifier returns the label with the highest value.
  // This class can be used with the <a href="RavlN.DesignClassifierNeuralNetworkC.html"> DesignClassifierNeuralNetworkC </a> class. 

  class ClassifierNeuralNetworkC 
    : public ClassifierC
  {
  public:
    ClassifierNeuralNetworkC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    ClassifierNeuralNetworkC(UIntT nLayers, UIntT nInputs, UIntT nHidden, UIntT nOutputs, const DListC<StringC> & nn)
      : ClassifierC(*new ClassifierNeuralNetworkBodyC(nLayers, nInputs, nHidden, nOutputs, nn))
    {}
    //: Create classifier from a discriminant function.
    
    ClassifierNeuralNetworkC(std::istream &strm);
    //: Load from stream.
    
    ClassifierNeuralNetworkC(BinIStreamC &strm);
    //: Load from binary stream.
    
  protected:
    ClassifierNeuralNetworkC(ClassifierNeuralNetworkBodyC &bod)
      : ClassifierC(bod)
    {}
    //: Body constructor.

    ClassifierNeuralNetworkC(ClassifierNeuralNetworkBodyC *bod)
      : ClassifierC(bod)
    {}
    //: Body constructor.
    
    ClassifierNeuralNetworkBodyC &Body()
    { return static_cast<ClassifierNeuralNetworkBodyC &>(ClassifierC::Body()); }
    //: Access body.

    const ClassifierNeuralNetworkBodyC &Body() const
    { return static_cast<const ClassifierNeuralNetworkBodyC &>(ClassifierC::Body()); }
    //: Access body.
    
  public:
    
  };
  
  inline std::istream &operator>>(std::istream &strm,ClassifierNeuralNetworkC &obj) {
    obj = ClassifierNeuralNetworkC(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.
  
  inline std::ostream &operator<<(std::ostream &out,const ClassifierNeuralNetworkC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
  inline BinIStreamC &operator>>(BinIStreamC &strm,ClassifierNeuralNetworkC &obj) {
    obj = ClassifierNeuralNetworkC(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.
  
  inline BinOStreamC &operator<<(BinOStreamC &out,const ClassifierNeuralNetworkC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
}


#endif
