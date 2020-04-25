// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DATASETBASE_HEADER
#define RAVL_DATASETBASE_HEADER 1
//! rcsid="$Id$"
//! author="Kieron Messer"
//! docentry="Ravl.API.Pattern Recognition.Data Set"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/DataSet/DataSetBase.hh"

#include "Ravl/Collection.hh"
#include "Ravl/PatternRec/Sample.hh"

namespace RavlN {
  
  class DataSetBaseC;
  
  //! userlevel=Develop
  //: Data set base class.
  
  class DataSetBaseBodyC 
    : public RCBodyVC
  {
    
  public:
    DataSetBaseBodyC()
    {}
    //: Default constructor.
    
    DataSetBaseBodyC(UIntT estSize)
    {}
    //: Construct with an estimated size.
    
    static CollectionC<UIntT> RandomIndexSet(UIntT size);
    //: Generate a random index set from this collection of with the given size.
    // Size is limited to the size of this set.
    
    virtual void Shuffle();
    //: Shuffle the order of the dataset.
    
  protected:
    
  };
  
  //! userlevel=Normal
  //: Data set base class
  
  class DataSetBaseC
    : public RCHandleC<DataSetBaseBodyC>
  {
  public:
    DataSetBaseC()
    {}
    //: Default dataset base constructor.
    
  protected:    
    DataSetBaseC(DataSetBaseBodyC &bod)
      : RCHandleC<DataSetBaseBodyC >(bod)
    { }
    //: Body constructor.
    
    DataSetBaseBodyC &Body()
    { return RCHandleC<DataSetBaseBodyC>::Body(); }
    //: Access body.

    const DataSetBaseBodyC &Body() const
    { return RCHandleC<DataSetBaseBodyC>::Body(); }
    //: Access body.
    
  public:
    void Shuffle()
    { Body().Shuffle(); }
    //: Shuffle the order of the dataset.
    // This is an in place operation.
    
    friend class DataSetBaseBodyC;
  };
  


}

#endif
