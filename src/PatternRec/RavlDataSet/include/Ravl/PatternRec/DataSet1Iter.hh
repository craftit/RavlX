// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DATASET1ITER_HEADER
#define RAVL_DATASET1ITER_HEADER 1
/////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Pattern Recognition.Data Set"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/DataSet/DataSet1Iter.hh"

#include "Ravl/PatternRec/DataSet1.hh"
#include "Ravl/DArray1dIter.hh"

namespace RavlN {
  
  //! userlevel=Normal
  //: Iterate through a data set.
  
  template<class SampleT>
  class DataSet1IterC 
    : public DArray1dIterC<typename SampleT::ElementT>
  {
  public:
#if RAVL_VISUALCPP_TYPENAME_BUG
    typedef SampleT::ElementT Element1T;
#else
    typedef typename SampleT::ElementT Element1T;
#endif
    
    DataSet1IterC()
    {}
    //: Default construtor.
    
    DataSet1IterC(const DataSet1C<SampleT> &nds)
      : DArray1dIterC<Element1T>(nds.Sample1().DArray())
    {}
    //: Construct from a data set.
    
  };
  
}



#endif
