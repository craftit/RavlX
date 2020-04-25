// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_FDSTREAMBUF_HEADER
#define RAVL_FDSTREAMBUF_HEADER 1
////////////////////////////////////////////////////////////////////////
//! author="Charles Galambos"
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Base/fdstreambuf.hh"

#include "Ravl/config.h"
#include "Ravl/Assert.hh"

#if (RAVL_COMPILER_GCC3 && !RAVL_COMPILER_GCC3_4 && !RAVL_COMPILER_GCC4) || RAVL_COMPILER_LLVM

#include <streambuf>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#ifndef BUFSIZ
#define BUFSIZ 4096
#endif

namespace RavlN {
  using namespace std;
  template<typename CharT, typename TraitsT>
  class basic_fdbuf 
    : public std::basic_streambuf<CharT, TraitsT>
  {
  public:
    typedef CharT                     	        char_type;
    typedef TraitsT                    	        traits_type;
    typedef typename traits_type::int_type 	int_type;
    typedef typename traits_type::pos_type 	pos_type;
    typedef typename traits_type::off_type 	off_type;
    
    basic_fdbuf()
      : fd(-1),
        m_buffer(0),
        m_bufferSize(0),
        m_reqBufferSize(static_cast<size_t>(BUFSIZ))
    {}
    //: Constructor.
    
    basic_fdbuf(int nfd,ios_base::openmode mode,size_t reqBuffSize = static_cast<size_t>(BUFSIZ))
      : fd(-1),
        m_buffer(0),
        m_bufferSize(0),
        m_reqBufferSize(reqBuffSize)
    {
      if ((mode & ios_base::in) && m_reqBufferSize <= 5) {
        m_reqBufferSize = 5;
      }
      fdbuf_init(nfd,mode);
    }
    //: Constructor.
    
    virtual ~basic_fdbuf() 
    { close(); }

    bool is_open() const { return fd >= 0; }
    //: Is file handle open ?
    
    basic_fdbuf<CharT,TraitsT> *open(int nfd, ios_base::openmode mode) {
      fdbuf_init(fd,mode);
      return this;
    }
    //: Open buffer.
    
    basic_fdbuf<CharT,TraitsT> *close() {
      ::close(fd);
      if(m_buffer) {
        this->setg(0, 0, 0);
	this->setp(0, 0);
	delete [] m_buffer;
	m_bufferSize = 0;
      }
      fd = -1;
      return this;
    }
    //: Close buffer.

    void setbuf();
  private:
    int fd;
    char_type *m_buffer;
    size_t m_bufferSize;
    size_t m_reqBufferSize; // Requested buffer size
    
  protected:
    void fdbuf_init(int nfd,ios_base::openmode mode) {
      fd = nfd;
      m_bufferSize = m_reqBufferSize;
      if(m_bufferSize <= 0)
        return ;
      m_buffer = new char_type[m_bufferSize];
      if (mode & ios_base::in) {
        this->setg(m_buffer+4,m_buffer+4,m_buffer+4);
      }
      if (mode & ios_base::out) {
	this->setp(m_buffer,m_buffer + m_bufferSize);
      }
      // Don't support input and output from the same buffer...
      RavlAssert(!((mode & ios_base::out) && (mode & ios_base::in )));
    }
    //: Setup buffer.
    
    virtual int sync() {
      //std::cerr << "basic_fdbuf::sync() Called. Ptrs:" << (void *) this->pbase() << " " << (void *) this->pptr() << " " << (void *) this->epptr() << "\n";
      char *at = (char *) this->pbase();
      while(at < this->pptr()) {
	int blen = ((char *)this->pptr()) - at;
	int n = write(fd,at,blen);
	if(n == 0) {
	  //std::cerr << "Write of zero bytes. \n";
	}
	if(n < 0) {
	  if(errno == EINTR || errno == EAGAIN)
	    continue;
	  //std::cerr << "Write failed. \n";
	  //setstate(ios_base::badbit);
	  return -1;
	}
	at += n;
      }
      this->setp(m_buffer,m_buffer + m_bufferSize);
      return 0;
    }
    //: Flush buffer to output.

    virtual int_type overflow(int_type c = traits_type::eof()) {
      //cerr << "basic_fdbuf::overflow() Called. Ptrs:" << (void *) pbase() << " " << (void *) pptr() << " " << (void *) epptr() << "\n";
      sync();
      if(c != traits_type::eof()) {
        if(this->pptr() == 0) {
          char_type buff = c;
          int n = 0;
          char *at = &buff;
          do {
            n = write(fd,at,((char *) ((&buff)+1)) - ((char *)at));
            //std::cerr << "Wrote: " << n << " of " << << "\n";
            if(n < 0) {
              if(errno == EAGAIN || errno == EINTR)
                continue;
              return traits_type::eof();
            }
            at += n;
          } while(at < ((&buff) + 1));
          return c;
        }
        *(this->pptr()) = c;
        this->pbump(1);
      }
      return c;
    }
    //: Overflow buffer on write.


    virtual int_type underflow() {
     // std::cerr << "basic_fdbuf::underflow() Called. BufferSize:" << m_bufferSize << " Ptrs:" << (void *) m_buffer << " " << (void *) this->gptr() << " " << (void *) this->egptr() << "\n";
      if(this->gptr() < this->egptr()) {
        //std::cerr << "Got data.... \n";
        return traits_type::to_int_type(*this->gptr());
      }
      RavlAssert(m_buffer != 0);
      int nPutback;
      nPutback = this->gptr() - this->eback();
      if(nPutback > 4)
        nPutback = 4;
      memmove(m_buffer+(4-nPutback),this->gptr() - nPutback,nPutback);
      CharT *buf = m_buffer+4;
      int rnum = m_bufferSize-4;

      //std::cerr << "Attempting to read " << rnum << " chars. \n";
      int num = 0;
      do {
        num = read(fd,buf,rnum * sizeof(CharT));
        if(num > 0)
          break;
        if(num <= 0) {
          if(errno  == EINTR || errno == EAGAIN)
            continue;
          //std::cerr << "Error reading chars.. " << errno << " \n";
          return traits_type::eof();
        }
      } while(true);
      //std::cerr << "Read " << num << " chars. \n";
      this->setg(m_buffer+(4-nPutback),m_buffer+4,m_buffer+4+num);
      return traits_type::to_int_type(*this->gptr());
    }
    //: Underflow the buffer on read.

    virtual int_type pbackfail(int_type c = traits_type::eof()) {
      std::cerr << "basic_fdbuf::pbackfail() Called. \n";
      RavlAssert(0);
      return c;
    }
    //: Putback failed.
    
    virtual basic_streambuf<char_type, traits_type> *setbuf(char_type* s, streamsize n) {
      std::cerr << "basic_fdbuf::setbuf() Called. \n";
      RavlAssert(0);
      return this;
    }
    //: Set the buffer to use.
    
    virtual pos_type seekoff(off_type off,ios_base::seekdir way,ios_base::openmode mode = ios_base::in | ios_base::out) {
      std::cerr << "basic_fdbuf::seekoff() Called. \n";
      RavlAssert(0);
      return 0;
    }
    //: Seek relative.
    
    virtual pos_type seekpos(pos_type sp,ios_base::openmode mode = ios_base::in | ios_base::out) {
      std::cerr << "basic_fdbuf::seekpos() Called. \n";
      RavlAssert(0);
      return 0;
    }
    //: Seek absolute position.

  protected:

  };
  
}

#endif
#endif
