// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_BINSTREAM_HEADER
#define RAVL_BINSTREAM_HEADER 1
///////////////////////////////////////////////////////
//! file="Ravl/Core/Base/BinStream.hh"
//! lib=RavlCore
//! userlevel=Normal
//! author="Charles Galambos"
//! date="09/02/1999"
//! docentry="Ravl.API.Core.IO.Streams"

#include "Ravl/config.h"
#include "Ravl/Stream.hh"
#include "Ravl/Index.hh"
#include "Ravl/TFVector.hh"

// The following 3 flags dictate what kind of binary IO functionality we have.
// if RAVL_ENDIAN_COMPATILIBITY is set to 1 the streams will support both big
// and little endian files.

#define RAVL_ENDIAN_COMPATILIBITY    1
#define RAVL_BINSTREAM_ENDIAN_LITTLE 0
#define RAVL_BINSTREAM_ENDIAN_BIG    1

// The following defines translate defined(xyz) into 1 or 0 values so the can be used in
// code if() {}  statements.

#if RAVL_LITTLEENDIAN
#define RAVL_ENDIAN_LITTLE 1
#define RAVL_ENDIAN_BIG 0
#else
#define RAVL_ENDIAN_LITTLE 0
#define RAVL_ENDIAN_BIG 1
#endif

#if RAVL_BIGENDIANDOUBLES
#define RAVL_DOUBLE_ENDIAN_LITTLE 0
#define RAVL_DOUBLE_ENDIAN_BIG 1
#else
#define RAVL_DOUBLE_ENDIAN_LITTLE 1
#define RAVL_DOUBLE_ENDIAN_BIG 0
#endif

// Setup conditions to use.
// The assumption is that the compiler will remove unreachable code, so using if(0) { xyz; } will
// remove xyz from the generated code.

#if RAVL_ENDIAN_COMPATILIBITY
#define RAVL_ENDIAN_IF (useNativeEndian)
#else
#define RAVL_ENDIAN_IF ((RAVL_ENDIAN_LITTLE) == (RAVL_BINSTREAM_ENDIAN_LITTLE))
#endif

// Decide how to setup the bin streams by default.

#if (RAVL_LITTLEENDIAN) == (RAVL_BINSTREAM_ENDIAN_LITTLE)
#define RAVL_BINSTREAM_DEFAULT 1
#else
#define RAVL_BINSTREAM_DEFAULT 0
#endif


namespace RavlN  {
  bool DefaultToCompatibilityMode32Bit();
  //: Test if we should default to 32 bit compatibility mode.

  void SetCompatibilityMode32Bit(bool activate);
  //: Set 32 bit compatibility mode.

#if RAVL_HAVE_BYTESWAP
#include <byteswap.h>
#else
  // If we don't have byte swap define these.

  inline short bswap_16(const short &buf) {
    union {
      short s;
      char c[2];
    } ret,val;
    val.s = buf;
    ret.c[0] = val.c[1];
    ret.c[1] = val.c[0];
    return ret.s;
  }
  //! userlevel=Advanced
  //: Swap bytes of a 16 bit number.

  inline int bswap_32(const int &buf) {
    union {
      int i;
      char c[4];
    } ret,val;
    val.i = buf;
    ret.c[0] = val.c[3];
    ret.c[1] = val.c[2];
    ret.c[2] = val.c[1];
    ret.c[3] = val.c[0];
    return ret.i;
  }
  //! userlevel=Advanced
  //: Swap bytes of a 32 bit number.

  inline long long int bswap_64(const long long int & buf) {
    union {
      long long int i ;
      char c[8] ;
    } ret, val ;
    val.i=buf ;
    ret.c[0] = val.c[7] ;
    ret.c[1] = val.c[6] ;
    ret.c[2] = val.c[5] ;
    ret.c[3] = val.c[4] ;
    ret.c[4] = val.c[3] ;
    ret.c[5] = val.c[2] ;
    ret.c[6] = val.c[1] ;
    ret.c[7] = val.c[0] ;
    return ret.i ;
  }
  //: userlevel=Advanced
  //: Swap bytes of a 64 bit number

#endif

  //:-

  // Type     Bytes.
  // Int16T    2
  // UInt16T   2
  // IntT      4
  // UIntT     4
  // RealT     8
  // FloatT    4


  //! userlevel=Normal
  //: Machine independent binary input stream.
  // Note: All numbers are stored in big endian format.

  class BinIStreamC {
  public:
    BinIStreamC(const IStreamC &nIn)
      : in(nIn),
	useNativeEndian(RAVL_BINSTREAM_DEFAULT),
	m_compatibiltyMode32Bit(DefaultToCompatibilityMode32Bit()),
        m_arraySizeLimit((size_t) -1)
    {}
    //: Constructor.
    // From a IStreamC.

#if RAVL_HAVE_INTFILEDESCRIPTORS
    BinIStreamC(int fd)
      : in(fd,true,true),
	useNativeEndian(RAVL_BINSTREAM_DEFAULT),
	m_compatibiltyMode32Bit(DefaultToCompatibilityMode32Bit()),
        m_arraySizeLimit((size_t) -1)
    {}
    //: Constructor.
    // From a file descriptor.
#endif

    BinIStreamC(const StringC &nIn,bool buffered = true)
      : in(nIn,true,buffered),
	useNativeEndian(RAVL_BINSTREAM_DEFAULT),
	m_compatibiltyMode32Bit(DefaultToCompatibilityMode32Bit()),
        m_arraySizeLimit((size_t) -1)
    {}
    //: Constructor.

    BinIStreamC()
      : useNativeEndian(RAVL_BINSTREAM_DEFAULT),
        m_compatibiltyMode32Bit(DefaultToCompatibilityMode32Bit()),
        m_arraySizeLimit((size_t) -1)
    {}
    //: Default constructor.

    ~BinIStreamC();
    //: Destructor.

    inline BinIStreamC &IBuff(char *buff,IntT len);
    //: Character buffer. NB. Length is NOT saved.

    inline BinIStreamC &operator>>(char &dat);
    //: Read in a character.

    inline BinIStreamC &operator>>(signed char &dat);
    //: Read in a signed character.

    inline BinIStreamC &operator>>(unsigned char &dat);
    //: Read in an unsigned character.

    inline BinIStreamC &operator>>(Int16T &dat);
    //: Read in a 16 bit integer.

    inline BinIStreamC &operator>>(UInt16T &dat);
    //: Read in an unsigned 16 bit integer.

    inline BinIStreamC &operator>>(Int32T &dat);
    //: Read in an signed 32 bit integer.

    inline BinIStreamC &operator>>(UInt32T &dat);
    //: Read in an unsigned 32 bit integer.

    inline BinIStreamC &operator>>(Int64T &dat);
    //: Read in an signed 64 bit integer.

    inline BinIStreamC &operator>>(UInt64T &dat);
    //: Read in an signed 64 bit integer.

    inline BinIStreamC &operator>>(FloatT &dat);
    //: Read in a floating point number.

    inline BinIStreamC &operator>>(RealT &dat);
    //: Read in a double precision floating point number.

    inline BinIStreamC &operator>>(bool &dat);
    //: Read in a boolean value.

    std::streampos Tell() const { return in.Tell(); }
    //: Where are we in the stream.

    void Seek(std::streampos to) { in.Seek(to); }
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

    bool Close()
    { return Stream().Close(); }
    //: Close stream

    RCHandleC<RCBodyVC> &PointerManager()
    { return in.PointerManager(); }
    //: Access the pointer manager.

    void UseNativeEndian(bool nUseNativeEndian) {
      RavlAssert(RAVL_ENDIAN_COMPATILIBITY);
      useNativeEndian = nUseNativeEndian;
    }
    //: Set native endian

    void UseBigEndian(bool nUseBigEndian) {
      RavlAssert(RAVL_ENDIAN_COMPATILIBITY);
      useNativeEndian = (RAVL_ENDIAN_BIG == nUseBigEndian);
    }
    //: Set endian to use

    bool NativeEndian() const
    { return useNativeEndian; }
    //: Using native endian ?

    bool NativeEndianTest() const
    { return RAVL_ENDIAN_IF; }
    //: Using native endian ?

    SizeT ArraySizeLimit() const
    { return m_arraySizeLimit; }
    //: Maximum size of array's in bytes that is expected in the stream.
    // This allows the loaded to detect stream corruption and abort a load before
    // memory is exhausted.  This size should be given in bytes.
    // Typically this will be set to the size of the file being loaded, or packet
    // being processed.

    void SetArraySizeLimit(SizeT maxSize)
    { m_arraySizeLimit = maxSize; }
    //: Set the array size limit in bytes for this handle.
    // This allows the loaded to detect stream corruption and abort a load before
    // memory is exhausted.  This size should be given in bytes.
    // Typically this will be set to the size of the file being loaded, or packet
    // being processed.

    bool CompatibilityMode32Bit() const
    { return m_compatibiltyMode32Bit; }
    //: Set 32 bit compatibility mode.
    // In this mode container sizes will be written as 32 bit unsigned ints.

    void SetCompatibilityMode32Bit(bool model)
    { m_compatibiltyMode32Bit = model; }
    //: Set 32 bit compatibility mode
    // In this mode container sizes will be written as 32 bit unsigned ints.

  protected:
    IStreamC in;
    bool useNativeEndian;
    bool m_compatibiltyMode32Bit;
    SizeT m_arraySizeLimit;
  };

  //! userlevel=Normal
  //: Machine independent binary output stream.
  // Note: All numbers are stored in big endian format.

  class BinOStreamC {
  public:
    BinOStreamC(const OStreamC &nOut)
      : out(nOut),
	useNativeEndian(RAVL_BINSTREAM_DEFAULT),
	m_compatibiltyMode32Bit(DefaultToCompatibilityMode32Bit())
    {}
    //: Constructor.

#if RAVL_HAVE_INTFILEDESCRIPTORS
    BinOStreamC(int fd)
      : out(fd,true,true),
	useNativeEndian(RAVL_BINSTREAM_DEFAULT),
	m_compatibiltyMode32Bit(DefaultToCompatibilityMode32Bit())
    {}
    //: Constructor.
    // From a file descriptor.
#endif

    BinOStreamC(const StringC &nOut,bool buffered = true)
      : out(nOut,true,buffered),
	useNativeEndian(RAVL_BINSTREAM_DEFAULT),
	m_compatibiltyMode32Bit(DefaultToCompatibilityMode32Bit())
    {}
    //: Constructor.

    BinOStreamC()
      : useNativeEndian(RAVL_BINSTREAM_DEFAULT),
        m_compatibiltyMode32Bit(DefaultToCompatibilityMode32Bit())
    {}
    //: Default construtor.
    // Creates an invalid stream.

    ~BinOStreamC();
    //: Destructor.

    inline BinOStreamC &OBuff(const char *buff,IntT len);
    //: Character buffer. NB. Length is NOT saved.

    inline BinOStreamC &operator<<(char dat);
    //: Write a character.

    inline BinOStreamC &operator<<(signed char dat);
    //: Write a signed character.

    inline BinOStreamC &operator<<(unsigned char dat);
    //: Write an unsigned character.

    inline BinOStreamC &operator<<(Int16T dat);
    //: Write a 16 bit integer.

    inline BinOStreamC &operator<<(UInt16T dat);
    //: Write an unsigned 16 bit integer.

    inline BinOStreamC &operator<<(Int32T dat);
    //: Write a 32 bit integer.

    inline BinOStreamC &operator<<(UInt32T dat);
    //: Write a 32 bit unsigned integer.

    inline BinOStreamC &operator<<(Int64T dat);
    //: Read in an signed 64 bit integer.

    inline BinOStreamC &operator<<(UInt64T dat);
    //: Read in an signed 64 bit integer.

    inline BinOStreamC &operator<<(FloatT dat);
    //: Write a floating point number.

    inline BinOStreamC &operator<<(RealT dat);
    //: Write a double precision floating point number.

    inline BinOStreamC &operator<<(bool dat);
    //: Write a boolean value.

    BinOStreamC &operator<<(const char *text);
    //: Write a 'C' style string.

    std::streampos Tell() const { return out.Tell(); }
    //: Where are we in the stream.

    void Seek(std::streampos to) { out.Seek(to); }
    //: Goto a position in the stream.

    inline const StringC &Name() const
    { return out.Name(); }
    //: Returns the name of the stream.

    OStreamC &Stream() { return out; }
    //: Access underlying stream.

    const OStreamC &Stream() const { return out; }
    //: Access underlying stream.

    bool Close()
    { return Stream().Close(); }
    //: Flush and close stream

    RCHandleC<RCBodyVC> &PointerManager()
    { return out.PointerManager(); }
    //: Access the pointer manager.

    void UseNativeEndian(bool nUseNativeEndian) {
      RavlAssertMsg(nUseNativeEndian || RAVL_ENDIAN_COMPATILIBITY,"Compatibility binary streams not enabled. ");
      useNativeEndian = nUseNativeEndian;
    }
    //: Set native endian

    void UseBigEndian(bool nUseBigEndian) {
      RavlAssertMsg((nUseBigEndian == RAVL_BINSTREAM_ENDIAN_BIG) || RAVL_ENDIAN_COMPATILIBITY,"Compatibility binary streams not enabled. ");
      useNativeEndian = (RAVL_ENDIAN_BIG == nUseBigEndian);
    }
    //: Set endian to use

    bool NativeEndian() const
    { return useNativeEndian; }
    //: Using native endian ?

    bool NativeEndianTest() const
    { return RAVL_ENDIAN_IF; }
    //: Using native endian ?

    bool CompatibilityMode32Bit() const
    { return m_compatibiltyMode32Bit; }
    //: Set 32 bit compatibility mode.
    // In this mode container sizes will be written as 32 bit unsigned ints.

    void SetCompatibilityMode32Bit(bool model)
    { m_compatibiltyMode32Bit = model; }
    //: Set 32 bit compatibility mode
    // In this mode container sizes will be written as 32 bit unsigned ints.

  protected:
    OStreamC out;
    bool useNativeEndian;
    bool m_compatibiltyMode32Bit;
  };


  /////////////////////////////////////////////////////



  inline
  BinIStreamC & BinIStreamC::IBuff(char *buff,IntT len) {
    in.read(buff,len);
    return *this;
  }

  BinIStreamC &BinIStreamC::operator>>(char &dat)
  { in.read(&dat,sizeof(char));  return *this; }

  BinIStreamC &BinIStreamC::operator>>(signed char &dat)
  { in.read((char *) &dat,sizeof(signed char));  return *this; }

  BinIStreamC &BinIStreamC::operator>>(unsigned char &dat)
  { in.read((char *)&dat,sizeof(unsigned char));  return *this; }

  BinIStreamC &BinIStreamC::operator>>(Int16T &dat) {
    if(RAVL_ENDIAN_IF) {
      in.read((char *)&dat,2);
    } else {
      short buf;
      in.read((char *)&buf,2);
      dat = bswap_16(buf);
    }
    return *this;
  }

  BinIStreamC &BinIStreamC::operator>>(UInt16T &dat) {
    if(RAVL_ENDIAN_IF) {
      in.read((char *)&dat,2);
    } else {
      short buf;
      in.read((char *) &buf,2);
      dat = (UInt16T) bswap_16(buf);
    }
    return *this;
  }

  BinIStreamC &BinIStreamC::operator>>(Int32T &dat) {
    if(RAVL_ENDIAN_IF) {
      in.read((char *)&dat,4);
    } else {
      int buf;
      in.read((char *) &buf,4);
      dat = (IntT) bswap_32(buf);
    }
    return *this;
  }

  BinIStreamC &BinIStreamC::operator>>(UInt32T &dat) {
    if(RAVL_ENDIAN_IF) {
      in.read((char *)&dat,4);
    } else {
      int buf;
      in.read((char *) &buf,4);
      dat = (UIntT) bswap_32(buf);
    }
    return *this;
  }

  inline
  BinIStreamC &BinIStreamC::operator>>(Int64T &dat) {
    if(RAVL_ENDIAN_IF) {
      in.read((char *)&dat,8);
    } else {
      union {
	Int64T lli;
	IntT i[2];
      } val;
      (*this) >> val.i[1];
      (*this) >> val.i[0];
      dat = val.lli;
    }
    return *this;
  }

  inline
  BinIStreamC &BinIStreamC::operator>>(UInt64T &dat) {
    if(RAVL_ENDIAN_IF) {
      in.read((char *)&dat,8);
    } else {
      union {
	UInt64T lli;
	UIntT i[2];
      } val;
      (*this) >> val.i[1];
      (*this) >> val.i[0];
      dat = val.lli;
    }
    return *this;
  }

  inline
  BinIStreamC &BinIStreamC::operator>>(FloatT &dat) {
    RavlAssert(sizeof(FloatT) == sizeof(IntT));
    union {
      FloatT f;
      IntT i;
    } val;
    (*this) >> val.i;
    dat = val.f;
    return *this;
  }

  inline
  BinIStreamC &BinIStreamC::operator>>(RealT &dat) {
    RavlAssert(sizeof(RealT) == (sizeof(IntT) * 2));
    union {
      RealT d;
      IntT i[2];
    } val;
    if(RAVL_ENDIAN_IF == (RAVL_DOUBLE_ENDIAN_BIG == RAVL_ENDIAN_BIG)) {
      in.read((char *)&(val.i[0]),8);
    } else {
      IntT buff[2];
      in.read((char *)buff,8);
      val.i[0] = bswap_32(buff[1]);
      val.i[1] = bswap_32(buff[0]);
    }
    dat = val.d;
    return *this;
  }

  inline
  BinIStreamC &BinIStreamC::operator>>(bool &dat) {
    char flag;
    (*this) >> flag;
    dat = flag?true:false;
    return (*this);
  }

  /////////////////////////////////////////////////////

  inline
  BinOStreamC &BinOStreamC::OBuff(const char *buff,IntT len) {
    out.write(buff,len);
    return *this;
  }

  BinOStreamC &BinOStreamC::operator<<(char dat)
  { out.write(&dat,1);  return *this; }

  BinOStreamC &BinOStreamC::operator<<(signed char dat)
  { out.write((char *) &dat,1);  return *this; }

  BinOStreamC &BinOStreamC::operator<<(unsigned char dat)
  { out.write((char *)&dat,1);  return *this; }

  BinOStreamC &BinOStreamC::operator<<(Int16T dat) {
    if(RAVL_ENDIAN_IF) {
      out.write((char *)&dat,2);
    } else {
      short buf = bswap_16(dat);
      out.write((char *) &buf,2);
    }
    return *this;
  }

  BinOStreamC &BinOStreamC::operator<<(UInt16T dat) {
    if(RAVL_ENDIAN_IF) {
      out.write((char *)&dat,2);
    } else {
      short buf = bswap_16(dat);
      out.write((char *) &buf,2);
    }
    return *this;
  }

  BinOStreamC &BinOStreamC::operator<<(Int32T dat) {
    if(RAVL_ENDIAN_IF) {
      out.write((char *)&dat,4);
    } else {
      int buf = bswap_32(dat);
      out.write((char *) &buf,4);
    }
    return *this;
  }

  BinOStreamC &BinOStreamC::operator<<(UInt32T dat) {
    if(RAVL_ENDIAN_IF) {
      out.write((char *)&dat,4);
    } else {
      int buf = bswap_32(dat);
      out.write((char *) &buf,4);
    }
    return *this;
  }

  BinOStreamC &BinOStreamC::operator<<(Int64T dat) {
    if(RAVL_ENDIAN_IF) {
      out.write((char *)&dat,8);
    } else {
      union {
	Int64T lli;
	UIntT i[2];
      } val;
      val.lli = dat;
      (*this) << val.i[1];
      (*this) << val.i[0];
    }
    return *this;
  }

  BinOStreamC &BinOStreamC::operator<<(UInt64T dat) {
    if(RAVL_ENDIAN_IF) {
      out.write((char *)&dat,8);
    } else {
      union {
	UInt64T lli;
	UIntT i[2];
      } val;
      val.lli = dat;
      (*this) << val.i[1];
      (*this) << val.i[0];
    }
    return *this;
  }

  inline
  BinOStreamC &BinOStreamC::operator<<(FloatT dat) {
    RavlAssert(sizeof(FloatT) == sizeof(IntT));
    union {
      FloatT f;
      IntT i;
    } val;
    val.f = dat;
    (*this) << val.i;
    return *this;
  }

  inline
  BinOStreamC &BinOStreamC::operator<<(RealT dat) {
    RavlAssert(sizeof(RealT) == (sizeof(IntT) * 2));
    union {
      RealT r;
      IntT i[2];
    } val;
    val.r = dat;
    IntT buff[2];
    if(RAVL_ENDIAN_IF == (RAVL_DOUBLE_ENDIAN_BIG == RAVL_ENDIAN_BIG)) {
      buff[0] = val.i[0];
      buff[1] = val.i[1];
    } else {
      buff[0] = bswap_32(val.i[1]);
      buff[1] = bswap_32(val.i[0]);
    }
    out.write((char *) buff,8);
    return *this;
  }

  inline
  BinOStreamC &BinOStreamC::operator<<(bool dat) {
    char out;
    out = dat?1:0;
    (*this) << out;
    return (*this);
  }

  ///// A few extra's to make life easier. /////////////////////////////////////////

  BinIStreamC &operator>>(BinIStreamC &s,IndexC &ind);
  //: Read from binary stream

  BinOStreamC &operator<<(BinOStreamC &s,const IndexC &ind);
  //: Write to binary stream

  template<class DataT,unsigned int N>
  inline
  BinOStreamC &operator<<(BinOStreamC &out,const TFVectorC<DataT,N> &dat) {
    for(UIntT i = 0;i < N;i++)
      out << dat[i];
    return out;
  }

  template<class DataT,unsigned int N>
  inline
  BinIStreamC &operator>>(BinIStreamC &in,TFVectorC<DataT,N> &dat) {
    for(UIntT i = 0;i < N;i++)
      in >> dat[i];
    return in;
  }

}

#endif
