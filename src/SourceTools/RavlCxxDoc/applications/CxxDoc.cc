// This file is part of CxxDoc, The RAVL C++ Documentation tool 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU General 
// Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
//////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlCxxDoc
//! file="Ravl/SourceTools/CxxDoc/CxxDoc.cc"
//! docentry="Ravl.API.Source Tools.CxxDoc"
//! author="Charles Galambos"
//! userlevel=Normal

//: C++ Documentation Tool
// This executable is designed to work the QMake 'doc' command to produce
// documentation for C++ libraries and applications.

#include "Ravl/CxxDoc/Parser.hh"
#include "Ravl/CxxDoc/Document.hh"
#include "Ravl/CxxDoc/DocTree.hh"
#include "Ravl/CxxDoc/Executables.hh"
#include "Ravl/Option.hh"
#include "Ravl/EntryPnt.hh"
#include "Ravl/DList.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/OS/Directory.hh"
#include "Ravl/SourceTools/DefsFile.hh"
#include "Ravl/SourceTools/SourceCodeManager.hh"
#include "Ravl/SourceTools/SourceFile.hh"
#include "Ravl/Resource.hh"

#ifndef NDEBUG
#define YYDEBUG 1
#else
#define YYDEBUG 0
#endif

#include <stdlib.h>


using namespace RavlN;
using namespace RavlCxxDocN;

#if YYDEBUG
extern "C" {
  extern int yydebug;
}
#endif

extern bool stopOnError;
extern bool verbose;

StringC projName;
StringC projDesc;

enum ExeTypeT { ET_Application,ET_Example,ET_Test };
  

void BuildTemplates(FilenameC templFile,RavlCxxDocN::ObjectListC &ol,StringC &outFile,DocTreeC &dt) {
  if(templFile.IsDirectory()) {
    DirectoryC dir(templFile);
    DListC<StringC> fl = dir.FiltList("*.tmpl");
    for(DLIterC<StringC> it(fl);it;it++) 
      BuildTemplates(dir + filenameSeperator + *it,ol,outFile,dt);
    return;
  }
  
  RavlCxxDocN::DocumentC doc(templFile,outFile,dt,projName,projDesc);
  doc.Document(ol);
}

//: Read a source file for an exe.

bool ReadExeCode(const StringC &fileName,ExeTypeT et,RavlCxxDocN::ParserC &pt) {
  std::cerr << "DocTreeBodyC::ReadExeCode(), Reading " << fileName << "\n";
  SourceFileC tf(fileName);
  
  HashC<StringC,StringC> vars;
  StringC name = FilenameC(fileName).NameComponent();
  ObjectExeC oexe(name.before('.')); 
  oexe.SetVar("source",fileName);
  switch(et) {
  case ET_Application: oexe.SetVar("exetype","application"); break;
  case ET_Example:     oexe.SetVar("exetype","example");     break;
  case ET_Test:        oexe.SetVar("exetype","test");        break;
  default:
    break;
  }
  
  tf.GetDocVars(oexe.Comment().Vars());
  if(!tf.LeadingComment(oexe.Comment().Header(),oexe.Comment().Text())) {
    //    std::cerr << "Failed to find leading comment for '" << fileName << "'\n";
    // Check if vars set.
    oexe.Comment().Header() = oexe.Comment().Vars()["brief"];
    oexe.Comment().Text() = oexe.Comment().Vars()["detail"];
  } else {
    oexe.Comment().Vars()["brief"] = oexe.Comment().Header();
    oexe.Comment().Vars()["detail"] = oexe.Comment().Text();
  }
  pt.Data().Append(oexe);
  return true;
}


bool DocSource(StringC &dir,DefsMkFileC &defs,RavlCxxDocN::ParserC &pt) {
  StringListC mains = defs["MAINS"];
  StringListC examples = defs["EXAMPLES"];
  StringListC testexes = defs["TESTEXES"];
  DLIterC<StringC> it(mains);
  for(;it;it++) {
    FilenameC fn(dir + filenameSeperator + *it);
    if(!fn.Exists()) {
      std::cerr << "Can't find source code for '" << fn << "'\n";
      continue;
    }
    ReadExeCode(fn,ET_Application,pt);
  }
  it = examples;
  for(;it;it++) {
    FilenameC fn(dir + filenameSeperator + *it);
    if(!fn.Exists()) {
      std::cerr << "Can't find source code for '" << fn << "'\n";
      continue;
    }
    ReadExeCode(fn,ET_Example,pt);
  }
  it = testexes;
  for(;it;it++) {
    FilenameC fn(dir + filenameSeperator + *it);
    if(!fn.Exists()) {
      std::cerr << "Can't find source code for '" << fn << "'\n";
      continue;
    }
    ReadExeCode(fn,ET_Test,pt);
  }
  return true;
}


int BuildCxx(int argc, char **argv)
{
  // Setup default location for project out.
  const char *prjo = getenv("PROJECT_OUT");
  if(prjo == 0) 
    prjo = PROJECT_OUT; // Hmmm ... where else ?
  
  OptionC opt(argc,argv);
  projName = opt.String("pn", "MyProject", "Project Name. ").TopAndTail();
  projDesc = opt.String("pd", projName + " Documentation", "Project description. ").TopAndTail();

  StringC localProjOut = opt.String("p", prjo, "Project out. ");
  StringC inFiles = opt.String("i", localProjOut + "/include" , "Directory containing header files");
  StringC outFile = opt.String("o", localProjOut + "/share/doc/" + projName + "/Auto", "output document");

  StringC sourceTree = opt.String("st", ".", "Source tree.");

  StringC instAdmin = opt.String("ia", localProjOut + "/transient", "Install Admin.");
  StringC ehtFiles = opt.String("eht", instAdmin + "/CxxDoc/EHT", "Location of EHT files. ");
  StringC installHome = opt.String("ih", ResourceRoot() + "/share/CxxDoc", "Install Home.");
  StringC templFiles = opt.String("tc", installHome + "/default/Class", "Directory of template files for class pages, or single template file");
  StringC docNodeFiles = opt.String("td", installHome + "/default/DocNode", "Directory of template files for doc node page, or single template file");
  
  stopOnError = opt.Boolean("s",false,"Stop on error. ");
  bool noParse = opt.Boolean("np",false,"Don't do a header parse. ");
  verbose = opt.Boolean("v",false,"Verbose ");
#if YYDEBUG
  yydebug = opt.Boolean("d",false,"Enable parser debuging.");
#endif
  bool dump = opt.Boolean("l",false,"List database contents. ");
  bool doDoc = opt.Boolean("w",true,"Write documentation. ");
  
  opt.Check();

  RavlCxxDocN::ParserC pt(true);
  
  if(!noParse)
    pt.ParseTree(inFiles);

  if(verbose)
    std::cerr << "Resolving references.\n";
  pt.Resolve();
  
  DocTreeC docTree(projName);
  if(ehtFiles != "") {
    if(verbose)
      std::cerr << "Reading EHT files. \n";
    docTree.ReadEHTSet(ehtFiles);
  }
  if(verbose)
    std::cerr << "Checking source tree.\n";
  if(!sourceTree.IsEmpty()) {
    SourceCodeManagerC chkit(sourceTree);
    if(verbose)
      chkit.SetVerbose(true);  
    DefsMkFileC tmp;
    chkit.ForAllDirs(CallFunc3C<StringC&,DefsMkFileC&,RavlCxxDocN::ParserC &,bool>(&DocSource,sourceTree,tmp,pt),false);
  }
  if(dump) {
    std::cerr << "C++ Tree:\n";
    pt.Dump(std::cout); // Dump parse tree.
    std::cerr << "Docnodes:\n";
    docTree.Dump(std::cout);
  }
  
  if(verbose)
    std::cerr << "Building documentation.\n";
  
  if(doDoc) {
    pt.Data().Append(docTree.Root());
    BuildTemplates(templFiles,pt.Data(),outFile,docTree);
    if(docNodeFiles != "")
      BuildTemplates(docNodeFiles,pt.Data(),outFile,docTree);
  }
  return 0;
}


//: This puts a wrapper around the main program that catches
//: exceptions and turns them into readable error messages.

RAVL_ENTRY_POINT(BuildCxx);
