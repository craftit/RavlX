// This file is part of VCPP, Visual C++ porting tool for QMake projects 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU General 
// Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
//////////////////////////////////////////
//! rcsid="$Id: autoport.cc 5298 2006-01-06 11:56:18Z kier $"
//! file="Ravl/SourceTools/VisualCPP/autoport.cc"
//! userlevel=Normal
//! docentry="Ravl.API.Source Tools.AutoPort"
//! author="Charles Galambos"
//! lib=RavlAutoPort2

#include "Ravl/Option.hh"

#include "Ravl/AutoPort2/AutoPortSource.hh"
#include "Ravl/AutoPort2/AutoPortGenerator.hh"
#include "Ravl/OS/Directory.hh"
#include "Ravl/Stream.hh"
#include "Ravl/IO.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

using namespace RavlN;

// If PROJECT_OUT isn't defined, make one up.

#ifndef PROJECT_OUT
#define PROJECT_OUT "."
#endif

void BuildTemplates(StringC &templFile,AutoPortSourceC &src,StringC &outFile,const StringC &projectOut, const ExtLibTableC & extLibs) {
  if(FilenameC(templFile).IsDirectory()) {
    cout << "Processing templates in :'" << templFile << "'\n";
    DirectoryC dir(templFile);
    DListC<StringC> fl = dir.FiltList("*.tmpl");
    
    // Make sure GUID's are generated first.
    for(DLIterC<StringC> it(fl);it;it++) {
      if(!it->contains("guid"))
	continue;
      
      StringC subDir = dir + filenameSeperator + *it;      
      ONDEBUG(cerr << "Templates in :'" << subDir << "'\n");
      BuildTemplates(subDir,src,outFile,projectOut,extLibs);
    }
    
    // Do the rest
    for(DLIterC<StringC> it(fl);it;it++) {
      if(it->contains("guid"))
	continue;
      
      StringC subDir = dir + filenameSeperator + *it;
      ONDEBUG(cerr << "Templates in :'" << subDir << "'\n");
      BuildTemplates(subDir,src,outFile,projectOut,extLibs);
    }
    return;
  }
  cout << "Processing template file : '" << templFile << "'\n";
  
  
  AutoPortGeneratorC fg(src,templFile,outFile,projectOut,extLibs);
  fg.BuildFiles();
}

// -----------------------------------------------------------------------------

int main(int nargs,char **argv) {
  
  OptionC option(nargs,argv,true);

  StringC fn    = option.String("i",".",     "(input)  Directory for unix src tree. ");
  StringC fout  = option.String("o","MSVC",      "(output) Directory for VCPP src tree.  ");
  StringC pathtempl= option.String("pt", PROJECT_OUT "/share/RAVL/AutoPort2","Where to look for template files.");
  StringC projectOut= option.String("p", "C:/Build","Project out to use in windows. (With UNIX style seperators)");
  StringC doLibsFile =option.String("dl", "", "Text file containing names of Ravl libraries to port.  If no file provided all libraries will be done");
  DListC<StringC> resourceList = option.List("r","libGTK LibDirectShow","List of resources to use. ");
  projectOut.gsub("/","\\"); // "../ProjectOut"
  
  //bool rec    = option.Boolean("r",true,    "recurse into subdirectories. ");
  //bool all    = option.Boolean("a",false,   "Go into inactive directories as well. ");
  bool verb   = option.Boolean("v",false,   "Verbose mode.");
  
  option.Check();
  
  // Put list of resources into a set
  HSetC<StringC> resources;
  for(DLIterC<StringC> it(resourceList);it;it++)
    resources += *it;
  
  // Load info about external libraries
  InitExtLibFormat();
  InitExtLibTableFormat();
  
  // Look for local version of externalLib first.
  FilenameC inFile = "externalLib.xml";
  if(!inFile.Exists())
    inFile = pathtempl + '/' + inFile;
  
  ExtLibTableC extLibs;
  if(!Load(inFile, extLibs))
    RavlIssueError("Unable to open external libs file: " + inFile);
  
  // Load in doLibs file if provided
  DListC<StringC> doLibs;
  if(option.IsOnCommandLine("dl")) {
    TextFileC file(doLibsFile);
    for(UIntT i=1;i<=file.NoLines();i++) {
      StringC line = file[i];
      line = line.TopAndTail();
      doLibs.InsLast(line);
    }
  }
  
  
  AutoPortSourceC portInfo(fn, doLibs,resources);
  if(verb) {
    portInfo.Dump();
    portInfo.SetVerbose(verb);
  }
  BuildTemplates(pathtempl,portInfo,fout,projectOut,extLibs);
  return 0;
}

// -----------------------------------------------------------------------------
