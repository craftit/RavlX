// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DATASET4_HEADER
#define RAVL_DATASET4_HEADER 1
//! rcsid="$Id$"
//! author="Kieron Messer"
//! docentry="Ravl.API.Pattern Recognition.Data Set"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/DataSet/DataSet4.hh"

#include "Ravl/PatternRec/DataSet3.hh"
#include "Ravl/PatternRec/Sample.hh"
#include "Ravl/Vector.hh"
#include "Ravl/DArray1dIter4.hh"

namespace RavlN {
  template<class Sample1T,class Sample2T,class Sample3T,class Sample4T> class DataSet4C;
  
  //! userlevel=Develop
  //: Data set 
  
  template <class Sample1T,class Sample2T,class Sample3T,class Sample4T> 
  class DataSet4BodyC 
    : public DataSet3BodyC<Sample1T,Sample2T,Sample3T>
  {
  public:
    typedef typename Sample1T::ElementT Element1T;
    typedef typename Sample2T::ElementT Element2T;
    typedef typename Sample3T::ElementT Element3T;
    typedef typename Sample4T::ElementT Element4T;
    
    DataSet4BodyC(UIntT sizeEstimate)
      : DataSet3BodyC<Sample1T,Sample2T,Sample3T>(sizeEstimate),
	samp4(sizeEstimate)
    {}
    //: Default constructor.
    
    DataSet4BodyC(const Sample1T & samp1,
		  const Sample2T & samp2,
		  const Sample3T & samp3,
		  const Sample4T & samp4);
    //: Create a dataset from a sample
    
    IndexC Append(const Element1T &data1,
		 const Element2T &data2,
		 const Element3T &data3,
		 const Element4T &data4);
    //: Append a data entry.
    // returns its index.

    void Append(const DataSet4C<Sample1T,Sample2T,Sample3T,Sample4T> &data);
    //: Append a data set.
    //: Note that the entries are not copied
    
    Sample4T &Sample4()
    { return samp4; }
    //: Access complete sample.
    
    const Sample4T &Sample4() const
    { return samp4; }
    //: Access complete sample.

    DataSet4C<Sample1T,Sample2T,Sample3T,Sample4T> ExtractSample(RealT proportion);
    //: Extract a sample.
    // The elements are removed from this set. NB. The order
    // of this dataset is NOT preserved.    
    
    virtual void Shuffle();
    //: Shuffle the order of the dataset.
    
  protected:
    Sample4T samp4;
    //: the actual data
  };

  //! userlevel=Normal
  //: Data set 
  
  template<class Sample1T,class Sample2T,class Sample3T,class Sample4T>
  class DataSet4C
    : public DataSet3C<Sample1T,Sample2T,Sample3T>
  {
  public:
    typedef typename Sample1T::ElementT Element1T;
    typedef typename Sample2T::ElementT Element2T;
    typedef typename Sample3T::ElementT Element3T;
    typedef typename Sample4T::ElementT Element4T;
    
    DataSet4C()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    DataSet4C(UIntT sizeEstimate)
      : DataSet3C<Sample1T,Sample2T,Sample3T>(*new DataSet4BodyC<Sample1T,Sample2T,Sample3T,Sample4T>(sizeEstimate))
    {}
    //: Constructor.
    // Constructs an empty dataset, with enough space to hold 'sizeEstimate' elements without and extra allocations.
    
    DataSet4C(const Sample1T & dat1,const Sample2T & dat2,const Sample3T & dat3,const Sample4T & dat4)
      : DataSet3C<Sample1T,Sample2T,Sample3T>(*new DataSet4BodyC<Sample1T,Sample2T,Sample3T,Sample4T>(dat1,dat2,dat3,dat4))
    {}
    //: Create a dataset from some samples
    
  protected:    
    DataSet4C(DataSet4BodyC<Sample1T,Sample2T,Sample3T,Sample4T> &bod)
      : DataSet3C<Sample1T,Sample2T,Sample3T>(bod)
    {}
    //: Body constructor.
    
    DataSet4BodyC<Sample1T,Sample2T,Sample3T,Sample4T> &Body()
    { return static_cast<DataSet4BodyC<Sample1T,Sample2T,Sample3T,Sample4T> &>(DataSetBaseC::Body()); }
    //: Access body.

    const DataSet4BodyC<Sample1T,Sample2T,Sample3T,Sample4T> &Body() const
    { return static_cast<const DataSet4BodyC<Sample1T,Sample2T,Sample3T,Sample4T> &>(DataSetBaseC::Body()); }
    //: Access body.
    
  public:
    Sample4T &Sample4()
    { return Body().Sample4(); }
    //: Access complete sample.
    
    const Sample4T &Sample4() const
    { return Body().Sample4(); }
    //: Access complete sample.
    
    IndexC Append(const Element1T &data1,const Element2T &data2,const Element3T &data3,const Element4T &data4)
    { return Body().Append(data1,data2,data3,data4); }
    //: Append a data entry.
    // returns its index.
    
    void Append(const DataSet4C<Sample1T,Sample2T,Sample3T,Sample4T> &data)
    { return Body().Append(data); }
    //: Append a data set to this one
    // Note that the elements are not copied
    
    DataSet4C<Sample1T,Sample2T,Sample3T,Sample4T> ExtractSample(RealT proportion)
    { return Body().ExtractSample(proportion); }
    //: Extract a sample.
    // The elements are removed from this set. NB. The order
    // of this dataset is NOT preserved.
    
    friend class DataSet4BodyC<Sample1T,Sample2T,Sample3T,Sample4T>;
  };
  
  template<class Sample1T,class Sample2T,class Sample3T,class Sample4T>
  std::ostream &operator<<(std::ostream &s,const DataSet4C<Sample1T,Sample2T,Sample3T,Sample4T> &dat) {
    s << dat.Sample1() << '\n' << dat.Sample2() << '\n' << dat.Sample3() << '\n' << dat.Sample4();
    return s;
  }
  //: Output to stream.
  
  template<class Sample1T,class Sample2T,class Sample3T,class Sample4T>
  std::istream &operator>>(std::istream &s,DataSet4C<Sample1T,Sample2T,Sample3T,Sample4T> &dat) {
    Sample1T tmp1;
    Sample2T tmp2;
    Sample3T tmp3;
    Sample4T tmp4;
    s >> tmp1 >> tmp2 >> tmp3 >> tmp4;
    dat = DataSet4C<Sample1T,Sample2T,Sample3T,Sample4T>(tmp1,tmp2,tmp3,tmp4);
    return s;
  }
  //: Read from stream.
  
  template<class Sample1T,class Sample2T,class Sample3T,class Sample4T>
  BinOStreamC &operator<<(BinOStreamC &s,const DataSet4C<Sample1T,Sample2T,Sample3T,Sample4T> &dat ) {
    s << dat.Sample1() << dat.Sample2() << dat.Sample3() << dat.Sample4();
    return s;
  }
  //: Output to a binary stream.
  
  template<class Sample1T,class Sample2T,class Sample3T,class Sample4T>
  BinIStreamC &operator>>(BinIStreamC &s,DataSet4C<Sample1T,Sample2T,Sample3T,Sample4T> &dat) {
    Sample1T tmp1;
    Sample2T tmp2;
    Sample3T tmp3;
    Sample4T tmp4;
    s >> tmp1 >> tmp2 >> tmp3 >> tmp4;
    dat = DataSet4C<Sample1T,Sample2T,Sample3T,Sample4T>(tmp1,tmp2,tmp3,tmp4);
    return s;
  }
  //: Read from a binary stream.
  
  template<class Sample1T,class Sample2T,class Sample3T,class Sample4T>
  DataSet4BodyC<Sample1T,Sample2T,Sample3T,Sample4T>::DataSet4BodyC(const Sample1T &dat1,const Sample2T &dat2,
								    const Sample3T &dat3,const Sample4T &dat4)
    : DataSet3BodyC<Sample1T,Sample2T,Sample3T>(dat1,dat2,dat3),
      samp4(dat4)
  {}
  
  template<class Sample1T,class Sample2T,class Sample3T,class Sample4T>
  IndexC DataSet4BodyC<Sample1T,Sample2T,Sample3T,Sample4T>::Append(const Element1T &data1,const Element2T &data2,
								   const Element3T &data3,const Element4T &data4) {
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
      this->samp3.Append(data3);
#if RAVL_CHECK
    // This avoids an unused variable warning where RavlAssert() is not used.
    IndexC no4 =
#endif
      samp4.Append(data4);
    RavlAssert(no1==no2 && no1==no3 && no1==no4);
    return no1;
  }
  
  template<class Sample1T,class Sample2T,class Sample3T,class Sample4T>
  void DataSet4BodyC<Sample1T,Sample2T,Sample3T,Sample4T>::Append(const DataSet4C<Sample1T,Sample2T,Sample3T,Sample4T> &data) {
    this->samp1.Append(data.Sample1());
    this->samp2.Append(data.Sample2());
    this->samp3.Append(data.Sample3());
    samp4.Append(data.Sample4());
  }
  
  template<class Sample1T,class Sample2T,class Sample3T,class Sample4T>
  DataSet4C<Sample1T,Sample2T,Sample3T,Sample4T> DataSet4BodyC<Sample1T,Sample2T,Sample3T,Sample4T>::ExtractSample(RealT proportion) {
    RavlAssert(proportion >= 0 && proportion <= 1);
    SizeT size = this->Size();
    SizeT entries = (UIntT) (proportion * (RealT) size);
    DataSet4C<Sample1T,Sample2T,Sample3T,Sample4T> ret(size);
    for(;entries > 0;entries--) {
      SizeT entry = RandomInt() % size;
      ret.Append(this->samp1.PickElement(entry),
                 this->samp2.PickElement(entry),
                 this->samp3.PickElement(entry),
                 samp4.PickElement(entry));
      size--;
    }
    return ret;
  }
  
  template<class Sample1T,class Sample2T,class Sample3T,class Sample4T>
  void DataSet4BodyC<Sample1T,Sample2T,Sample3T,Sample4T>::Shuffle() {
    SizeT size = this->Size();
    for(DArray1dIter4C<Element1T,Element2T,Element3T,Element4T> it(this->Sample1().DArray(),
                                                                   this->Sample2().DArray(),
                                                                   this->Sample3().DArray(),
                                                                   Sample4().DArray());
	it;it++) {
      UIntT entry = RandomInt() % size;
      Swap(it.Data1(),this->samp1.Nth(entry));
      Swap(it.Data2(),this->samp2.Nth(entry));
      Swap(it.Data3(),this->samp3.Nth(entry));
      Swap(it.Data4(),samp4.Nth(entry));
    }
  }


}
#endif
