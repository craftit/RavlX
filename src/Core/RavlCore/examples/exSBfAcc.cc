// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Container/Buffer/exSBfAcc.cc"
//! userlevel=Normal
//! author="Radek Marik"
//! docentry="Ravl.API.Core.Arrays.Buffer"
//! date="26/2/1997"

//: Sized Buffer Access

#include "Ravl/EntryPnt.hh"
#include "Ravl/Types.hh"
#include "Ravl/SBfAcc.hh"
#include "Ravl/Stream.hh"

using namespace RavlN;

class SBAccC: public SizeBufferAccessC<char>
{
  public:
    SizeBufferAccessC<char> Copy() const
    { return SizeBufferAccessC<char>::Copy();}
};

int  ExSBfAcc(int, char * [] )
{
  std::cout << "Buffer access with known size example\n"
           << "=====================================\n";

  char buffer[] = "absdefghijklmnopqrstuvwxyz\n";
  SizeBufferAccessC<char> acc1(buffer, 25); // map a memory
  SizeBufferAccessC<char> acc2 = acc1 + 3;  // map other access with a 
                                             // different reference point
  SizeBufferAccessC<char> acc3;             // a default access
  SizeBufferAccessC<char> acc4;             // another default access
  acc4 = buffer + 5;                         // remap the access, but Size()==0
  SizeBufferAccessC<char> acc5 = acc4;      // map an access
  acc5.Attach(acc2);                         // remap as acc2
  SBAccC acc6;                               // a default access
  acc6.Attach(acc4, 10);                   // remap as acc4, but with a size
  SizeBufferAccessC<char> acc7(acc6.Copy()); // a new buffer with an access
  acc6.Fill('*');                            // modify the contents
  
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
           << "Buffer2:   " << acc2.ReferenceElm()
           << "Buffer4:   " << acc4.ReferenceElm()
           << "Buffer5:   " << acc5.ReferenceElm()
           << "Buffer6:   " << acc6.ReferenceElm()
           << "Buffer7:   " << acc7.ReferenceElm()  << '\n'
           << "Buffer2.Size()   " << acc2.Size()    << '\n'
           << "Buffer2.Limits() " << acc2.Limits()  << '\n'
           << "Buffer2.Range()  " << acc2.Range()   << '\n'
           << "Buffer2[3]:" << acc2[1]              << '\n'
           << "\n"; 
  std::cout << "End of the example\n";
  return 0;
}

RAVL_ENTRY_POINT(ExSBfAcc)
