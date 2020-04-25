// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2013, React AI Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL) Version 2.1  See the lgpl.licence file
// for details or see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_IO_IPORTIDX_HEADER
#define RAVL_IO_IPORTIDX_HEADER 1

//! docentry = "Ravl.API.Core.IO.IDX Files"
//! author = "Charles Galambos"
//! lib = RavlFileIDX

#include "Ravl/DP/SPort.hh"
#include "Ravl/IO/DataStreamIDX.hh"

namespace RavlN { namespace ION {
  using RavlN::IntT;
  using RavlN::UIntT;
  using RavlN::StreamPosT;

  ///////////////////////////////////
  //: Access a stream of data from IDX file.
  // FIXME:- Detect truncation.
  
  template<typename DataT>
  class DPIPortIDXBodyC
    : public RavlN::DPISPortBodyC<DataT >,
      public DataStreamIDXC
  {
  public:  
    DPIPortIDXBodyC(const RavlN::IStreamC &nStrm)
     : m_at(0)
    {
      RavlIssueError("not implemented");
    }
    //: Constructor from stream 
    
    DPIPortIDXBodyC(const StringC &fn)
     : m_at(0)
    {
      if(!DataStreamIDXC::Open(fn))
        throw RavlN::ExceptionInvalidStreamC("Can't open file.");
    }
    //: Constructor from filename
    
    virtual bool Seek(UIntT off)
    {
      m_at = off;
      return true;
    }
    //: Seek to location in stream.
    
    virtual bool Seek64(StreamPosT off) {
      if(off < 0)
        return false;
      m_at = off;
      return true;
    }

    virtual bool DSeek(IntT off)
    {
      m_at += off;
      return true;
    }
    //: Delta Seek, goto location relative to the current one.

    virtual bool DSeek64(StreamPosT off)
    {
      m_at += off;
      return true;
    }
    //: Delta Seek, goto location relative to the current one.

    virtual UIntT Tell() const
    { return m_at; }
    //: Find current location in stream.

    virtual RavlN::StreamPosT Tell64() const
    { return m_at; }
    //: Find current location in stream.
    
    virtual UIntT Size() const
    {
      return StepCount();
    }
    //: Find the total size of the stream.

    virtual RavlN::StreamPosT Size64() const
    { return StepCount(); }
    //: Find the total size of the stream.

    virtual DataT Get()
    {
      DataT ret;
      if(!Get(ret))
        throw RavlN::ExceptionEndOfStreamC("Failed to read element");
      return ret;
    }
    //: Get next element.
    
    virtual bool Get(DataT &buff)
    { return FetchVector(m_at++,buff); }
    //: Get next element.
    
    virtual bool GetAt(StreamPosT off,DataT &buffer)
    { return FetchVector(off,buffer); }
    //: Fetch vector at a given position

    virtual bool IsGetReady() const 
    { return m_at < (Int64T) StepCount(); }
    //: Is some data ready ?
    // TRUE = yes.
    // Defaults to !IsGetEOS().
    
    virtual bool IsGetEOS() const
    { return m_at < (Int64T) StepCount(); }
    //: Has the End Of Stream been reached ?
    // TRUE = yes.
    
    virtual bool GetAttr(const StringC &attrName,StringC &attrValue)
    { return DataStreamIDXC::GetAttribute(attrName,attrValue); }
    //: Get a attribute.

    virtual bool SetAttr(const StringC &attrName,const StringC &attrValue)
    { return DataStreamIDXC::SetAttribute(attrName,attrValue); }
    //: Set a attribute.

  protected:
    Int64T m_at;
  };
  
  //! userlevel=Normal
  //: Access a stream of data from an IDX file.
  
  template<typename DataT>
  class DPIPortIDXC
    : public RavlN::DPISPortC<DataT >
  {
  public:
    DPIPortIDXC(const StringC &fn)
     : RavlN::DPEntityC(*new DPIPortIDXBodyC<DataT>(fn))
    {}
    //: Constructor from filename.  
  };
  
}}
#endif
