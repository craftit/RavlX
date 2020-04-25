// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_SDARRAY1D_HEADER
#define RAVL_SDARRAY1D_HEADER 1
/////////////////////////////////////////////////////////////
//! file="Ravl/Core/Container/SArray/SDArray1d.hh"
//! lib=RavlCore
//! userlevel=Advanced
//! author="Charles Galambos"
//! date="4/9/1996"
//! docentry="Ravl.API.Core.Arrays.1D"

#include "Ravl/Types.hh"
#include "Ravl/Index.hh"
#include "Ravl/Assert.hh"

#if RAVL_HAVE_TEMPLATEREQUIREALLDEFINITIONS 
#include "Ravl/Stream.hh"
#include "Ravl/BinStream.hh"
#endif

#if RAVL_HAVE_ANSICPPHEADERS
#include <new>
#else
#include <new.h>
#endif

namespace RavlN {
  
  //! userlevel=Advanced
  //:1D Simple Dynamic Array.
  // This does not intalise data before it's needed. <p>
  // SMALL OBJECT
  
  template<class T>
  class SDArray1dC {
  public:
    SDArray1dC(UIntT initSize = 0);
    //: Constructor.
    
    SDArray1dC(const SDArray1dC<T> &Oth);
    //: Copy constructor.
    
    SDArray1dC(const SDArray1dC<T> &Oth,IntT Delta);
    //: Copy constructor, expanding array by delta. 
    
    ~SDArray1dC();
    //: Destructor.
    
    void Resize(UIntT size);
    //: Resize SDArray1d.
    
    void Empty(void);
    //: Remove all contents.
    
    SDArray1dC<T> Copy() const
    { return *this; }
    //: Make a copy of this array.
    
    SDArray1dC<T> DeepCopy(UIntT levels) const;
    //: Make a copy of this array.
    
    UIntT Add(const T &Data);
    //: Append data to end.
    // Returns the index of the appended item.
    // This function will automaticly expand the array
    // if necessary.
    
    inline UIntT Chop(void);
    //: Chop last number from array.
    //NB. This does NOT shrink the array in the current
    // implementation, this may change in future.
    
    inline T &operator[](UIntT i) {
      RavlAssert(i < Used);
      return ((T *)Data)[i];
    }
    //: Access element.
    
    inline const T &operator[](UIntT i) const {
      RavlAssert(i < Used);
      return ((T *)Data)[i];
    }
    //: Access element.
    
    inline T &operator[](const IndexC &i) {
      RavlAssert(((UIntT) i.V()) < Used);
      return ((T *)Data)[i.V()];
    }
    //: Access element.
    
    inline const T &operator[](const IndexC &i) const {
      RavlAssert(((UIntT) i.V()) < Used);
      return ((T *)Data)[i.V()];
    }
    //: Access element.

    inline T &operator[](const SizeC &i) {
      RavlAssert(((UIntT) i.V()) < Used);
      return ((T *)Data)[i.V()];
    }
    //: Access element.
    
    inline const T &operator[](const SizeC &i) const {
      RavlAssert(((UIntT) i.V()) < Used);
      return ((T *)Data)[i.V()];
    }
    //: Access element.

    const SDArray1dC<T> &operator=(const SDArray1dC<T> &Oth) ;
    //: Assign to another SDArray1d.
    
    inline const T &Last() const 
      { RavlAssert(Used > 0); return ((T *)Data)[Used-1]; }
    //: Get last element in array.
    
    bool Compare(const SDArray1dC<T> &Oth) const;
    //: Compare with another SDArray1d.
    
    inline bool operator==(const SDArray1dC<T> &Oth) const;
    //: Comparison.
    
    inline UIntT Size(void) const
      { return Used;  }
    //: Number of elements used.
    
    inline UIntT MaxN(void) const { return size; }
    //: Number of elements used.
    
    inline bool IsSpace() const { return Used < size; }
    //: Is space for more data ?
    
    inline bool IsEmpty() const { return Used == 0; }
    //: Is empty ?
  private:
    UIntT Used; // Number used in the SDArray1d.
    UIntT size; // size of allocated array.
    char *Data;  // Ptr to data array.
  };
  
  template<class T>
  std::ostream &operator<<(std::ostream &out,const SDArray1dC<T> &arr) {
    out << "0 " << arr.Size() << "\n";
    for(SizeT i = 0;i < arr.Size();i++)
      out << arr[i] << "\n";
    return out;
  }
  
  template<class T>
  std::istream &operator>>(std::istream &in,SDArray1dC<T> &arr) {
    UIntT asize,isize;
    in >> isize >> asize;
    RavlAssert(isize == 0);
    arr = SDArray1dC<T>(asize);
    for(UIntT i = 0;i < asize;i++)
      in >> arr[i];
    return in;
  }

  template<class T>
  BinOStreamC &operator<<(BinOStreamC &out,const SDArray1dC<T> &arr) {
    out << arr.Size();
    for(SizeT i = 0;i < arr.Size();i++)
      out << arr[i];
    return out;
  }
  
  template<class T>
  BinIStreamC &operator>>(BinIStreamC &in,SDArray1dC<T> &arr) {
    SizeT asize;
    in >> asize;
    if((asize * sizeof(T)) > in.ArraySizeLimit() || asize > in.ArraySizeLimit())
      throw ExceptionOutOfRangeC("Incoming array size exceeds limit for stream.");
    arr = SDArray1dC<T>(asize);
    for(UIntT i = 0;i < asize;i++)
      in >> arr[i];
    return in;
  }
  
  /////////////////////////
  // Constructors.
  
  template<class T>
  SDArray1dC<T>::SDArray1dC(UIntT Initsize) {
    Used = 0;
    size = Initsize;
    if(size > 0)
      Data = new char [sizeof(T) * size];
    else
      Data = 0;
  }
  
  template<class T>
  SDArray1dC<T>::SDArray1dC(const SDArray1dC<T> &Oth) 
    : Used(Oth.Used),
      size(Oth.size)
  {
    Data = new char [sizeof(T) * size];
    T *Place,*OPlace,*End = &((T *)Data)[Used];
    for(Place = (T *) Data,OPlace = (T *) Oth.Data;Place != End;Place++,OPlace++)
      new((void *)Place) T(*OPlace);
  }
  
  template<class T>
  SDArray1dC<T>::SDArray1dC(const SDArray1dC<T> &Oth,IntT Delta)
    : Used(Oth.Used),
      size(Oth.size + Delta)
  {
    RavlAssert(Delta >= 0);
    Data = new char [sizeof(T) * size];
    T *Place,*OPlace,*End = &((T *)Data)[Used];
    for(Place = (T *) Data,OPlace = (T *) Oth.Data;Place != End;Place++,OPlace++)
      new((void *)Place) T(*OPlace);  
  }
  
  template<class T>
  SDArray1dC<T>::~SDArray1dC() {
    Empty();
    delete [] Data;
  }
  
  template<class T>
  SDArray1dC<T> SDArray1dC<T>::DeepCopy(UIntT levels) const {
    SDArray1dC<T> ret(Used);
    RavlAssertMsg(levels != 0,"SDArray1dC is not refrence counted, level 0 copy is impossible. ");
    switch(levels) {
    case 1: ret = *this; break;
    case 2: {
      levels--;
      const T *end = &((const T *)Data)[Used];
      for(const T *place = (T *) Data;place != end;place++)
	ret.Add(StdCopy(*place));
    } break;
    default: {
      levels--;
      const T *end = &((const T *)Data)[Used];
      for(const T *place = (T *)Data;place != end;place++)
	ret.Add(StdDeepCopy(*place,levels));
    } break;
    }
    return ret;
  }


  template<class T>
  void SDArray1dC<T>::Resize(UIntT nsize) {
    T *Place,*NPlace,*End = &((T *)Data)[Used];
    if(nsize < Used) {
      RavlAssert(0); // This shouldn't really happen !
      for(Place = &((T *)Data)[nsize];Place != End;Place++)
	Place->~T(); // Destroy unused data.
      End = &((T *)Data)[nsize];
      Used = nsize;
    }
    char *NewD = new char [sizeof(T) * nsize];
    for(NPlace = (T *)NewD,Place = (T *)Data;Place != End;Place++,NPlace++) {
      new((void *)NPlace) T(*Place); // Dupicate in new array.
      Place->~T(); // Destroy old data.
    }
    delete [] Data;
    Data = NewD;
    size = nsize;
  };
  
  
  template<class T>
  const SDArray1dC<T> &SDArray1dC<T>::operator= (const SDArray1dC<T> &Oth) {
    if(&Oth == this) return *this;
    Empty();
    if(size < Oth.Used) {
      delete [] Data;
      size = Oth.size + 4;
      Data = new char [sizeof(T) * size];
    }
    Used = Oth.Used;
    T *Place,*OPlace,*End = &((T *)Data)[Used];
    for(Place = (T *) Data,OPlace = (T *) Oth.Data;Place != End;Place++,OPlace++) {
      new((void *)Place) T(*OPlace);
    }
    return *this;
  }
  
  template<class T>
  void SDArray1dC<T>::Empty(void) {
    T *Place,*End = &(((T *)Data)[Used]);
    for(Place = (T *) Data;Place != End;Place++)
      Place->~T();
    Used = 0;
  }
  
  template<class T>
  UIntT SDArray1dC<T>::Add(const T &Val) {
    if(Used >= size) {
      if(size == 0)
	Resize(4);
      else
	Resize(size << 1);
    }
    new((void *) & ((T *)Data)[Used]) T(Val);  
    return Used++;
  }
  
  template<class T>
  inline UIntT SDArray1dC<T>::Chop(void) { 
    RavlAssert(Used > 0); 
    ((T *) Data)[(--Used)].~T(); 
    return Used; 
  }
  
  template<class T>
  bool SDArray1dC<T>::Compare(const SDArray1dC<T> &Oth) const {
    if(&Oth == this) return true; // Short cut.
    if(Oth.Used != Used) return false;
    T *Place,*OPlace,*End = &((T *)Data)[Used];
    for(Place = (T *)Data,OPlace = (T *)Oth.Data;Place != End;Place++,OPlace++)
      if(!(*Place == *OPlace))
	return false;
    return true;
  }
  
  template<class T>
  inline bool SDArray1dC<T>::operator==(const SDArray1dC<T> &Oth) const { 
    return Compare(Oth); 
  }
    
}

#endif
