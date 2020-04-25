// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Base/testString.cc"
//! docentry="Ravl.API.Core.Strings"
//! userlevel=Develop

#include "Ravl/String.hh"

using namespace RavlN;

int testString();
int testSubString();

int main()
{
  int no;
  if((no = testString()) > 0) {
    std::cerr << "Basic test failed line :" << no << "\n";
    return 1;
  }
  if((no = testSubString()) > 0) {
    std::cerr << "Basic test failed line :" << no << "\n";
    return 1;
  }
  std::cerr << "Test passed ok .\n";
  return 0;
}

int testString() {
  StringC hello("hello");
  if(hello.length() != 5)
    return __LINE__;
  if(hello.index('h') != 0)
    return __LINE__;
  if(hello.index("ll") != 2)
    return __LINE__;
  if(hello != "hello")
    return __LINE__;
  if(StringC(" one  ").TopAndTail() != "one")
    return __LINE__;
  if(StringC("one").TopAndTail() != "one")
    return __LINE__;
  {
    StringC test(" * * * ");
    test.gsub("*","Hello");
    if(test.freq("Hello") != 3) return __LINE__;
  }
  {
    StringC num("357");
    StringC notnum("35p");
    int ans;
    if (!num.IntValue(ans) || (ans != 357)) return __LINE__;
    if (notnum.IntValue(ans)) return __LINE__;
  }
  if ((StringC('a')+'b') != "ab") return __LINE__;
  if (('a'+StringC('b')) != "ab") return __LINE__;
  
  StringC five("01234"); // test to check we can delete 1st char of string
  while (!five.IsEmpty())
    {
      StringC madeup; // constructed to match what "five" should be
      for (IntT c=4; madeup.length() < five.length(); --c)
        madeup = (char) ((int)'0'+c) + madeup;
      //cerr << five << ' ' << madeup << endl;
      if (five != madeup) return __LINE__;
      five.del(0,1); 
    }

  for(Int64T i = 0;i < 10;i++) {
    StringC ui(i);
    //cerr<< " '" << ui << "' " << ui.Int64Value() << "\n";
    if(ui.Int64Value() != i) return __LINE__;

    StringC sui((UInt64T)i);
    if(sui.UInt64Value() != (UInt64T)i) return __LINE__;

    Int64T val = i + 0x8000000000000000LL;
    StringC hex;
    hex.form("0x%016llx", val);
    if (hex.Int64Value() != val) return __LINE__;
    if (hex.UInt64Value() != (UInt64T)val) return __LINE__;
  }

  return 0;
}

int testSubString() {
  StringC hello("hello");
  SubStringC ss = hello;
  if(ss.length() != 5)
    return __LINE__;
  if(ss != hello)
    return __LINE__;
  if(!(ss == hello))
    return __LINE__;
  if(ss.index('l') != hello.index('l'))
    return __LINE__;
  if(hello.index("ll") != 2)
    return __LINE__;
  ss = hello.after(0);
  if(ss != "ello") {
    return __LINE__;
  }
  if(ss != hello.from(1))
    return __LINE__;
  if(ss.index("ll") != 1)
    return __LINE__;
  if(ss.length() != 4)
    return __LINE__;
  ss = ss.before(3);
  if(ss.length() != 3)
    return __LINE__;
  if(ss != "ell")
    return __LINE__;
  if(ss.firstchar() != 'e')
    return __LINE__;
  if(ss.lastchar() != 'l')
    return __LINE__;
  if(ss.index('h') >= 0)
    return __LINE__;
  if(ss.index('o') >= 0)
    return __LINE__;
  if(ss.index("lo") >= 0)
    return __LINE__;
  if(ss.index("he") >= 0)
    return __LINE__;
  if(ss.index("ell") != 0)
    return __LINE__;
  if(ss.index(StringC("ll")) != 1)
    return __LINE__;
  if(ss.index(hello.after(2).before(1)) != 1)
    return __LINE__;

  StringC org = ss;
  if(org != ss)
    return __LINE__;
  if(ss == hello)
    return __LINE__;
  if(SubStringC(" one  ").TopAndTail() != "one")
    return __LINE__;
  if(SubStringC("one").TopAndTail() != "one")
    return __LINE__;
  return 0;
}
