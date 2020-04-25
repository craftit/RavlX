// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DATASET2_HEADER
#define RAVL_DATASET2_HEADER 1
//! rcsid="$Id$"
//! author="Kieron Messer"
//! docentry="Ravl.API.Pattern Recognition.Data Set"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/DataSet/DataSet2.hh"

#include "Ravl/PatternRec/DataSet1.hh"
#include "Ravl/PatternRec/Sample.hh"
#include "Ravl/PatternRec/SampleIter.hh"
#include "Ravl/PatternRec/SampleStream.hh"
#include "Ravl/Vector.hh"
#include "Ravl/DArray1dIter2.hh"

namespace RavlN {
  template<class Sample1T,class Sample2T> class DataSet2C;
  
  //! userlevel=Develop
  //: Data set 
  
  template <class Sample1T,class Sample2T> 
  class DataSet2BodyC 
    : public DataSet1BodyC<Sample1T>
  {
  public:
    typedef typename Sample1T::ElementT Element1T;
    typedef typename Sample2T::ElementT Element2T;
    
    DataSet2BodyC(UIntT sizeEstimate)
      : DataSet1BodyC<Sample1T>(sizeEstimate),
	samp2(sizeEstimate)
    {}
    //: Constructor.
    // Constructs an empty dataset, with enough space to hold 'sizeEstimate' elements without and extra allocations.
    
    DataSet2BodyC(const Sample1T & samp1,
		  const Sample2T & samp2);
    //: Create a dataset from a sample

    IndexC Append(const Element1T &data1,
		 const Element2T &data2);
    //: Append a data entry.
    // returns its index.

    void Append(const DataSet2C<Sample1T,Sample2T> &data);
    //: Append a data set.
    // Note that the entries are not copied

    void Append(const SampleC<Element1T> & sample1, const Element2T & sample2);
    //: Append a sample of inputs and assign the output as the same for all inputs
    
    void Append(SampleStream2C<Element1T, Element2T> & data);
    //: Append a sample stream
    
    void Append(SampleStreamC<Element1T> & sample1, const Element2T & sample2);
    //: Append a sample stream of inputs and assign the output as the same for all inputs
    
    DataSet2C<Sample1T,Sample2T> ExtractSample(RealT proportion);
    //: Extract a sample.
    // The elements are removed from this set. NB. The order
    // of this dataset is NOT preserved.
    
    Sample2T &Sample2()
    { return samp2; }
    //: Access complete sample.
    
    const Sample2T &Sample2() const
    { return samp2; }
    //: Access complete sample.
    
    virtual void Shuffle();
    //: Shuffle the order of the dataset.
    
  protected:
    Sample2T samp2;
    //: the actual data
  };

  //! userlevel=Normal
  //: Data set 
  
  template<class Sample1T,class Sample2T>
  class DataSet2C
    : public DataSet1C<Sample1T>
  {
  public:
    typedef typename Sample1T::ElementT Element1T;
    typedef typename Sample2T::ElementT Element2T;
    
    DataSet2C()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    DataSet2C(UIntT sizeEstimate)
      : DataSet1C<Sample1T>(*new DataSet2BodyC<Sample1T,Sample2T>(sizeEstimate))
    {}
    //: Constructor.
    // Constructs an empty dataset, with enough space to hold 'sizeEstimate' elements without and extra allocations.
    
    DataSet2C(const Sample1T & dat1,const Sample2T & dat2)
      : DataSet1C<Sample1T>(*new DataSet2BodyC<Sample1T,Sample2T>(dat1,dat2))
    {}
    //: Create a dataset from some samples.
    
  protected:
    DataSet2C(DataSet2BodyC<Sample1T,Sample2T> &bod)
      : DataSet1C<Sample1T>(bod)
    {}
    //: Body constructor.
    
    DataSet2BodyC<Sample1T,Sample2T> &Body()
    { return static_cast<DataSet2BodyC<Sample1T,Sample2T> &>(DataSetBaseC::Body()); }
    //: Access body.

    const DataSet2BodyC<Sample1T,Sample2T> &Body() const
    { return static_cast<const DataSet2BodyC<Sample1T,Sample2T> &>(DataSetBaseC::Body()); }
    //: Access body.
    
  public:
    Sample2T &Sample2()
    { return Body().Sample2(); }
    //: Access complete sample.
    
    const Sample2T &Sample2() const
    { return Body().Sample2(); }
    //: Access complete sample.
    
    //DataSet2C<Sample1T,Sample2T> Shuffle() const
    //{ return Body().Shuffle(); }
    //: Shuffle the samples in the dataset
    
    IndexC Append(const Element1T &data1,const Element2T &data2)
    { return Body().Append(data1,data2); }
    //: Append a data entry.
    // returns its index.

    void Append(const DataSet2C<Sample1T,Sample2T> &data)
    { Body().Append(data); }
    //: Append a data set to this one
    // Note that the elements are not copied

    void Append(const SampleC<Element1T> & sample1, const Element2T & element2)
    { Body().Append(sample1, element2); }
    //: Append a sample of inputs and assign the output as the same for all inputs
    
    void Append(SampleStream2C<Element1T, Element2T> & data)
    { Body().Append(data); }
    //: Append a sample stream

    void Append(SampleStreamC<Element1T> & sample1, const Element2T & sample2)
    { Body().Append(sample1, sample2); }
    //: Append a sample stream of inputs and assign the output as the same for all inputs
    
    DataSet2C<Sample1T,Sample2T> ExtractSample(RealT proportion)
    { return Body().ExtractSample(proportion); }
    //: Extract a sample.
    // The elements are removed from this set. NB. The order
    // of this dataset is NOT preserved.
    
    friend class DataSet2BodyC<Sample1T,Sample2T>;
  };

  template<class Sample1T,class Sample2T>
  std::ostream &operator<<(std::ostream &s,const DataSet2C<Sample1T,Sample2T> &dat) {
    s << dat.Sample1() << '\n' << dat.Sample2();
    return s;
  }
  //: Output to stream.
  
  template<class Sample1T,class Sample2T>
  std::istream &operator>>(std::istream &s,DataSet2C<Sample1T,Sample2T> &dat) {
    Sample1T tmp1;
    Sample2T tmp2;
    s >> tmp1 >> tmp2;
    dat = DataSet2C<Sample1T,Sample2T>(tmp1,tmp2);
    return s;
  }
  //: Read from stream.
  
  template<class Sample1T,class Sample2T>
  BinOStreamC &operator<<(BinOStreamC &s,const DataSet2C<Sample1T,Sample2T> &dat ) {
    s << dat.Sample1() << dat.Sample2();
    return s;
  }
  //: Output to a binary stream.
  
  template<class Sample1T,class Sample2T>
  BinIStreamC &operator>>(BinIStreamC &s,DataSet2C<Sample1T,Sample2T> &dat) {
    Sample1T tmp1;
    Sample2T tmp2;
    s >> tmp1 >> tmp2;
    dat = DataSet2C<Sample1T,Sample2T>(tmp1,tmp2);
    return s;
  }
  //: Read from a binary stream.
  
  
  template<class Sample1T,class Sample2T>
  DataSet2BodyC<Sample1T,Sample2T>::DataSet2BodyC(const Sample1T & dat1,const Sample2T &dat2)
    : DataSet1BodyC<Sample1T>(dat1),
      samp2(dat2)
  {}
  
  template<class Sample1T,class Sample2T>
  IndexC DataSet2BodyC<Sample1T,Sample2T>::Append(const Element1T &data1,const Element2T &data2) {
    IndexC no1 = this->samp1.Append(data1);
#if RAVL_CHECK
    // This avoids an unused variable warning where RavlAssert() is not used.
    IndexC no2 =
#endif
      samp2.Append(data2);
    RavlAssert(no1==no2);
    return no1;
  }

  template<class Sample1T,class Sample2T>
  void DataSet2BodyC<Sample1T,Sample2T>::Append(const DataSet2C<Sample1T,Sample2T> &data) {
    this->samp1.Append(data.Sample1());
    samp2.Append(data.Sample2());
  }

  template<class Sample1T,class Sample2T>
  void DataSet2BodyC<Sample1T,Sample2T>::Append(const SampleC<Element1T> & sample, const Element2T & output) {
    for(SampleIterC<Element1T>it(sample);it;it++)
      Append(*it, output);
  }
  
  template<class Sample1T,class Sample2T>
  void DataSet2BodyC<Sample1T,Sample2T>::Append(SampleStream2C<Element1T, Element2T> & strm) {
    Tuple2C<Element1T, Element2T>data;
    while(strm.Get(data))
      this->Append(data.Data1(), data.Data2());
  }
  
  template<class Sample1T,class Sample2T>
  void DataSet2BodyC<Sample1T,Sample2T>::Append(SampleStreamC<Element1T> & strm, const Element2T & output) {
    Element1T data;    
    while(strm.Get(data))
      this->Append(data, output);
  }
  
  template<class Sample1T,class Sample2T>
  DataSet2C<Sample1T,Sample2T> DataSet2BodyC<Sample1T,Sample2T>::ExtractSample(RealT proportion) {
    RavlAssert(proportion >= 0 && proportion <= 1);
    UIntT size = this->Size();
    UIntT entries = (UIntT) (proportion * (RealT) size);
    DataSet2C<Sample1T,Sample2T> ret(size);
    for(;entries > 0;entries--) {
      UIntT entry = RandomInt() % size;
      ret.Append(this->samp1.PickElement(entry),samp2.PickElement(entry));
      size--;
    }
    return ret;
  }
  
  template<class Sample1T,class Sample2T>
  void DataSet2BodyC<Sample1T,Sample2T>::Shuffle() {
    UIntT size = this->Size();
    for(DArray1dIter2C<Element1T,Element2T> it(this->Sample1().DArray(),Sample2().DArray());it;it++) {
      UIntT entry = RandomInt() % size;
      Swap(it.Data1(),this->samp1.Nth(entry));
      Swap(it.Data2(),samp2.Nth(entry));
    }
  }
 
}

#endif
