// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2013, React AI Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL) Version 2.1  See the lgpl.licence file
// for details or see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_PORTIDX_OPORTPORTIDX_HEADER
#define RAVL_PORTIDX_OPORTPORTIDX_HEADER 1

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
  //: Write a stream of data to an IDX file.
  // FIXME:- Detect truncation.
  
  template<typename DataT>
  class DPOPortIDXBodyC
    : public RavlN::DPOSPortBodyC<DataT >,
      public DataStreamIDXC
  {
  public:  
    DPOPortIDXBodyC(const RavlN::IStreamC &nStrm)
     : DataStreamIDXC(true),
       m_at(0)
    {
      SetDataType(typeid(DataT));
      RavlIssueError("not implemented");
    }
    //: Constructor from stream 
    
    DPOPortIDXBodyC(const StringC &fn)
     : DataStreamIDXC(true),
       m_at(0)
    {
      SetDataType(typeid(DataT));
      if(!DataStreamIDXC::OpenWrite(fn))
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
    
    virtual bool Put(const DataT &buff)
    { return WriteVector(m_at++,buff); }
    //: Get next element.

    virtual bool PutAt(StreamPosT off,const DataT &buffer)
    { return WriteVector((Int64T) off,buffer); }
    //: Write vector

    virtual bool GetAttr(const StringC &attrName,StringC &attrValue)
    {
      if(DataStreamIDXC::GetAttribute(attrName,attrValue))
        return true;
      return RavlN::DPOSPortBodyC<DataT>::GetAttr(attrName,attrValue);
    }
    //: Get a attribute.

    virtual bool SetAttr(const StringC &attrName,const StringC &attrValue)
    {
      if(DataStreamIDXC::SetAttribute(attrName,attrValue))
        return true;
      return RavlN::DPOSPortBodyC<DataT>::SetAttr(attrName,attrValue);
    }
    //: Set a attribute.

  protected:
    Int64T m_at;
  };
  
  //! userlevel=Normal
  //: Write a stream of data to an IDX file.
  
  template<typename DataT>
  class DPOPortIDXC
    : public RavlN::DPOSPortC<DataT >
  {
  public:
    DPOPortIDXC(const StringC &fn)
     : RavlN::DPEntityC(*new DPOPortIDXBodyC<DataT>(fn))
    {}
    //: Constructor from filename.  
  };
  
}}
#endif
