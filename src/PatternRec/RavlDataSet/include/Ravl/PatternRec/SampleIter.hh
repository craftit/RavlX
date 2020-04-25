// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_SAMPLEITER_HEADER
#define RAVL_SAMPLEITER_HEADER 1
/////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Pattern Recognition.Data Set"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/DataSet/SampleIter.hh"
//! author="Charles Galambos"

#include "Ravl/PatternRec/Sample.hh"
#include "Ravl/DArray1dIter.hh"

namespace RavlN {
  
  //! userlevel=Normal
  //: Iterate through a sample.
  
  template<class DataT>
  class SampleIterC 
    : public DArray1dIterC<DataT>
  {
  public:
    SampleIterC()
    {}
    //: Default construtor.
    
    SampleIterC(const SampleC<DataT> &nds)
      : DArray1dIterC<DataT>(nds.DArray())
    {}
    //: Construct from a data set.
    
  };
  
}

#endif
