// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_SAMPLE_HEADER
#define RAVL_SAMPLE_HEADER 1
//! rcsid="$Id$"
//! author="Kieron Messer"
//! docentry="Ravl.API.Pattern Recognition.Data Set"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/DataSet/Sample.hh"

#include "Ravl/DArray1d.hh"
#include "Ravl/DArray1dIter.hh"
#include "Ravl/Collection.hh"
#include "Ravl/CollectionIter.hh"

namespace RavlN {

  template<class SampleT> class DataSet1IterC;
  template<class Sample1T,class Sample2T> class DataSet2IterC;
  template<class Sample1T,class Sample2T,class Sample3T> class DataSet3IterC;
  template<class SampleT> class SampleIterC;
  template<class SampleT> class SampleC;
  
  
  //! userlevel=Normal
  //: Sample of DataT's
  
  template <class DataT> 
  class SampleC 
    : protected DArray1dC<DataT>
  {
  public:
    typedef DataT ElementT;
    //: Type of data in sample.
    
    SampleC(SizeT maxSize=10)
      : DArray1dC<DataT>(maxSize,true)
    {}
    //: Create a sample of data with a maximum size
    
    SampleC(const SArray1dC<DataT> & dat)
      : DArray1dC<DataT>(dat)
    {}
    //: Create a sample of data from an array
    // Note: The data is NOT copied; any operations done
    // on the sample may effect the contents of the original array.
    
    SampleC(const DArray1dC<DataT> & dat)
     : DArray1dC<DataT> (dat) 
    {}
    //: Create a sample of data from a DArray 
    // Note: The data is NOT copied; any operations done
    // on the sample may effect the contents of the original array.

    inline SampleC<DataT> Copy (void) const
    { return SampleC<DataT> ((static_cast<DArray1dC<DataT> > (*this) ).Copy() ) ;}
    //: Make a copy of this object 

    SampleC<DataT> SubSample(const CollectionC<UIntT> &x);
    //: Take a subsample of the given indexes in x.
    
    SampleC<DataT> CompactFrom(IndexC start,SizeT size)
    { return DArray1dC<DataT>::CompactFrom(start,size); }
    //: Get sub array from this one.
    // The new array will be indexed from zero and continuous
    // This does not copy the elements, it only creates a new access to existing ones.

    const SampleC<DataT> CompactFrom(IndexC start,SizeT size) const
    { return const_cast<SampleC<DataT> *>(this)->DArray1dC<DataT>::CompactFrom(start,size); }
    //: Get sub array from this one.
    // The new array will be indexed from zero and continuous
    // This does not copy the elements, it only creates a new access to existing ones.

    DataT ExtractEntry(int ind);
    //: Extract an entry from sample.

    IndexC Append(const DataT & dat)
    { return DArray1dC<DataT>::Append(dat).V(); }
    //: Insert a single sample into sample
    
    void Fill(const DataT &value)
    { DArray1dC<DataT>::Fill(value); }
    //: Fill sample array with value.
    
    IndexC Append(const SampleC<DataT> &newData)
    { return DArray1dC<DataT>::Append(newData); }
    //: Append data to this array.
    // Note: The data is NOT copied; any operations done
    // on the sample may effect the contents of the original array. <br>
    // The number of items appended is returned.
    
    bool RemoveLast()
    { return DArray1dC<DataT>::RemoveLast(); }
    //: Remove last element from the array.

    IndexC operator+=(const DataT & dat)
    { return Append(dat); }
    //: Indentical to Append().
    
    SizeT Size() const
    { return DArray1dC<DataT>::Size(); }
    //: Return the number of valid samples in the collection
    
    DataT PickElement(UIntT i);
    //: Pick a item i from the collection
    // Note: The order of the collection is NOT preserved.
    // This minimises the disruption to the underlying
    // representation by removing an element from the end
    // of the array and placing it in the hole left by 
    // removing 'i'.

    DataT &Nth(UIntT i)
    { return DArray1dC<DataT>::Nth(i); }
    //: Access nth element in sample.
    
    const DataT &Nth(UIntT i) const
    { return DArray1dC<DataT>::Nth(i); }
    //: Access nth element in sample.
    
    DataT Pick()
    { return PickElement(RandomInt() % Size()); }
    //: Pick a random item from the collection
    // Note: The order of the collection is NOT preserved.
    
    inline const DataT & Sample(void) const 
    { return Nth( RandomInt() % Size() ) ; } 
    //: Access a random sample (const)  
    // The sample is NOT removed 

    inline DataT & Sample (void) 
    { return Nth( RandomInt() % Size() ) ; } 
    //: Access a random sample 
    // The sample is NOT removed 

    DataT &operator[](IndexC ind)
    { return DArray1dC<DataT>::operator[](ind); }
    //: Access a sample.
    
    const DataT &operator[](IndexC ind) const
    { return DArray1dC<DataT>::operator[](ind); }
    //: Access a sample.
    
    friend class DataSet1IterC<DataT>;
    
    DArray1dC<DataT> &DArray()
    { return *this; }
    //: Access DArray.
    // For internal use only.
    
    const DArray1dC<DataT> &DArray() const
    { return *this; }
    //: Access DArray.
    // For internal use only.

    DataT &First()
    { return DArray1dC<DataT>::First(); }
    //: Access first element in the array.
    
    const DataT &First() const
    { return DArray1dC<DataT>::First(); }
    //: Access first element in the array.
    
    DataT &Last()
    { return DArray1dC<DataT>::Last(); }
    //: Access last element in the array.
    
    const DataT &Last() const
    { return DArray1dC<DataT>::Last(); }
    //: Access last element in the array.
    
    bool IsEmpty() const
    { return DArray1dC<DataT>::IsEmpty(); }
    //: Is this empty ?
    
    bool IsValid() const
    { return DArray1dC<DataT>::IsValid(); }
    //: Is this a valid handle ?
  
  }; // end of class SampleC 
  
  template <class DataT>
  std::ostream &operator<<(std::ostream &s,const SampleC<DataT> &dat ) {
    if(dat.IsValid()) {
      s  << dat.Size() << "\n";
      for(DArray1dIterC<DataT> it(dat.DArray());it;it++)
	s << *it << "\n";
    } else {
      s << "0\n";
    }
    return s;
  }
  //: Output to stream.
  
  template <class DataT>
  std::istream &operator>>(std::istream &s,SampleC<DataT> &dat ) {
    SizeT size;
    s >> size;
    dat = SampleC<DataT>(size);
    for(SizeT i = 0;i < size;i++) {
      DataT v;
      s >> v;
      dat += v;
    }
    return s;
  }
  //: Read from stream.

  template <class DataT>
  BinOStreamC &operator<<(BinOStreamC &s,const SampleC<DataT> &dat ) {
    if(dat.IsValid()) {
      s  << dat.Size();
      for(DArray1dIterC<DataT> it(dat.DArray());it;it++)
	s << *it;
    } else {
      s << (UIntT) 0;
    }
    return s;
  }
  //: Output to a binary stream.
  
  template <class DataT>
  BinIStreamC &operator>>(BinIStreamC &s,SampleC<DataT> &dat ) {
    SizeT size;
    s >> size;
    dat = SampleC<DataT>(size);
    for(SizeT i = 0;i < size;i++) {
      DataT v;
      s >> v;
      dat += v;
    }
    return s;
  }
  //: Read from a binary stream.
  
  template <class DataT>
  SampleC<DataT> SampleC<DataT>::SubSample(const CollectionC<UIntT> &x) {
    SampleC<DataT> ret(x.Size());
    for(CollectionIterC<UIntT> it(const_cast<CollectionC<UIntT> &>(x));it;it++)
      ret.Append((*this)[*it]);
    return ret;
  }
  
  template <class DataT>
  DataT SampleC<DataT>::ExtractEntry(int ind) {
    RavlAssert(this->Contains(ind));
    DataT ret = (*this)[ind];
    this->Remove(ind);
    return ret;
  }
  
  template <class DataT>
  DataT SampleC<DataT>::PickElement(UIntT i) {
    DataT &val = this->Nth(i);
    DataT ret = val;
    val = Last();
    this->RemoveLast();
    return ret;
  }

  
  
}

#endif
