// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2008, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_FUNCSTREAM_HEADER
#define RAVL_FUNCSTREAM_HEADER 1
//! file="Ravl/Core/Base/FuncStream.hh"
//! lib=RavlCore
//! date="29/05/2008"
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.IO.Streams"
//! userlevel=Normal

#include "Ravl/Stream.hh"
#include "Ravl/Calls.hh"

namespace RavlN {
  
  //: Write stream to a method.
  
  class FuncOStreamBufC 
    : public std::streambuf
  {
  public:
    typedef traits_type::int_type int_type;
    
    //: Constructor
    FuncOStreamBufC(const CallFunc2C<const char *,SizeT> &writeCall, SizeT bufferSize = 8192);

    //: Destructor.
    virtual ~FuncOStreamBufC();

    //: Buffer overflow
    virtual int_type overflow(int_type c);
    
    //: Sync stream.
    virtual int sync();
  protected:    
    void Flush();
    
    char *m_buffer;
    SizeT m_bufferSize;
    CallFunc2C<const char *,SizeT> m_write; // Method to call to write data.
  };
  
  
  //: Read stream from a method.
  
  class FuncIStreamBufC 
    : public std::streambuf
  {
  public:
    typedef traits_type::int_type int_type;
    
    //: Constructor.
    //: The read function should be of the form.

    FuncIStreamBufC(const CallFunc2C<char *,SizeT,SizeT> &readCall, SizeT bufferSize = 8192);
    
    //: Destructor.
    virtual ~FuncIStreamBufC();

    //: Underflow.
    virtual int_type underflow();
    
  protected:
    char *m_buffer;
    SizeT m_bufferSize;
    CallFunc2C<char *,SizeT,SizeT> m_read;
  };
  
  //: function based std::ostream
  
  class funcostream
    : public std::ostream 
  {
  public:
    funcostream(const CallFunc2C<const char *,SizeT> &writeCall, SizeT bufferSize = 8192)
      : std::ostream(0),
        m_streamBuf(writeCall, bufferSize)
    {
      rdbuf(&m_streamBuf);
    }
    
  protected:
    FuncOStreamBufC m_streamBuf;
  };
  
  //: function based std::ostream
  
  class funcistream
    : public std::istream 
  {
  public:
    funcistream(const CallFunc2C<char *,SizeT,SizeT> &readCall, SizeT bufferSize = 8192)
      : std::istream(0),
        m_streamBuf(readCall, bufferSize)
    {
      rdbuf(&m_streamBuf);
    }
    
  protected:
    FuncIStreamBufC m_streamBuf;
  };
}

#endif
