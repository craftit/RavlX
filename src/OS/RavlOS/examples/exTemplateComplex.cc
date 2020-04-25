// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlOS
//! file="Ravl/OS/Text/exTemplateComplex.cc"
//! docentry="Ravl.API.OS.Text Processing"
//! author="Charles Galambos"
//! userlevel=Normal
//! date="23/02/2000"

#include "Ravl/Text/TemplateComplex.hh"
#include "Ravl/EntryPnt.hh"
#include "Ravl/Option.hh"
#include "Ravl/Stream.hh"

using namespace RavlN;

int exTemplateFile(int argc,char **argv) 
{
  OptionC  option(argc,argv);
  StringC input = option.String("","template2.x","Input file. ");
  StringC output = option.String("","template.out","Output file. ");
  option.Check();
  
  OStreamC out(output);
  TemplateComplexC tf(input);
  
  tf.Build(output);
  
  return 0;
}

RAVL_ENTRY_POINT(exTemplateFile);
