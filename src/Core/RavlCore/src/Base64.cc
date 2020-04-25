// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlCore
//! file="Ravl/Core/System/Base64.cc"

// Based on code from BASE64 by John Walker (http://www.fourmilab.ch/)

#include "Ravl/Base64.hh"
#include "Ravl/SArray1dIter.hh"

namespace RavlN {
  static ByteT *BuildEncodeTable();
  static const ByteT *BuildDecodeTable();
  
  static const ByteT *encodeTable = BuildEncodeTable();
  static const ByteT *decodeTable = BuildDecodeTable();
  
  ByteT *BuildEncodeTable() {
    static ByteT dtable[64];
    int i;
    for(i= 0;i<9;i++){
      dtable[i]= 'A'+i;
      dtable[i+9]= 'J'+i;
      dtable[26+i]= 'a'+i;
      dtable[26+i+9]= 'j'+i;
    }
    for(i= 0;i<8;i++){
      dtable[i+18]= 'S'+i;
      dtable[26+i+18]= 's'+i;
    }
    for(i= 0;i<10;i++)
      dtable[52+i]= '0'+i;
    dtable[62]= '+';
    dtable[63]= '/';
    return dtable;
  }
  
  const ByteT *BuildDecodeTable() {
    static ByteT dtable[256];
    int i;
    for(i= 0;i<255;i++)
      dtable[i]= 0x80;
    for(i= 'A';i<='I';i++)
      dtable[i]= 0+(i-'A');
    for(i= 'J';i<='R';i++)
      dtable[i]= 9+(i-'J');
    for(i= 'S';i<='Z';i++)
      dtable[i]= 18+(i-'S');
    for(i= 'a';i<='i';i++)
      dtable[i]= 26+(i-'a');
    for(i= 'j';i<='r';i++)
      dtable[i]= 35+(i-'j');
    for(i= 's';i<='z';i++)
      dtable[i]= 44+(i-'s');
    for(i= '0';i<='9';i++)
      dtable[i]= 52+(i-'0');
    dtable[(int) '+']= 62;
    dtable[(int) '/']= 63;
    dtable[(int) '=']= 0;
    return dtable;
  }
  
  //: Encode buffer.
  
  StringC Base64C::Encode(const SArray1dC<char> &buffer) {
    StringC ret("",Floor((RealT) buffer.Size() * 1.41)+4);
    ByteT igroup[3];
    StringC tmp("1234");
    ByteT *ogroup = (ByteT *) &(tmp[0]);
    int n,ln = 0;
    for(SArray1dIterC<char> it(buffer);it;) {
      igroup[0] = igroup[1]= igroup[2]= 0;
      for(n= 0;n<3 && it;n++,it++)
	igroup[n]= *it;
      ogroup[0]= encodeTable[igroup[0]>>2];
      ogroup[1]= encodeTable[((igroup[0]&3)<<4)|(igroup[1]>>4)];
      ogroup[2]= encodeTable[((igroup[1]&0xF)<<2)|(igroup[2]>>6)];
      ogroup[3]= encodeTable[igroup[2]&0x3F];
      if(n<3){
	ogroup[3]= '=';
	if(n<2){
	  ogroup[2]= '=';
	}
      }
      ret += tmp;
      if(ln++ >= 18) {
	ln = 0;
	ret += '\n';
      }
    }
    return ret;
  }
  
  //: Decode a string into raw binary.
  
  SArray1dC<char> Base64C::Decode(const StringC &buffer) {
    if(buffer.Size() == 0)
      return SArray1dC<char>();
    SArray1dC<char> ret(Floor((RealT) buffer.Size() * 0.75)+4);
    SArray1dIterC<char> it(ret);
    const char *at = &(buffer[0]);
    const char *end = &(at[buffer.Size()]);
    ByteT a[4],b[4];
    IntT i;
    while(at != end) {
      for(i = 0;i < 4;i++,at++){
	if(at == end) {
	  if(i == 0) { // Just some white space at the end of the string ?
	    // Easier just to return here, rather than escape from the loop.
	    UIntT size = it.Index().V();
	    return SArray1dC<char>(ret,size);
	  }
	  throw ExceptionOperationFailedC("Base64C::Decode(), Unexpected end of string. ");
	}
	ByteT dc = decodeTable[(IntT) *at];
	if(dc & 0x80) {
	  i--;
	  continue;
	}
	a[i]= *at;
	b[i]= dc;
      }
      *it = (b[0]<<2)|(b[1]>>4); it++;
      if(a[2] == '=') break;
      *it = (b[1]<<4)|(b[2]>>2); it++;
      if(a[3] == '=') break;      
      *it = (b[2]<<6)|b[3]; it++;
    }
    UIntT size = it.Index().V();
    return SArray1dC<char>(ret,size);
  }
  
}
