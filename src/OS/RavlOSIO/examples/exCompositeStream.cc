// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////
//! author="Charles Galambos"
//! rcsid="$Id$"
//! docentry="Ravl.API.Core.Data Processing.Composite" 
//! lib=RavlOSIO
//! file="Ravl/OS/IO/exCompositeStream.cc"

#include "Ravl/Option.hh"
#include "Ravl/DP/FileFormatComposite.hh"
#include "Ravl/DP/FileFormatIO.hh"
#include "Ravl/String.hh"
#include "Ravl/EntryPnt.hh"
#include "Ravl/Stream.hh"

//: Example of composite streams

using namespace RavlN;
namespace RavlN {
  extern int IncludeFileFormatComposite();
}

//
// Contents of t1.txt :
//-----------------------------------------
//IntT
//1 2 3
//-----------------------------------------
//
// Contents of t2.txt :
//-----------------------------------------
//IntT
//10 11 12
//-----------------------------------------


int exStreamComposite(int argc,char **argv) 
{
  OptionC option(argc,argv,true);
  StringC file1 = option.String("i1","t1.txt","Input file 1");
  StringC file2 = option.String("i2","t2.txt","Input file 2");
  StringC file3 = option.String("i3","t1.txt","Input file 4");
  StringC compFile = option.String("c","","Composite playfile name. ");
  option.Check();
  
  IncludeFileFormatComposite();
  cerr << "Building play list. \n";
  
  PlayListC pl;
  if(compFile == "") {
    DListC<EditSpecC> lst;
    lst.InsLast(EditSpecC(file1));
    lst.InsLast(EditSpecC(file2));
    lst.InsLast(EditSpecC(file3));
    pl = PlayListC(lst);
  } else {
    if(!RavlN::Load(compFile,pl,"",true)) {
      cerr << "Failed to open play list '" << compFile << "'\n";
      return 1;
    }
  }
  
  cerr << pl;
  cerr << "Opening composite stream. \n";
  
  CompositeIStreamC<IntT> cis(pl);
  int i = 0;
  cerr << "Printing elements from stream. " << cis.IsGetReady() << "\n";
  for(;;i++) {
    IntT v;
    if(!cis.Get(v))
      break;
    cerr << i << " : " << v << "\n";
  }
  cerr << "Done. \n";
  return 0;
}

RAVL_ENTRY_POINT(exStreamComposite);

