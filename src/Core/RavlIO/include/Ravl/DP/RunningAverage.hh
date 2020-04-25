// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DPRUNNINGAVE_HEADER
#define RAVL_DPRUNNINGAVE_HEADER 1
/////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! example=exDataProc.cc
//! docentry="Ravl.API.Core.Data Processing.Processes"
//! lib=RavlIO
//! author="Charles Galambos"
//! date="04/07/1998"
//! file="Ravl/Core/IO/RunningAverage.hh"

#include "Ravl/DP/Process.hh"
#include "Ravl/Types.hh"

namespace RavlN {

  ////////////////////////////
  //! userlevel=Develop
  //: Running average body class.

  template<class NumTypeT>
  class DPRunningAverageBodyC 
    : public DPProcessBodyC<NumTypeT,NumTypeT>
  {
  public:
    DPRunningAverageBodyC(NumTypeT init = NumTypeT(),IntT nlen = 10)
      : total(init),
	len(nlen)    
    {}
    //: Constructor.
  
    DPRunningAverageBodyC(std::istream &strm)
      : DPProcessBodyC<NumTypeT,NumTypeT>(strm)
    { strm >> len >> total; }
    //: Stream constructor.
  
    virtual NumTypeT Apply(const NumTypeT & arg) {
      total = (total * (len-1)/len) + arg;
      return total/len;
    }
    //: Apply operation.
  
    virtual IntT ApplyArray(const SArray1dC<NumTypeT> &in,SArray1dC<NumTypeT>  &out) {
      RavlAssert(in.Size() <= out.Size());
      for(SArray1dIter2C<NumTypeT,NumTypeT> it(out,in);it;it++) {
	total = (total * (len-1)/len) + it.Data2();
	it.Data1() = total;
      }
      return in.Size();
    }

    //: Apply operation to an array of elements.
    // returns the number of elements processed.
  
    virtual bool Save(std::ostream &out) const {
      DPProcessBodyC<NumTypeT,NumTypeT>::Save(out);
      out << len << " " << total;
      return true;
    }
    //: Save to std::ostream.
  
    virtual RCBodyVC &Copy() const
    { return *new DPRunningAverageBodyC<NumTypeT>(total,len); }
    //: Deep copy of object.
  
  private:
    NumTypeT total;
    IntT len;
  };

  //////////////////////////
  //! userlevel=Normal
  //: Running average on a stream of data.

  template<class NumTypeT>
  class DPRunningAverageC 
    : public DPProcessC<NumTypeT,NumTypeT>
  {
  public:
    DPRunningAverageC(NumTypeT init = NumTypeT(),IntT len = 10)
      : DPProcessC<NumTypeT,NumTypeT>(*new DPRunningAverageBodyC<NumTypeT>(init,len))
    {}
    //: Constructor.  
  };

  ////////////////////////////////


  template<class NumTypeT>
  DPProcessC<NumTypeT,NumTypeT> DPRunningAverage(const NumTypeT &init,IntT len = 10)
  { return DPRunningAverageC<NumTypeT>(init,len); }
  //: Construct a running average.


}
#endif
