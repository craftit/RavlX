

// This file is part of VCPP, Visual C++ porting tool for QMake projects
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU General
// Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
//////////////////////////////////////////
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
DirectoryC g_incDir;
DirectoryC g_srcDir;
DirectoryC libDir;
DirectoryC binDir;

#ifndef PROJECT_OUT
#define PROJECT_OUT "."
#endif

void Remove(StringListC &sl,const StringC &str)
{
  for(DLIterC<StringC> it(sl);it;it++) {
    if(*it == str) {
      it.Del();
      return ;
    }
  }
}

void DeDup(StringListC &sl)
{
  for(DLIterC<StringC> it(sl);it;it++) {
    DLIterC<StringC> it2 = it;
    it2++;
    if(!it2)
      break;
    for(;it2;it2++) {
      if(*it2 == *it) {
        it2.Del();
        it2--;
      }
    }
  }
}

class LibraryInfoC
{
public:
  LibraryInfoC()
  {}

  void Tidy()
  {
    DeDup(m_usesLibs);
    DeDup(m_progLibs);
    Remove(m_usesLibs,m_name);
    Remove(m_usesLibs,"None");
    Remove(m_progLibs,m_name);
  }

  StringC m_path;
  StringC m_name;
  StringListC m_headers;
  StringListC m_sources;
  StringListC m_testExes;
  StringListC m_mainExes;
  StringListC m_usesLibs;
  StringListC m_progLibs;
  StringListC m_examples;
  StringListC m_exeDepends;
  StringListC m_mustLinks;
  HashC<StringC,StringListC> m_auxFiles;
};

RavlN::HashC<StringC,LibraryInfoC> g_libraries;

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

    if(!CheckPlatform(defs))
      return true;
    subdirs += dir + " ";
    //: Set up the output Makefile and the relevant template file
    StringC outFile = dir + "/Makefile";
    cout << "Writing makefile: " << outFile << endl;
    TemplateComplexC makefile(templateFile);

    //: first lets set the output directories
    makefile.SetVar("platform", platform);
    makefile.SetVar("libdir", libDir);
    makefile.SetVar("incDir", g_incDir);
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

  return true;
}

bool CopyFiles(const StringC &dir,const StringC &toLib,const StringC &sources,const StringC &dest,StringListC &fileList)
{
  StringListC str(sources);
  DirectoryC toDir = toLib + "/" + dest;
  if (!toDir.Exists()) toDir.Create();
  for (DLIterC <StringC> It(str); It.IsElm(); It.Next()) {
    FilenameC from = dir + "/" + It.Data();
    FilenameC to = toDir + "/" + It.Data();
    StringC subPath = dest + "/" + It.Data();
    fileList.Append(subPath);
    cout << "copying: " << from << " to " << toDir << endl << flush;
    if (!dryRun && !from.CopyTo(to)) {
      IssueError(__FILE__, __LINE__, "Unable to copy file");
    }
  }
  return true;
}

static bool CopySources(StringC &dir,DefsMkFileC &defs) {

  //: We need to check that linux is supported

  if(!CheckPlatform(defs))
      return true;

  StringC targetLib = defs["PLIB"];
  if(targetLib.IsEmpty()) {
    std::cout <<"No target library in " << dir << "\n";
    return true;
  }

  LibraryInfoC &libInfo = g_libraries[targetLib];

  DirectoryC toLib = installDir + "/src/" + targetLib;
  if (!toLib.Exists()) toLib.Create();
  // Need to initialise info ?
  if(libInfo.m_name != targetLib) {
    libInfo.m_name = targetLib;
    libInfo.m_path = toLib;
  }

  StringC headers = defs["HEADERS"];
  StringC package = defs["PACKAGE"];
  {
    StringListC str(headers);
    DirectoryC toDir = toLib + "/include/" + package;
    if (!toDir.Exists()) toDir.Create();

    for (DLIterC <StringC> It(str); It.IsElm(); It.Next()) {
      FilenameC from = dir + "/" + It.Data();
      StringC subPath = "include/" + package + "/" + It.Data();
      libInfo.m_headers.Append(subPath);
      FilenameC to = toLib + "/" + subPath;
      cout << "copying: " << from << " to " << toDir << endl << flush;
      if (!dryRun && !from.CopyTo(to)) {
        IssueError(__FILE__, __LINE__, "Unable to copy file");
      }
    }
  }

  CopyFiles(dir,toLib,defs["SOURCES"],"src",libInfo.m_sources);
  CopyFiles(dir,toLib,defs["EXAMPLES"],"examples",libInfo.m_examples);
  CopyFiles(dir,toLib,defs["TESTEXES"],"tests",libInfo.m_testExes);

  {
    StringC auxFiles = defs["AUXFILES"];
    StringC auxDir = defs["AUXDIR"];

    StringListC str(auxFiles);
    DirectoryC toDir = toLib + "/data/" + auxDir;
    if (!toDir.Exists()) toDir.Create();

    for (DLIterC <StringC> It(str); It.IsElm(); It.Next()) {
      FilenameC from = dir + "/" + It.Data();
      FilenameC to = toDir + "/" + It.Data();
      StringC subPath = "data/" + auxDir + "/" + It.Data();
      libInfo.m_auxFiles[auxDir].Append(subPath);
      cout << "copying: " << from << " to " << toDir << endl << flush;
      if (!dryRun && !from.CopyTo(to)) {
        IssueError(__FILE__, __LINE__, "Unable to copy file");
      }
    }
  }
  {
    StringListC str(defs["USESLIBS"]);
    libInfo.m_usesLibs.MoveLast(str);
  }
  {
    StringListC str(defs["PROGLIBS"]);
    libInfo.m_progLibs.MoveLast(str);
  }

  return true;
}


class CMakeGenBodyC
    : public TemplateComplexBodyC
{
public:
  CMakeGenBodyC(const StringC &templFilename,LibraryInfoC &libInfo)
   : TemplateComplexBodyC(templFilename),
     m_libInfo(libInfo)
  {
    Init();
  }

  void Init() {
    SetupCommand("forall",*this,&CMakeGenBodyC::ForAll);
    SetVar("lib",m_libInfo.m_name);
    SetVar("path",m_libInfo.m_path);
    SetVar("useslibs",m_libInfo.m_usesLibs.Cat(" "));
    SetVar("proglibs",m_libInfo.m_progLibs.Cat(" "));
    SetVar("sources",m_libInfo.m_sources.Cat(" "));
    SetVar("headers",m_libInfo.m_headers.Cat(" "));
    SetVar("examples",m_libInfo.m_examples.Cat(" "));
    SetVar("testexes",m_libInfo.m_testExes.Cat(" "));
    SetVar("mainexes",m_libInfo.m_mainExes.Cat(" "));
    SetVar("mustlinks",m_libInfo.m_mustLinks.Cat(" "));

  }

  bool ForAll(StringC &data);

  LibraryInfoC &m_libInfo;
};

bool CMakeGenBodyC::ForAll(StringC &data)
{
  int templStart = data.index(':');
  if(templStart < 1) {
    std::cerr << "Malformed 'forall' in template. '" << data << "' ignoring \n";
    return false;
  }
  StringC typedata = data.before(templStart);
  StringListC strList;
  if(typedata == "useslibs") {
    strList = m_libInfo.m_usesLibs;
  } else if(typedata == "examples") {
    strList = m_libInfo.m_examples;
  } else if(typedata == "testexes") {
    strList = m_libInfo.m_testExes;
  } else if(typedata == "mainexes") {
    strList = m_libInfo.m_mainExes;
  } else if(typedata == "mustlinks") {
    strList = m_libInfo.m_mustLinks;
  }else {
    std::cerr << "Unknown forall group " << typedata << std::endl;
  }
#if 1
  StringC subtempltxt = data.after(templStart);
  for(DLIterC<StringC> it(strList);it;it++) {
    vars.Push(RCHashC<StringC,StringC>(vars.Top().Copy()) ); // Push base variables.
    SetVar("src",*it);
    FilenameC fn(*it);
    SetVar("exename",fn.BaseNameComponent());
    TextFileC subTextBuff(subtempltxt,true,true);
    BuildSub(subTextBuff);
    vars.DelTop(); // Restore old set.
  }
#endif

  return true;
}

class CMakeGenC
    : public TemplateComplexC
{
public:
  CMakeGenC(const StringC &templFilename,LibraryInfoC &libInfo)
   : TemplateComplexC(*new CMakeGenBodyC(templFilename,libInfo))
  {}
};


bool MakeList(LibraryInfoC &libInfo) {
  StringC dir = libInfo.m_name;
  StringC outFile = installDir + "/src/" + dir + "/CMakeLists.txt";
  cout << "Writing cmake file : " << outFile << endl;
  CMakeGenC gen(templateFile,libInfo);
  gen.Build(outFile);

  //  makefile.SetVar("sources", sources);

  return true;
}


int main(int nargs,char **argv) {

  OptionC option(nargs,argv);
  StringC fn = option.String("i",".","Input filename. ");
  installDir = option.String("o", "", "absolute path of desired install directory");
  dryRun = option.Boolean("d",false,"Do a dry run. Don't change anything. ");
  setFileloc = option.Boolean("fl",setFileloc,"If true the file location will be updated. ");
  bool all = option.Boolean("a",true,"Go into inactive directories as well. ");
  templateFile = option.String("templ", PROJECT_OUT "/share/RAVL/AutoPort/CMakeLists.txt.tmpl", "The template file for each directory.");
  StringC MasterMakefile = option.String("master", PROJECT_OUT "/share/RAVL/AutoPort/CMakeRoot.txt.tmpl", "The master makefile pulling all directories together.");
  platform = option.String("platform", "linux", "What platform to make it for");

  verb = option.Boolean("v",false,"Verbose mode.");
  option.Compulsory("o");
  option.Check();


  //: First thing to do is to check install directories are there (e.t.c)
  g_incDir = installDir + "/include";
  g_srcDir = installDir + "/src";
  libDir = installDir + "/lib";
  binDir = installDir + "/bin";

  if(!installDir.Exists()) installDir.Create();
  if(!g_incDir.Exists()) g_incDir.Create();
  if(!g_srcDir.Exists()) g_srcDir.Create();
  //if(!libDir.Exists()) libDir.Create();
  // if(!binDir.Exists()) binDir.Create();

  //: Next thing to do is to go through all directories and copy the
  //: header files across
  SourceCodeManagerC chkit(fn);
  chkit.ForAllDirs(CallFunc2C<StringC&,DefsMkFileC&,bool>(&CopySources),all);

  StringC libs;
  for(HashIterC<StringC,LibraryInfoC> it(g_libraries);it;it++) {
    it.Data().Tidy();
    MakeList(it.Data());
    if(!libs.IsEmpty())
      libs += " ";
    libs += it.Key();
  }

  {
    StringC outFile = installDir + "/CMakeLists.txt";
    TemplateComplexC gen(MasterMakefile);

    gen.SetVar("libs",libs);
    gen.Build(outFile);
  }

  return 0;
}

// -----------------------------------------------------------------------------
