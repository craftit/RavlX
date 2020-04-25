// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
// 24-Feb-93, J. Matas, created.
// 22-Oct-93 Radek Marik adopted for C++
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/System/exOption.cc"
//! date="12/11/1993"
//! userlevel=Basic
//! docentry="Ravl.API.Core.Misc"

#include "Ravl/Stream.hh"
#include "Ravl/Option.hh"

using namespace RavlN;

int 
main(int argc, char **argv)
{
  OptionC  option(argc,argv,true);
  option.Comment("exOption: an example of using OptionC class");
  option.Comment("===========================================");
  
  IntT  a = option.Int("alpha",10,"first parameter");
  IntT  d = option.Int("beta",a/10,"lower threshold");
  RealT f = option.Real("pi",3.14,"define you own pi");
  
  StringC filename  = option.String("i","","input filename");
  unsigned char sw = option.UnsignedChar("filter",'c',
					 "switch selector for my filter");
  bool expand   = option.Boolean("expand","expand my panel");
  StringC optionN    = option.String("o", "", "option string");
  DListC<StringC> opts = option.List("x","List of strings. ");
  StringC file1 = option.String("","","test file 1");
  StringC file2 = option.String("","","test file 2");
  //DListC<StringC> opts2 = option.List(0,"The rest ");
  
  option.CompulsoryArgs(1);
  option.Compulsory("i");
  option.DependXor("pi filter");
  //option.DependIf("filter",'z',sw,"expand");
  
  option.Check();
  
  if (option.IsOnCommandLine(optionN))
    std::cerr << "The option " << optionN << " is on command line.\n";
  
  std::cout << "var a:     " << a << '\n'
       << "var d:     " << d << '\n'
       << "var f:     " << f << '\n'
       << "switch:    " << sw << '\n'
       << "expand:    " << expand << '\n'
       << "filename:  " << filename << '\n'
       << "file 1:    " << file1 << '\n'
       << "file 2:    " << file2 << '\n';
  
  std::cout << "List=";
  for(DLIterC<StringC> it(opts);it;it++)
    std::cout << "'" << *it << "' ";
  std::cout << "\n";
  option.PrintUsage();
  return 0;
} 
