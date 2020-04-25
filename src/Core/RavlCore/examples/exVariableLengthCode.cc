// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlCore
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Misc"
//! date="07/06/1999"
//! file="Ravl/Core/System/exVariableLengthCode.cc"

#include "Ravl/VariableLengthCode.hh"  
#include "Ravl/DList.hh"
#include "Ravl/DLIter.hh"  
#include "Ravl/EntryPnt.hh"
#include "Ravl/BitStream.hh"
#include "Ravl/Option.hh"

using namespace RavlN;

VariableLengthCodeInitC codeTabInit[] = { 
  {"0",0},
  {"10",1},
  {"11",2}
};
    

int exVarLenCode (int argc, char** argv)
{
  OptionC opt(argc, argv);
  StringC streamfile = opt.String("strm", "/tmp/testVarLenCode", "bitsream file name");
  opt.Check();
  
  // build tables
  
  VariableLengthCodeC code_table(codeTabInit);  
  VariableLengthEncoderC encoder(codeTabInit);

  // build example bitstream 
  {
    BitOStreamC ostrm(streamfile);
    encoder.Put(2, ostrm);
    encoder.Put(0, ostrm);
    encoder.Put(0, ostrm);
    encoder.Put(1, ostrm);
    encoder.Put(1, ostrm);
    ostrm.Flush();
  }
  
  // input bitstream
  BitIStreamC istrm(streamfile);
  
  // decode bitstream
  VariableLengthCodeIterC decoder(code_table);
  for(int i =0;i < 5;i++) {
    decoder.NextToken(istrm); 
    std::cout <<  decoder.Data() << " ";
  }
  std::cout << "\n";

  return 0;
}



//: This puts a wrapper around the main program that catches
//: exceptions and turns them into readable error messages.

RAVL_ENTRY_POINT(exVarLenCode);
