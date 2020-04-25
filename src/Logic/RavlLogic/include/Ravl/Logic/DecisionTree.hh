// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLLOGIC_DECISIONTREE_HEADER
#define RAVLLOGIC_DECISIONTREE_HEADER 1
/////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlLogic
//! file="Ravl/Logic/DecisionTree/DecisionTree.hh"
//! author="Charles Galambos"
//! docentry="Ravl.API.Logic.Decision Tree"

#include "Ravl/Logic/Literal.hh"
#include "Ravl/Logic/DecisionTreeBase.hh"

namespace RavlLogicN {
  
  //! userlevel=Develop
  //: Decision tree body.

  template<class DataT>
  class DecisionTreeBodyC 
    : public DecisionTreeBaseBodyC
  {
  public:
    DecisionTreeBodyC()
    {}
    //: Default constructor.
    
  protected:
    
  };
  
  //! userlevel=Normal
  //: Decision tree.
  
  template<class DataT>
  class DecisionTreeC 
    : public DecisionTreeBaseC 
  {
  public:
    DecisionTreeC()
    {}
    //: Default constructor.
    // creates an invalid handle.

    DecisionTreeC(istream &strm)
      : DecisionTreeBaseC(* new DecisionTreeBodyC<DataT>(strm))      
    {}
    //: Construct from a stream.
    
    DecisionTreeC(BinIStreamC &strm)
      : DecisionTreeBaseC(* new DecisionTreeBodyC<DataT>(strm))
    {}
    //: Construct from a binary stream.
    
  protected:
    DecisionTreeC(DecisionTreeBodyC<DataT> &bod)
      : DecisionTreeElementC(bod)
    {}
    //: Body constructor.
    
    DecisionTreeBodyC<DataT> &Body()
    { return static_cast<DecisionTreeBodyC<DataT> &>(DecisionTreeBaseC::Body()); }
    //: Access body.
    
    const DecisionTreeBodyC<DataT> &Body() const
    { return static_cast<const DecisionTreeBodyC<DataT> &>(DecisionTreeBaseC::Body()); }
    //: Access body.
    
  public:

  };
  
  template<class DataT>
  inline istream &operator>>(istream &strm,DecisionTreeC<DataT> &obj) {
    obj = DecisionTreeC<DataT>(strm);
    return strm;
  }
  //: Load from a stream.
  
  template<class DataT>
  inline ostream &operator<<(ostream &out,const DecisionTreeC<DataT> &obj) {
    //obj.Save(out);
    out << obj.Name();
    return out;
  }
  //: Save to a stream.
  
  template<class DataT>
  inline BinIStreamC &operator>>(BinIStreamC &strm,DecisionTreeC<DataT> &obj) {
    obj = DecisionTreeC<DataT>(strm);
    return strm;
  }
  //: Load from a binary stream.
  
  template<class DataT>
  inline BinOStreamC &operator<<(BinOStreamC &out,const DecisionTreeC<DataT> &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.

}

#endif
