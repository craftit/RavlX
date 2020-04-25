// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_STDIO_FDSTREAM_HEADER
#define RAVL_STDIO_FDSTREAM_HEADER 1
/////////////////////////////////////////////////////////////////
//! author="Charles Galambos"
//! lib=RavlCore
//! file="Ravl/Core/Base/stdio_fdstream.hh"
//! docentry="Ravl.API.Core.IO.Streams"

#include "Ravl/config.h"

#if (RAVL_COMPILER_GCC3 || RAVL_COMPILER_GCC4) && !RAVL_COMPILER_LLVM

#include "Ravl/Stream.hh"
#include <ext/stdio_filebuf.h>

#define RAVL_USE_FDSTREAMBUF 1

namespace RavlN {
  using namespace __gnu_cxx;
  
#if !RAVL_COMPILER_GCC3_4 && !RAVL_COMPILER_GCC4
  //! userlevel=Develop
  //: Basic input from a fd stream.
  
  template<typename CharT, typename TraitsT = std::char_traits<CharT> >
  class stdio_ifdstream 
    : public basic_istream<CharT, TraitsT>
  {
  public:
    typedef CharT 			    char_type;
    typedef TraitsT 			    traits_type;
    typedef typename traits_type::int_type  int_type;
    typedef typename traits_type::pos_type  pos_type;
    typedef typename traits_type::off_type  off_type;

    typedef stdio_filebuf<char_type, traits_type> buf_type;
    
    explicit stdio_ifdstream(int fd, ios_base::openmode mode = ios_base::in, bool freeFd = true,int buffSize = 4096)
      : basic_istream<char_type, traits_type>(0),
	filebuf(fd,mode,freeFd,buffSize)
    { this->init(&filebuf); }
    //: Construct from a file handle and an open mode.
    
    buf_type* rdbuf() const
    { return const_cast<buf_type *>(&filebuf); }
    //: Access fdbuffer.
    
    stdio_ifdstream()
      : basic_istream<char_type, traits_type>(0), 
	filebuf()
    { init(&filebuf); }
    //: Default constructor.
    
    bool is_open() { return filebuf.is_open(); }
    //: Check if its open.
    
    void open(int fd, ios_base::openmode mode = ios_base::in) {
      if (!filebuf.open(fd,mode | ios_base::in))
	setstate(ios_base::failbit);
    }
    //: open a file handle.
    
    void close() {
      if (!filebuf.close())
	setstate(ios_base::failbit);
    }
    //: Close a file.
    
  private:
    buf_type filebuf;
  };
  
  //! userlevel=Develop
  //: Derivation of general output streams, specific to unix file handles.
  
  template<typename CharT, typename TraitsT = std::char_traits<CharT> >
  class stdio_ofdstream 
    : public basic_ostream<CharT,TraitsT>
  {
  public:
    typedef CharT 			    char_type;
    typedef TraitsT 			    traits_type;
    typedef typename traits_type::int_type  int_type;
    typedef typename traits_type::pos_type  pos_type;
    typedef typename traits_type::off_type  off_type;

    typedef stdio_filebuf<char_type, traits_type> buf_type;
    
    explicit stdio_ofdstream(int fd,ios_base::openmode mode = ios_base::out|ios_base::trunc,bool freeFd = true,int buffSize = 4096)
      : basic_ostream<char_type, traits_type>(0),
	filebuf(fd,mode,freeFd,buffSize)
    { init(&filebuf); }
    //: Construct from a file handle.
    //:param:freeFd if true file descriptor is free'd after use.
    
    stdio_ofdstream()
      : basic_ostream<char_type, traits_type>(0), 
	filebuf()
    { init(&filebuf); }
    //: Default constructor.
    
    buf_type* rdbuf() const
    { return const_cast<buf_type *>(&filebuf); }
    //: Access fd buffer.
    
    bool is_open() 
    { return filebuf.is_open(); }
    //: Is stream open ?

    void open(int fd,ios_base::openmode mode = ios_base::out | ios_base::trunc) {
      if (!filebuf.open(fd, mode | ios_base::out))
	setstate(ios_base::failbit);
    }
    //: Open a stream with file handle 'fd'.
    
    void close() {
      if (!filebuf.close())
	setstate(ios_base::failbit);
    }
    //: Close a file handle.
  private:
    buf_type filebuf;
  };
  
  
  //! userlevel=Develop
  //: Basic fdstream.
  
  template<typename CharT, typename TraitsT>
  class stdio_fdstream 
    : public basic_iostream<CharT, TraitsT>
  {
  public:
    typedef CharT 		           char_type;
    typedef TraitsT 		           traits_type;
    typedef typename traits_type::int_type int_type;
    typedef typename traits_type::pos_type pos_type;
    typedef typename traits_type::off_type off_type;
    
    typedef stdio_filebuf<char_type, traits_type> buf_type;
    
    explicit stdio_fdstream(int fd,ios_base::openmode mode = ios_base::in | ios_base::out,bool freeFd = true,int buffSize = 4096)
      : basic_iostream<char_type, traits_type>(0), 
	filebuf(fd,mode,freeFd,buffSize)
    { init(&filebuf); }
    //: Cosntructor.
    
    stdio_fdstream()
      : basic_iostream<CharT, TraitsT>(0),
	filebuf()
    { init(&filebuf); }
    //: Default constructor.
    
    buf_type* rdbuf() const
    { return const_cast<buf_type *>(&filebuf); }
    
    bool is_open() { return fdbuf.is_open(); }
    //: Is stream open ?
    
    void open(int fd,ios_base::openmode mode = ios_base::in | ios_base::out) {
      if (!filebuf.open(fd, mode))
	setstate(ios_base::failbit);
    }
    //: Open a stream with the given file handle.
    
    void close() {
      if(!filebuf.close())
	setstate(ios_base::failbit);
    }
    //: Close file handle.
  private:
    buf_type  filebuf;
  };
  
#else
  
  //! userlevel=Develop
  //: Basic input from a fd stream.
  
  template<typename CharT, typename TraitsT = std::char_traits<CharT> >
  class stdio_ifdstream 
    : public basic_istream<CharT, TraitsT>
  {
  public:
    typedef CharT 			    char_type;
    typedef TraitsT 			    traits_type;
    typedef typename traits_type::int_type  int_type;
    typedef typename traits_type::pos_type  pos_type;
    typedef typename traits_type::off_type  off_type;

    typedef stdio_filebuf<char_type, traits_type> buf_type;
    
    explicit stdio_ifdstream(int fd, ios_base::openmode mode = ios_base::in, bool freeFd =true,int buffSize = 4096)
      : basic_istream<char_type, traits_type>(0),
	filebuf(fd,mode,buffSize)
    { 
      this->init(&filebuf);
      RavlAssert(freeFd); // Not given a choice anymore
    }
    //: Construct from a file handle and an open mode.
    
    buf_type* rdbuf() const
    { return const_cast<buf_type *>(&filebuf); }
    //: Access fdbuffer.
    
    stdio_ifdstream()
      : basic_istream<char_type, traits_type>(0), 
	filebuf()
    { this->init(&filebuf); }
    //: Default constructor.
    
  private:
    buf_type filebuf;
  };
  
  //! userlevel=Develop
  //: Derivation of general output streams, specific to unix file handles.
  
  template<typename CharT, typename TraitsT = std::char_traits<CharT> >
  class stdio_ofdstream 
    : public basic_ostream<CharT,TraitsT>
  {
  public:
    typedef CharT 			    char_type;
    typedef TraitsT 			    traits_type;
    typedef typename traits_type::int_type  int_type;
    typedef typename traits_type::pos_type  pos_type;
    typedef typename traits_type::off_type  off_type;

    typedef stdio_filebuf<char_type, traits_type> buf_type;
    
    explicit stdio_ofdstream(int fd,ios_base::openmode mode = ios_base::out|ios_base::trunc,bool freeFd=true,int buffSize = 4096)
      : basic_ostream<char_type, traits_type>(0),
	filebuf(fd,mode,buffSize)
    {
      this->init(&filebuf);
      RavlAssert(freeFd); // Not given a choice anymore
    }
    //: Construct from a file handle.
    
    stdio_ofdstream()
      : basic_ostream<char_type, traits_type>(0), 
	filebuf()
    { }
    //: Default constructor.
    
    buf_type* rdbuf() const
    { return const_cast<buf_type *>(&filebuf); }
    //: Access fd buffer.
    
  private:
    buf_type filebuf;
  };
  
  
  //! userlevel=Develop
  //: Basic fdstream.
  
  template<typename CharT, typename TraitsT>
  class stdio_fdstream 
    : public basic_iostream<CharT, TraitsT>
  {
  public:
    typedef CharT 		           char_type;
    typedef TraitsT 		           traits_type;
    typedef typename traits_type::int_type int_type;
    typedef typename traits_type::pos_type pos_type;
    typedef typename traits_type::off_type off_type;
    
    typedef stdio_filebuf<char_type, traits_type> buf_type;
    
    explicit stdio_fdstream(int fd,ios_base::openmode mode = ios_base::in | ios_base::out,bool freeFd = true,int buffSize = 4096)
      : basic_iostream<char_type, traits_type>(0), 
	filebuf(fd,mode,freeFd,buffSize)
    { }
    //: Cosntructor.
    
    stdio_fdstream()
      : basic_iostream<CharT, TraitsT>(0),
	filebuf()
    { }
    //: Default constructor.
    
    buf_type* rdbuf() const
    { return const_cast<buf_type *>(&filebuf); }
    
  private:
    buf_type  filebuf;
  };
#endif
  
  typedef stdio_ifdstream<char> stdifdstream;
  typedef stdio_ofdstream<char> stdofdstream;

}
#endif
#endif
