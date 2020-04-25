// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_FDSTREAM_HEADER
#define RAVL_FDSTREAM_HEADER 1
/////////////////////////////////////////////////////////////////
//! author="Charles Galambos"
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Base/fdstream.hh"
//! docentry="Ravl.API.Core.IO.Streams"

#include "Ravl/config.h"

#if RAVL_COMPILER_GCC3 || RAVL_COMPILER_GCC4

#include "Ravl/Stream.hh"

#if !RAVL_COMPILER_GCC3_4 && !RAVL_COMPILER_GCC4 || RAVL_COMPILER_LLVM

#include "Ravl/fdstreambuf.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Basic input from a fd stream.
  
  template<typename CharT, typename TraitsT = char_traits<CharT> >
  class basic_ifdstream 
    : public basic_istream<CharT, TraitsT>
  {
  public:
    typedef CharT 			    char_type;
    typedef TraitsT 			    traits_type;
    typedef typename traits_type::int_type  int_type;
    typedef typename traits_type::pos_type  pos_type;
    typedef typename traits_type::off_type  off_type;
    
    typedef RavlN::basic_fdbuf <char_type, traits_type>  buf_type;
    
    explicit basic_ifdstream(int fd, ios_base::openmode mode = ios_base::in,
                             size_t buffSize = static_cast<size_t>(BUFSIZ))
      : basic_istream<char_type, traits_type>(0),
	filebuf(fd,mode,buffSize)
    {
      this->init(&filebuf);
    }
    //: Construct from a file handle and an open mode.
    
    buf_type* rdbuf() const
    { return const_cast<buf_type *>(&filebuf); }
    //: Access fdbuffer.
    
  private:
    buf_type filebuf;
  };
  
  //! userlevel=Develop
  //: Derivation of general output streams, specific to unix file handles.
  
  template<typename CharT, typename TraitsT = char_traits<CharT> >
  class basic_ofdstream 
    : public basic_ostream<CharT,TraitsT>
  {
  public:
    typedef CharT 			    char_type;
    typedef TraitsT 			    traits_type;
    typedef typename traits_type::int_type  int_type;
    typedef typename traits_type::pos_type  pos_type;
    typedef typename traits_type::off_type  off_type;

    typedef basic_fdbuf <char_type, traits_type>  buf_type;
    
    explicit basic_ofdstream(int fd,
                             ios_base::openmode mode = ios_base::out|ios_base::trunc,
                             size_t buffSize = static_cast<size_t>(BUFSIZ))
      : basic_ostream<char_type, traits_type>(0),
	filebuf(fd,mode,buffSize)
    { this->init(&filebuf); }
    //: Construct from a file handle.
    
  private:
    buf_type filebuf;
  };
  
  //! userlevel=Develop
  //: Basic fdstream.
  
  template<typename CharT, typename TraitsT>
  class basic_fdstream 
    : public basic_iostream<CharT, TraitsT>
  {
  public:
    typedef CharT 		           char_type;
    typedef TraitsT 		           traits_type;
    typedef typename traits_type::int_type int_type;
    typedef typename traits_type::pos_type pos_type;
    typedef typename traits_type::off_type off_type;
    
    typedef basic_fdbuf <char_type, traits_type>  buf_type;
    
    explicit basic_fdstream(int fd,
                            ios_base::openmode mode = ios_base::in | ios_base::out,
                            size_t buffSize = static_cast<size_t>(BUFSIZ))
      : basic_iostream<char_type, traits_type>(NULL), 
	filebuf(fd,mode,buffSize)
    { this->init(&filebuf); }
    //: Constructor.
    
  private:
    buf_type  filebuf;
  };
  
  typedef basic_ifdstream<char> ifdstream;
  typedef basic_ofdstream<char> ofdstream;

}
#else

#include <ext/stdio_filebuf.h>
#include <iosfwd>

namespace RavlN {
  using namespace __gnu_cxx;

  //! userlevel=Develop
  //: Basic input from a fd stream.
  
  template<typename CharT, typename TraitsT = std::char_traits<CharT> >
  class basic_ifdstream 
    : public basic_istream<CharT, TraitsT>
  {
  public:
    typedef CharT 			    char_type;
    typedef TraitsT 			    traits_type;
    typedef typename traits_type::int_type  int_type;
    typedef typename traits_type::pos_type  pos_type;
    typedef typename traits_type::off_type  off_type;
    
    typedef  stdio_filebuf  <char_type, traits_type>  buf_type;
    
    explicit basic_ifdstream(int fd,
                             ios_base::openmode mode = ios_base::in,
                             size_t buffSize = static_cast<size_t>(BUFSIZ))
      : basic_istream<char_type, traits_type>(0),
	filebuf(fd,mode,buffSize)
    {
      this->init(&filebuf);
    }
    //: Construct from a file handle and an open mode.
    
    buf_type* rdbuf() const
    { return const_cast<buf_type *>(&filebuf); }
    //: Access fdbuffer.
    
  private:
    buf_type filebuf;
  };
  
  //! userlevel=Develop
  //: Derivation of general output streams, specific to unix file handles.
  
  template<typename CharT, typename TraitsT = std::char_traits<CharT> >
  class basic_ofdstream 
    : public basic_ostream<CharT,TraitsT>
  {
  public:
    typedef CharT 			    char_type;
    typedef TraitsT 			    traits_type;
    typedef typename traits_type::int_type  int_type;
    typedef typename traits_type::pos_type  pos_type;
    typedef typename traits_type::off_type  off_type;

    typedef stdio_filebuf <char_type, traits_type>  buf_type;
    
    explicit basic_ofdstream(int fd,
                             ios_base::openmode mode = ios_base::out|ios_base::trunc,
                             size_t buffSize = static_cast<size_t>(BUFSIZ)
                             )
      : basic_ostream<char_type, traits_type>(0),
	filebuf(fd,mode,buffSize)
    { this->init(&filebuf); }
    //: Construct from a file handle.
    
  private:
    buf_type filebuf;
  };
  
  
  //! userlevel=Develop
  //: Basic fdstream.
  
  template<typename CharT, typename TraitsT>
  class basic_fdstream 
    : public basic_iostream<CharT, TraitsT>
  {
  public:
    typedef CharT 		           char_type;
    typedef TraitsT 		           traits_type;
    typedef typename traits_type::int_type int_type;
    typedef typename traits_type::pos_type pos_type;
    typedef typename traits_type::off_type off_type;
    
    typedef stdio_filebuf <char_type, traits_type>  buf_type;
    
    explicit basic_fdstream(int fd,
                            ios_base::openmode mode = ios_base::in | ios_base::out,
                            size_t buffSize = static_cast<size_t>(BUFSIZ))
      : basic_iostream<char_type, traits_type>(NULL), 
	filebuf(fd,mode, buffSize)
    { this->init(&filebuf); }
    //: Constructor.
    
  private:
    buf_type  filebuf;
  };
  
  typedef basic_ifdstream<char> ifdstream;
  typedef basic_ofdstream<char> ofdstream;

}

#endif
#endif
#endif
