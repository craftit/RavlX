// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DATASET1_HEADER
#define RAVL_DATASET1_HEADER 1
//! rcsid="$Id$"
//! author="Kieron Messer"
//! docentry="Ravl.API.Pattern Recognition.Data Set"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/DataSet/DataSet1.hh"

#include "Ravl/PatternRec/DataSetBase.hh"
#include "Ravl/Vector.hh"
#include "Ravl/DArray1dIter.hh"

namespace RavlN {
  template<class SampleT> class DataSet1C;
  
  //! userlevel=Develop
  //: Data set 
  
  template <class SampleT> 
  class DataSet1BodyC 
    : public DataSetBaseBodyC
  {
  public:
    typedef typename SampleT::ElementT Element1T;
    
    DataSet1BodyC(UIntT sizeEstimate)
      : DataSetBaseBodyC(sizeEstimate),
	samp1(sizeEstimate)
    {}
    //: Create an empty dataset.
    
    DataSet1BodyC(const SampleT & samp);
    //: Create a dataset from a sample
    
    SampleT &Sample1()
    { return samp1; }
    //: Access sample.

    const SampleT &Sample1() const
    { return samp1; }
    //: Access sample.
    
    IndexC Append(const Element1T &data);
    //: Append a data entry.
    // returns its index.

    void Append(const DataSet1C<SampleT> &data);
    //: Append another data set
    // Note that the elements are not copied.
    
    DataSet1C<SampleT> ExtractSample(RealT proportion);
    //: Extract a sample.
    // The elements are removed from this set. NB. The order
    // of this dataset is NOT preserved.
    
    virtual void Shuffle();
    //: Shuffle the order of the dataset.
    
    SizeT Size() const
    { return samp1.Size(); }
    //: Get the size of the dataset.
    
  protected:
    SampleT samp1;
    //: the actual data
    
  };

  //! userlevel=Normal
  //: Data set 
  
  template<class SampleT>
  class DataSet1C
    : public DataSetBaseC
  {
  public:
    typedef typename SampleT::ElementT Element1T;
    
    DataSet1C()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    DataSet1C(UIntT sizeEstimate)
      : DataSetBaseC(*new DataSet1BodyC<SampleT>(sizeEstimate))
    {}
    //: Constructor.
    // Constructs an empty dataset, with enough space to hold 'sizeEstimate' elements without and extra allocations.
    
    DataSet1C(const SampleT & dat)
      : DataSetBaseC(*new DataSet1BodyC<SampleT>(dat))
    {}
    //: Create a dataset from a sample
    
  protected:    
    DataSet1C(DataSet1BodyC<SampleT> &bod)
      : DataSetBaseC(bod)
      {}
    //: Body constructor.
    
    DataSet1BodyC<SampleT> &Body()
    { return static_cast<DataSet1BodyC<SampleT> &>(DataSetBaseC::Body()); }
    //: Access body.

    const DataSet1BodyC<SampleT> &Body() const
    { return static_cast<const DataSet1BodyC<SampleT> &>(DataSetBaseC::Body()); }
    //: Access body.
    
  public:
    SampleT &Sample1()
    { return Body().Sample1(); }
    //: Access complete sample.
    
    const SampleT &Sample1() const
    { return Body().Sample1(); }
    //: Access complete sample.
    
    IndexC Append(const Element1T &data)
    { return Body().Append(data); }
    //: Append a data entry.
    // returns its index.

    void Append(const DataSet1C<SampleT> &data)
    { Body().Append(data); }
    //: Append a data set to this one.
    // Note that the elements are not copied
    
    DataSet1C<SampleT> ExtractSample(RealT proportion)
    { return Body().ExtractSample(proportion); }
    //: Extract a sample.
    // The elements are removed from this set. NB. The order
    // of this dataset is NOT preserved.

    UIntT Size() const
    { return Body().Size(); }
    //: Get the size of the dataset.
    
    friend class DataSet1BodyC<SampleT>;
  };
  
  template <class SampleT>
  std::ostream &operator<<(std::ostream &s,const DataSet1C<SampleT> &dat) {
    s << dat.Sample1();
    return s;
  }
  //: Output to stream.
  
  template <class SampleT>
  std::istream &operator>>(std::istream &s,DataSet1C<SampleT> &dat) {
    SampleT tmp;
    s >> tmp;
    dat = DataSet1C<SampleT>(tmp);
    return s;
  }
  //: Read from stream.
  
  template <class SampleT>
  BinOStreamC &operator<<(BinOStreamC &s,const DataSet1C<SampleT> &dat ) {
    s << dat.Sample1();
    return s;
  }
  //: Output to a binary stream.
  
  template <class SampleT>
  BinIStreamC &operator>>(BinIStreamC &s,DataSet1C<SampleT> &dat) {
    SampleT tmp;
    s >> tmp;
    dat = DataSet1C<SampleT>(tmp);
    return s;
  }
  //: Read from a binary stream.
  
  template<class SampleT>
  DataSet1BodyC<SampleT>::DataSet1BodyC(const SampleT & sp)
    : DataSetBaseBodyC(sp.Size()),
      samp1(sp)
  {}
  
  template<class SampleT>
  IndexC DataSet1BodyC<SampleT>::Append(const Element1T &data) {
    return samp1.Append(data);
  }

  template<class SampleT>
  void DataSet1BodyC<SampleT>::Append(const DataSet1C<SampleT> &data) {
    samp1.Append(data.Sample1());
  }
  
  template<class SampleT>
  DataSet1C<SampleT> DataSet1BodyC<SampleT>::ExtractSample(RealT proportion) {
    RavlAssert(proportion >= 0 && proportion <= 1);
    UIntT size = Size();
    UIntT entries = (UIntT) (proportion * (RealT) size);
    DataSet1C<SampleT> ret(size);
    for(;entries > 0;entries--) {
      UIntT entry = RandomInt() % size;
      ret.Append(samp1.PickElement(entry));
      size--;
    }
    return ret;
  }
  
  template<class SampleT>
  void DataSet1BodyC<SampleT>::Shuffle() {
    UIntT size = Size();
    for(DArray1dIterC<Element1T> it(samp1.DArray());it;it++)
      Swap(*it,samp1.Nth(RandomInt() % size));
  }
  
}

#endif
