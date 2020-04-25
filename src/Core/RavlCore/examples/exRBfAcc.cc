// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Container/Buffer/exRBfAcc.cc"
//! userlevel=Normal
//! author="Radek Marik"
//! docentry="Ravl.API.Core.Arrays.Buffer"
//! date="26/2/1997"

//: Range Buffer Access

#include "Ravl/EntryPnt.hh"
#include "Ravl/Types.hh"
#include "Ravl/RBfAcc.hh"
#include "Ravl/Stream.hh"

using namespace RavlN;

// This class enables to access member function
// RangeBufferAccessC<char>::Copy().

class RBAccC
  : public RangeBufferAccessC<char>
{
  public:
    RangeBufferAccessC<char> Copy() const
    { return RangeBufferAccessC<char>::Copy();}
};


int  ExRBfAcc(int, char * [] )
{
  std::cout << "Buffer access with known size example\n"
           << "=====================================\n";

  char buffer[] = "absdefghijklmnopqrstuvwxyz\n";
  RangeBufferAccessC<char> acc1(buffer, IndexRangeC(25)); // map a memory
  RangeBufferAccessC<char> acc2 = acc1 + 3;  // map other access with a 
                                              // different reference point
  RangeBufferAccessC<char> acc3;             // a default access
  RangeBufferAccessC<char> acc4;             // another default access
  acc4 = buffer + 5;                          // remap the access, but Size()==0
  RangeBufferAccessC<char> acc5 = acc4;      // map an access
  acc5.Attach(acc2);                          // remap as acc2
  RBAccC acc6;                                // a default access
  acc6.Attach(acc4, IndexRangeC(10));                      // remap as acc4, but with a size
  RangeBufferAccessC<char> acc7(acc6.Copy()); // a new buffer with an access
  acc6.Fill('*');                             // modify the contents
  acc2 += 10;                                 // shift indeces
  
  RangeBufferAccessC<char> acc8(acc6); // a new buffer with an access
  acc8.ShiftIndexes(1);
  std::cout << acc8[0] <<  acc6[0] << "\n";
  std::cout << "Buffer1 ?  " << acc1.IsValid() << ' ' << acc1.IsEmpty() << ' '
                            << acc1.Size() << ' ' << acc1.ReferenceVoid() << '\n'
           << "Buffer2 ?  " << acc2.IsValid() << ' ' << acc2.IsEmpty() << ' '
                            << acc2.Size() << ' ' << acc2.ReferenceVoid() << '\n'
           << "Buffer3 ?  " << acc3.IsValid() << ' ' << acc3.IsEmpty() << ' '
                            << acc3.Size() << ' ' << acc3.ReferenceVoid() << '\n'
           << "Buffer4 ?  " << acc4.IsValid() << ' ' << acc4.IsEmpty() << ' '
                            << acc4.Size() << ' ' << acc4.ReferenceVoid() << '\n'
           << "Buffer5 ?  " << acc5.IsValid() << ' ' << acc5.IsEmpty() << ' '
                            << acc5.Size() << ' ' << acc5.ReferenceVoid() << '\n'
           << "Buffer6 ?  " << acc6.IsValid() << ' ' << acc6.IsEmpty() << ' '
                            << acc6.Size() << ' ' << acc6.ReferenceVoid() << '\n'
           << "Buffer7 ?  " << acc7.IsValid() << ' ' << acc7.IsEmpty() << ' '
                            << acc7.Size() << ' ' << acc7.ReferenceVoid() << '\n'
           << "Buffer1:   " << acc1.ReferenceElm() 
           << "Buffer2:   " << acc2.DataStart()
           << "Buffer4:   " << acc4.DataStart()
           << "Buffer5:   " << acc5.DataStart()
           << "Buffer6:   " << acc6.DataStart()
           << "Buffer7:   " << acc7.DataStart()  << '\n'
           << "Buffer8:   " << acc7.DataStart()  << '\n'
           << "Buffer2.Size()   " << acc2.Size()    << '\n'
           << "Buffer2.Range()  " << acc2.Range()   << '\n'
           << "Buffer2[16]:     " << acc2[16]       << '\n'
           << "\n"; 
  std::cout << "End of the example\n";
  return 0;
}

RAVL_ENTRY_POINT(ExRBfAcc)




