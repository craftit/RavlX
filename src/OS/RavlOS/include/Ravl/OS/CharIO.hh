#ifndef RAVLCHARIO_HH
#define RAVLCHARIO_HH

#include "Ravl/Types.hh"

//! file="Ravl/OS/Misc/CharIO.hh"
//! lib=RavlOS
//! userlevel=Normal
//! docentry="Ravl.API.OS"
//! example=exReadWithoutReturn.cc
//! author="Believed to be Linus Torvald.  Modified for multibyte i/p by Bill Christmas"


namespace RavlN {

  UInt64T GetKeypress(bool EchoOn=false);
  //: Returns keypress value as UInt64T
  // Hence handles keypresses of up to 8 bytes<br>
  // If <code>EchoOn</code> is true, the character is echoed to stdout

}

#endif
