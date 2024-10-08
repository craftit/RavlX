

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
#include <cassert>

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
StringC g_configTemplateFile;
DirectoryC installDir;
DirectoryC g_incDir;
DirectoryC g_srcDir;
DirectoryC libDir;
DirectoryC binDir;
HSetC<StringC> g_usedLibs;
HSetC<StringC> g_localLibs;

StringC g_rootDir;
bool g_genMakeFilesOnly = false;

#ifndef PROJECT_OUT
#define PROJECT_OUT "."
#endif

bool Contains(StringListC &sl,const StringC &str)
{
  for(DLIterC<StringC> it(sl);it;it++) {
    if(*it == str)
      return true;
  }
  return false;
}

void Remove(StringListC &sl,const StringC &str)
{
  for(DLIterC<StringC> it(sl);it;it++) {
    if(*it == str) {
      it.Del();
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
        assert(it2 != it);
        it2.Del();
      }
    }
  }
}

void RemoveFrom(StringListC &sl,const StringListC &other)
{
  for(DLIterC<StringC> it(sl);it;it++) {
    for(DLIterC<StringC> it2(other);it2;it2++) {
      if(*it2 == *it) {
        assert(it2 != it);
        it.Del();
        break;
      }
    }
  }
}

class ExtLibraryInfoC
{
public:
  StringC m_name;
  StringC m_libname;
  StringC m_foundFlag;
  StringC m_link;
  StringC m_include;
  StringC m_library_path;
  StringC m_findCmd;
};

HashC<StringC,ExtLibraryInfoC> g_extLibraries;
StringListC m_orderedExtLibraries;

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
    RemoveFrom(m_examples,m_testExes);
    RemoveFrom(m_examples,m_mainExes);
    RemoveFrom(m_testExes,m_mainExes);
    auto list = m_progLibs.Copy();
    m_allLibs = m_usesLibs.Copy();
    m_allLibs.MoveLast(list);
    DeDup(m_allLibs);
    for(DLIterC<StringC> it(m_allLibs);it;it++) {
      if(it->index(".opt") >= 0) {
        StringC libName = it->before(".opt");
        m_optLibs.InsLast(libName);
        it.Del(); // Remove it from allLibs
      }
    }
    for(DLIterC<StringC> it(m_usesLibs);it;it++) {
      if(it->index(".opt") >= 0) {
        m_usesLibsOpt.InsLast(it->before(".opt"));
        it.Del(); // Remove it from allLibs
      }
    }
  }

  StringC m_path;
  StringC m_topLevel;
  StringC m_name;
  StringListC m_headers;
  StringListC m_sources;
  StringListC m_testExes;
  StringListC m_mainExes;
  StringListC m_usesLibs;
  StringListC m_usesLibsOpt;
  StringListC m_progLibs;
  StringListC m_allLibs;
  StringListC m_optLibs;
  StringListC m_examples;
  StringListC m_exeDepends;
  StringListC m_mustLinks;
  StringListC m_requires;
  StringListC m_supports;
  StringListC m_eht;
  HashC<StringC,StringListC> m_auxFiles; // Aux files indexed by destination directory.
  bool m_isProgram = false;
};

RavlN::HashC<StringC,LibraryInfoC> g_libraries;
StringListC g_libOrder;
RavlN::HashC<StringC,StringListC> g_topLevel;
StringListC g_topLevelOrder;
HashC<StringC,StringC> g_libmap;

static bool CheckPlatform(DefsMkFileC &defs)
{
  StringC supp = defs["SUPPORT_ONLY"];
  StringC dontsupp = defs["DO_NOT_SUPPORT"];

  if(dontsupp.contains(platform)) return false;
  if(supp.contains(platform)) return true;
  if(supp.IsEmpty()) return true;

  return false;
}

bool CopyFiles(const StringC &dir,const StringC &toLib,const StringC &sources,const StringC &dest,StringListC &fileList,HSetC<StringC> &done)
{
  StringListC str(sources);
  DirectoryC toDir = toLib + "/" + dest;
  if (!toDir.Exists()) toDir.Create();
  for (DLIterC <StringC> It(str); It.IsElm(); It.Next()) {
    FilenameC from = dir + "/" + It.Data();
    FilenameC to = toDir + "/" + It.Data();
    StringC subPath = dest + "/" + It.Data();
    if(!done[It.Data()]) {
      fileList.Append(subPath);
      done += It.Data();
    }
    cout << "copying: " << from << " to " << toDir << endl << flush;
    if(!g_genMakeFilesOnly) {
      if (It.Data().index('/') >= 0) {
        DirectoryC adir = to.PathComponent();
        if (!adir.Exists())
          adir.Create();
      }
      if (!dryRun && !from.CopyTo(to)) {
        IssueError(__FILE__, __LINE__, "Unable to copy file");
      }
    }
  }
  return true;
}

static bool CopySources(StringC &dir,DefsMkFileC &defs) {

  bool isProgram = false;
  StringC targetLib = defs["PLIB"].TopAndTail();
  if(targetLib.IsEmpty()) {
    StringListC theMains = defs["MAINS"];
    if (theMains.IsEmpty()) {
      std::cout <<"No target in directory " << dir << "\n";
      return true;
    }
    FilenameC targetProg = theMains.First();
    targetLib = targetProg.BaseNameComponent();
    isProgram = true;
  } else {
    g_localLibs += targetLib;
  }

  StringC topLevelDir = dir.after(g_rootDir).after(0);
  int endIndex = topLevelDir.index('/');
  if(endIndex >= 0)
    topLevelDir = topLevelDir.before(endIndex);
  std::cout << "TopLevel: '" << topLevelDir << "'  Root:" << dir.after(g_rootDir).after(0) << std::endl;
  LibraryInfoC &libInfo = g_libraries[targetLib];
  DirectoryC toLib;
  if(topLevelDir.IsEmpty() || topLevelDir == targetLib) {
    topLevelDir = "Misc";
  }

  if(!g_topLevel.IsElm(topLevelDir)) {
    g_topLevelOrder.InsLast(topLevelDir);
  }

  toLib = installDir + "/src/" +topLevelDir + "/" + targetLib;

  if (!toLib.Exists()) toLib.Create();
  // Need to initialise info ?
  if(libInfo.m_name != targetLib) {
    g_topLevel[topLevelDir].InsLast(targetLib);
    g_libOrder.InsLast(targetLib); // Make an ordered list of libraries.
    libInfo.m_name = targetLib;
    libInfo.m_path = toLib;
    libInfo.m_topLevel = topLevelDir;
    libInfo.m_isProgram = isProgram;
    libInfo.m_requires = StringListC(defs["REQUIRES"]," ");

    libInfo.m_supports = StringListC(defs["SUPPORT_ONLY"]," ");
    StringListC doNotSupport(defs["DONOT_SUPPORT"]," ");
    for(DLIterC<StringC> it(doNotSupport);it;it++)
      libInfo.m_supports.InsLast(StringC("!") + *it);
  }

#if 0
  StringC mustLink = defs["MUSTLINK"];
  if(!mustLink.IsEmpty())
    libInfo.m_mustLinks.InsLast(mustLink);
#endif

  StringC headers = defs["HEADERS"].TopAndTail();
  StringC package = defs["PACKAGE"].TopAndTail();
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

  HSetC<StringC> xdone;
  CopyFiles(dir,toLib,defs["SOURCES"],"src",libInfo.m_sources,xdone);
  HSetC<StringC> ydone;
  CopyFiles(dir,toLib,defs["EHT"],"doc",libInfo.m_eht,ydone);
  HSetC<StringC> done;
  CopyFiles(dir,toLib,defs["MAINS"],"applications",libInfo.m_mainExes,done);
  CopyFiles(dir,toLib,defs["TESTEXES"],"tests",libInfo.m_testExes,done);
  CopyFiles(dir,toLib,defs["EXAMPLES"],"examples",libInfo.m_examples,done);
  CopyFiles(dir,toLib,defs["MUSTLINK"],"src",libInfo.m_mustLinks,done);

  StringListC dump;
  CopyFiles(dir,toLib,defs["LOCALHEADERS"],"src",dump,done);


  {
    StringC auxFiles = defs["AUXFILES"];
    StringC auxDir = defs["AUXDIR"];

    StringListC str(auxFiles);
    DirectoryC toDir = toLib + "/data";
    if (!toDir.Exists()) toDir.Create();

    for (DLIterC <StringC> It(str); It.IsElm(); It.Next()) {
      FilenameC from = dir + "/" + It.Data();
      FilenameC to = toDir + "/" + It.Data();
      libInfo.m_auxFiles[auxDir].Append(It.Data());
      StringC subPath = "data/" + It.Data();
      cout << "copying: " << from << " to " << toDir << endl << flush;
      if (!dryRun && !from.CopyTo(to)) {
        IssueError(__FILE__, __LINE__, "Unable to copy file");
      }
    }
  }
  {
    StringListC str(defs["USESLIBS"]);
    for(DLIterC<StringC> it = str;it;it++) {
      StringC add;
      if(it->index(".opt") >= 0) {
        add = it->before(".opt");
      } else {
        add = *it;
      }
      std::cout << "Using '" << add << "' " << std::endl;
      g_usedLibs += add;
    }
    libInfo.m_usesLibs.MoveLast(str);
  }
  {
    StringListC str(defs["PROGLIBS"]);
    for(DLIterC<StringC> it = str;it;it++) {
      StringC add;
      if(it->index(".opt") >= 0) {
        add = it->before(".opt");
      } else {
        add = *it;
      }
      std::cout << "Using '" << add << "' " << std::endl;
      g_usedLibs += add;
    }
    libInfo.m_progLibs.MoveLast(str);
  }

  return true;
}


class CMakeModuleGenBodyC
    : public TemplateComplexBodyC
{
public:
  CMakeModuleGenBodyC(const StringC &templFilename,LibraryInfoC &libInfo)
   : TemplateComplexBodyC(templFilename),
     m_libInfo(libInfo)
  {
    Init();
  }

  void Init() {
    SetupCommand("forall",*this,&CMakeModuleGenBodyC::ForAll);
    SetupCommand("foraux",*this,&CMakeModuleGenBodyC::ForAux);
    SetupCommand("ifany",*this,&CMakeModuleGenBodyC::IfAny);
    SetupCommand("ifreq",*this,&CMakeModuleGenBodyC::IfReq);
    SetupCommand("ifext",*this,&CMakeModuleGenBodyC::IfExt);
    SetupCommand("iflocal",*this,&CMakeModuleGenBodyC::IfLocal);

    SetVar("lib",m_libInfo.m_name);
    SetVar("toplevel",m_libInfo.m_topLevel);
    SetVar("path",m_libInfo.m_path);
    SetVar("useslibs",m_libInfo.m_usesLibs.Cat(" "));
    SetVar("proglibs",m_libInfo.m_progLibs.Cat(" "));
    SetVar("alllibs",m_libInfo.m_allLibs.Cat(" "));
    SetVar("sources",m_libInfo.m_sources.Cat(" "));
    SetVar("headers",m_libInfo.m_headers.Cat(" "));
    SetVar("examples",m_libInfo.m_examples.Cat(" "));
    SetVar("testexes",m_libInfo.m_testExes.Cat(" "));
    SetVar("mainexes",m_libInfo.m_mainExes.Cat(" "));
    if(!m_libInfo.m_mustLinks.IsEmpty()) {
      SetVar("mustlinks", m_libInfo.m_mustLinks.Cat(" "));
    }

    SetVar("requires",m_libInfo.m_requires.Cat(" "));
    SetVar("supports",m_libInfo.m_supports.Cat(" "));
    if(m_libInfo.m_isProgram) {
      SetVar("isprogram", "1");
    }
  }

  bool IfReq(StringC &data);
  bool IfExt(StringC &data);
  bool IfLocal(StringC &data);

  bool ForAuxi(StringC &data,bool ifAny);
  bool ForAux(StringC &data)
  { return ForAuxi(data,false); }

  bool ForAlli(StringC &data,bool ifAny);
  bool ForAll(StringC &data)
  { return ForAlli(data,false); }
  bool IfAny(StringC &data)
  { return ForAlli(data,true); }

  LibraryInfoC &m_libInfo;
};

bool CMakeModuleGenBodyC::IfExt(StringC &data)
{
  int templStart = data.index(':');
  if(templStart < 1) {
    std::cerr << "Malformed 'ifreq' in template. '" << data << "' ignoring \n";
    return false;
  }
  StringC typedata = data.before(templStart);
  StringC subtempltxt = data.after(templStart);
  if(typedata.size() > 0 && typedata.firstchar() == '$') {
    typedata = GetVar(typedata.after(0));
  }
  StringC other = g_libmap[typedata];
  // Check it isn't local or something we've dealt with already
  if(!g_localLibs.IsMember(typedata) && !g_extLibraries.IsElm(other) && !g_extLibraries.IsElm(typedata)) { //
    //vars.Push(RCHashC<StringC,StringC>(vars.Top().Copy()) ); // Push base variables.
    std::cerr << "External: '" << typedata << "' " << std::endl;
    TextFileC subTextBuff(subtempltxt,true,true);
    BuildSub(subTextBuff);
    //vars.DelTop(); // Restore old set.
  }
  return true;
}

bool CMakeModuleGenBodyC::IfLocal(StringC &data)
{
  int templStart = data.index(':');
  if(templStart < 1) {
    std::cerr << "Malformed 'iflocal' in template. '" << data << "' ignoring \n";
    return false;
  }
  StringC typedata = data.before(templStart);
  StringC subtempltxt = data.after(templStart);
  if(typedata.size() > 0 && typedata.firstchar() == '$') {
    typedata = GetVar(typedata.after(0));
  }
  if(g_localLibs.IsMember(typedata)) {
    //vars.Push(RCHashC<StringC,StringC>(vars.Top().Copy()) ); // Push base variables.
    std::cerr << "Local: '" << typedata << "' " << std::endl;
    TextFileC subTextBuff(subtempltxt,true,true);
    BuildSub(subTextBuff);
    //vars.DelTop(); // Restore old set.
  }
  return true;
}


bool CMakeModuleGenBodyC::IfReq(StringC &data)
{
  int templStart = data.index(':');
  if(templStart < 1) {
    std::cerr << "Malformed 'ifreq' in template. '" << data << "' ignoring \n";
    return false;
  }
  StringC typedata = data.before(templStart);
  StringC libName = GetVar("lib");
  LibraryInfoC &li = g_libraries[libName];
  StringC subtempltxt = data.after(templStart);

  if(Contains(li.m_requires,typedata)) {
    //vars.Push(RCHashC<StringC,StringC>(vars.Top().Copy()) ); // Push base variables.
    TextFileC subTextBuff(subtempltxt,true,true);
    BuildSub(subTextBuff);
    //vars.DelTop(); // Restore old set.
  }

  return true;
}

bool CMakeModuleGenBodyC::ForAuxi(StringC &data,bool ifAny)
{
  int templStart = data.index(':');
  if(templStart < 1) {
    std::cerr << "Malformed 'foraux' in template. '" << data << "' ignoring \n";
    return false;
  }
  StringC typedata = data.before(templStart);
  StringC subtempltxt = data.after(templStart);

  if(typedata.size() > 0 && typedata[0] == '$') {
    typedata = GetVar(typedata.after(0));
  }

  for (DLIterC <StringC> it(m_libInfo.m_auxFiles[typedata]); it; it++)
  {
    vars.Push(RCHashC<StringC, StringC>(vars.Top().Copy())); // Push base variables.
    SetVar("file", *it);
    SetVar( "dir",typedata);
    TextFileC subTextBuff(subtempltxt, true, true);
    BuildSub(subTextBuff);
    vars.DelTop(); // Restore old set.
  }

  return true;
}

bool CMakeModuleGenBodyC::ForAlli(StringC &data,bool ifAny)
{
  int templStart = data.index(':');
  if(templStart < 1) {
    std::cerr << "Malformed 'forall' in template. '" << data << "' ignoring \n";
    return false;
  }
  StringC typedata = data.before(templStart);
  StringC subtempltxt = data.after(templStart);

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
  } else if(typedata == "libs") {
    strList = m_libInfo.m_allLibs;
  } else if(typedata == "alllibs") {
    strList = m_libInfo.m_allLibs;
  } else if(typedata == "auxdirs") {
    for(HashIterC<StringC,StringListC> it(m_libInfo.m_auxFiles);it;it++)
      strList.InsLast(it.Key());
  } else if(typedata == "auxfiles") {
    for(HashIterC<StringC,StringListC> it(m_libInfo.m_auxFiles);it;it++) {
      for(DLIterC<StringC> it2(it.Data());it2;it2++)
        strList.InsLast(*it2);
    }
  } else if(typedata == "optlibs") {
    strList = m_libInfo.m_optLibs;
  } else if(typedata == "requires") {
    strList = StringListC(m_libInfo.m_requires);
    if(!ifAny) {
      for (DLIterC <StringC> it(strList); it; it++) {
        vars.Push(RCHashC<StringC, StringC>(vars.Top().Copy())); // Push base variables.
        SetVar("mainlib",GetVar("lib"));

        ExtLibraryInfoC *elibInfo = g_extLibraries.Lookup(*it);
        if(elibInfo != 0) {
          SetVar("lib", elibInfo->m_name);
          SetVar("found", elibInfo->m_foundFlag);
          SetVar("include", elibInfo->m_include);
          SetVar("link", elibInfo->m_link);
          if(!elibInfo->m_findCmd.IsEmpty())
            SetVar("findCmd", elibInfo->m_findCmd);
        } else {
          std::cout << "Unknown requires: '" << *it << "' " << std::endl;
          SetVar("lib", *it);
          SetVar("found", *it + "_FOUND");
          SetVar("include", *it + "_INCLUDE_DIRS");
          SetVar("link", *it + "_LIBRARIES");
          std::cerr << "Unknown external lib " << *it << std::endl;
        }
        TextFileC subTextBuff(subtempltxt, true, true);
        BuildSub(subTextBuff);
        vars.DelTop(); // Restore old set.
      }
      return true;
    }
  } else if(typedata == "supports") {
    strList = StringListC(m_libInfo.m_supports);
    if(!ifAny) {
      for (DLIterC <StringC> it(strList); it; it++) {
        vars.Push(RCHashC<StringC, StringC>(vars.Top().Copy())); // Push base variables.

        ExtLibraryInfoC *elibInfo = g_extLibraries.Lookup(*it);
        if(elibInfo != 0) {
          SetVar("found", elibInfo->m_foundFlag);
        } else {
          SetVar("found", "FALSE");
          std::cerr << "Unknown platform " << *it << std::endl;
        }
        TextFileC subTextBuff(subtempltxt, true, true);
        BuildSub(subTextBuff);
        vars.DelTop(); // Restore old set.
      }
      return true;
    }
  } else {
    std::cerr << "Unknown forall group " << typedata << std::endl;
  }
  if(ifAny) {
    if(!strList.IsEmpty()) {
      TextFileC subTextBuff(subtempltxt, true, true);
      BuildSub(subTextBuff);
    }
  } else {
    for (DLIterC <StringC> it(strList); it; it++) {
      vars.Push(RCHashC<StringC, StringC>(vars.Top().Copy())); // Push base variables.
      SetVar("mainlib",GetVar("lib"));
      LibraryInfoC *libInfo = g_libraries.Lookup(*it);
      if(libInfo != 0) {
        SetVar("path", libInfo->m_path);
        SetVar("toplevel", libInfo->m_topLevel);
      } else {
        SetVar("path",m_libInfo.m_path);
        SetVar("toplevel", m_libInfo.m_topLevel);
      }
      {
        StringC aName = g_libmap[*it];
        if(aName.IsEmpty())
          aName = *it;
        ExtLibraryInfoC *elibInfo = g_extLibraries.Lookup(aName);
        if(elibInfo != 0) {
          //std::cout << "Lookup " << *it << " -> " << aName << " -> " << elibInfo->m_link << std::endl;
          SetVar("linklib", elibInfo->m_link);
          SetVar("inclib", elibInfo->m_include);
        } else {
          //std::cout << "Lookup " << *it << " -> " << aName << " -> ?"  << std::endl;
          SetVar("linklib", *it);
          SetVar("inclib", "");
        }
      }

      SetVar("lib", *it);
      SetVar("src", *it);
      FilenameC fn(*it);
      SetVar("exename", fn.BaseNameComponent());
      SetVar("dir", fn.PathComponent());
      TextFileC subTextBuff(subtempltxt, true, true);
      BuildSub(subTextBuff);
      vars.DelTop(); // Restore old set.
    }
  }

  return true;
}

class CMakeModuleGenC
    : public TemplateComplexC
{
public:
  CMakeModuleGenC(const StringC &templFilename,LibraryInfoC &libInfo)
   : TemplateComplexC(*new CMakeModuleGenBodyC(templFilename,libInfo))
  {}
};



class CMakeTopGenBodyC
        : public TemplateComplexBodyC
{
public:
  CMakeTopGenBodyC(const StringC &templFilename)
    : TemplateComplexBodyC(templFilename)
  {
    Init();
  }

  CMakeTopGenBodyC(const StringC &templFilename,const StringListC &midLibs)
    : TemplateComplexBodyC(templFilename),
      m_midLibs(midLibs)
  {
    Init();
  }

  void Init() {
    SetVar("libs",g_libOrder.Cat(" "));
    SetupCommand("forall",*this,&CMakeTopGenBodyC::ForAll);
    SetupCommand("ifreq",*this,&CMakeTopGenBodyC::IfReq);
    SetupCommand("mapreq",*this,&CMakeTopGenBodyC::MapReq);
    SetupCommand("ifused",*this,&CMakeTopGenBodyC::IfUsed);
  }

  bool ForAll(StringC &data);
  bool IfReq(StringC &data);
  bool IfUsed(StringC &data);
  bool MapReq(StringC &data);

  StringListC m_midLibs;
};

bool CMakeTopGenBodyC::ForAll(StringC &data)
{
  int templStart = data.index(':');
  if(templStart < 1) {
    std::cerr << "Malformed 'forall' in template. '" << data << "' ignoring \n";
    return false;
  }
  StringC typedata = data.before(templStart);
  StringC subtempltxt = data.after(templStart);
  StringListC strList;
  if(typedata == "toplevel") {
    strList = g_topLevelOrder;
  } else if(typedata == "libs") {
    strList = g_libOrder;
  } else if(typedata == "midlibs") {
    strList = m_midLibs;
  } else if(typedata == "extlibs") {
    strList = m_orderedExtLibraries;
    for(DLIterC<StringC> it(strList);it;it++) {
      vars.Push(RCHashC<StringC,StringC>(vars.Top().Copy()) ); // Push base variables.
      SetVar("lib",*it);
      ExtLibraryInfoC &li = g_extLibraries[*it];
      SetVar("found",li.m_foundFlag);
      SetVar("link",li.m_link);
      SetVar( "libname",li.m_libname);
      SetVar( "findCmd",li.m_findCmd);
      if(!li.m_include.IsEmpty())
        SetVar("header_path",li.m_include);
      if(!li.m_library_path.IsEmpty())
        SetVar("library_path",li.m_library_path);
      TextFileC subTextBuff(subtempltxt,true,true);
      BuildSub(subTextBuff);
      vars.DelTop(); // Restore old set.
    }
    return true;
  } else {
    std::cerr << "Unknown forall group " << typedata << std::endl;
  }
#if 1
  for(DLIterC<StringC> it(strList);it;it++) {
    vars.Push(RCHashC<StringC,StringC>(vars.Top().Copy()) ); // Push base variables.
    SetVar("lib",*it);
    LibraryInfoC &li = g_libraries[*it];
    SetVar("toplevel",li.m_topLevel);
    SetVar("requires",li.m_requires.Cat(" "));
    TextFileC subTextBuff(subtempltxt,true,true);
    BuildSub(subTextBuff);
    vars.DelTop(); // Restore old set.
  }
#endif

  return true;
}

bool CMakeTopGenBodyC::MapReq(StringC &data)
{
  int div = data.index(':');
  if(div < 0) {
    std::cerr << "Malformed 'mapreq' in template. 1 '" << data << "' ignoring \n";
    return false;
  }
  StringC typedata = data.before(div);
  ExtLibraryInfoC &eli = g_extLibraries[typedata];
  m_orderedExtLibraries.InsLast(typedata);
  eli.m_name = typedata;

  StringC subtempltxt = data.after(div);
  {
    div = subtempltxt.index(':');
    if (div < 0) {
      std::cerr << "Malformed 'mapreq' in template. 2 '" << data << "' ignoring \n";
      return false;
    }

    eli.m_libname = subtempltxt.before(div);
    subtempltxt = subtempltxt.after(div);
  }
  {
    div = subtempltxt.index(':');
    if (div < 0) {
      std::cerr << "Malformed 'mapreq' in template. 3 '" << data << "' ignoring \n";
      return false;
    }
    eli.m_foundFlag = subtempltxt.before(div);
    subtempltxt = subtempltxt.after(div);
  }
  {
    div = subtempltxt.index(':');
    if (div < 0) {
      std::cerr << "Malformed 'mapreq' in template. 4 '" << data << "' ignoring \n";
      return false;
    }

    eli.m_link = subtempltxt.before(div);
    subtempltxt = subtempltxt.after(div);
  }
  {
    div = subtempltxt.index(':');
    if (div < 0) {
      std::cerr << "Malformed 'mapreq' in template. 5 '" << data << "' ignoring \n";
      return false;
    }

    eli.m_include = subtempltxt.before(div);
    subtempltxt = subtempltxt.after(div);
  }
  {
    div = subtempltxt.index(':');
    if (div < 0) {
      std::cerr << "Malformed 'mapreq' in template. 6 '" << data << "' ignoring \n";
      return false;
    }

    eli.m_library_path = subtempltxt.before(div);
    eli.m_findCmd = subtempltxt.after(div);
  }

  //g_localLibs += eli.m_libname;
  g_libmap[eli.m_libname] = eli.m_name;

  return true;
}


bool CMakeTopGenBodyC::IfReq(StringC &data)
{
  int templStart = data.index(':');
  if(templStart < 1) {
    std::cerr << "Malformed 'ifreq' in template. '" << data << "' ignoring \n";
    return false;
  }
  StringC typedata = data.before(templStart);
  StringC subtempltxt = data.after(templStart);

  StringC libName = GetVar("lib");
  LibraryInfoC &li = g_libraries[libName];

  if(Contains(li.m_requires,typedata)) {
    //vars.Push(RCHashC<StringC,StringC>(vars.Top().Copy()) ); // Push base variables.
    TextFileC subTextBuff(subtempltxt,true,true);
    BuildSub(subTextBuff);
    //vars.DelTop(); // Restore old set.
  }

  return true;
}

bool CMakeTopGenBodyC::IfUsed(StringC &data)
{
  int templStart = data.index(':');
  if(templStart < 1) {
    std::cerr << "Malformed 'ifused' in template. '" << data << "' ignoring \n";
    return false;
  }
  StringC typedata = data.before(templStart);
  StringC subtempltxt = data.after(templStart);

  if(typedata.size() > 0 && typedata.firstchar() == '$') {
    typedata = GetVar(typedata.after(0));
  }

  bool found = false;
  if(typedata.IsEmpty())
    found = true;
  else
    found  = g_usedLibs.IsMember(typedata);

  std::cerr << "Checking if '" <<typedata << "' found " << ((int) found)<< std::endl;
  if(found) {
    //vars.Push(RCHashC<StringC,StringC>(vars.Top().Copy()) ); // Push base variables.
    TextFileC subTextBuff(subtempltxt,true,true);
    BuildSub(subTextBuff);
    //vars.DelTop(); // Restore old set.
  }

  return true;
}

class CMakeTopGenC
        : public TemplateComplexC
{
public:
  CMakeTopGenC(const StringC &templFilename)
    : TemplateComplexC(*new CMakeTopGenBodyC(templFilename))
  {}

  CMakeTopGenC(const StringC &templFilename,const StringListC &midList)
    : TemplateComplexC(*new CMakeTopGenBodyC(templFilename,midList))
  {}
};




bool MakeList(LibraryInfoC &libInfo) {
  StringC dir = libInfo.m_name;
  {
    StringC outFile = libInfo.m_path + "/CMakeLists.txt";
    cout << "Writing cmake file : " << outFile << endl;
    CMakeModuleGenC gen(templateFile, libInfo);
    gen.Build(outFile);
  }
  {
    StringC outFile = libInfo.m_path + "/cmake/" + dir + "Config.cmake.in";
    cout << "Writing cmake file : " << outFile << endl;
    CMakeModuleGenC gen(g_configTemplateFile, libInfo);
    gen.SetVar("target",dir);
    gen.SetVar("libname",libInfo.m_name);
    if(!libInfo.m_mustLinks.IsEmpty()) {
      gen.SetVar("body", dir + "_Body");
    }
    gen.Build(outFile);
  }
  if(!libInfo.m_mustLinks.IsEmpty()) {
    StringC outFile = libInfo.m_path + "/cmake/" + dir + "_BodyConfig.cmake.in";
    cout << "Writing cmake file : " << outFile << endl;
    CMakeModuleGenC gen(g_configTemplateFile, libInfo);
    gen.SetVar("libname",libInfo.m_name + "_Body");
    gen.SetVar("target",dir + "_Body");
    gen.Build(outFile);
  }
  //  makefile.SetVar("sources", sources);

  return true;
}


int main(int nargs,char **argv)
{
  OptionC option(nargs,argv);
  g_rootDir = option.String("i",".","Input filename. ");
  installDir = option.String("o", "", "absolute path of desired install directory");
  dryRun = option.Boolean("d",false,"Do a dry run. Don't change anything. ");
  g_genMakeFilesOnly = option.Boolean("m",false,"Generate makefiles only, don't touch the source. ");
  setFileloc = option.Boolean("fl",setFileloc,"If true the file location will be updated. ");
  bool all = option.Boolean("a",true,"Go into inactive directories as well. ");
  templateFile = option.String("templ", PROJECT_OUT "/share/RAVL/AutoPort/CMakeLists.txt.tmpl", "The template file for each directory.");
  g_configTemplateFile = option.String("conftempl", PROJECT_OUT "/share/RAVL/AutoPort/CMakeConfig.cmake.tmpl", "The template file for cmake config.");
  StringC MasterMakefile = option.String("master", PROJECT_OUT "/share/RAVL/AutoPort/CMakeRoot.txt.tmpl", "The master makefile pulling all directories together.");
  StringC midTemplate = option.String("mid", PROJECT_OUT "/share/RAVL/AutoPort/CMakeMid.txt.tmpl", "The mid level makefile pulling libraries together in a directory.");
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
  SourceCodeManagerC chkit(g_rootDir);
  chkit.ForAllDirs(CallFunc2C<StringC&,DefsMkFileC&,bool>(&CopySources),all);

  // Make top level cmake file
  {
    StringC outFile = installDir + "/CMakeLists.txt";
    CMakeTopGenC gen(MasterMakefile);
    gen.Build(outFile);
  }

  // Make mid level CMake files
  for(HashIterC<StringC,StringListC> mit(g_topLevel);mit;mit++)
  {
    StringC outFile = installDir + "/src/" + mit.Key() + "/CMakeLists.txt";
    CMakeTopGenC gen(midTemplate,mit.Data());
    gen.Build(outFile);
  }

  // Make library cmake files
  for(DLIterC<StringC> it(g_libOrder);it;it++) {
    LibraryInfoC &libInfo = g_libraries[*it];
    libInfo.Tidy();
    MakeList(libInfo);
  }


  return 0;
}

// -----------------------------------------------------------------------------
