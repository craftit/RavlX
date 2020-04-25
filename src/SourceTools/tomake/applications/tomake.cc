// This file is part of VCPP, Visual C++ porting tool for QMake projects 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU General 
// Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
//////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/SourceTools/ToMake/tomake.cc"
//! docentry="Ravl.API.Source Tools"
//! author="Charles Galambos"
//! userlevel=Normal

#include "Ravl/Option.hh"
#include "Ravl/Tuple2.hh"
#include "Ravl/SourceTools/DefsFile.hh"
#include "Ravl/SourceTools/SourceCodeManager.hh"
#include "Ravl/SourceTools/SourceFile.hh"
#include "Ravl/Text/TextFile.hh"
#include "Ravl/Text/ConfigFile.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/OS/Directory.hh"
#include "Ravl/Text/TemplateFile.hh"
#include "Ravl/Text/TemplateComplex.hh"

#define DODEBUG	0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

using namespace RavlN;

// If PROJECT_OUT isn't defined, make one up.
bool verb = false;
bool dryRun = false;
bool setFileloc = false;
HashC<Tuple2C<StringC,StringC>,TextFileC> srcHeaders;// Tuple is license, file extension
HashC<StringC,StringC> defaultVars;
StringC defaultLicense = "LGPL";
StringC defaultOrganisation;
StringC defaultName;
StringC defaultDesc;
StringC subdirs;
StringC platform;
StringC templateFile;
DirectoryC installDir;
DirectoryC incDir;
DirectoryC libDir;
DirectoryC binDir;

#ifndef PROJECT_OUT
#define PROJECT_OUT "."
#endif


static bool CheckPlatform(DefsMkFileC &defs)
{
  StringC supp = defs["SUPPORT_ONLY"];
  StringC dontsupp = defs["DO_NOT_SUPPORT"];
   
  if(dontsupp.contains(platform)) return false;
  if(supp.contains(platform)) return true;
  if(supp.IsEmpty()) return true;
 
  return false;
}

static bool CheckDirectory(StringC &dir,DefsMkFileC &defs) {

  cerr << "Checking '" << dir << "' \n";

  //: We need to check that linux is supported

  if(CheckPlatform(defs)) {

    subdirs += dir + " ";
    //: Set up the output Makefile and the relevant template file
    StringC outFile = dir + "/Makefile";
    cout << "Writing makefile: " << outFile << endl;
    TemplateComplexC makefile(templateFile);

    //: first lets set the output directories
    makefile.SetVar("platform", platform);
    makefile.SetVar("libdir", libDir);
    makefile.SetVar("incdir", incDir);
    makefile.SetVar("bindir", binDir); 

    //: lets make the headers
    StringC package = defs["PACKAGE"];
    if(package.IsEmpty()) package = (StringC)"Ravl";
    makefile.SetVar("package", package);
    StringC headers = defs["HEADERS"];
    if(!headers.IsEmpty()) makefile.SetVar("headers", headers);
    
    //: Lets see if we want to make a library
    StringC libname = defs["PLIB"];
    StringC sources = defs["SOURCES"];
    sources.gsub(".cc", ".o");
    sources.gsub(".c", ".o");
    if(!libname.IsEmpty()) makefile.SetVar("libname", libname);
    if(!sources.IsEmpty()) makefile.SetVar("sources", sources);
    
    //: OK special case of MUSTLINK
    StringC mustlink = defs["MUSTLINK"];
    if(!mustlink.IsEmpty()) makefile.SetVar("mustlink", mustlink);

    //: Lets make the example executables
    StringC examples = defs["EXAMPLES"];
    if(!examples.IsEmpty()) makefile.SetVar("examples", examples);

    //: Lets make the main executables
    StringC mains = defs["MAINS"];
    if(!mains.IsEmpty()) makefile.SetVar("mains", mains);

    //: Lets make the test executables
    StringC testexes = defs["TESTEXES"];
    if(!testexes.IsEmpty()) makefile.SetVar("testexes", testexes);

    //: Lets make the nested directories
    StringListC nestedlist = defs.Nested();
    StringC nested = "";
    for(DLIterC<StringC>It(nestedlist);It.IsElm();It.Next()) {
      nested += It.Data() + " ";
    }
    if(!nested.IsEmpty()) makefile.SetVar("nested", nested);

    //: what about the USESLIBS and PROGLIBS
    StringC libs = defs["USESLIBS"];
    if(!libs.IsEmpty()) libs += " ";
    libs += defs["PROGLIBS"];
    if(!libs.IsEmpty()) makefile.SetVar("libs", libs);

    //: Finally lets build the template file
    makefile.Build(outFile);
  }

  return true;
}

static bool CopyHeaders(StringC &dir,DefsMkFileC &defs) {

  //: We need to check that linux is supported

  if(CheckPlatform(defs)) {
    StringC headers = defs["HEADERS"];
    StringC package = defs["PACKAGE"];
    StringListC str(headers);
    for(DLIterC<StringC>It(str);It.IsElm();It.Next()) {
      DirectoryC toDir = incDir + "/" + package;
      if(!toDir.Exists()) toDir.Create();
      FilenameC from = dir + "/" + It.Data();
      FilenameC to = toDir + "/" + It.Data();
      cout << "copying: " << from << " to " << toDir << endl << flush;
      if(!from.CopyTo(to)) {
	    IssueError(__FILE__,__LINE__, "Unable to copy file");
      }
    }
  }
  
  return true;
}

int main(int nargs,char **argv) {

  OptionC option(nargs,argv);
  StringC fn = option.String("i",".","Input filename. ");
  installDir = option.String("o", "", "absolute path of desired install directory");
  dryRun = option.Boolean("d",false,"Do a dry run. Don't change anything. ");
  setFileloc = option.Boolean("fl",setFileloc,"If true the file location will be updated. ");
  bool all = option.Boolean("a",true,"Go into inactive directories as well. ");
  templateFile = option.String("templ", PROJECT_OUT "/share/RAVL/AutoPort/Makefile.tmpl", "The template file for each directory.");
  StringC MasterMakefile = option.String("master", PROJECT_OUT "/share/RAVL/AutoPort/MasterMakefile.tmpl", "The master makefile pulling all directories together.");
  platform = option.String("platform", "linux", "What platform to make it for");

  verb = option.Boolean("v",false,"Verbose mode.");
  option.Compulsory("o");
  option.Check();


  //: First thing to do is to check install directories are there (e.t.c)
  incDir = installDir + "/inc";
  libDir = installDir + "/lib";
  binDir = installDir + "/bin";

  // if(!installDir.Exists()) installDir.Create();
  // if(!incDir.Exists()) incDir.Create();
  // if(!libDir.Exists()) libDir.Create();
  // if(!binDir.Exists()) binDir.Create();

  //: Next thing to do is to go through all directories and copy the
  //: header files across
  SourceCodeManagerC chkit(fn);
  //chkit.ForAllDirs(CallFunc2C<StringC,DefsMkFileC,bool>(&CopyHeaders),all);
 
  
  //: Next, we can go and build all the Makefiles in the seperate
  //: directories
  subdirs = (StringC)"";
  if(verb)
    chkit.SetVerbose(true);
  chkit.ForAllDirs(CallFunc2C<StringC &,DefsMkFileC&,bool>(&CheckDirectory),all);
  
  //: Finally we need to create the basic Makefile that holds it
  //: altogether
  //StringC masterMake = fn + "/Makefile";
  //OStreamC ofs(masterMake);
  //TemplateFileC mainMakefile(MasterMakefile, ofs);
  //mainMakefile.SetVar("subdirs", subdirs);
  //mainMakefile.Build();

  return 0;
}

// -----------------------------------------------------------------------------
