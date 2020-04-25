// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlOS
//! file="Ravl/OS/Text/exTemplateFile.cc"
//! docentry="Ravl.API.OS.Text Processing"
//! userlevel=Normal
//! author="Charles Galambos"

#include "Ravl/Text/TemplateFile.hh"
#include "Ravl/EntryPnt.hh"
#include "Ravl/Option.hh"
#include "Ravl/Stream.hh"

using namespace RavlN;

int exTemplateFile(int argc,char **argv) 
{
  OptionC  option(argc,argv);
  StringC input = option.String("i","template.x","Input file. ");
  StringC output = option.String("o","template.out","Output file. ");
  bool useBuild = option.Boolean("b",false,"Use 'Build' for template subst. (For Testing.)");
  option.Check();
  
  OStreamC out(output);
  TemplateFileC tf(input,out);
  
  tf.SetVar("xx","A Variable");
  if(!useBuild) {
    for(tf.First();tf.IsElm();tf.Next()) {
      StringC lab = tf.Data();
      if(lab == "hello")
	tf.Output() << "hi";
      else {
	tf.ErrorLabel(); // This will print an error message.
	return 1;
      }
    }
  } else {
    tf.SetVar("hello","hi");
    if(!tf.Build()) {
      cerr << "Build failed. \n";
      return 1;
    }
    
  }
  return 0;
}

RAVL_ENTRY_POINT(exTemplateFile);
