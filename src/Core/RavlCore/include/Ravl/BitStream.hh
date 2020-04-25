// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_BITSTREAM_HEADER
#define RAVL_BITSTREAM_HEADER 1
/////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Core/Base/BitStream.hh"
//! lib=RavlCore
//! docentry="Ravl.API.Core.IO.Streams"
//! author="Charles Galambos"
//! date="04/06/1999"
//! userlevel=Default

#include "Ravl/Stream.hh"
#include "Ravl/config.h"

namespace RavlN {
  //! userlevel=Develop
  //: Bit stream base.
  
  class BitStreamBaseC  {
  public:  
    BitStreamBaseC()
      : at(1024),
	buff(0)
    {}
    //: Default constructor.
    
    bool IsByteAligned() const { return at == 7; }
    //: Is byte aligned ?
    
    UIntT Mask(UIntT nbits) const 
    { return ((UIntT)1 << (nbits)) -1; }
    //: Create a mask nbits long.
    
  protected:
    IntT  at;   // Bit offset.
    UByteT buff;// Byte ptr.
#ifdef NEW_ANSI_DRAFT
    static const UIntT buffSize = 8;
#else 
    static const UIntT buffSize;
#endif
  };
  
  //! userlevel=Normal
  //: Input bit stream
  // 'at' points to the next bit to read.
  
  class BitIStreamC 
    : public BitStreamBaseC
  {
  public:
    BitIStreamC()
    {}
    //: Default constructor.
  
    BitIStreamC(const StringC &filename)
      : strm(filename)
    { at = -1; }
    //: Constructor.
  
    BitIStreamC(const IStreamC &strm)
      : strm(strm)
    { at = -1; }
    //: Construct from a normal strm.
    
    inline bool ReadBit() { 
      if(at < 0) {
	strm.read((char *) &buff,1);
	at = buffSize - 1;
      }
      bool ret = (buff >> at) & 1;
      at--;
      return ret;
    }
    //: Read 1 bit from stream.
    
    inline bool PeekBit() {
      if(at < 0) {
	strm.read((char *) &buff,1);
	at = buffSize - 1;
      }
      bool ret = (buff >> at) & 1;
      return ret;
    }
    //: Look at the next bit to be read. but don't retrieve it.
    
    inline UByteT ReadUByte()  {
      UByteT ret;
      if(at < 0) {
	strm.read((char *) &ret,1);
      } else {
	ret = buff << (7-at); // Assumes byte is 0 padded. 
	strm.read((char *) &buff,1);
	ret |= buff >> (at+1);
      }
      //cerr << "Read UByteT: " << ((UIntT) ret) << " Bits:8 at(after):" << at << " \n";
      return ret;
    }
    //: Read a unsigned byte from stream.
    
    UByteT ReadUByte(IntT nbits);
    //: Read 1 to 7 bits from stream into a byte.
    
    UIntT ReadUInt() { 
      UIntT ret;
#if RAVL_BIGENDIAN
      ((UByteT *)&ret)[0] = ReadUByte(); 
      ((UByteT *)&ret)[1] = ReadUByte();
      ((UByteT *)&ret)[2] = ReadUByte();
      ((UByteT *)&ret)[3] = ReadUByte();
#else
      ((UByteT *)&ret)[3] = ReadUByte(); 
      ((UByteT *)&ret)[2] = ReadUByte();
      ((UByteT *)&ret)[1] = ReadUByte();
      ((UByteT *)&ret)[0] = ReadUByte();
#endif
      return ret;
    }
    //: Read an 32-bit unsigned integer.
    // MSB First.
    
    UIntT ReadUInt(UIntT bits);
    //: Read an unsigned integer of 1 to 31 bits
    // MSB First.
    
    IntT ReadInt(UIntT bits) {
      UIntT val = ReadUInt(bits);
      UIntT signbit = (1 << (bits-1));
      if(val & signbit) // Not the best way but it'll do for now.
	return signbit - (val & ~signbit);
      return val;
    }
    //: Read a signed integer of 1 to 31 bits
    // MSB First.
    
    inline void DiscardBit() {
      if(at < 0) {
	strm.read((char *) &buff,1);
	at = buffSize - 1;
      }
      at--;
    }
    //: Discard 1 bit from stream.
  
    UByteT NextAlignedByte() {
      UByteT ret;
      strm.read((char *) &ret,1);
      at = -1 ;
      return ret;
    }
    //: Read next byte-aligned byte.
    // This will discard bits between current position
    // and next alignment.
    
    inline bool good() const
    { return strm.good(); }
    //: Is stream good ?
    
    bool Close() 
    { return strm.Close(); }
    //: Close the stream.
    
    IStreamC &Stream()
    { return strm; }
    //: Access underlying stream.
  protected:
    IStreamC strm;
  };
  
  //! userlevel=Normal
  //: Output bit stream.
  // 'at' points to next bit to be written.
  // Numbering from LSB=0
  
  class BitOStreamC 
    : public BitStreamBaseC
  {
  public:
    BitOStreamC() { 
      at = buffSize-1; 
      buff = 0;
    }
    //: Default constructor.
    
    BitOStreamC(const StringC &filename)
      : strm(filename)
    { 
      at = buffSize-1; 
      buff = 0;
    }
    //: Constructor.
    
    BitOStreamC(const OStreamC &strm)
      : strm(strm)
    {
      at = buffSize-1; 
      buff = 0;
    }
    //: Construct from a normal stream
    // NB. 'Flush' must be used before any write operations
    // are done on 'strm' after using the class.
    
    ~BitOStreamC();
    //: Close bit stream.
    
    inline void WriteBit(bool val) {
      at--;
      if(val) 
	buff |= 1 << (at+1);
      // Don't write 0's buffer defaults to 0.
      if(at < 0) {
	strm.write((const char *) &buff,1);
	buff = 0;
	at = buffSize-1;
      }
    }
    //: Write 1 bit to stream.
    
    inline void WriteUByte(UByteT byte) {
      //cerr << "Writing UByteT: " << hex << ((UIntT) byte) << " Bits:8 At:" << at << "\n";
      if(at == 7) {
	strm.write((const char *) &byte,1);
      } else {
	buff |= byte >> (buffSize-(at+1));  // Assumes byte is 0 padded.
	strm.write((const char *) &buff,1);
	buff = byte << (at+1);
      }
    }
    //: Write a byte to stream.
    
    void WriteUByte(UByteT data,IntT nbits);
    //: Write from 1 to 7 bits.
    
    void WriteUInt(UIntT data) { 
#if RAVL_BIGENDIAN
      WriteUByte(((const UByteT *)&data)[0]); 
      WriteUByte(((const UByteT *)&data)[1]);
      WriteUByte(((const UByteT *)&data)[2]);
      WriteUByte(((const UByteT *)&data)[3]);
#else
      WriteUByte(((const UByteT *)&data)[3]); 
      WriteUByte(((const UByteT *)&data)[2]);
      WriteUByte(((const UByteT *)&data)[1]);
      WriteUByte(((const UByteT *)&data)[0]);
#endif
    }
    //: Write an unsigned 32 bit integer 
    // MSB First.
    
    void WriteUInt(UIntT data,UIntT bits);
    //: Write an unsigned integer with 1 to 31 bits.
    // MSB First.

    void WriteInt(IntT data,UIntT bits)
    { WriteUInt((UIntT) data,bits); }
    //: Write an integer with 1 to 31 bits.
    // MSB First.
    
    void Flush() {
      if(at < (IntT) (buffSize-1)) {
	strm.write((const char *) &buff,1);
	at = buffSize -1;
	buff = 0;
      }
    }
    //: Flush output buffer. (Use only before closing.)
    
    inline bool good() const
    { return strm.good(); }
    //: Is stream good ?

    bool Close() { 
      Flush(); 
      return strm.Close(); 
    }
    //: Close the stream.

    OStreamC &Stream()
    { return strm; }
    //: Access underlying stream.
    
  protected:
    OStreamC strm; 
  };
}


#endif
