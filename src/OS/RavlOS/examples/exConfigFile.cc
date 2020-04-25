// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlOS
//! file="Ravl/OS/Text/exConfigFile.cc"
//! author="Charles Galambos"
//! date="14/05/1998"
//! docentry="Ravl.API.OS.Configuration Files"
//! userlevel=Normal

#include "Ravl/Text/ConfigFile.hh"
#include "Ravl/EntryPnt.hh"
#include "Ravl/Option.hh"
#include "Ravl/Stream.hh"

using namespace RavlN;

int exConfigFile(int argc,char **argv) 
{
  OptionC  option(argc,argv);
  StringC fn = option.String("","test.conf","Config file. ");
  bool dump = option.Boolean("d",false,"Dump config file contents. ");
  StringC var = option.String("p","","Paramiter to print. \n");
  option.Check();
  ConfigFileC conf;
  if(!conf.Load(fn)) {
    cerr << "Failed to load file '" << fn << "'\n";
    return 1;
  }
  if(var != "")
    cout << " " << var << " = '" << conf[var] << "'\n";
  if(dump)
    conf.Dump(cout);
  return 0;
}

RAVL_ENTRY_POINT(exConfigFile);

