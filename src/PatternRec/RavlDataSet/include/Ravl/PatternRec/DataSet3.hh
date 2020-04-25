// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DATASET3_HEADER
#define RAVL_DATASET3_HEADER 1
//! rcsid="$Id$"
//! author="Kieron Messer"
//! docentry="Ravl.API.Pattern Recognition.Data Set"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/DataSet/DataSet3.hh"

#include "Ravl/PatternRec/DataSet2.hh"
#include "Ravl/PatternRec/Sample.hh"
#include "Ravl/Vector.hh"
#include "Ravl/DArray1dIter3.hh"

namespace RavlN {
  template<class Sample1T,class Sample2T,class Sample3T> class DataSet3C;
  
  //! userlevel=Develop
  //: Data set 
  
  template <class Sample1T,class Sample2T,class Sample3T> 
  class DataSet3BodyC 
    : public DataSet2BodyC<Sample1T,Sample2T>
  {
  public:
    typedef typename Sample1T::ElementT Element1T;
    typedef typename Sample2T::ElementT Element2T;
    typedef typename Sample3T::ElementT Element3T;
    
    DataSet3BodyC(UIntT sizeEstimate)
      : DataSet2BodyC<Sample1T,Sample2T>(sizeEstimate),
	samp3(sizeEstimate)
    {}
    //: Default constructor.
    
    DataSet3BodyC(const Sample1T & samp1,
		  const Sample2T & samp2,
		  const Sample3T & samp3);
    //: Create a dataset from a sample
    
    IndexC Append(const Element1T &data1,
		 const Element2T &data2,
		 const Element3T &data3);
    //: Append a data entry.
    // returns its index.

    void Append(const DataSet3C<Sample1T,Sample2T,Sample3T> &data);
    //: Append a data set.
    //: Note that the entries are not copied
    
    Sample3T &Sample3()
    { return samp3; }
    //: Access complete sample.
    
    const Sample3T &Sample3() const
    { return samp3; }
    //: Access complete sample.

    DataSet3C<Sample1T,Sample2T,Sample3T> ExtractSample(RealT proportion);
    //: Extract a sample.
    // The elements are removed from this set. NB. The order
    // of this dataset is NOT preserved.    
    
    virtual void Shuffle();
    //: Shuffle the order of the dataset.
    
  protected:
    Sample3T samp3;
    //: the actual data
  };

  //! userlevel=Normal
  //: Data set 
  
  template<class Sample1T,class Sample2T,class Sample3T>
  class DataSet3C
    : public DataSet2C<Sample1T,Sample2T>
  {
  public:
    typedef typename Sample1T::ElementT Element1T;
    typedef typename Sample2T::ElementT Element2T;
    typedef typename Sample3T::ElementT Element3T;
    
    DataSet3C()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    DataSet3C(UIntT sizeEstimate)
      : DataSet2C<Sample1T,Sample2T>(*new DataSet3BodyC<Sample1T,Sample2T,Sample3T>(sizeEstimate))
    {}
    //: Constructor.
    // Constructs an empty dataset, with enough space to hold 'sizeEstimate' elements without and extra allocations.
    
    DataSet3C(const Sample1T & dat1,const Sample2T & dat2,const Sample3T & dat3)
      : DataSet2C<Sample1T,Sample2T>(*new DataSet3BodyC<Sample1T,Sample2T,Sample3T>(dat1,dat2,dat3))
    {}
    //: Create a dataset from some samples
    
  protected:    
    DataSet3C(DataSet3BodyC<Sample1T,Sample2T,Sample3T> &bod)
      : DataSet2C<Sample1T,Sample2T>(bod)
    {}
    //: Body constructor.
    
    DataSet3BodyC<Sample1T,Sample2T,Sample3T> &Body()
    { return static_cast<DataSet3BodyC<Sample1T,Sample2T,Sample3T> &>(DataSetBaseC::Body()); }
    //: Access body.

    const DataSet3BodyC<Sample1T,Sample2T,Sample3T> &Body() const
    { return static_cast<const DataSet3BodyC<Sample1T,Sample2T,Sample3T> &>(DataSetBaseC::Body()); }
    //: Access body.
    
  public:
    Sample3T &Sample3()
    { return Body().Sample3(); }
    //: Access complete sample.
    
    const Sample3T &Sample3() const
    { return Body().Sample3(); }
    //: Access complete sample.
    
    IndexC Append(const Element1T &data1,const Element2T &data2,const Element3T &data3)
    { return Body().Append(data1,data2,data3); }
    //: Append a data entry.
    // returns its index.
    
    void Append(const DataSet3C<Sample1T,Sample2T,Sample3T> &data)
    { Body().Append(data); }
    //: Append a data set to this one
    // Note that the elements are not copied
    
    DataSet3C<Sample1T,Sample2T,Sample3T> ExtractSample(RealT proportion)
    { return Body().ExtractSample(proportion); }
    //: Extract a sample.
    // The elements are removed from this set. NB. The order
    // of this dataset is NOT preserved.
    
    friend class DataSet3BodyC<Sample1T,Sample2T,Sample3T>;
  };
  
  template<class Sample1T,class Sample2T,class Sample3T>
  std::ostream &operator<<(std::ostream &s,const DataSet3C<Sample1T,Sample2T,Sample3T> &dat) {
    s << dat.Sample1() << '\n' << dat.Sample2() << '\n' << dat.Sample3();
    return s;
  }
  //: Output to stream.
  
  template<class Sample1T,class Sample2T,class Sample3T>
  std::istream &operator>>(std::istream &s,DataSet3C<Sample1T,Sample2T,Sample3T> &dat) {
    Sample1T tmp1;
    Sample2T tmp2;
    Sample3T tmp3;
    s >> tmp1 >> tmp2 >> tmp3;
    dat = DataSet3C<Sample1T,Sample2T,Sample3T>(tmp1,tmp2,tmp3);
    return s;
  }
  //: Read from stream.
  
  template<class Sample1T,class Sample2T,class Sample3T>
  BinOStreamC &operator<<(BinOStreamC &s,const DataSet3C<Sample1T,Sample2T,Sample3T> &dat ) {
    s << dat.Sample1() << dat.Sample2() << dat.Sample3();
    return s;
  }
  //: Output to a binary stream.
  
  template<class Sample1T,class Sample2T,class Sample3T>
  BinIStreamC &operator>>(BinIStreamC &s,DataSet3C<Sample1T,Sample2T,Sample3T> &dat) {
    Sample1T tmp1;
    Sample2T tmp2;
    Sample3T tmp3;
    s >> tmp1 >> tmp2 >> tmp3;
    dat = DataSet3C<Sample1T,Sample2T,Sample3T>(tmp1,tmp2,tmp3);
    return s;
  }
  //: Read from a binary stream.

  
  template<class Sample1T,class Sample2T,class Sample3T>
  DataSet3BodyC<Sample1T,Sample2T,Sample3T>::DataSet3BodyC(const Sample1T & dat1,const Sample2T &dat2,const Sample3T &dat3)
    : DataSet2BodyC<Sample1T,Sample2T>(dat1,dat2),
      samp3(dat3)
  {}
  
  template<class Sample1T,class Sample2T,class Sample3T>
  IndexC DataSet3BodyC<Sample1T,Sample2T,Sample3T>::Append(const Element1T &data1,const Element2T &data2,const Element3T &data3) {
    IndexC no1 = this->samp1.Append(data1);
#if RAVL_CHECK
    // This avoids an unused variable warning where RavlAssert() is not used.
    IndexC no2 =
#endif
      this->samp2.Append(data2);
#if RAVL_CHECK
    // This avoids an unused variable warning where RavlAssert() is not used.
    IndexC no3 =
#endif
      samp3.Append(data3);
    RavlAssert(no1==no2 && no1==no3);
    return no1;
  }
  
  template<class Sample1T,class Sample2T,class Sample3T>
  void DataSet3BodyC<Sample1T,Sample2T,Sample3T>::Append(const DataSet3C<Sample1T,Sample2T,Sample3T> &data) {
    this->samp1.Append(data.Sample1());
    this->samp2.Append(data.Sample2());
    samp3.Append(data.Sample3());
  }

  template<class Sample1T,class Sample2T,class Sample3T>
  DataSet3C<Sample1T,Sample2T,Sample3T> DataSet3BodyC<Sample1T,Sample2T,Sample3T>::ExtractSample(RealT proportion) {
    RavlAssert(proportion >= 0 && proportion <= 1);
    SizeT size = this->Size();
    SizeT entries = (UIntT) (proportion * (RealT) size);
    DataSet3C<Sample1T,Sample2T,Sample3T> ret(size);
    for(;entries > 0;entries--) {
      UIntT entry = RandomInt() % size;
      ret.Append(this->samp1.PickElement(entry),this->samp2.PickElement(entry),samp3.PickElement(entry));
      size--;
    }
    return ret;
  }
  
  template<class Sample1T,class Sample2T,class Sample3T>
  void DataSet3BodyC<Sample1T,Sample2T,Sample3T>::Shuffle() {
    SizeT size = this->Size();
    for(DArray1dIter3C<Element1T,Element2T,Element3T> it(this->Sample1().DArray(),this->Sample2().DArray(),Sample3().DArray());
	it;it++) {
      UIntT entry = RandomInt() % size;
      Swap(it.Data1(),this->samp1.Nth(entry));
      Swap(it.Data2(),this->samp2.Nth(entry));
      Swap(it.Data3(),samp3.Nth(entry));
    }
  }


}

#endif
