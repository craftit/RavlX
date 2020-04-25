// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/System/VariableLengthCode.cc"

#include "Ravl/config.h"
#include "Ravl/Types.hh"
#include "Ravl/VariableLengthCode.hh"  
#include <string.h>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x) 
#endif

namespace RavlN {

  //: Default constructor.

  VariableLengthCodeC::VariableLengthCodeC(UIntT maxCodes)
    : data(maxCodes),
      free(1)
  {
    data[0] = PairC<UIntT>(Marker(),Marker());
  }
  
  //: Constructor.

  VariableLengthCodeC::VariableLengthCodeC(const VariableLengthCodeInitC *idata)
    : free(1)
  {
    ONDEBUG(std::cerr << "VariableLengthCodeC::VariableLengthCodeC(), Building table. \n");
    IntT count = 0;
    // Get length.
    for(const VariableLengthCodeInitC *it = idata;it->code != 0;it++)
      count++;
    ONDEBUG(std::cerr << "VariableLengthCodeC::VariableLengthCodeC(), Len:" << dec <<count << ". \n");
    data = SArray1dC<PairC<UIntT> >(count*2);
    data[0] = PairC<UIntT>(Marker(),Marker());
    // Initalise data.
    for(;idata->code != 0;idata++) 
      if(!Add(idata->code,idata->result))
	break; // Must be some error.
    ONDEBUG(std::cerr << "VariableLengthCodeC::VariableLengthCodeC(), Building table done. \n");
  }

  //: Add code to table.

  bool VariableLengthCodeC::Add(const char *code,UIntT result)
  {
    const char *place = code;
    RavlAssert(!(result & Marker()));
    IntT at = 0;
    UIntT i;
    while(1) {
      if(*place == ' ') { // Skip spaces.
	place++;
	continue ;
      }
      if(*place == '0') 
	i = 0;
      else i = 1;
      if(place[1] == 0) // Is this the last bit ?
	break; // Last one, put the data here.
      UIntT &entry = data[at][i];
      if(entry == Marker()) { // Free ?
	entry = free++ | Marker();
	if(free > data.Size()) {
	  std::cerr << "VariableLengthCodeC::Add(), Out of tree nodes. \n";
	  return false;
	}
	at = entry & ~Marker();
	data[at] = PairC<UIntT>(Marker(),Marker());
	place++;
	continue;
      }
      if(!(entry & Marker())) {
	cerr << "VariableLengthCodeC::Add(), Incompatable sequence '" << code << "' at:" << at << " With result " << entry << "\n";
	return false; 
      }
      at = entry & ~Marker();
      place++;
    }
    data[at][i] = result;
    return true;
  }

  ///////////////////////////////////////////////////////////////////////

  //: Constructor.
  
  VariableLengthEncoderC::VariableLengthEncoderC(const SArray1dC< SArray1dC<bool> > & table)
    : data(table)
  {}
  
  //: Construct from initialisation table.
  
  VariableLengthEncoderC::VariableLengthEncoderC(const VariableLengthCodeInitC *idata) {
    ONDEBUG(std::cerr << "VariableLengthEncodeC::VariableLengthEncodeC(), Building table. \n");
    IntT count = 0;
    // Get length.
    const VariableLengthCodeInitC *it;
    for(it = idata;it->code != 0;it++)
      count++;
    ONDEBUG(std::cerr << "VariableLengthCodeC::VariableLengthEncodeC(), Len:" << dec <<count << ". \n");
    data = SArray1dC<SArray1dC<bool> >(count);
    // Initalise data.
    it = idata;
    for(SArray1dIterC<SArray1dC<bool> > dit(data);dit;dit++,it++) {
      UIntT len = strlen(it->code);
      SArray1dC<bool> bitcode(len);
      const char *place = it->code;
      for(SArray1dIterC<bool> bit(bitcode);bit;bit++,place++) {
	RavlAssert(*place == '0' || *place == '1'); 
	*bit = (*place == '0') ? false :true ;
      }
      *dit = bitcode;
    }
    //ONDEBUG(std::cerr << "Data=" << data << "\n");
    ONDEBUG(std::cerr << "VariableLengthCodeC::VariableLengthEncodeC(), Building table done. \n");
  }
  
  //: Put 'token' into the bitstream
  
  void VariableLengthEncoderC::Put(UIntT token, BitOStreamC & bitstream) const {
    RavlAssert(token < data.Size());
    const SArray1dC<bool> &bits = data[token]; // check if token exists in table?
    RavlAssertMsg(bits.Size() > 0,"No bit sequence for token.");
    for(BufferAccessIterC<bool> bits_it(bits); bits_it; bits_it++)
      bitstream.WriteBit(bits_it.Data());
  }

}  
