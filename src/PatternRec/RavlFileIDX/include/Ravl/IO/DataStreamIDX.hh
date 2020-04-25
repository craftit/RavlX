// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2013, React AI Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL) Version 2.1  See the lgpl.licence file
// for details or see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DATASTREAMIDX_HEADER
#define RAVL_DATASTREAMIDX_HEADER 1

//! docentry = "Ravl.API.Core.IO.IDX Files"
//! author = "Charles Galambos"
//! lib = RavlFileIDX

#include "Ravl/String.hh"
#include "Ravl/SArray1d.hh"
#include "Ravl/TVector.hh"
#include "Ravl/Stream.hh"
#include "Ravl/BinStream.hh"

namespace RavlN { namespace ION {

  using RavlN::StringC;
  using RavlN::UInt32T;
  using RavlN::Int64T;
  using RavlN::IntT;
  using RavlN::ByteT;
  using RavlN::TVectorC;

  //! IDX file handling.

  class DataStreamIDXC
  {
  public:
    //! Default constructor.
    DataStreamIDXC(bool forWrite = false);

    //! Destructor
    ~DataStreamIDXC();

    //! Open a file for reading
    //! Will return false
    bool Open(const StringC &filename);

    //! Open a stream for reading
    //! Will return false if header doesn't match
    bool Open(RavlN::IStreamC &strm);

    //! Set the element type
    bool SetDataType(const std::type_info &elemType);

    //! Open a file for writing
    //! Will return true on success.
    bool OpenWrite(const StringC &filename);

    //! Open a file for reading and writing
    //! Will return true on success
    bool OpenIO(const StringC &filename);

    //! Data types held in the file.
    enum DataTypeT {
      DT_Unknown = 0x00,
      DT_UByteT = 0x08,
      DT_SByteT = 0x09,
      DT_ShortT = 0x0B,
      DT_Int32T = 0x0C,
      DT_Float32T = 0x0D,
      DT_Float64T = 0x0E
    };

    //!
    Int64T StepCount() const
    { return m_dims[0]; }

    //! Access data type
    DataTypeT DataType() const
    { return m_dataType; }

    //! Fetch a given set of data as a vector
    bool FetchVector(Int64T frame,RavlN::SArray1dC<float> &vec);

    //! Fetch a given set of data
    bool FetchVector(Int64T frame,ByteT &vec);

    //! Write a given set of data as a vector
    bool WriteVector(Int64T frame,const RavlN::SArray1dC<float> &vec);

    //! Write a given set of data
    bool WriteVector(Int64T frame,ByteT vec);

    //! Get attribute if available.
    bool GetAttribute(const StringC &attrName,StringC &value) const;

    //! Set attribute
    bool SetAttribute(const StringC &attrName,const StringC &value);

    //! Access dimensions of data.
    const RavlN::SArray1dC<size_t> &Dims() const
    { return m_dims; }

    //! Access location of next frame.
    size_t NextFrame() const
    { return m_dims[0]; }

  protected:
    //! Initialise for  element type
    bool InitElemType();

    //! Seek to given frame
    bool SeekToWrite(Int64T frame);

    //! Seek to given frame
    bool SeekToRead(Int64T frame);

    //! Write header to file
    void WriteHeader();

    //! Read header from file
    bool ReadHeader();

    RavlN::BinOStreamC m_ostream;
    RavlN::BinIStreamC m_istream;
    DataTypeT m_dataType;
    RavlN::SArray1dC<size_t> m_dims;
    Int64T m_elemSizeBytes;  //!< Element size in bytes
    Int64T m_frameSizeBytes;
    Int64T m_frameSizeElements;
    RavlN::SArray1dC<char> m_buff;
    bool m_forWrite;
    bool m_writeInitDone;

  };

}}

#endif
