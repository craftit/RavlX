// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Base/BitStream.cc"

#include "Ravl/BitStream.hh"

namespace RavlN {
#ifndef NEW_ANSI_DRAFT
  const UIntT BitStreamBaseC::buffSize = 8;
#endif
  
  //: Read a bit vector.
  
  UByteT BitIStreamC::ReadUByte(IntT nbits) {
    UByteT ret;
    RavlAssert(nbits <= 8);
    if(at < 0) {
      strm.read((char *) &buff,1);
      at = buffSize-1;
    }
    IntT bitsInBuff = at + 1;
    if(bitsInBuff >= nbits) {
      // This remainder is in the buffer already.
      at -= nbits;
      ret = (buff >> (at+1)) & Mask(nbits) ;
	//cerr << "\nAll bits in buff :" << ((UIntT) ret) << " at:" << at <<" NBits:" << nbits <<" Mask:" << hex << Mask(nbits) <<"\n";
    } else {
      // This remainder goes over a byte boundary.
      at -= bitsInBuff;
      ret = ((buff >> (at+1)) & Mask(bitsInBuff)) << (nbits-bitsInBuff);
      nbits -= bitsInBuff;
      strm.read((char *) &buff,1);
      at = (buffSize-1) - nbits;
      ret |= (buff >> (at+1)) & Mask(nbits);
    }
    //cerr << "Read UIntT: " << hex << ((UIntT) ret) << " Bits:" << nbits << " at(after):" << at << " \n";
    return ret;
  }

  //: Read an unsigned integer.
  // MSB First.
  
  UIntT BitIStreamC::ReadUInt(UIntT bits) { 
    if(bits == 0)
      return 0;
    RavlAssert(bits < 32);
    UIntT ret = 0;
#if RAVL_BIGENDIAN
    //cerr << "Read UIntT: Bits:" << bits << " at(before):" << at << " \n";
    UByteT *place = &(((UByteT *)&ret)[3-(bits/8)]);
    UIntT rem = bits % 8;
    *place = ReadUByte(rem);
    place++;
    bits -= rem;
    while(bits > 0) {
      *(place++) = ReadUByte();
      bits -=8;
    }
    //cerr <<"Value :" << ret << "\n";
#else
    UByteT *place = &(((UByteT *)&ret)[bits/8]);
    UByteT rem = bits % 8;
    *place = ReadUByte(rem);
    bits -= rem;
    place--;
    while(bits > 7) {
      *place = ReadUByte();
      place--;
      bits -= 8;
    }
    // Read the remainder.
#endif
    return ret;
  }
  
  //////////////////////////////////////////////////////////////////////////
  
  BitOStreamC::~BitOStreamC() { 
    // Flush stream.
    Flush();
  }
  
  //: Write an array of n bits.
  
  void BitOStreamC::WriteUByte(UByteT data,IntT nbits) {
    //cerr << "Writing UIntT: " << hex << ((UIntT) *v) << " Bits:" << nbits << " at:" << at <<"\n";
    RavlAssert(nbits <= 8);
    // Write the remainder.
    IntT bitsInBuff = at+1;
    if(bitsInBuff >= nbits) {
      at -= nbits;
      buff |= (data & Mask(nbits)) << (at+1);
      if(at < 0) {
	strm.write((const char *) &buff,1);
	buff = 0;
	at = buffSize-1;
      }
    } else {
      // at == -1
      buff |= (data >> (nbits-bitsInBuff)) & Mask(bitsInBuff);
      strm.write((const char *) &buff,1);
      nbits -= bitsInBuff;
	at = (buffSize-1)-nbits;
	buff = (data & Mask(nbits)) << (at+1);
    }
  }

  
  void BitOStreamC::WriteUInt(UIntT data,UIntT bits) { 
    if(bits == 0)
      return ;
    RavlAssert(bits < 32);
#if RAVL_BIGENDIAN
    //cerr << "Writing UIntT: " << hex << data << " Bits:" << bits << " \n";
    UByteT *place = &((UByteT *)&data)[3-(bits/8)];
    UIntT rem = bits % 8;
    WriteUByte(*place,rem);
    place++;
    bits -= rem;
    while(bits > 0) {
      WriteUByte(*(place++));
      bits -=8;
    }
#else
    UByteT *place = &((UByteT *) &data)[bits/8];
    WriteUByte(*place,bits % 8);
    place--;
    while(bits > 7) {
      WriteUByte(*(place--)); 
      bits -= 8;
    }
#endif
  }

}
