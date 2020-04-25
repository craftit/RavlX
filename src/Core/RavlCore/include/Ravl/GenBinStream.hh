// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_GENBINSTREAM_HEADER
#define RAVL_GENBINSTREAM_HEADER 1
///////////////////////////////////////////////////////
//! file="Ravl/Core/Base/GenBinStream.hh"
//! lib=RavlCore
//! userlevel=Normal
//! author="Charles Galambos, Daniele Muntoni"
//! date="26/03/2003"
//! docentry="Ravl.API.Core.IO.Streams"
//! rcsid="$Id$"

#include "Ravl/config.h"
#include "Ravl/Stream.hh"
#include "Ravl/Index.hh"
#include "Ravl/TFVector.hh"
#include "Ravl/BinStream.hh"

namespace RavlN  {
  
  //! userlevel=Normal
  //: General binary input stream.

  class GenBinIStreamC {
  public:
    GenBinIStreamC(const IStreamC &nIn, const bool swapEndian = false)
      : in(nIn)
    {toSwap = swapEndian;}
    
    //: Constructor.
    // From a IStreamC, swapEndian is true if you want to switch between big and little endian.
    
#if RAVL_HAVE_INTFILEDESCRIPTORS
    GenBinIStreamC(int fd, const bool swapEndian = false)
      : in(fd)
      {toSwap = swapEndian;}
    //: Constructor.
    // From a file descriptor, swapEndian is true if you want to switch between big and little endian.
#endif

    GenBinIStreamC(const StringC &nIn, const bool swapEndian = false ,bool buffered = true)
      : in(nIn,true,buffered) // Open binary stream.
      {toSwap = swapEndian;}
    //: Constructor.
    // From a file name, swapEndian is true if you want to switch between big and little endian.
         
    GenBinIStreamC()
      {}
    //: Default construtor.
    
    inline GenBinIStreamC &IBuff(char *buff,IntT len); 
    //: Character buffer. NB. Length is NOT saved.
    
    inline GenBinIStreamC &operator>>(char &dat);
    //: Read in a character.
    
    inline GenBinIStreamC &operator>>(signed char &dat);
    //: Read in a signed character.
    
    inline GenBinIStreamC &operator>>(unsigned char &dat);  
    //: Read in an unsigned character.
    
    inline GenBinIStreamC &operator>>(Int16T &dat);
    //: Read in a 16 bit integer.
    
    inline GenBinIStreamC &operator>>(UInt16T &dat);
    //: Read in an unsigned 16 bit integer.

    inline GenBinIStreamC &operator>>(Int64T &dat);
    //: Read in a signed 64 bit integer 

     inline GenBinIStreamC &operator>>(UInt64T &dat);
     //: Read in an unsigned 64 bit integer 
    
    inline GenBinIStreamC &operator>>(IntT &dat);
    //: Read in an signed 32 bit integer.
    
    inline GenBinIStreamC &operator>>(UIntT &dat);  
    //: Read in an unsigned 32 bit integer.
    
    inline GenBinIStreamC &operator>>(FloatT &dat);  
    //: Read in a floating point number.
    
    inline GenBinIStreamC &operator>>(RealT &dat);  
    //: Read in a double precision floating point number.
    
    inline GenBinIStreamC &operator>>(bool &dat);
    //: Read in a boolean value.
    
    streampos Tell() const { return in.Tell(); }
    //: Where are we in the stream.
    
    void Seek(streampos to) { in.Seek(to); }
    //: Goto a position in the stream.
    
    inline const StringC &Name() const
      { return in.Name(); }
    //: Returns the name of the stream.
    
    IStreamC &Stream() { return in; }
    //: Access underlying stream.
    
    const IStreamC &Stream() const { return in; }
    //: Access underlying stream.
    
    inline bool IsEndOfStream() 
      { return in.IsEndOfStream(); }
    //: Test if at end of the stream.
  
  protected:
    void Dummy(void);
    //: Dummy func.
    
    bool toSwap; //true if the stream is littleendian but the machine is not (or viceversa). False by default.

    IStreamC in;
    IntT idAlloc;
  //HashC<IntT,void *> ptrMap;
  };

  //! userlevel=Normal
  //: General binary output stream.
  
  class GenBinOStreamC {
  public:
    GenBinOStreamC(const OStreamC &nOut, const bool swapEndian = false)
      : out(nOut)
    {toSwap = swapEndian;}
    //: Constructor. 
    // From an OstreamC object. swapEndian is true if you want to switch between big and little endian.

#if RAVL_HAVE_INTFILEDESCRIPTORS
    GenBinOStreamC(int fd, const bool swapEndian = false)
      : out(fd)
    {toSwap = swapEndian;}
    //: Constructor.
    // From a file descriptor, swapEndian is true if you want to switch between big and little endian.
#endif
    
    GenBinOStreamC(const StringC &nOut, const bool swapEndian = false, bool buffered = true)
      : out(nOut,true,buffered) // Open binary stream.
    {toSwap = swapEndian;}
    //: Constructor.
    // From a file name, swapEndian is true if you want to switch between big and little endian.

    GenBinOStreamC()
    {}
    //: Default constructor.
    // Creates an invalid stream.
    
    bool toSwap; //true if the stream is littleendian but the machine is not (or vice versa). False by default.

    inline GenBinOStreamC &OBuff(const char *buff,IntT len); 
    //: Character buffer. NB. Length is NOT saved.
    
    inline GenBinOStreamC &operator<<(char dat);
    //: Write a character.
    
    inline GenBinOStreamC &operator<<(signed char dat);
    //: Write a signed character.
    
    inline GenBinOStreamC &operator<<(unsigned char dat);  
    //: Write an unsigned character.
    
    inline GenBinOStreamC &operator<<(Int16T dat);
    //: Write a 16 bit integer.
    
    inline GenBinOStreamC &operator<<(UInt16T dat);
    //: Write an unsigned 16 bit integer.
    
    inline GenBinOStreamC &operator<<(IntT dat);    
    //: Write a 32 bit integer.
    
    inline GenBinOStreamC &operator<<(UIntT dat);  
    //: Write a 32 bit unsigned integer.

    inline GenBinOStreamC & operator <<(Int64T dat);
    //: Write a 64bit integer 

    inline GenBinOStreamC & operator << (UInt64T dat) ;
    //: Write a 64 bit unsigned integer
    
    inline GenBinOStreamC &operator<<(FloatT dat);  
    //: Write a floating point number.
    
    inline GenBinOStreamC &operator<<(RealT dat);  
    //: Write a double precision floating point number.
    
    inline GenBinOStreamC &operator<<(bool dat);
    //: Write a boolean value.
    
    GenBinOStreamC &operator<<(const char *text);
    //: Write a 'C' style string.
    
    streampos Tell() const { return out.Tell(); }
    //: Where are we in the stream.
    
    void Seek(streampos to) { out.Seek(to); }
    //: Goto a position in the stream.
    
    inline const StringC &Name() const
    { return out.Name(); }
    //: Returns the name of the stream.
    
    OStreamC &Stream() { return out; }
    //: Access underlying stream.
    
    const OStreamC &Stream() const { return out; }
    //: Access underlying stream.
    
  protected:
    void Dummy(void);
    //: Dummy func.
    
    OStreamC out;
    //IntT idAlloc;
    //HashC<void *,IntT> ptrMap;
  };

  
  /////////////////////////////////////////////////////
  
  inline 
  GenBinIStreamC & GenBinIStreamC::IBuff(char *buff,IntT len) {
    in.read(buff,len);
    return *this;
  }

  GenBinIStreamC &GenBinIStreamC::operator>>(char &dat)
  { in.read(&dat,sizeof(char));  return *this; }
  
  GenBinIStreamC &GenBinIStreamC::operator>>(signed char &dat)
  { in.read((char *) &dat,sizeof(signed char));  return *this; }

  GenBinIStreamC &GenBinIStreamC::operator>>(unsigned char &dat)
  { in.read((char *)&dat,sizeof(unsigned char));  return *this; }
  
  GenBinIStreamC &GenBinIStreamC::operator>>(Int16T &dat) {
    if(toSwap) in.read((char *)&dat,2); 
    else {
      Int16T buf;
      in.read((char *)&buf,2);  
      dat = bswap_16(buf);
    }
    return *this; 
  }
  
  GenBinIStreamC &GenBinIStreamC::operator>>(UInt16T &dat) {
    if(toSwap) in.read((char *)&dat,2); 
    else{
      UInt16T buf;
      in.read((char *) &buf,2);  
      dat = (UInt16T) bswap_16(buf);
    }
    return *this; 
  }

GenBinIStreamC & GenBinIStreamC::operator>>(UInt64T & dat) {
	if (toSwap) in.read((char*)&dat,8) ;
	else{
	UInt64T buf ;
	in.read((char*) &buf,8);
	dat = (UInt64T) bswap_64(buf);
	}
	return *this ;
}

GenBinIStreamC & GenBinIStreamC::operator>>(Int64T & dat) {
	if (toSwap) in.read((char*) &dat,8) ;
	else{
	Int64T buf ; 
	in.read((char*) &buf,8) ;
	dat = (Int64T) bswap_64(buf) ;
	}
	return *this ;
}	
  
  GenBinIStreamC &GenBinIStreamC::operator>>(IntT &dat) {
    if(toSwap) in.read((char *)&dat,4);
    else{
    IntT buf;
    in.read((char *) &buf,4);
    dat = (IntT) bswap_32(buf);
    }
    return *this; 
  }
  
  GenBinIStreamC &GenBinIStreamC::operator>>(UIntT &dat) {
    if(toSwap)in.read((char *)&dat,4);
    else{
    UIntT buf;
    in.read((char *) &buf,4);
    dat = (UIntT) bswap_32(buf);
    }
    return *this; 
  }
  
  inline 
  GenBinIStreamC &GenBinIStreamC::operator>>(FloatT &dat) {
    RavlAssert(sizeof(FloatT) == sizeof(IntT));
    (*this) >> (*((IntT *) &dat));
    return *this;
  }
  
  inline 
  GenBinIStreamC &GenBinIStreamC::operator>>(RealT &dat) {
    RavlAssert(sizeof(RealT) == (sizeof(IntT) * 2));
    IntT *at = (IntT *) (&dat);
    if(toSwap) (*this) >> at[0] >> at[1];
    else (*this) >> at[1] >> at[0];
    return *this;
  }
  
  inline 
  GenBinIStreamC &GenBinIStreamC::operator>>(bool &dat) {
    char flag;
    (*this) >> flag;
    dat = flag?true:false;
    return (*this);
  }
  
  /////////////////////////////////////////////////////
  
  inline 
  GenBinOStreamC &GenBinOStreamC::OBuff(const char *buff,IntT len) {
    out.write(buff,len);
    return *this;
  }
  
  GenBinOStreamC &GenBinOStreamC::operator<<(char dat)
  { out.write(&dat,1);  return *this; }

  GenBinOStreamC &GenBinOStreamC::operator<<(signed char dat)
  { out.write((char *) &dat,1);  return *this; }

  GenBinOStreamC &GenBinOStreamC::operator<<(unsigned char dat)
  { out.write((char *)&dat,1);  return *this; }

  GenBinOStreamC &GenBinOStreamC::operator<<(Int16T dat) {
    if(toSwap) out.write((char *)&dat,2);  
    else{
      short buf = bswap_16(dat);
      out.write((char *) &buf,2);  
    }
    return *this; 
  }
  
  GenBinOStreamC &GenBinOStreamC::operator<<(UInt16T dat) {
    if(toSwap) out.write((char *)&dat,2);  
    else{
      UInt16T buf = bswap_16(dat);
      out.write((char *) &buf,2);  
    }
    return *this; 
  }
  
  GenBinOStreamC &GenBinOStreamC::operator<<(IntT dat) {
    if(toSwap) out.write((char *)&dat,4);
    else{
      IntT buf = bswap_32(dat);
      out.write((char *) &buf,4);  
    }
    return *this; 
  }
  
  GenBinOStreamC &GenBinOStreamC::operator<<(UIntT dat) {
    if(toSwap) out.write((char *)&dat,4);
    else{
      UIntT buf = bswap_32(dat);
      out.write((char *) &buf,4);
    }
    return *this; 
  }
  
  GenBinOStreamC &GenBinOStreamC::operator<<(Int64T dat) {
    if(toSwap) out.write((char *)&dat,8);
    else{
      Int64T buf = bswap_64(dat);
      out.write((char *) &buf,8);  
    }
    return *this; 
  }
  
  GenBinOStreamC &GenBinOStreamC::operator<<(UInt64T dat) {
    if(toSwap) out.write((char *)&dat,8);
    else{
      UInt64T buf = bswap_64(dat);
      out.write((char *) &buf,8);
    }
    return *this; 
  }
  
  inline 
  GenBinOStreamC &GenBinOStreamC::operator<<(FloatT dat) {
    RavlAssert(sizeof(FloatT) == sizeof(IntT));
    union {
      FloatT f;
      IntT i;
    } tmp;
    tmp.f = dat;
    (*this) << tmp.i;
    return *this;
  }
  
  inline 
  GenBinOStreamC &GenBinOStreamC::operator<<(RealT dat) {
    RavlAssert(sizeof(RealT) == (sizeof(IntT) * 2));
    union {
      RealT f;
      IntT i[2];
    } tmp;
    tmp.f = dat;
    if(toSwap) (*this) << tmp.i[0] << tmp.i[1];
    else (*this) << tmp.i[1] << tmp.i[0];
    return *this;
  }
  
  inline 
  GenBinOStreamC &GenBinOStreamC::operator<<(bool dat) {
    char out;
    out = dat?1:0;
    (*this) << out;
    return (*this);
  }

  ///// A few extra's to make life easier. /////////////////////////////////////////
  
  inline
  GenBinIStreamC &operator>>(GenBinIStreamC &s,IndexC &ind)  {
    s >> ind.V();
    return s;
  }
  
  inline
  GenBinOStreamC &operator<<(GenBinOStreamC &s,const IndexC &ind)  {
    s << ind.V();
    return s;
  }

  template<class DataT,unsigned int N>
  inline
  GenBinOStreamC &operator<<(GenBinOStreamC &out,const TFVectorC<DataT,N> &dat) {
    for(UIntT i = 0;i < N;i++)
      out << dat[i];
    return out;
  }
  
  template<class DataT,unsigned int N>
  inline
  GenBinIStreamC &operator>>(GenBinIStreamC &in,TFVectorC<DataT,N> &dat) {
    for(UIntT i = 0;i < N;i++)
      in >> dat[i];
    return in;
  }

}

#endif
