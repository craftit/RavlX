// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_SARRAY1D_HEADER
#define RAVL_SARRAY1D_HEADER
////////////////////////////////////////////////////////////////////////////
//! file="Ravl/Core/Container/SArray/SArray1d.hh"
//! lib=RavlCore
//! author="Radek Marik"
//! docentry="Ravl.API.Core.Arrays.1D"
//! date="21/2/1996"

#include "Ravl/Buffer.hh"
#include "Ravl/SBfAcc.hh"
#include "Ravl/Pair.hh"
#include "Ravl/BfAccIter.hh"
#include "Ravl/BfAccIter2.hh"
#include "Ravl/BfAccIter3.hh"
#include "Ravl/StdHash.hh"
#include "Ravl/Stream.hh"
#include "Ravl/SingleBuffer.hh"
#include "Ravl/DeepCopy.hh"
#include "Ravl/QuickSort.hh"

namespace RavlN {

  template<class DataT> class SArray1dIterC;
  template<class DataT> class Array1dC;
  template<class DataT> class Slice1dC;
  template<class DataT> class Slice1dIterC;
  class BinIStreamC;
  class BinOStreamC;

  //! userlevel=Advanced
  //: A simple 1 dimensional array
  // The class SArray1dC is a simple reference counted
  // container with a fixed number of items
  // which can be accessed randomly using indexing.
  // The lower index of the array is 0.
  // The item is expected
  // to have a default constructor, a copy constructor, and an assigment.
  // The class SArray1dC is a big object, eg. it has a reference counter.

  template <class DataT>
  class SArray1dC
    : public SizeBufferAccessC<DataT>
  {
  public:
    typedef DataT ElementT;
    //: Allow function templates to find type of array.

    typedef IndexC KeyT;
    //: Allow function templates to find type of index.

    typedef SArray1dIterC<DataT> IteratorT;
    //: Type of iterator.

    //:---------------------------------------
    // Constructors, assignment, and destructor

    SArray1dC()
      : SizeBufferAccessC<DataT>(),
        buff()
    {}
    //: Create an array of zero length

    explicit inline SArray1dC(const SizeT dim)
      : SizeBufferAccessC<DataT>(),
        buff(SingleBufferC<DataT>(dim))
    { this->Attach(buff,dim); }
    //: Creates an uninitialised array with the range <code>{0 ... dim-1}</code>.

    static SArray1dC<DataT> ConstructAligned(const SizeT dim,UIntT align)
    { return SArray1dC<DataT>(SingleBufferC<DataT>(dim,align),dim); }
    //: Creates an uninitialised array with the range <code>{0 ... dim-1}</code> and byte alignment of the first element 'align'
    // align must be a power of 2.

    SArray1dC(const Slice1dC<DataT> &slice,bool alwaysCopy = true);
    //: Make an array from a slice.
    // This will create an array with the values from the slice.
    // if the stride of the slice is not 1, and alwaysCopy is true the
    // a copy is done.

    inline SArray1dC(const PairC<DataT> & pr);
    //: Creates an array with two elements from a PairC object.

    inline SArray1dC(const SArray1dC<DataT> & vv)
      : SizeBufferAccessC<DataT>(vv),
        buff(vv.buff)
    {}
    //: Copy constructor.
    // Another access to the array 'vv'.

    inline SArray1dC(const SArray1dC<DataT> & vv,SizeT dim,SizeT offsetInBuff = 0);
    //: The subarray of the 'vv' with size 'dim'.

    inline SArray1dC(DataT *data,const SizeT  dim,bool removable);
    //: The array is created from the memory location <code>data</code> with the range
    //: <code>{0 ... dim-1}</code>.
    // <font color="red">Warning:</font>  the <code>data</code> argument is a pointer, with all the attendant problems.
    // The data is <i>not</i> copied.
    //!param: data  - address of the data to be used as the array contents
    //!param: removable - if true, <code>data</code> is  de-allocated from the heap during destruction of the array.<br>If <code>data</code> is not allocated on the heap, this arg <b>MUST</b> set false.<br>N.B>: this arg used to have default value = true

    // <p> It can be used to create an <code>SArray1dC</code> initialised from some constant array, like this:<pre>
    //   static RealT values[9] = {
    //        -0.02311234,   0.00958230,   0.10377361,
    //         0.22375219,   0.27955917,   0.22375219,
    //         0.10377361,   0.00958230,  -0.02311234
    //    };
    //    SArray1dC&lt;RealT&gt; coeffs(values, 9, false);
    //</pre>
    // Here, <code>removable</code> is set <b>false</b> as the data was not allocated on the heap in the first place.<br>
    // Note: it is the programmer's responsibility to make the <code>range</code> argument match the data size.</p>

    inline SArray1dC(const BufferC<DataT> & bf,SizeT dim,SizeT offsetInBuff = 0)
      : SizeBufferAccessC<DataT>(const_cast<BufferC<DataT> &>(bf).BufferAccess() + offsetInBuff, dim),
        buff(bf)
    {}
    //: Creates the array of size 'dim' using the buffer 'bf'.
    // Use buffer 'bf',  make access of 'dim' elements.
    // the start of the buffer should be 'offsetInBuff' elements into
    // the buffer.

    inline SArray1dC(const BufferC<DataT> & bf, const SizeBufferAccessC<DataT> &sbf)
      : SizeBufferAccessC<DataT>(sbf),
        buff(bf)
    {}
    //: Construct from a buffer an size buffer access.

    inline SArray1dC(const BufferC<DataT> & bf,DataT *start, const SizeT dim)
      : SizeBufferAccessC<DataT>(start, dim),
        buff(bf)
    {}
    //: Creates the array of size 'dim' using the buffer 'bf', with
    //: 0'th element at start.
    // Used for building SArray's from Arrays.

    SArray1dC<DataT> Align(UIntT alignment) const {
      if((((size_t) SizeBufferAccessC<DataT>::ReferenceElm()) & (alignment-1)) == 0)
        return *this; // Data is already aligned
      SArray1dC<DataT> ret = ConstructAligned(Size(),alignment);
      for(BufferAccessIter2C<DataT,DataT> it(*this,ret);it;it++)
        it.Data2() = it.Data1();
      return ret;
    }
    //: Create a version of the array with first element on the given byte boundary.
    // If the alignment is not a correct a copy of the array with the correct alignment is returned.
    // Note: Alignment must be a power of two.

    SArray1dC<DataT> Copy() const;
    //: Creates a copy of the whole array.

    SArray1dC<DataT> Copy(SizeT extend) const;
    //: Creates a new physical copy of the array.
    // 'extend' extra elements initialised by the default constructor
    // are appended to the end of the array.

    SArray1dC<DataT> DeepCopy(UIntT levels = ((UIntT) -1)) const;
    //: Creates a new physical copy of the array.

    inline const SArray1dC<DataT> & operator=(const SArray1dC<DataT> & vv);
    //: Assign this handle to the data in VV.

    SArray1dC<DataT> operator+(const SArray1dC<DataT> & arr) const;
    //: Sums 2 numerical arrays.
    // The operator returns the result as a new array.

    SArray1dC<DataT> operator-(const SArray1dC<DataT> & arr) const;
    //: Subtract 2 numerical arrays.
    // The operator returns the result as a new array.

    SArray1dC<DataT> operator*(const SArray1dC<DataT> & arr) const;
    //: Multiplies 2 numerical arrays.
    // The operator returns the result as a new array.

    SArray1dC<DataT> operator/(const SArray1dC<DataT> & arr) const;
    //: Divides 2 numerical arrays.
    // The operator returns the result as a new array.

    SArray1dC<DataT> operator*(const DataT &number) const;
    //: Multiplies the array by the 'number'.
    // The operator returns the result as a new array.

    SArray1dC<DataT> operator/(const DataT &number) const;
    //: Divides all array items by the 'number'.
    // The operator returns the result as a new array.

    SArray1dC<DataT> operator+(const DataT &number) const;
    //: Adds 'number' to the array.
    // The operator returns the result as a new array.

    SArray1dC<DataT> operator-(const DataT &number) const;
    //: Subtracts 'number' from all array items.
    // The operator  returns the result as a new array.

    const SArray1dC<DataT> & operator+=(const SArray1dC<DataT> & arr);
    //: Adds the 2nd array to this array.

    const SArray1dC<DataT> & operator-=(const SArray1dC<DataT> & arr);
    //: Subtracts the 2nd array from this array.

    const SArray1dC<DataT> & operator*=(const SArray1dC<DataT> & arr);
    //: Multiplies the 2nd array to this array.

    const SArray1dC<DataT> & operator/=(const SArray1dC<DataT> & arr);
    //: Divides the 2nd array from this array.

    const SArray1dC<DataT> & operator+=(const DataT &number);
    //: Adds 'number' to all array items.

    const SArray1dC<DataT> & operator-=(const DataT &number);
    //: Subtracts 'number' from all array items.

    const SArray1dC<DataT> & operator*=(const DataT &number);
    //: Multiplies the array by the 'number'.

    const SArray1dC<DataT> & operator/=(const DataT &number);
    //: Divides the array elements by the 'number'.

    DataT Sum() const;
    //: Calculate the sum of all elements in the array

    DataT SumOfSqr() const;
    //: Calculate the sum of the squares of all elements in the array

    IndexC IndexOfMax() const;
    //: Find the index of the maximum element in the array

    IndexC IndexOfMin() const;
    //: Find the index of the minimum element in the array

    //:-----------------------------------
    // Access to the object and its parts.

    inline const SArray1dC<DataT> & SArray1d() const
    { return *this; }
    //: Access to the whole constant array.

    inline SArray1dC<DataT> & SArray1d()
    { return *this; }
    //: Access to the whole array.

    SizeT Size() const
    { return SizeBufferAccessC<DataT>::Size(); }
    //: Access size of array

    IndexRangeC Range() const
    { return SizeBufferAccessC<DataT>::Range(); }
    //: Returns the usable range of indeces expressed by this object.

    Slice1dC<DataT> Slice1d()
    { return Slice1dC<DataT>(buff,DataStart(),Size(),1); }
    //: Access array as a slice.

    //:-----------------------------------
    // Modifications of the representation

    SArray1dC<DataT> Join(const SArray1dC<DataT> &Oth) const;
    // Join this Array and another into a new Array which
    // is returned. This does not change either of its arguments.
    // This is placed in the array first, followed by 'Oth'.

    SArray1dC<DataT> Join(const DataT &Oth) const;
    // Join this Array and an element into a new Array which
    // is returned. This does not change either of its arguments.
    // This is placed in the array first, followed by 'Oth'.

    inline SArray1dC<DataT> & Append(const SArray1dC<DataT> & a)
    { (*this) = Join(a); return *this;  }
    // This array is extended by the length of the array 'a' and the contents
    // of both arrays are copied to it. Empty arrays are handled correctly.

    inline SArray1dC<DataT> & Append(const DataT & a)
    { (*this) = Join(a); return *this;  }
    // This array is extended by 1 and the contents of this array are
    // copied to this array followed by the new element.
    // Empty arrays are handled correctly.

    SArray1dC<DataT> From(UIntT offset) {
      if(offset >= Size())
	return SArray1dC<DataT>(); // Empty array.
      SizeT nsize = Size() - offset;
      return SArray1dC<DataT>(*this,nsize,offset);
    }
    //: Return array from offset to the end of the array.
    // If offset is larger than the array an empty array
    // is returned,

    SArray1dC<DataT> From(UIntT offset,UIntT size) {
      if((offset + size) > Size())
	return SArray1dC<DataT>(); // Empty array.
      return SArray1dC<DataT>(*this,size,offset);
    }
    //: Return array from offset to the end of the array.
    // If offset is larger than the array an empty array
    // is returned,

    SArray1dC<DataT> After(UIntT offset)
    { return From(offset+1); }
    //: Return array after offset to the end of the array.
    // If offset is larger than the array an empty array
    // is returned,

    SArray1dC<DataT> Before(UIntT offset)
    { return From(0,offset); }
    //: Return array from the start to the element before offset
    // If offset is larger then the whole array will be returned.

    //:------------------
    // Special operations

    BufferC<DataT> &Buffer()
    { return buff; }
    //: Access base data buffer.
    // Experts only.

    const BufferC<DataT> &Buffer() const
    { return buff; }
    //: Constant access base data buffer.
    // Experts only.

    typedef bool (*QuickSortCmpT)(const DataT &l1,const DataT &l2);
    //: Comparison function for quick sort.

    void QuickSort(typename SArray1dC<DataT>::QuickSortCmpT cmp = &SArray1dC<DataT>::DefaultComparisonOp) {
      if (this->Size() > 1)
        RavlN::QuickSort(*this, (SizeT)0, (SizeT)(this->Size()-1), cmp);
    }
    //: Sort the array with comparison function 'cmp'.
    // The default is to use the "<" operator; this creates an array sorted in
    // <i>ascending</i> order.<br>
    // Where a comparison operator for DataT does not exist, you must provide
    // your own in place of the default argument.
    // See <a href="../../Examples/exDList.cc.html">example</a> for how to write your own.

    void Sort();
    //: Sort the array in place.
    // This does a simple bubble sort.
    //!deprecated: use QuickSort instead (note reversed sorting order)

    void Sort(bool(*leq)(DataT &o1,DataT &o2));
    //: Sort the array into ascending order in place
    // Uses the provided comparison function which should be equivalent to
    // less than or equal to.
    //!deprecated: use QuickSort instead (note reversed sorting order)

    bool operator==(const SArray1dC<DataT> & vv) const;
    //: Comparison operator
    // Returns true if the two arrays are the same length and
    // their contents are identical.

    bool operator!=(const SArray1dC<DataT> & vv) const
    { return !operator==(vv); }
    //: Comparison operator
    // Returns true if the two arrays have different lengths or
    // contents..

    SizeT Hash() const;
    //: Compute a hash value for the array.

    inline DataT * DataStart() const
    { return SizeBufferAccessC<DataT>::ReferenceElm(); }
    //: Returns the address of element 0.
    // If the array has zero length a null pointer may
    // be returned.
  private:

    BufferC<DataT> buff;  // The reference counted storage.

    friend class SArray1dIterC<DataT>;
    friend class Array1dC<DataT>;

    static bool DefaultComparisonOp(const DataT &l1,const DataT &l2)
    { return l1 < l2; }
    // Default comparison method.

  };

  template <class DataT>
  BinOStreamC & operator<<(BinOStreamC & s, const SArray1dC<DataT> & arr);
  // Prints array into the stream 's'.

  template <class DataT>
  BinIStreamC & operator>>(BinIStreamC& s, SArray1dC<DataT> & arr);
  // Assigns the values into the array 'arr'.

  template <class DataT>
  SArray1dC<DataT> & BubbleSort(SArray1dC<DataT> & arr);
  // Sorts arr using operator <.

  template <class DataT>
  std::istream & operator>>(std::istream & s, SArray1dC<DataT> & arr);
  // Assigns the values into the array 'arr'.

  template <class DataT>
  SArray1dC<DataT> & BubbleSort(SArray1dC<DataT> & arr);
  // Sorts arr using operator <.

  /////////////////////////////////////////////////////////////////////////////

  template <class DataT>
  inline
  const SArray1dC<DataT> &
  SArray1dC<DataT>::operator=(const SArray1dC<DataT> & vv) {
    if (this == &vv) return *this;
    SizeBufferAccessC<DataT>::operator=(vv);
    buff=vv.buff;
    return *this;
  }

  template <class DataT>
  SArray1dC<DataT>::SArray1dC(const PairC<DataT> & pr)
    : SizeBufferAccessC<DataT>(),
      buff(2)
  {
    this->Attach(buff,2);
    (*this)[0] = pr[0];
    (*this)[1] = pr[1];
  }

  template <class DataT>
  SArray1dC<DataT>::SArray1dC(const Slice1dC<DataT> &slice,bool alwaysCopy) {
    if(!alwaysCopy && slice.Stride() == 1) {
      buff = slice.Buffer();
      SizeBufferAccessC<DataT>::operator=(SizeBufferAccessC<DataT>(const_cast<DataT *>(&(slice[0])),
								   slice.Size()));
      return ;
    }
    buff = SingleBufferC<DataT>(slice.Size());
    this->Attach(buff,slice.Size());
    // Copy data.
    DataT *at = buff.ReferenceElm();
    for(Slice1dIterC<DataT> it(slice);it;it++,at++)
      *at = *it;
  }

  template <class DataT>
  SArray1dC<DataT>::SArray1dC(DataT *data,
			      SizeT dim,
			      bool removable)
    : SizeBufferAccessC<DataT>(data, dim),
      buff(dim,data,false, removable)
  {}

  template <class DataT>
  SArray1dC<DataT>::SArray1dC(const SArray1dC<DataT> & vv,
			      const SizeT  dim,
			      SizeT offsetInBuff)
    : SizeBufferAccessC<DataT>(vv.SAccess() + offsetInBuff, dim),
      buff(vv.buff)
  {
    RavlAssert(vv.Size() >= (offsetInBuff + dim)); // Make sure its big enought.
  }

  template <class DataT>
  SArray1dC<DataT> SArray1dC<DataT>::Copy() const {
    return SArray1dC<DataT>(SizeBufferAccessC<DataT>::Copy().ReferenceElm(),
			    Size(),true);
  }

  template <class DataT>
  SArray1dC<DataT> SArray1dC<DataT>::Copy(SizeT extend) const {
    SArray1dC<DataT> ret(Size() + extend);
    for(BufferAccessIter2C<DataT,DataT> it(*this,ret);it;it++)
      it.Data2() = it.Data1();
    return ret;
  }

  //: Creates a new physical copy of the array.

  template <class DataT>
  SArray1dC<DataT> SArray1dC<DataT>::DeepCopy(UIntT levels) const {
    if(levels <= 1)
      return Copy();
    SArray1dC<DataT> ret(Size());
    for(BufferAccessIter2C<DataT,DataT> it(*this,ret);it;it++)
      it.Data2() = StdDeepCopy(it.Data1(),levels-1);
    return ret;
  }

  template <class DataT>
  SArray1dC<DataT>
  SArray1dC<DataT>::Join(const SArray1dC<DataT> &Oth) const  {
    const SizeT len1 = Size();
    const SizeT len2 = Oth.Size();
    SArray1dC<DataT> newarr(len1 + len2);
    for(BufferAccessIter2C<DataT,DataT> it(*this,newarr);it;it++)
      it.Data2() = it.Data1();
    for(BufferAccessIter2C<DataT,DataT> it2(Oth,newarr,0,len1);it2;it2++)
      it2.Data2() = it2.Data1();
    return newarr;
  }

  template <class DataT>
  SArray1dC<DataT>
  SArray1dC<DataT>::Join(const DataT &a) const  {
    const SizeT len1 = Size();
    SArray1dC<DataT> newarr(len1 + 1);
    for(BufferAccessIter2C<DataT,DataT> it(*this,newarr);it;it++)
      it.Data2() = it.Data1();
    newarr[len1] = a;
    return newarr;
  }

  template <class DataT>
  std::ostream &
  operator<<(std::ostream & s, const SArray1dC<DataT> & arr) {
    s << arr.Size() << '\n';
    for(BufferAccessIterC<DataT> it(arr);it;it++)
      s << ((const DataT &) *it) << '\n';
    return s;
  }

  template <class DataT>
  std::istream &
  operator>>(std::istream & s, SArray1dC<DataT> & arr) {
    SizeT n = 0;
    s >> n;
    s.get(); // Get '\n' after size to avoid trouble with reading StringC's.
    if (n != arr.Size()) {
      SArray1dC<DataT> brr(n);
      arr = brr;
    }
    for(BufferAccessIterC<DataT> it(arr);it;it++)
      s >> *it;
    return s;
  }

  template <class DataT>
  SArray1dC<DataT> &
  BubbleSort(SArray1dC<DataT> & arr) {
    IndexC    low = 0;
    IndexC    upp = arr.Size()-1;
    bool  change;
    DataT     ev;
    do {
      change = false;
      for (IndexC i = low; i < upp; ++i) {
	if (arr[i] < arr[i+1]) {
	  ev = arr[i];
	  arr[i] = arr[i+1];
	  arr[i+1] = ev;
	  change = true;
	}
      }
      --upp; // at the moment the last item is the smallest one
    }while(change);
    return arr;
  }


  template<class DataT>
  BinOStreamC &
  operator<<(BinOStreamC & s, const SArray1dC<DataT> & arr) {
    s << arr.Size();
    for(BufferAccessIterC<DataT> it(arr);it;it++)
      s << it.Data();
    return s;
  }

  template<class DataT>
  BinIStreamC &
  operator>>(BinIStreamC & s, SArray1dC<DataT> & arr) {
    SizeT size;
    s >> size;
    if((size * sizeof(DataT)) > s.ArraySizeLimit() || size > s.ArraySizeLimit())
      throw ExceptionOutOfRangeC("Incoming array size exceeds limit for stream.");
    arr = SArray1dC<DataT>(size);
    for(BufferAccessIterC<DataT> it(arr);it;it++)
      s >> it.Data();
    return s;
  }

  template<class DataT>
  SArray1dC<DataT> SArray1dC<DataT>::operator+(const SArray1dC<DataT> & arr) const {
    SArray1dC<DataT> ret(Size());
    for(BufferAccessIter3C<DataT,DataT,DataT> it(ret,*this,arr);it;it++)
      it.Data1() = it.Data2() + it.Data3();
    return ret;
  }

  template<class DataT>
  SArray1dC<DataT> SArray1dC<DataT>::operator-(const SArray1dC<DataT> & arr) const {
    SArray1dC<DataT> ret(Size());
    for(BufferAccessIter3C<DataT,DataT,DataT> it(ret,*this,arr);it;it++)
      it.Data1() = it.Data2() - it.Data3();
    return ret;
  }

  template<class DataT>
  SArray1dC<DataT> SArray1dC<DataT>::operator*(const SArray1dC<DataT> & arr) const {
    SArray1dC<DataT> ret(Size());
    for(BufferAccessIter3C<DataT,DataT,DataT> it(ret,*this,arr);it;it++)
      it.Data1() = it.Data2() * it.Data3();
    return ret;
  }

  template<class DataT>
  SArray1dC<DataT> SArray1dC<DataT>::operator/(const SArray1dC<DataT> & arr) const {
    SArray1dC<DataT> ret(Size());
    for(BufferAccessIter3C<DataT,DataT,DataT> it(ret,*this,arr);it;it++)
      it.Data1() = it.Data2() / it.Data3();
    return ret;
  }

  template<class DataT>
  SArray1dC<DataT> SArray1dC<DataT>::operator+(const DataT &number) const {
    SArray1dC<DataT> ret(Size());
    for(BufferAccessIter2C<DataT,DataT> it(ret,*this);it;it++)
      it.Data1() = it.Data2() + number;
    return ret;
  }

  template<class DataT>
  SArray1dC<DataT> SArray1dC<DataT>::operator-(const DataT &number) const {
    SArray1dC<DataT> ret(Size());
    for(BufferAccessIter2C<DataT,DataT> it(ret,*this);it;it++)
      it.Data1() = it.Data2() - number;
    return ret;
  }

  template<class DataT>
  SArray1dC<DataT> SArray1dC<DataT>::operator*(const DataT &number) const {
    SArray1dC<DataT> ret(Size());
    for(BufferAccessIter2C<DataT,DataT> it(ret,*this);it;it++)
      it.Data1() = it.Data2() * number;
    return ret;
  }

  template<class DataT>
  SArray1dC<DataT> SArray1dC<DataT>::operator/(const DataT &number) const {
    SArray1dC<DataT> ret(Size());
    for(BufferAccessIter2C<DataT,DataT> it(ret,*this);it;it++)
      it.Data1() = it.Data2() / number;
    return ret;
  }

  template<class DataT>
  const SArray1dC<DataT> & SArray1dC<DataT>::operator+=(const SArray1dC<DataT> & arr) {
    for(BufferAccessIter2C<DataT,DataT> it(*this,arr);it;it++)
      it.Data1() += it.Data2();
    return *this;
  }

  template<class DataT>
  const SArray1dC<DataT> & SArray1dC<DataT>::operator-=(const SArray1dC<DataT> & arr) {
    for(BufferAccessIter2C<DataT,DataT> it(*this,arr);it;it++)
      it.Data1() -= it.Data2();
    return *this;
  }

  template<class DataT>
  const SArray1dC<DataT> & SArray1dC<DataT>::operator*=(const SArray1dC<DataT> & arr) {
    for(BufferAccessIter2C<DataT,DataT> it(*this,arr);it;it++)
      it.Data1() *= it.Data2();
    return *this;
  }

  template<class DataT>
  const SArray1dC<DataT> & SArray1dC<DataT>::operator/=(const SArray1dC<DataT> & arr) {
    for(BufferAccessIter2C<DataT,DataT> it(*this,arr);it;it++)
      it.Data1() /= it.Data2();
    return *this;
  }

  template<class DataT>
  const SArray1dC<DataT> & SArray1dC<DataT>::operator+=(const DataT &number) {
    for(BufferAccessIterC<DataT> it(*this);it;it++)
      it.Data1() += number;
    return *this;
  }


  template<class DataT>
  const SArray1dC<DataT> & SArray1dC<DataT>::operator-=(const DataT &number) {
    for(BufferAccessIterC<DataT> it(*this);it;it++)
      it.Data1() -= number;
    return *this;
  }

  template<class DataT>
  const SArray1dC<DataT> & SArray1dC<DataT>::operator*=(const DataT &number) {
    for(BufferAccessIterC<DataT> it(*this);it;it++)
      it.Data1() *= number;
    return *this;
  }

  template<class DataT>
  const SArray1dC<DataT> & SArray1dC<DataT>::operator/=(const DataT &number) {
    for(BufferAccessIterC<DataT> it(*this);it;it++)
      it.Data1() /= number;
    return *this;
  }

  template<class DataT>
  DataT SArray1dC<DataT>::Sum() const {
    if(Size() == 0) {
      DataT ret;
      SetZero(ret);
      return ret;
    }
    BufferAccessIterC<DataT> it(*this);
    DataT ret = StdCopy(*it);
    for(it++;it;it++)
      ret += *it;
    return ret;
  }

  template<class DataT>
  IndexC SArray1dC<DataT>::IndexOfMax() const {
    RavlAssertMsg(Size() > 0,"SArray1dC::IndexOfMax() Called on an empty array");
    const DataT *valueOfMax = &(this->operator[](0));
    for (BufferAccessIterC<DataT> i(*this); i; i++) {
      if (*valueOfMax < *i)
	valueOfMax = &(*i);
    }
    return IndexC((IntT)(valueOfMax - this->ReferenceElm()));
  }

  template<class DataT>
  IndexC SArray1dC<DataT>::IndexOfMin() const {
    RavlAssertMsg(Size() > 0,"SArray1dC::IndexOfMin() Called on an empty array");
    const DataT *valueOfMin = &(this->operator[](0));
    for (BufferAccessIterC<DataT> i(*this); i; i++) {
      if (*valueOfMin > *i)
	valueOfMin = &(*i);
    }
    return IndexC((IntT)(valueOfMin - this->ReferenceElm()));
  }


  template<class DataT>
  DataT SArray1dC<DataT>::SumOfSqr() const {
    if(Size() == 0) {
      DataT ret;
      SetZero(ret);
      return ret;
    }
    BufferAccessIterC<DataT> it(*this);
    DataT ret = Sqr(*it);
    for(it++;it;it++)
      ret += Sqr(*it);
    return ret;
  }

  template<class DataT>
  void SArray1dC<DataT>::Sort() {
    BufferAccessIterC<DataT> it(*this);
    if(!it)
      return ; // Empty, so we're done.
    DataT *lv;
    bool changed = true;
    do {
      changed = false;
      it.First(*this);
      lv = &(*it);
      it++;
      for(;it;it++) {
	if(*lv < *it) {
	  DataT t = *lv;
	  *lv = *it;
	  *it = t;
	  changed = true;
	}
	lv = &(*it);
      }
    } while(changed) ;
    return ;
  }

  template<class DataT>
  void SArray1dC<DataT>::Sort(bool (*leq)(DataT &o1,DataT &o2)) {
    BufferAccessIterC<DataT> it(*this);
    if(!it)
      return ; // Empty, so we're done.
    DataT *lv;
    bool changed = true;
    do {
      changed = false;
      it.First(*this);
      lv = &(*it);
      it++;
      for(;it;it++) {
        if(!leq(*lv,*it)) {
          DataT t = *lv;
          *lv = *it;
          *it = t;
          changed = true;
        }
        lv = &(*it);
      }
    } while(changed) ;
    return ;
  }

  template<class DataT>
  bool SArray1dC<DataT>::operator==(const SArray1dC<DataT> & vv) const {
    if(Size() != vv.Size())
      return false;
    for(BufferAccessIter2C<DataT,DataT> it(*this,vv);it;it++)
      if(it.Data1() != it.Data2())
	return false;
    return true;
  }

  template<class DataT>
  SizeT SArray1dC<DataT>::Hash() const {
    SizeT ret = Size();
    for(BufferAccessIterC<DataT> it(*this);it;it++)
      ret += StdHash(it.Data()) ^ (ret << 5) ;
    return ret;
  }

}
#endif

