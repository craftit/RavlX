// This file is part of CxxDoc, The RAVL C++ Documentation tool 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU General 
// Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlSourceTools
//! file="Ravl/SourceTools/CodeManager/exBuildInfo.cc"
//! author="Charles Galambos"
//! docentry="Ravl.API.Source Tools"
//! userlevel=Normal

#include "Ravl/Option.hh"
#include "Ravl/SourceTools/DefsFile.hh"
#include "Ravl/SourceTools/SourceCodeManager.hh"
#include "Ravl/SourceTools/BuildInfo.hh"

using namespace RavlN;

bool verb;
BuildInfoC info;

static bool CheckDirectory(StringC &dir,DefsMkFileC &defs) {
  if(verb)
    std::cerr << "Checking '" << dir << "' \n";
  info.ScanDirectory(dir,defs);
  if(defs.IsDefined("PLIB")) {
    StringC lib = defs["PLIB"];
    std::cerr << lib << " :";
    for(DLIterC<StringC> it(info.LibDepends(lib));it;it++)
      std::cerr << " " << *it;
    std::cerr << "\n";
  }
  return true;
}

int main(int nargs,char **argv) {
  OptionC opt(nargs,argv);
  StringC fn = opt.String("",".","Start directory. ");
  verb = opt.Boolean("-v",false,"Verbose mode. ");
  opt.Check();
  
  SourceCodeManagerC chkit(fn);
  if(verb)
    chkit.SetVerbose(true);  
  chkit.ForAllDirs(CallFunc2C<StringC&,DefsMkFileC&,bool>(&CheckDirectory),false);

  return 0;
}
