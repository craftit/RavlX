// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_SAMPLESTREAM_HEADER
#define RAVL_SAMPLESTREAM_HEADER 1
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! author="Charles Galambos"
//! docentry="Ravl.API.Pattern Recognition.Data Set"
//! examples=exSampleStream.cc
//! file="Ravl/PatternRec/DataSet/SampleStream.hh"

#include "Ravl/DP/SPort.hh"
#include "Ravl/Tuple2.hh"
#include "Ravl/Tuple3.hh"

namespace RavlN {
  
  //! userlevel=Normal
  //: Stream of samples.
  
  template<typename DataT>
  class SampleStreamC 
    : public DPISPortC<DataT>
  {
  public:
    SampleStreamC()
      : DPEntityC(true)
    {}
    //: Default constructor.

    SampleStreamC(const DPISPortC<DataT> &port)
      : DPEntityC(port),
        DPISPortC<DataT>(port)
    {}
    //: Base constructor.
    
    bool First()
    { return this->Seek(0); }
    //: Goto first element in list.
    
  protected:
    SampleStreamC(DPISPortBodyC<DataT> &bod)
      : DPISPortC<DataT>(bod)
    {}
    //: Body constructor.
    
  };
  
  //! userlevel=Normal
  //: Stream of samples with 2 values.
  
  template<typename Data1T,typename Data2T>
  class SampleStream2C 
    : public SampleStreamC<Tuple2C<Data1T,Data2T> >
  {
  public:
    SampleStream2C()
      : DPEntityC(true)
    {}
    //: Default constructor.
    
    SampleStream2C(const DPISPortC<Tuple2C<Data1T,Data2T> > &port)
      : DPEntityC(port),
        SampleStreamC<Tuple2C<Data1T,Data2T> >(port)
    {}
    //: Base constructor.
  protected:
    SampleStream2C(DPISPortBodyC<Tuple2C<Data1T,Data2T> > &bod)
      : SampleStreamC<Tuple2C<Data1T,Data2T> >(bod)
    {}
    //: Body constructor.

  };
  
  //! userlevel=Normal
  //: Stream of samples with 3 values.
  
  template<typename Data1T,typename Data2T,typename Data3T>
  class SampleStream3C 
    : public SampleStreamC<Tuple3C<Data1T,Data2T,Data3T> >
  {
  public:
    SampleStream3C()
      : DPEntityC(true)
    {}
    //: Default constructor.
    
    SampleStream3C(const DPISPortC<Tuple3C<Data1T,Data2T,Data3T> > &port)
      : DPEntityC(port),
        SampleStreamC<Tuple3C<Data1T,Data2T,Data3T> >(port)
    {}
    //: Base constructor.

  protected:
    SampleStream3C(DPISPortBodyC<Tuple3C<Data1T,Data2T,Data3T> > &bod)
      : SampleStreamC<Tuple3C<Data1T,Data2T,Data3T>  >(bod)
    {}
    //: Body constructor.
    
  };
  
}


#endif
