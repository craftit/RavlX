// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, Omniperception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlGUIGnome

#include "Ravl/GUI/Gnome.hh"
#include "Ravl/Option.hh"

using namespace RavlN;
using namespace RavlGUIN;

int main(int nargs,char **argv) {
  OptionC opts(nargs,argv);
  
  opts.Check();
  
  GnomeInit("exGnome","1.0",nargs,argv);
  
  return 0;
}
