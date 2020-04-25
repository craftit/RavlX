// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2013, React AI Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL) Version 2.1  See the lgpl.licence file
// for details or see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here

#if RAVL_COMPILER_LLVM
#undef _POSIX_C_SOURCE
#endif

#include <fstream>

#include "Ravl/IO/DataStreamIDX.hh"
#include "Ravl/SysLog.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/OS/Filename.hh"
#include <unistd.h>


namespace RavlN { namespace ION {

  //! Default constructor.
  DataStreamIDXC::DataStreamIDXC(bool forWrite)
   : m_dataType(DT_Unknown),
     m_elemSizeBytes(0),
     m_frameSizeBytes(0),
     m_frameSizeElements(0),
     m_forWrite(forWrite),
     m_writeInitDone(false)
  {}

  //! Destructor
  DataStreamIDXC::~DataStreamIDXC()
  {
    if(m_forWrite)
      WriteHeader();
  }

  //! Open a file
  //! Will return false
  bool DataStreamIDXC::Open(const StringC &filename)
  {
    RavlN::IStreamC strm(filename);
    return Open(strm);
  }

  //! Set the element type
  bool DataStreamIDXC::SetDataType(const std::type_info &elemType)
  {
    if(m_writeInitDone) {
      // Check if the types are consistent.
      switch(m_dataType)
      {
        case DT_UByteT:
          if(elemType != typeid(RavlN::ByteT)) {
            RavlError("Type mismatch. ");
            throw RavlN::ExceptionOperationFailedC("Type mismatch");
          }
          break;
        case DT_Float32T:
          if(elemType != typeid(RavlN::FloatT)) {
            RavlError("Type mismatch. ");
            throw RavlN::ExceptionOperationFailedC("Type mismatch");
          }
          break;
        default:
          RavlError("Don't know how to handle element type '%s' ",RavlN::TypeName(elemType.name()));
          return false;
      }
      return true;
    }
    if(elemType == typeid(RavlN::ByteT)) {
      m_dataType = DT_UByteT;
      m_dims = RavlN::SArray1dC<size_t>(1);
    } else if(elemType == typeid(RavlN::TVectorC<float>)) {
      m_dataType = DT_Float32T;
      m_dims = RavlN::SArray1dC<size_t>(2);
    } else {
      RavlError("Don't know how to handle element type '%s' ",RavlN::TypeName(elemType.name()));
      return false;
    }
    m_dims.Fill(0);
    if(!InitElemType()) {
      RavlError("Don't know how to handle element type '%s' ",RavlN::TypeName(elemType.name()));
      return false;
    }
    return false;
  }

  //! Open a file for writing
  //! Will return false
  bool DataStreamIDXC::OpenWrite(const StringC &filename)
  {
    m_ostream = RavlN::BinOStreamC(filename);
    m_ostream.UseBigEndian(true);
    if(!m_ostream.Stream().IsOpen()) {
      RavlError("Failed to open output '%s' ",filename.c_str());
      return false;
    }
    m_forWrite = true;
    return true;
  }

  //! Open a file
  //! Will return false if header doesn't match
  bool DataStreamIDXC::Open(RavlN::IStreamC &strmx)
  {
    m_istream = strmx;
    m_istream.UseBigEndian(true);
    return ReadHeader();
  }


  //! Open a file for reading and writing
  //! Will return true on success
  bool DataStreamIDXC::OpenIO(const StringC &filename)
  {
    RavlN::FilenameC fn(filename);
    bool exists = fn.Exists();

    std::fstream *fstrm = new std::fstream(filename.c_str(),std::ios::out | std::ios::in | std::ios::binary);

    if(fstrm->bad()) {
      RavlError("Failed to open file.");
      return false;
    }

    m_ostream = RavlN::BinOStreamC(RavlN::OStreamC(*fstrm,true));
    m_ostream.UseBigEndian(true);
    m_istream = RavlN::BinIStreamC(RavlN::IStreamC(*fstrm,false));
    m_istream.UseBigEndian(true);
    m_forWrite = true;

    if(exists) {
      if(!ReadHeader()) {
        RavlError("File '%s' exists, but can't read header. ",filename.c_str());
        return false;
      }
      m_writeInitDone = true;
    }

    return true;
  }


  //! Seek to given frame
  bool DataStreamIDXC::SeekToWrite(Int64T frame)
  {
    RavlAssert(m_frameSizeBytes != 0); // Check things have been setup ok.
    std::streampos at = 4 + m_frameSizeBytes * frame  + m_dims.Size() * 4;
    if(frame < 0) {
      RavlError("Seek to negative frame for write.");
      return false;
    }
    if((Int64T) m_dims[0] < frame)
      m_dims[0] = frame;
    m_ostream.Seek(at);
    return true;
  }

  //! Seek to given frame
  bool DataStreamIDXC::SeekToRead(Int64T frame)
  {
    RavlAssert(m_frameSizeBytes != 0); // Check things have been setup ok.
    std::streampos at = 4 + m_frameSizeBytes * frame  + m_dims.Size() * 4;
    if(frame < 0 || frame >= (Int64T) m_dims[0]) {
      RavlDebug("Asked for out of range frame.");
      return false;
    }
    m_istream.Seek(at);
    return true;
  }

  //! Read header from file
  bool DataStreamIDXC::ReadHeader()
  {
    char header[4];
    m_istream.IBuff(header,4);
    if(m_istream.Stream().gcount() != 4) {
      RavlDebug("File truncated.");
      return false;
    }
    if(header[0] != 0 || header[1] != 0)
      return false;
    m_dataType = (DataTypeT) header[2];
    if(!InitElemType())
      return false;
    //RavlDebug("Type:%x ",(int) m_dataType);

    unsigned dims = header[3];
    if(dims < 1)
      return false;

    m_dims = RavlN::SArray1dC<size_t>(dims);

    //bstrm.Set

    m_frameSizeBytes = m_elemSizeBytes;
    m_frameSizeElements = 1;
    for(unsigned i = 0;i < dims;i++) {
      UInt32T size;
      m_istream >> size;
      m_dims[i] = size;
      if(i > 0) {
        m_frameSizeBytes *= size;
        m_frameSizeElements *= size;
      }
      //RavlDebug("Size:%d ",(int) size);
    }


    return true;
  }

  bool DataStreamIDXC::InitElemType()
  {
    switch(m_dataType)
    {
      case DT_UByteT: m_elemSizeBytes  = 1; break;
      case DT_SByteT: m_elemSizeBytes  = 1; break;
      case DT_ShortT: m_elemSizeBytes  = 2; break;
      case DT_Int32T: m_elemSizeBytes  = 4; break;
      case DT_Float32T: m_elemSizeBytes  = 4; break;
      case DT_Float64T: m_elemSizeBytes  = 8; break;
        break;
      case DT_Unknown:
      default:
        m_dataType = DT_Unknown;
        return false;
    }
    return true;
  }

  //! Write header to file
  void DataStreamIDXC::WriteHeader()
  {
    ByteT header[4];
    header[0] = 0;
    header[1] = 0;
    header[2] = (ByteT) m_dataType;
    header[3] = (ByteT) m_dims.Size();
    m_ostream.Seek(0);
    m_ostream.OBuff((char *)header,4);
    for(unsigned i = 0;i < m_dims.Size();i++) {
      RavlN::Int32T theSize = m_dims[i];
      m_ostream << theSize;
    }
  }

  //! Fetch a given set of data as a vector
  bool DataStreamIDXC::FetchVector(Int64T frame,RavlN::SArray1dC<float> &vec)
  {
    if(!SeekToRead(frame))
      return false;

    if(vec.Size() != m_frameSizeElements)
      vec = RavlN::SArray1dC<float>(m_frameSizeElements);

    switch(m_dataType) {
      case DT_UByteT: {
        if(m_buff.Size() != m_frameSizeBytes)
          m_buff = RavlN::SArray1dC<char>(m_frameSizeBytes);
        m_istream.IBuff(m_buff.ReferenceElm(),m_frameSizeBytes);
        if(m_istream.Stream().gcount() != m_frameSizeBytes) {
          RavlError("Read failed");
          return false;
        }
        RavlAssert(m_frameSizeElements == m_frameSizeBytes);
        for(unsigned i = 0;i < m_frameSizeElements;i++) {
          vec[i] = (float) static_cast<RavlN::UByteT>(m_buff[i]) / 255.0;
        }
      } break;
      case DT_SByteT: {
        if(m_buff.Size() != m_frameSizeBytes)
          m_buff = RavlN::SArray1dC<char>(m_frameSizeBytes);
        m_istream.IBuff(m_buff.ReferenceElm(),m_frameSizeBytes);
        if(m_istream.Stream().gcount() != m_frameSizeBytes) {
          RavlError("Read failed");
          return false;
        }
        RavlAssert(m_frameSizeElements == m_frameSizeBytes);
        for(unsigned i = 0;i < m_frameSizeElements;i++) {
          vec[i] = (float) static_cast<RavlN::SByteT>(m_buff[i]) / 255.0;
        }
      } break;
      case DT_Float32T: {
        for(unsigned i = 0;i < m_frameSizeElements;i++)
          m_istream >> vec[i];
      } break;
      default:
        RavlError("Don't know how to deal with data type.");
        return false;
    }

    return true;
  }

  //! Fetch a given set of data
  bool DataStreamIDXC::FetchVector(Int64T frame,ByteT &vec)
  {
    if(!SeekToRead(frame))
      return false;

    std::streampos at = 4 + m_frameSizeBytes * frame + m_dims.Size() * 4;
    m_istream.Seek(at);
    switch(m_dataType) {
      case DT_UByteT: {
        RavlN::UByteT val;
        m_istream.IBuff((char *)&val,1);
        if(m_istream.Stream().gcount() != 1) {
          RavlError("Read failed. ");
        }
        vec = val;
      } break;
      case DT_SByteT: {
        RavlN::UByteT val;
        m_istream.IBuff((char *)&val,1);
        if(m_istream.Stream().gcount() != 1) {
          RavlError("Read failed. ");
        }
        vec = val;
      } break;
      default:
        RavlError("Don't know how to deal with data type.");
        return false;
    }

    return true;
  }

  //! Write a given set of data as a vector
  bool DataStreamIDXC::WriteVector(Int64T frame,const RavlN::SArray1dC<float> &vec)
  {
    RavlAssert(m_dataType == DT_Float32T);
    if(!m_writeInitDone) {
      m_dims[1] = vec.Size();
      m_frameSizeElements = vec.Size();
      m_frameSizeBytes = m_frameSizeElements * m_elemSizeBytes;
      m_writeInitDone = true;
    }
    if(!SeekToWrite(frame))
      return false;
    for(unsigned i = 0;i < vec.Size();i++)
      m_ostream << vec[i];
    return true;
  }

  //! Write a given set of data
  bool DataStreamIDXC::WriteVector(Int64T frame,ByteT vec)
  {
    if(!m_writeInitDone) {
      //m_dims[1] = 1;
      m_frameSizeElements = 1;
      m_frameSizeBytes = m_frameSizeElements * m_elemSizeBytes;
      m_writeInitDone = true;
    }
    RavlAssert(m_dataType == DT_UByteT);
    if(!SeekToWrite(frame))
      return false;
    m_ostream << vec;
    return true;
  }

  //! Get attribute if available.
  bool DataStreamIDXC::GetAttribute(const StringC &attrName,StringC &value) const
  {
    return false;
  }

  //! Set attribute
  bool DataStreamIDXC::SetAttribute(const StringC &attrName,const StringC &value)
  {
    return false;
  }

}}
