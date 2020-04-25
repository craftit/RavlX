// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_OFFSETSCALE_HEADER
#define RAVL_OFFSETSCALE_HEADER 1
/////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! example=exDataProc.cc
//! docentry="Ravl.API.Core.Data Processing.Processes"
//! lib=RavlIO
//! author="Charles Galambos"
//! date="04/07/1998"
//! file="Ravl/Core/IO/OffsetScale.hh"

#include "Ravl/DP/Process.hh"
#include "Ravl/Types.hh"

namespace RavlN {

  ////////////////////////////
  //! userlevel=Develop
  //: OffsetScale and offset by value body

  template<class NumTypeT,class ScaleT>
  class DPOffsetScaleBodyC 
    : public DPProcessBodyC<NumTypeT,NumTypeT>
  {
  public:
    DPOffsetScaleBodyC(NumTypeT off,ScaleT fact)
      : factor(fact),
	offset(off)
    {}
    //: Constructor.
  
    DPOffsetScaleBodyC(std::istream &strm)
      : DPProcessBodyC<NumTypeT,NumTypeT>(strm)
    { strm >> factor >> offset; }
    //: Stream constructor.
  
    virtual NumTypeT Apply(const NumTypeT & arg)
    { return (arg + offset) * factor;  }
    //: Apply operation.

    virtual IntT ApplyArray(const SArray1dC<NumTypeT> &in,SArray1dC<NumTypeT>  &out) {
      RavlAssert(in.Size() <= out.Size());
      for(SArray1dIter2C<NumTypeT,NumTypeT> it(in,out);it;it++)
	it.Data2() = (it.Data1() + offset) * factor;
      return in.Size();
    }
    //: Apply operation to an array of elements.
    // returns the number of elements processed.
  
    virtual bool Save(std::ostream &out) const {
      DPProcessBodyC<NumTypeT,NumTypeT>::Save(out);
      out << factor << " " << offset;
      return true;
    }
    //: Save to std::ostream.
  
    virtual RCBodyVC &Copy() const
    { return *new DPOffsetScaleBodyC<NumTypeT,ScaleT>(offset,factor); }
    //: Deep copy of object.
  
    virtual bool IsStateless() const { return true; }
    //: Is operation stateless ?
  
  private:
    ScaleT factor;
    NumTypeT offset;
  };

  //////////////////////////
  //! userlevel=Normal
  //: OffsetScale a stream of data.

  template<class NumTypeT,class ScaleT>
  class DPOffsetScaleC
    : public DPProcessC<NumTypeT,NumTypeT>
  {
  public:
    DPOffsetScaleC(NumTypeT off,ScaleT fact)
      : DPProcessC<NumTypeT,NumTypeT>(*new DPOffsetScaleBodyC<NumTypeT,ScaleT>(off,fact))
    {}
    //: Constructor.  
  };

  /////////////////////////////////////////

  template<class NumTypeT,class ScaleT>
  DPProcessC<NumTypeT,NumTypeT> DPOffsetScale(const NumTypeT &off,const ScaleT &fact)
  { return DPOffsetScaleC<NumTypeT,ScaleT>(off,fact); }


}
#endif
