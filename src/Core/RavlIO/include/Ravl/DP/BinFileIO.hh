// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DPBINIO_HEADER
#define RAVL_DPBINIO_HEADER 1
//////////////////////////////////////////////////////////
//! docentry="Ravl.API.Core.Data Processing.IO"
//! example=exDataProc.cc
//! lib=RavlIO
//! author="Charles Galambos"
//! date="04/07/1998"
//! file="Ravl/Core/IO/BinFileIO.hh"
//! userlevel=Default

#include "Ravl/DP/SPort.hh"
#include "Ravl/String.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/DP/AttributeValueTypes.hh"

namespace RavlN {
  static const UInt16T RAVLBinaryID = 0x4143; // AC Ravl Binary file id.
  static const UInt16T RAVLInvBinaryID = 0x4341; // CA Swapped endian id.

  static const UInt16T RAVLBinaryID64 = 0x5238; // R8 Ravl Binary file id for 64 bit files.
  static const UInt16T RAVLBinaryID32 = 0x5234; // R4 Ravl Binary file id for 32 bit files.

  static const UInt16T RAVLInvBinaryID64 = 0x3852; // 8R Ravl Binary file id for 64 bit files endian swapped.
  static const UInt16T RAVLInvBinaryID32 = 0x3452; // 4R Ravl Binary file id for 32 bit files endian swapped.

  /////////////////////////////////////////////
  //! userlevel=Develop
  //: Save objects in binary to a file.
  // Object must have a binary stream output function.

  template<class DataT>
  class DPOBinFileBodyC
    : public DPOPortBodyC<DataT>
  {
  public:
    DPOBinFileBodyC()
     : version(0),
       m_flushAfterWrite(false)
    {
      RegisterAttributes();
    }
    //: Default constructor.

    DPOBinFileBodyC(const StringC &nfname,bool useHeader=false)
      : out(nfname),
        version(0),
        m_flushAfterWrite(false)
    {
      RegisterAttributes();
#ifdef RAVL_CHECK
      if(!out.Stream().good())
	cerr << "DPOBinFileBodyC<DataT>::DPOBinFileBodyC<DataT>(StringC,bool), Failed to open file.\n";
#endif
      if(useHeader) {
        out.UseNativeEndian(true);
#if RAVL_CPUTYPE_32
        out.SetCompatibilityMode32Bit(true);
        out << RavlN::RAVLBinaryID32;
#else
        out.SetCompatibilityMode32Bit(false);
        out << RavlN::RAVLBinaryID64;
#endif
	out << TypeName(typeid(DataT)) << version;
      }
    }
    //: Construct from a filename.

    inline DPOBinFileBodyC(BinOStreamC &strmout,bool useHeader=false)
      : out(strmout),
	version(0),
	m_flushAfterWrite(false)
    {
      RegisterAttributes();
#ifdef RAVL_CHECK
      if(!out.Stream().good())
	cerr << "DPOBinFileBodyC<DataT>::DPOBinFileBodyC<DataT>(OStreamC,bool), Passed bad output stream. \n";
#endif
      if(useHeader) {
        out.UseNativeEndian(true);
#if RAVL_CPUTYPE_32
        out.SetCompatibilityMode32Bit(true);
	out << RavlN::RAVLBinaryID32;
#else
        out.SetCompatibilityMode32Bit(false);
        out << RavlN::RAVLBinaryID64;
#endif
	out << TypeName(typeid(DataT)) << version;
#ifdef RAVL_CHECK
	if(!out.Stream().good())
	  cerr << "DPOBinFileBodyC<DataT>::DPOBinFileBodyC<DataT>(BinOStreamC,bool), Bad stream after writting header! \n";
#endif
      }
    }
    //: Stream constructor.

    virtual bool Put(const DataT &dat) {
#ifdef RAVL_CHECK
      if(!out.Stream().good()) {
	cerr << "DPOBinFileBodyC<DataT>::Put(), Failed because of bad output stream (before write!). \n";
	return false;
      }
#endif
      out << dat;
      if(m_flushAfterWrite)
        out.Stream().os() << flush;
#ifdef RAVL_CHECK
      if(!out.Stream().good())
	cerr << "DPOBinFileBodyC<DataT>::Put(), Failed because of bad output stream. \n";
#endif
      return out.Stream().good();
    }
    //: Put data.

    virtual IntT PutArray(const SArray1dC<DataT> &data) {
      if(!out.Stream().good())
	return data.Size();
      for(SArray1dIterC<DataT> it(data);it;it++) {
	out << *it;
	if(!out.Stream().good()) {
#ifdef RAVL_CHECK
	  cerr << "DPOBinFileBodyC<DataT>::PutArray(), Endded early because of bad output stream. \n";
#endif
	  return it.Index().V();
	}
      }
      if(m_flushAfterWrite)
        out.Stream().os() << flush;
      return data.Size();
    }
    //: Put an array of data to stream.
    // returns the number of elements processed.

    virtual bool IsPutReady() const
    { return out.Stream().good(); }
    //: Is port ready for data ?

    virtual bool Save(std::ostream &sout) const
    { sout << out.Name(); return true; }
    //: Save to std::ostream.

    virtual bool GetAttr(const StringC &attrName,bool &attrValue)
    {
      if(attrName=="flushAfterWrite") {
        attrValue = m_flushAfterWrite;
        return true;
      }
      return DPPortBodyC::GetAttr(attrName,attrValue);
    }
    //: Get a stream attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling stream attributes such as frame rate, and compression ratios.

    virtual bool SetAttr(const StringC &attrName,const bool &attrValue)
    {
      if(attrName=="flushAfterWrite") {
        m_flushAfterWrite = attrValue;
        return true;
      }
      return DPPortBodyC::SetAttr(attrName,attrValue);
    }
    //: Set a stream attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling stream attributes such as frame rate, and compression ratios.

  protected:
    void RegisterAttributes()
    {
      this->RegisterAttribute(AttributeTypeBoolC("flushAfterWrite","Flush output after each put ",true,true,false));
    }

  private:
    BinOStreamC out;
    UInt32T version;
    bool m_flushAfterWrite;
  };

  /////////////////////////////////////
  //! userlevel=Develop
  //: Load objects in binary from a file.
  // Object must have a binary stream input function
  // and a default constructor.

  template<class DataT>
  class DPIBinFileBodyC
    : public DPISPortBodyC<DataT>
  {
  protected:
      void ReadHeader() {
        StringC classname;
        UInt16T id;
        in >> id;

        switch(id)
        {
          case RavlN::RAVLBinaryID64:
            in.SetCompatibilityMode32Bit(false);
            break;
          case RavlN::RAVLInvBinaryID64:
            in.SetCompatibilityMode32Bit(false);
            in.UseNativeEndian(!in.NativeEndian());
            break;
          case RavlN::RAVLBinaryID32:
            in.SetCompatibilityMode32Bit(true);
            break;
          case RavlN::RAVLInvBinaryID32:
            in.SetCompatibilityMode32Bit(true);
            in.UseNativeEndian(!in.NativeEndian());
            break;
          case RavlN::RAVLInvBinaryID:
  #if RAVL_ENDIAN_COMPATILIBITY
            in.UseNativeEndian(!in.NativeEndian());
  #else
            throw ExceptionOperationFailedC("Stream is incompatible. endian mismatch. ");
  #endif
            /* no break */
          case RavlN::RAVLBinaryID:
            // Just use the default stream option.
            break;
          default:
            cerr << "DPIBinFileC ERROR: Bad file id. \n";
            throw ExceptionOperationFailedC("Bad file id. ");
        }
        in >> classname >> version;  // Read class name & version
        if(classname != TypeName(typeid(DataT)) && (classname != m_typenameAlias || m_typenameAlias.IsEmpty())) {
          cerr << "DPIBinFileC ERROR: Bad file type. '" << classname << "' Expected: '" << TypeName(typeid(DataT)) << "' \n";
          throw ExceptionOperationFailedC("File type mismatch. ");
        }
      }

  public:
    DPIBinFileBodyC()
      : version(-1),
	off(0),
	dataStart(0)
    {}
    //: Default constructor.


    DPIBinFileBodyC(const StringC &nfname,bool useHeader = false,const StringC &typenameAlias = StringC())
      : in(nfname),
	version(0),
	off(0),
        m_typenameAlias(typenameAlias)
    {
      if(useHeader)
        ReadHeader();
      dataStart = in.Tell(); // Remember where data starts.
    }
    //: Construct from a filename.

    inline DPIBinFileBodyC(BinIStreamC &strmin,bool useHeader = false,const StringC &typenameAlias = StringC())
      : in(strmin),
        version(0),
	off(0),
	m_typenameAlias(typenameAlias)
    {
      if(useHeader)
        ReadHeader();
      dataStart = in.Tell(); // Remember where data starts.
    }
    //: Stream constructor.

    virtual bool IsGetEOS() const
    { return (!in.Stream().good() || in.Stream().eof()); }
    //: Is valid data ?

    virtual DataT Get() {
      DataT ret;
      in >> ret;
      off++;
      return ret;
    }
    //: Get next piece of data.

    virtual bool Get(DataT &buff) {
      if(in.IsEndOfStream())
	return false;
      in >> buff;
      off++;
      return true;
    }
    //: Get next piece of data.

    virtual IntT GetArray(SArray1dC<DataT> &data) {
      for(SArray1dIterC<DataT> it(data);it;it++) {
	in >> *it;
	off++;
	if(!in.Stream().good()) {
#ifdef RAVL_CHECK
	  std::cerr << "DPIBinFileBodyC<DataT>::GetArray(), Ended early because of bad input stream. \n";
#endif
	  return it.Index().V();
	}
      }
      return data.Size();
    }
    //: Get multiple pieces of input data.

    virtual bool Save(std::ostream &out) const
    { out << in.Name(); return true; }
    //: Save to std::ostream.

    virtual bool Seek(UIntT newOff) {
      if(newOff == 0) {
	in.Seek(dataStart);
	off = newOff;
	return true;
      }
      return false;
    }
    //: Seek to position in stream.
    // Currently only understands seek to start.

    virtual UIntT Tell() const
    { return off; }
    //: Get current offset in stream.

    virtual UIntT Size() const
    { return (UIntT) (-1); }
    //: Get size of stream.

  private:
    BinIStreamC in;
    UInt32T version;
    UIntT off;
    streampos dataStart;
    StringC m_typenameAlias;
  };

  ///////////////////////////////
  //! userlevel=Normal
  //: Binary file output stream.
  // Object must have a binary stream output function.

  template<class DataT>
  class DPOBinFileC
    : public DPOPortC<DataT>
  {
  public:
    inline DPOBinFileC() {}
    //: Default constructor.

    inline DPOBinFileC(BinOStreamC &strm,bool useHeader=false)
      : DPEntityC(*new DPOBinFileBodyC<DataT>(strm,useHeader))
    {}
    //: Stream constructor.

    inline DPOBinFileC(const StringC &fname,bool useHeader=false)
      : DPEntityC(*new DPOBinFileBodyC<DataT>(fname,useHeader))
    {}

    //: Filename constructor.
  };

  //////////////////////////////////
  //! userlevel=Normal
  //: Binary file input stream.
  // Object must have a binary stream input function
  // and a default constructor.

  template<class DataT>
  class DPIBinFileC
    : public DPISPortC<DataT>
  {
  public:
    inline DPIBinFileC() {}
    //: Default constructor.

    inline DPIBinFileC(BinIStreamC &strm,bool useHeader=false,const StringC &typenameAlias = StringC())
      : DPEntityC(*new DPIBinFileBodyC<DataT>(strm,useHeader,typenameAlias))
    {}
    //: Stream constructor.

    inline DPIBinFileC(const StringC &afname,bool useHeader=false,const StringC &typenameAlias = StringC())
      : DPEntityC(*new DPIBinFileBodyC<DataT>(afname,useHeader,typenameAlias))
    {}
    //: Filename constructor.
  };
}


#endif
