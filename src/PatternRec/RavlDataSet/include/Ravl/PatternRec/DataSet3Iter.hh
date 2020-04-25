// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DATASET3ITER_HEADER
#define RAVL_DATASET3ITER_HEADER 1
/////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Pattern Recognition.Data Set"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/DataSet/DataSet3Iter.hh"

#include "Ravl/PatternRec/DataSet3.hh"
#include "Ravl/DArray1dIter3.hh"

namespace RavlN {
  
  //! userlevel=Normal
  //: Iterate through a data set.
  
  template<class Sample1T,class Sample2T,class Sample3T>
  class DataSet3IterC 
    : public DArray1dIter3C<typename Sample1T::ElementT,typename Sample2T::ElementT,typename Sample3T::ElementT>
  {
  public:
#if RAVL_VISUALCPP_TYPENAME_BUG
    typedef Sample1T::ElementT Element1T;
    typedef Sample2T::ElementT Element2T;
    typedef Sample3T::ElementT Element3T;
#else
    typedef typename Sample1T::ElementT Element1T;
    typedef typename Sample2T::ElementT Element2T;
    typedef typename Sample3T::ElementT Element3T;
#endif
    
    DataSet3IterC()
    {}
    //: Default construtor.
    
    DataSet3IterC(const DataSet3C<Sample1T,Sample2T,Sample3T> &nds)
      : DArray1dIter3C<Element1T,Element2T,Element3T>(nds.Sample1().DArray(),
						      nds.Sample2().DArray(),
						      nds.Sample3().DArray())
    {}
    //: Construct from a data set.
    
    DataSet3IterC(const Sample1T &s1,const Sample2T &s2, const Sample3T &s3)
      : DArray1dIter3C<Element1T,Element2T,Element3T>(s1.DArray(),
						      s2.DArray(),
						      s3.DArray())
    {}
    //: Construct from individual samples.
    
  };
  
}



#endif
