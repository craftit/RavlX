// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DPTAP_HEADER
#define RAVL_DPTAP_HEADER 1
///////////////////////////////////////////////////
//! docentry="Ravl.API.Core.Data Processing.Taps"
//! rcsid="$Id$"
//! file="Ravl/Core/IO/Tap.hh"
//! lib=RavlIO
//! author="Charles Galambos"
//! date="15/10/1998"

#include "Ravl/DP/Process.hh"
#include "Ravl/DP/Port.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Tap process body.

  template<class DataT>
  class DPTapBodyC 
    : public DPProcessBodyC<DataT,DataT>
  {
  public:
    DPTapBodyC(const DPOPortC<DataT> &nOut)
      : out(nOut)
    {}
    //: Constructor.
  
    DataT Apply(const DataT &dat) {
      out.Put(dat);
      return dat;
    }
    //: Apply.

    IntT ApplyArray(const SArray1dC<DataT> &in,SArray1dC<DataT> &outarr) {
      IntT ret = out.PutArray(in);
      outarr = in; // this may break something if a copy is expected to the output ?
      return ret;
    }
    //: Apply to an array.
  
  
  protected:
    DPOPortC<DataT> out;
  };

  //! userlevel=Normal
  //: Tap process.
  
  template<class DataT>
  class DPTapC : public DPProcessC<DataT,DataT>
  {
  public:
    DPTapC(const DPOPortC<DataT> &out)
      : DPProcessC<DataT,DataT>(*new DPTapBodyC<DataT>(out))
    {}
    //: Constructor.
  };

  ////////////////////////////////////

  template<class DataT>
  DPProcessC<DataT,DataT> DPTap(const DPOPortC<DataT> &aport)
  { return DPTapC<DataT>(aport); }  
}


#endif
