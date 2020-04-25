// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DATASET4ITER_HEADER
#define RAVL_DATASET4ITER_HEADER 1
/////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Pattern Recognition.Data Set"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/DataSet/DataSet4Iter.hh"

#include "Ravl/PatternRec/DataSet4.hh"
#include "Ravl/DArray1dIter4.hh"

namespace RavlN {
  
  //! userlevel=Normal
  //: Iterate through a data set.
  
  template<class Sample1T,class Sample2T,class Sample3T,class Sample4T>
  class DataSet4IterC 
    : public DArray1dIter4C<typename Sample1T::ElementT,typename Sample2T::ElementT,
			    typename Sample3T::ElementT,typename Sample4T::ElementT>
  {
  public:
#if RAVL_VISUALCPP_TYPENAME_BUG
    typedef Sample1T::ElementT Element1T;
    typedef Sample2T::ElementT Element2T;
    typedef Sample3T::ElementT Element3T;
    typedef Sample4T::ElementT Element4T;
#else
    typedef typename Sample1T::ElementT Element1T;
    typedef typename Sample2T::ElementT Element2T;
    typedef typename Sample3T::ElementT Element3T;
    typedef typename Sample4T::ElementT Element4T;
#endif
    
    DataSet4IterC()
    {}
    //: Default construtor.
    
    DataSet4IterC(const Sample1T &s1,const Sample2T &s2,const Sample3T &s3,const Sample4T &s4)
      : DArray1dIter4C<Element1T,Element2T,Element3T,Element4T>(s1.DArray(),
								s2.DArray(),
								s3.DArray(),
								s4.DArray())
    {}
    //: Construct from individual samples.

    DataSet4IterC(const DataSet4C<Sample1T,Sample2T,Sample3T,Sample4T> &nds)
      : DArray1dIter4C<Element1T,Element2T,Element3T,Element4T>(nds.Sample1().DArray(),
								nds.Sample2().DArray(),
								nds.Sample3().DArray(),
								nds.Sample4().DArray())
    {}
    //: Construct from a data set.
    
  };
  
}



#endif
