// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id: exChild.cc 5240 2005-12-06 17:16:50Z plugger $"
//! lib=RavlOS
//! file="Ravl/OS/Exec/exChild.cc"
//! author="Charles Galambos"
//! docentry="Ravl.API.OS.Exec"
//! date="24/11/1998"
//! userlevel=Normal

#include "Ravl/Option.hh"
#include "Ravl/OS/CPUInfo.hh"
#include "Ravl/CPUID.hh"
#include "Ravl/Assert.hh"
#include "Ravl/OS/Date.hh"
//#include <unistd.h>

using namespace RavlN;

int main(int nargs,char *args[])
{
  OptionC option(nargs,args,true);
  option.Check();

  std::cout << "CPUs:" << RavlN::GetNumCores() << "  HTT:" << RavlN::HTT() << "\n";
  return 0;
}

