// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DPBYTEFILEIO_HEADER
#define RAVL_DPBYTEFILEIO_HEADER 1
//////////////////////////////////////////////////////////
//! docentry="Ravl.API.Core.Data Processing.IO"
//! example=exDataProc.cc
//! file="Ravl/Core/IO/ByteFileIO.hh"
//! lib=RavlIO
//! author="Charles Galambos"
//! date="04/07/1998"
//! userlevel=Default

#include "Ravl/DP/SPort.hh"
#include "Ravl/String.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/Stream.hh"
#include "Ravl/DP/SByteStream.hh"

namespace RavlN {
  ////////////////////////////////////////////
  //! userlevel=Develop
  //: Save fixed length objects to a file.
  // Object must have a stream output function.

  class DPOByteFileBodyC
    : public DPOSByteStreamBodyC
  {
  public:
    DPOByteFileBodyC() {}
    //: Default constructor.

    DPOByteFileBodyC(const StringC &nfname)
      : out(nfname)
    {
#if RAVL_CHECK
      if(!out.good())
	cerr << "DPOByteFileBodyC<ByteT>::DPOByteFileBodyC<ByteT>(StringC,bool), Failed to open file '" << nfname << "'.\n";
#endif
    }
    //: Construct from a filename.

    inline DPOByteFileBodyC(OStreamC &strmout)
      : out(strmout)
    {
#if RAVL_CHECK
      if(!out.good())
	cerr << "DPOByteFileBodyC<ByteT>::DPOByteFileBodyC<ByteT>(OStreamC,bool), Passed bad output stream. \n";
#endif
    }
    //: Stream constructor.

    virtual ~DPOByteFileBodyC();
    //: Dtor.

    virtual bool Put(const ByteT &dat);
    //: Put data.

    virtual IntT PutArray(const SArray1dC<ByteT> &data);
    //: Put an array of data to stream.
    // returns the number of elements processed.

    virtual bool IsPutReady() const;
    //: Is port ready for data ?

    virtual void PutEOS();
    //: Indicate end of stream.

    virtual bool Seek(UIntT offset);
    //: Seek to a location in the stream.
    //!return: True if the seek is successful.

    virtual bool DSeek(IntT offset);
    //: Delta seek: Seek an offset distance relative to the current location.
    //!return: True if the seek is successful.

    virtual UIntT Tell() const;
    //: Find the current location in stream.
    //!return: static_cast<UIntT>(-1) if unsuccessful.

    virtual UIntT Size() const;
    //: Find the size of the stream.
    //!return: static_cast<UIntT>(-1) if unsuccessful.

    virtual UIntT Start() const;
    //: Find the offset where the stream begins.
    //!return: static_cast<UIntT>(-1) if unsuccessful.

    virtual bool Seek64(StreamPosT offset);
    //: Seek to a location in the stream.
    //!return: True if the seek is successful.

    virtual bool DSeek64(StreamPosT offset);
    //: Delta seek: Seek an offset distance relative to the current location.
    //!return: True if the seek is successful.

    virtual StreamPosT Tell64() const;
    //: Find the current location in stream.
    //!return: RavlN::streamPosUnknown if unsuccessful.

    virtual StreamPosT Size64() const;
    //: Find the size of the stream.
    //!return: RavlN::streamPosUnknown if unsuccessful.

    virtual StreamPosT Start64() const;
    //: Find the offset where the stream begins.
    //!return: RavlN::streamPosUnknown if unsuccessful.

    virtual bool Save(std::ostream &sout) const;
    //: Save to std::ostream.

  private:
    OStreamC out;
  };

  /////////////////////////////////////
  //! userlevel=Develop
  //: Load fixed length objects from a file.
  // Object must have a stream input function
  // and a default constructor.

  class DPIByteFileBodyC
    : public DPISByteStreamBodyC
  {
  public:
    DPIByteFileBodyC()
      : dataStart(0),
        off(0),
        size(0)
    {}
    //: Default constructor.

    DPIByteFileBodyC(const StringC &nfname)
      : in(nfname),
        dataStart(0),
        off(0),
        size(0)
    {
#if RAVL_CHECK
      if(!in.good())
	cerr << "DPOByteFileBodyC<ByteT>::DPOByteFileBodyC<ByteT>(StringC,bool), WARNING: Failed to open file '" << nfname << "'.\n";
#endif
      Init();
    }
    //: Construct from a filename.

    inline DPIByteFileBodyC(IStreamC &strmin)
      : in(strmin),
        dataStart(0),
        off(0),
        size(0)
    {
#if RAVL_CHECK
      if(!in.good())
	cerr << "DPIByteFileBodyC<ByteT>::DPIByteFileBodyC<ByteT>(OStreamC,bool), WARNING: Passed bad input stream. \n";
#endif
      Init();
    }
    //: Stream constructor.

    virtual bool IsGetEOS() const;
    //: Is valid data ?

    virtual ByteT Get();
    //: Get next piece of data.

    virtual bool Get(ByteT &buff);
    //: Get next piece of data.

    virtual IntT GetArray(SArray1dC<ByteT> &data);
    //: Get multiple pieces of input data.
    // returns the number of elements processed.

    virtual bool Save(std::ostream &out) const;
    //: Save to std::ostream.

    virtual bool Seek(UIntT newOff);
    //: Seek to position in stream.
    // Currently will only seek to begining of stream.

    virtual UIntT Tell() const;
    //: Get offset in stream.

    virtual UIntT Size() const;
    //: Get size of stream.

    virtual bool Seek64(StreamPosT newOff);
    //: Seek to position in stream.
    // Currently will only seek to begining of stream.

    virtual StreamPosT Tell64() const;
    //: Get offset in stream.

    virtual StreamPosT Size64() const;
    //: Get size of stream.

  private:
    void Init();

    IStreamC in;
    StreamPosT dataStart;
    StreamPosT off;
    StreamPosT size;
  };

  ///////////////////////////////
  //! userlevel=Normal
  //: File output stream for fixed length objects.
  // Object must have a stream output function.

  class DPOByteFileC
    : public DPOSByteStreamC
  {
  public:
    inline DPOByteFileC()
      : DPEntityC(true)
    {}
    //: Default constructor.

    inline DPOByteFileC(OStreamC &strm)
      : DPEntityC(*new DPOByteFileBodyC(strm))
    {}
    //: Stream constructor.

    inline DPOByteFileC(const StringC &fname)
      : DPEntityC(*new DPOByteFileBodyC(fname))
    {}

    //: ByteFilename constructor.
  };

  //////////////////////////////////
  //! userlevel=Normal
  //: Byte file input stream.
  // Object must have a stream input function
  // and a default constructor.

  class DPIByteFileC
    : public DPISByteStreamC
  {
  public:
    inline DPIByteFileC()
      : DPEntityC(true)
    {}
    //: Default constructor.

    inline DPIByteFileC(IStreamC &strm)
      : DPEntityC(*new DPIByteFileBodyC(strm))
    {}
    //: Stream constructor.

    inline DPIByteFileC(const StringC &afname)
      : DPEntityC(*new DPIByteFileBodyC(afname))
    {}
    //: filename constructor.
  };
}

#endif
