// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_VARIABLELENGTHCODING_HEADER
#define RAVL_VARIABLELENGTHCODING_HEADER 1
//////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Core/System/VariableLengthCode.hh"
//! lib=RavlCore
//! docentry="Ravl.API.Core.Misc"
//! author="Charles Galambos"
//! date="07/06/1999"

#include "Ravl/SArray1d.hh"
#include "Ravl/Pair.hh"
#include "Ravl/BitStream.hh"
#include "Ravl/SArray1dIter.hh"

namespace RavlN {
  class VariableLengthCodeIterC;
  
  //! userlevel=Normal
  //: Code table initialisation structure.
  // Structure than can be used for initialising
  // table. The last entry must set code == 0.
  
  struct VariableLengthCodeInitC
  { 
    const char *code;
    UIntT result;
  };
  
  //! userlevel=Normal
  //: Variable length code table.
  // Small object.
  
  class VariableLengthCodeC
  {
  public:
    VariableLengthCodeC(UIntT maxCodes);
    //: Constructor.
    
    VariableLengthCodeC(const VariableLengthCodeInitC *data);
    //: Constructor.
  
    bool Add(const char *bits,UIntT result);
    //: Add code to table.
    // result can be any value between 0 at 2^31
  
  protected:
    static UIntT Marker() { return 1 << 31; }
    //: Null entry.
  private:
    SArray1dC<PairC<UIntT> > data;
    UIntT free;
  
    friend class VariableLengthCodeIterC;
  };
  
  //! userlevel=Normal
  //: Variable length binary decoder.
  // Used to decode bit streams.
  
  class VariableLengthCodeIterC
  {
  public:
    VariableLengthCodeIterC()
      : at((UIntT) -1),
	lastBit((UIntT) -1)
    {}
    //: Default constructor.
  
    VariableLengthCodeIterC(const VariableLengthCodeC &tab)
      : at(0),
	lastBit((UIntT) -1),
	data(tab.data)
    {}
    //: Construct from code table.
    
    bool IsValid() const
    { return at < data.Size(); }
    //: Valid iterator.
    
    bool Next(bool bit) {
      UIntT nextVal = data[at][((int) bit)];
      if(nextVal & VariableLengthCodeC::Marker()) {
	nextVal = nextVal & ~VariableLengthCodeC::Marker();
	at = nextVal;
	if(nextVal == 0)
	  return true; // End of the line, but no data.
	return false;
      }
      lastBit = bit;
      return true;
    }
    //: Enter next bit
    // When it returns true we have a output token. <p>
    // NB. Remember to call first before doing the next lookup!
    
    UIntT NextToken(BitIStreamC & bitstream) {
      First();
      while (!Next(bitstream.ReadBit()));
      return Data();
    }
    //: Get the next token from the bitstream
    
    bool IsElm() const 
    { return lastBit < 2; }
    //: At a token ?
    
    operator bool() const
    { return IsElm(); }
    //: At a token ?
    
    void First() {
      at = 0;
      lastBit = ((UIntT) -1);
    }
    //: Reset decoder
    
    UIntT Data() const {
      RavlAssert(lastBit < 2);
      return data[at][lastBit];
    }
    //: Read current token.
    
    UIntT operator *() const
    { return Data(); }
    //: Read current token.
    
  private:
    UIntT at;
    UIntT lastBit;
    SArray1dC<PairC<UIntT> > data;
  };

  //! userlevel=Normal
  //: Variable length code table.
  // Small object.

  class VariableLengthEncoderC
  {
  public:
    VariableLengthEncoderC(const VariableLengthCodeInitC *data);
    //: construct from initialisation table.
    
    VariableLengthEncoderC(const SArray1dC< SArray1dC<bool> > & table);
    //: Construct from raw table
    
    void Put(UIntT token, BitOStreamC & bitstream) const;
    //: Put 'token' into the bitstream
    
  private:
    SArray1dC< SArray1dC<bool> > data;
  };
}
#endif
