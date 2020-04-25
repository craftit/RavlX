// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2012, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////////

#include "Ravl/Text/TemplateComplex.hh"
#include "Ravl/Option.hh"
#include "Ravl/Stream.hh"
#include "Ravl/Resource.hh"

using namespace RavlN;

int main(int argc,char **argv) 
{
#ifndef TEST_ROOT
 #ifndef PROJECT_OUT
  #define TEST_ROOT ""
 #else
  #define TEST_ROOT PROJECT_OUT
 #endif
#endif

  SetResourceRoot(TEST_ROOT);

  OptionC  option(argc,argv);
  StringC input = option.String("",RavlN::Resource("RAVL/testData", "testTC.tmpl"),"Input file. ");
  StringC output = option.String("",RavlN::Resource("RAVL/testData","testTC.out"),"Output file. ");
  StringC status;
  StringC NL("\n");
  StringC CRNL("\r\n");
  bool test_completed=false;
  bool error_found=false;

  option.Check();
  
  OStreamC out(output);
  TemplateComplexC tf(input);
  
  tf.Build(output);

  TextCursorC readback(output);

  readback.FirstLine();
  while (readback.IsElm())
  {
    status=readback.LineText();
    if (status.matches(NL)==false && status.matches(CRNL)==false)
    {
      if (status.matches("Test successfully reached the end", 0, false))
      {
        test_completed=true;
      }
      else
      {
        error_found=true;
        cerr << status << NL;
      }
    }
    readback.NextLine();
  }
  
  if ( test_completed==true && error_found==false)
  {
    return 0;
  }
  else
  {
    return 1;
  }
}

