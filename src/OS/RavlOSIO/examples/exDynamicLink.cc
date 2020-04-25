// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlOSIO
//! docentry="Ravl.API.OS.Dynamic Linking"
//! author="Charles Galambos"
//! file="Ravl/OS/IO/exDynamicLink.cc"

#include "Ravl/Option.hh"
#include "Ravl/DP/DynamicLink.hh"

using namespace RavlN;

int main(int nargs,char **argv) {
  OptionC opt(nargs,argv);
  DListC<StringC> sl = opt.List("l","List of libraries to load. ");
  opt.Check();
  
  for(DLIterC<StringC> it(sl);it;it++) {
    cerr << "Loading : " << *it << "\n";
    if(!DynamicLinkLoad(*it))
      cerr << " Load failed.\n";
  }
  
  return 0;
}
