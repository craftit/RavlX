// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////
//! author="Charles Galambos"
//! rcsid="$Id$"
//! lib=RavlIO
//! file="Ravl/Core/IO/exFileFormat.cc"
//! docentry="Ravl.API.Core.IO.Formats" 
//! date="13/08/1998"
//! userlevel=Normal

//: Using Load(...) and Save(...) methods.

#include "Ravl/Option.hh"
#include "Ravl/IO.hh"

using namespace RavlN;

int main(int argc,char **argv) 
{  
  OptionC option(argc,argv);
  StringC fn = option.String("i","/tmp/fsin","Input filename");
  StringC streamType = option.String("t","stream","Type of stream to use. ");
  bool loadOnly = option.Boolean("l",false,"Load only. ");
  bool verb = option.Boolean("v",false,"Verbose. ");
  option.Check();
  
  RealT val;
  val = 21;
  
  if(!loadOnly) {
    //: Save a value into file 'fn' from local variable 'val'.
    
    if(!RavlN::Save(fn,val,streamType,verb)) {
      cerr << "Failed to save data. \n";
      return 1;
    }
  }
  val = 0;

  //: Load a value from file 'fn' into local variable 'val'.
  
  if(!RavlN::Load(fn,val,"",verb)) {
    cerr << "Failed to load data. \n";
    return 1;
  }
  
  cout << "Loaded :" << val << endl;
  
  return 0;
}

