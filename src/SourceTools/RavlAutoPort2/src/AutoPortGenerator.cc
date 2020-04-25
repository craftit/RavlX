// This file is part of VCPP, Visual C++ porting tool for QMake projects 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU General 
// Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
///////////////////////////////////////////////////////
//! rcsid="$Id: AutoPortGenerator.cc 5509 2006-06-01 15:30:48Z craftit $"
//! file="Ravl/SourceTools/VisualCPP/AutoPortGenerator.cc"
//! lib=RavlAutoPort2

#include "Ravl/AutoPort2/AutoPortGenerator.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/OS/ChildOSProcess.hh"
#ifdef VISUAL_CPP
#include "Rpc.h"
#endif

#define DODEBUG 0
#if DODEBUG 
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  
  //: Constructor
  
  AutoPortGeneratorBodyC::AutoPortGeneratorBodyC(AutoPortSourceC &nsrc,StringC &templLoc,StringC &noutput,const StringC &_projectOut, const ExtLibTableC & _extLibs) 
    : TemplateComplexBodyC(templLoc),
      outputDir(noutput),
      projectOut(_projectOut),
      extLibs(_extLibs),
      src(nsrc)
  {
    Init();
  }
  
  //: Setup template stuff.
  
  void AutoPortGeneratorBodyC::Init() {
    HSetC<StringC> npresets;
    npresets += StringC("FilePattern");
    npresets += StringC("FileObject");
    SetupPresets(npresets);
    Scan(templFile);
    filePattern = GetVar("FilePattern");
    fileObject = GetVar("FileObject");    

    // Setup commands ?
    SetupCommand("forall",*this,&AutoPortGeneratorBodyC::DoForall);
    SetupCommand("ifany",*this,&AutoPortGeneratorBodyC::DoIfAny);
    SetupCommand("dos",*this,&AutoPortGeneratorBodyC::DoDos);
    SetupCommand("pathback",*this,&AutoPortGeneratorBodyC::DoPathBack);
    SetupCommand("shell",*this,&AutoPortGeneratorBodyC::DoShell);
    SetupCommand("topandtail",*this,&AutoPortGeneratorBodyC::DoTopAndTail);
    SetupCommand("IfFileExists",*this,&AutoPortGeneratorBodyC::DoIfFileExists);
    SetupCommand("LibDepends",*this,&AutoPortGeneratorBodyC::DoLibDepends);
  }
  
  //: Lookup variable.
  // if found put value into 'buff' and return true.
  // otherwise return false.
  
  bool AutoPortGeneratorBodyC::Lookup(const StringC &varname,StringC &buff) {
    if(varname == "libname") {
      LibInfoC &li = CurLibInfo();
      if(li.IsValid())
	buff = li.Name();
      else {
	buff = StringC("*unknown*");
	cerr << "ERROR: Variable 'libname' requested outside  library. Current target:" << target <<"\n";
      }
      return true;
    }
    if(varname == "outputdir") { // Output directory
      buff = outputDir;
      return true;
    }
    if(varname == "package") { // Package for header file.
      buff = context.Top().HeaderInfo().Package();
      return true;
    }
    if(varname == "srcfile") { // Source file for header.
      buff = context.Top().HeaderInfo().SrcFile();
      ONDEBUG(cerr << "srcfile="  << context.Top().HeaderInfo().SrcFile() << "\n");
      return true;
    }
    if(varname == "target") {
      buff = target;
      return true;
    }
    if(varname == "filename") { // Get the full path. same as 'srcfile'
      buff = CurFilename();
      ONDEBUG(cerr << "Filename="  << CurFilename() << "\n");
      return true;
    }
    if(varname == "name") { // Get the full path.  same as 'srcfile'
      buff = CurName();
      ONDEBUG(cerr << "name="  << CurFilename() << "\n");
      return true;
    }
    if(varname == "rootname") { // Just get the root name of the file.
      buff = FilenameC(CurFilename()).NameComponent();
      return true;
    }
    if(varname == "pathname") { // Get the path to the file.
      buff = FilenameC(CurFilename()).PathComponent();
      return true;
    }
    if(varname == "outputfile") {
      buff = outputfile;
      return true;
    }
    if(varname == "ProjectOut") {
      buff = projectOut;
      return true;
    }
    if(varname == "ExtraInclude") { // Extra include paths needed.
      buff = VarExtraInclude();
      return true;
    }
    if(varname == "ProgExtraLibPath") { // Extra library paths needed.
      buff =VarExtraLibPath();
      return true;
    }
    if(varname == "ProgExtraLib") { // Extra libraries needed.
      buff = VarExtraLibs();
      return true;
    }
    if(varname == "mustlink") {
      buff = context.Top().MustLink();
      return true;
    }
    if(varname == "auxdir") {
      buff = context.Top().AuxDir();
      return true;
    }
    if(TemplateComplexBodyC::Lookup(varname,buff))
      return true;
    
    return true;
  }

  //: Compute value of extra inclue variable.
  
  StringC AutoPortGeneratorBodyC::VarExtraInclude() {
    StringC buff;
    
    //: First lets make an exclusive list of all libraries
    DListC<StringC>exclusive;
    HashC<StringC, DListC<StringC> >deps = src.Deps();
    for(DLIterC<StringC>it(context.Top().UsesLibs());it;it++) {
      if(deps.IsElm(*it)) {
        for(DLIterC<StringC> inIt(deps[*it]);inIt;inIt++) 
          if(!exclusive.Contains(*inIt)) exclusive.InsLast(*inIt);
      }
      if(!exclusive.Contains(*it)) exclusive.InsLast(*it);
    }
      
    //: Now we want to check if there are any external libraries
    //: and add the include paths
    DListC<StringC>includePaths;
    for(DLIterC<StringC> it(exclusive);it;it++) {
      if(extLibs.IsElm(*it)) {
        for(DLIterC<StringC> inIt(extLibs[*it].IncludePaths());inIt;inIt++) 
          if(!includePaths.Contains(*inIt)) includePaths.InsLast(*inIt);
      }
    }
      
    //: Now make a single string of all includes
    buff="";
    for(DLIterC<StringC> it(includePaths);it;it++) {
      buff += ';';
      buff += StringC("&quot;") + *it + StringC("&quot;");
    }
    return buff;
  }
  
  StringC AutoPortGeneratorBodyC::VarExtraLibPath() {
    StringC buff;
    HSetC<StringC> done;
    for(DLIterC<StringC> it(context.Top().UsesLibs());it;it++)
      BuildExtraLibs(*it,done,buff,true);
    
    return buff;
  }
  
  //: Compute value of extra library paths
  
  StringC AutoPortGeneratorBodyC::VarExtraLibs() {
    StringC buff;    
    HSetC<StringC> done;
    for(DLIterC<StringC> it(context.Top().UsesLibs());it;it++) 
      BuildExtraLibs(*it,done,buff,false);
    
    return buff;
  }
  
  //: Build set of extra libraries needed.
  //!param: libName - Name of library to examine.
  //!param: done - Libraries already done.
  //!param: result - Resulting include string.
  
  bool AutoPortGeneratorBodyC::BuildExtraLibs(const StringC &plibName,HSetC<StringC> &done,StringC &result,bool doLibPaths) {
    StringC libName = plibName;
    if(libName.contains(".opt"))
      libName = libName.before(".opt");

    
    // Done already ?
    if(done.IsMember(libName))
      return true;
        
    done += libName;
    
    // this bits is crazy - but seems to work!
    if(extLibs.IsElm(libName)) {

      // external library path
      if(doLibPaths) {
        for(DLIterC<StringC> it(extLibs[libName].LibPaths());it;it++) {
          if(result.Size() > 0) 
            result += ';';
          result += *it;
        }
      }
      // external library
      else {
        for(DLIterC<StringC> it(extLibs[libName].Libs());it;it++) {
          result += ' ';
          result += *it;
        }
      }
    }
    
    else {
      
      for(DLIterC<StringC> it(src.Deps()[libName]);it;it++)
        BuildExtraLibs(*it,done,result,doLibPaths);    
      
      if(!extLibs.IsElm(libName) && !doLibPaths) {
        result += ' ';
        result += libName + ".lib";
      }
    }
    // end of craziness.....
    
    return true;
  }
  
  
  //: Generate a DOS filename.
  
  bool AutoPortGeneratorBodyC::DoDos(StringC &data) {
    StringC newstuff = Interpret(data);
    newstuff.gsub("/","\\");
    output.Top() << newstuff;
    return true;
  }
  
  //: Generate path back from a file.
  // So 'MyDir1/MySubDir/filename' will become '../../'
  
  bool AutoPortGeneratorBodyC::DoPathBack(StringC &data) {
    // Is string empty ?
    if(data.IsEmpty())
      return true;
    StringC idata = Interpret(data);
    
    idata.gsub("/./","/");
    
    IntT slashes = idata.freq('/');
    if(idata[0] == '/') // Is it an absolute path?
      slashes--;
    ONDEBUG(cerr << "AutoPortGeneratorBodyC::PathBack '" << idata << "' Slashes=" << slashes << "\n");
    for(int i = 0;i < slashes;i++)
      output.Top() << "../";
    return true;
  }

  //: Execute a shell command and include stdout.
  
  bool AutoPortGeneratorBodyC::DoShell(StringC &data) {
#ifndef VISUAL_CPP
    StringC idata = Interpret(data);
    ChildOSProcessC cproc(idata,true);
    if(!cproc.Wait(5.0) && !cproc.ExitedOk()) {
      cproc.Terminate();
      cerr << "AutoPortGeneratorBodyC::Shell, ERROR: Failed to complete execution of: " << idata << "\n";
      return false;
    }
    cproc.StdOut().CopyTo(output.Top());
#else
    if(data == "uuidgen") {
      UUID uuid;
      UuidCreate(&uuid);
      unsigned char * buf;
      UuidToStringA(&uuid, &buf);
      StringC s((char *)buf);
      output.Top() << s;
    } else {
      RavlIssueError("DoShell:Not supoorted on Windows\n");
    }
#endif
    return true;
  }

  //: Remove white space from beging and end of string.
  
  bool AutoPortGeneratorBodyC::DoTopAndTail(StringC &data) {
    StringC idata = Interpret(data);
    output.Top() << idata.TopAndTail();
    return true;
  }

  //: Execute code if file exists.
  
  bool AutoPortGeneratorBodyC::DoIfFileExists(StringC &data) {
    int templStart = data.index(':');
    if(templStart < 1) {
      cerr << "Malformed 'forall' in template. '" << data << "' ignoring \n";
      return false;
    }    
    FilenameC fn = Interpret(data.before(templStart));
    if(fn.Exists()) {
      StringC therest = data.after(templStart);
      output.Top() << Interpret(therest);
    }
    
    return true;
  }

  //: IfAny template function
  
  bool AutoPortGeneratorBodyC::DoIfAny(StringC &data) {
    int templStart = data.index(':');
    if(templStart < 1) {
      cerr << "Malformed 'ifany' in template. '" << data << "' ignoring \n";
      return false;
    }    
    StringC typedata = data.before(templStart);    
    
    // Check for a condition in the for loop.
    StringC condition;
    IntT condInd =typedata.index(',');
    if(condInd >= 0) {
      condition = typedata.after(condInd);
      //cerr << "Found condition '" << condition << "'\n";
      typedata = typedata.before(condInd);
      if(typedata == "") {
	cerr << "Malformed 'forall' in template. '" << data << "' ignoring \n";
	return false;
      }
     }
    
    // Variable in the element type ?
    if(typedata[0] == '$') {
      typedata = typedata.after(0);
      StringC buff;
      if(!Lookup(typedata,buff)) {
	cerr << "Unknown variable. '" << typedata << "' in forall. \n";
	return false;
      }
      typedata = buff;
      //cerr << "Got variable in  '" << typedata << "' in forall. \n";
    }
    
    StringC subtempltxt = data.after(templStart);    
    TextFileC subTextBuff(subtempltxt,true,true);



    if(typedata == "libs") {
      ONDEBUG(cerr << "forall libs:\n");
      if(!src.Libs().IsEmpty())
	BuildSub(subTextBuff);      
      return true;
    }
    if(typedata == "useslibs") {
      ONDEBUG(cerr << "forall useslibs: " << context.Top().UsesLibs().Size() << " Type=" << context.Top().ctype << " Name=" << context.Top().HeaderInfo().Name() << "\n");
      if(!context.Top().UsesLibs().IsEmpty())
	BuildSub(subTextBuff);
      return true;
    }
    // Source code.
    if(typedata == "sources") {
      ONDEBUG(cerr << "forall sources:\n");
      if(!context.Top().Sources().IsEmpty()) {
	ONDEBUG(cerr << "For all Sources=" << subTextBuff << "\n");
	BuildSub(subTextBuff);
      }	
      return true;
    }
    if(typedata == "headers") {
      ONDEBUG(cerr << "forall headers:\n");
      if(!context.Top().Headers().IsEmpty()) {
	BuildSub(subTextBuff);
      }	
      return true;
    }
    // Programs.
    if(typedata == "mains" || typedata == "examples" || typedata == "tests") {
      ONDEBUG(cerr << "forall exe, type=" << typedata << " :\n");
      DListC<ProgInfoC> pi;
      if(typedata == "mains")
	pi = src.Mains();
      else if(typedata == "examples")
	pi = src.Examples();
      else
	pi = src.Tests();
      if(!pi.IsEmpty()) {
	BuildSub(subTextBuff);	
      }
      return true;
    }
    
    if(typedata == "auxfiles") {
      ONDEBUG(cerr << "forall auxfiles:\n");
      if(!context.Top().AuxFiles().IsEmpty()) {
	BuildSub(subTextBuff);
      }	
      return true;
    }
    
    return true;
  }
  
  //: For all template function.
  
  bool AutoPortGeneratorBodyC::DoForall(StringC &data) {
    int templStart = data.index(':');
    if(templStart < 1) {
      cerr << "Malformed 'forall' in template. '" << data << "' ignoring \n";
      return false;
    }    
    StringC typedata = data.before(templStart);

    // Check for a condition in the for loop.
    StringC condition;
    IntT condInd =typedata.index(',');
    if(condInd >= 0) {
      condition = typedata.after(condInd);
      //cerr << "Found condition '" << condition << "'\n";
      typedata = typedata.before(condInd);
      if(typedata == "") {
	cerr << "Malformed 'forall' in template. '" << data << "' ignoring \n";
	return false;
      }
     }
    
    // Variable in the element type ?
    if(typedata[0] == '$') {
      typedata = typedata.after(0);
      StringC buff;
      if(!Lookup(typedata,buff)) {
	cerr << "Unknown variable. '" << typedata << "' in forall. \n";
	return false;
      }
      typedata = buff;
      //cerr << "Got variable in  '" << typedata << "' in forall. \n";
    }
    
    StringC subtempltxt = data.after(templStart);    
    TextFileC subTextBuff(subtempltxt,true,true);
    // Libraries.
    if(typedata == "libs") {
      ONDEBUG(cerr << "forall libs:\n");
      for(HashIterC<StringC,LibInfoC> it(src.Libs());it;it++) {
	context.Push(ContextC(*it));
	BuildSub(subTextBuff);
	context.DelTop();
      }
      return true;
    }
    if(typedata == "useslibs") {
      ONDEBUG(cerr << "forall useslibs: " << context.Top().UsesLibs().Size() << " Type=" << context.Top().ctype << " Name=" << context.Top().HeaderInfo().Name() << "\n");
      for(DLIterC<StringC> it(context.Top().UsesLibs());it;it++) {
	ONDEBUG(cerr << " " << *it << "\n");
	context.Push(ContextC(*it));
	BuildSub(subTextBuff);
	context.DelTop();
      }
      return true;
    }
    // Source code.
    if(typedata == "sources") {
      ONDEBUG(cerr << "forall sources:\n");
      DListC<StringC> lst = context.Top().Sources();
      for(DLIterC<StringC> it(lst);it;it++) {
	context.Push(ContextC(*it));
	ONDEBUG(cerr << "For all Sources=" << *it << "\n");
	BuildSub(subTextBuff);
	context.DelTop();
      }	
      return true;
    }
    if(typedata == "headers") {
      ONDEBUG(cerr << "forall headers:\n");
      DListC<HeaderInfoC> lst = context.Top().Headers();
      for(DLIterC<HeaderInfoC> it(lst);it;it++) {
	context.Push(ContextC(*it));
	BuildSub(subTextBuff);
	context.DelTop();
      }	
      return true;
      
    }
    if(typedata == "auxfiles") {
      ONDEBUG(cerr << "forall auxfiles:\n");
      DListC<StringC> lst = context.Top().AuxFiles();
      // Save old target file.
      StringC orgHdr = context.Top().hdrInfo.SrcFile();
      // Go through auxfiles.
      for(DLIterC<StringC> it(lst);it;it++) {
        context.Top().hdrInfo.SrcFile() = *it;
	BuildSub(subTextBuff);
      }	
      // Restore original value.
      context.Top().hdrInfo.SrcFile() = orgHdr;
      return true;
    }
    // Programs.
    if(typedata == "mains" || typedata == "examples" || typedata == "tests") {
      ONDEBUG(cerr << "forall exe, type=" << typedata << " :\n");
      DListC<ProgInfoC> pi;
      if(typedata == "mains")
	pi = src.Mains();
      else if(typedata == "examples")
	pi = src.Examples();
      else
	pi = src.Tests();
      for(DLIterC<ProgInfoC> it(pi);it;it++) {
	context.Push(ContextC(*it));
	BuildSub(subTextBuff);	
	context.DelTop();
      }
      return true;
    }
    
    cerr << "ERROR: Unknown forall object :" << typedata << "\n";
    return false;
  }
  
  //: Go through all dependant libraries.
  
  bool AutoPortGeneratorBodyC::DoLibDepends(StringC &data) {
    HSetC<StringC> done;
    for(DLIterC<StringC> it(context.Top().UsesLibs());it;it++)
      HelpLibDepends(data,*it,done);
    
    return true;
  }

  //: Helper functionf or DoLibDepends.
  
  bool AutoPortGeneratorBodyC::HelpLibDepends(StringC &data,const StringC &library,HSetC<StringC> &done) {
    StringC libName = library;
    if(libName.contains(".opt"))
      libName = libName.before(".opt");
    
    // Done already ?
    if(done.IsMember(libName))
      return true;
    done += libName;
    
    for(DLIterC<StringC> it(src.Deps()[libName]);it;it++)
      HelpLibDepends(data,*it,done);
    if(src.Libs().IsElm(library)) {
      LibInfoC libInfo = src.Libs()[library];
      //cerr << "AutoPortGeneratorBodyC::HelpLibDepends, " << libName << "  MUSTLINK=" << libInfo.MustLink() << "\n";
      context.Push(ContextC(libInfo));
      output.Top() << Interpret(data);
      context.Pop();
    }
    return true;
  }
  
  
  //: Make a file name for an object.
  StringC AutoPortGeneratorBodyC::MakeFilename(const StringC &obj) {
    StringC ret;
    if(!outputDir.IsEmpty()) {
      if(outputDir.lastchar() != filenameSeperator)
        ret = outputDir + filenameSeperator + filePattern;
    }
    else
      ret = filePattern;
    ret.gsub("%",obj);
    
    ret = Interpret(ret);
    
    // Save current target filename.
    outputfile = ret;
    return ret;
  }

  //: Build output files.
  
  bool AutoPortGeneratorBodyC::BuildFiles() {
    if(fileObject == "none") 
      return true;
    if(fileObject == "one") {
      ONDEBUG(cerr << "************** File one  **************************** \n");
      target = "one";
      StringC fn = MakeFilename("");
      cerr << "Building file:'" << fn << "'\n";
      Build(fn);
      return true;
    }
    if(fileObject == "lib") {
      for(HashIterC<StringC,LibInfoC> it(src.Libs());it;it++) {
	ONDEBUG(cerr << "************** File lib  Name=" << it->Name() << " **************************** \n");
	target = it.Key();
	context.Push(ContextC(*it));
	StringC fn = MakeFilename(it.Key());
	Build(fn);
	context.DelTop();
      }
      return true;
    }
    if(fileObject == "newlib") {
      for(HashIterC<StringC,LibInfoC> it(src.Libs());it;it++) {
	ONDEBUG(cerr << "************** File lib  Name=" << it->Name() << " **************************** \n");
	target = it.Key();
	context.Push(ContextC(*it));
	FilenameC fn = MakeFilename(it.Key());
        if(!fn.Exists()) // Only make if it doesn't exist.
          Build(fn);
	context.DelTop();
      }
      return true;
    }
    if(fileObject == "mains" || fileObject == "examples" || fileObject == "tests" ||
       fileObject == "newmains" || fileObject == "newexamples" || fileObject == "newtests"
       ) {
      DListC<ProgInfoC> pi;
      if(fileObject == "mains" || fileObject == "newmains")
	pi = src.Mains();
      else if(fileObject == "examples" || fileObject == "newexamples")
	pi = src.Examples();
      else
	pi = src.Tests();
      bool newOnly = fileObject.contains("new") == 1;
      for(DLIterC<ProgInfoC> it(pi);it;it++) {
	ONDEBUG(cerr << "************** File context Name=" << it->Name() << " Type=" << fileObject << " **************************** \n");
	ONDEBUG(cerr << " Libs=" << it->UsesLibs().Size() << " First=" << ( it->UsesLibs().IsEmpty() ? StringC("NULL") : it->UsesLibs().First() ) << "\n");
	target = it->Name();
	context.Push(ContextC(*it));
	FilenameC fn = MakeFilename(it->Name());
        if(!newOnly || !fn.Exists()) // Only make if it doesn't exist.
          Build(fn);
        
	ONDEBUG(cerr << " Libs=" << context.Top().progInfo.UsesLibs().Size() << " " << context.Top().UsesLibs().Size() << "\n");
	context.DelTop();
      }
      return true;
    }
    
    if(fileObject == "headers") {
      for(HashIterC<StringC,LibInfoC> it(src.Libs());it;it++) {
	ONDEBUG(cerr << "************** File header  Name=" << it->Name() << " **************************** \n");
        DListC<HeaderInfoC> hdrInfo = it.Data().Headers();
        for(DLIterC<HeaderInfoC> hit(hdrInfo);hit;hit++) {
          target = hit->Name(); // Set current target.
          context.Push(ContextC(*hit));
          StringC outName = FilenameC(CurFilename()).NameComponent();
          StringC fn = MakeFilename(outName);
          Build(fn);
          context.DelTop();
        }
      }
      
      return true;
    }

    if(fileObject == "newheaders") {
      for(HashIterC<StringC,LibInfoC> it(src.Libs());it;it++) {
	ONDEBUG(cerr << "************** File header  Name=" << it->Name() << " **************************** \n");
        DListC<HeaderInfoC> hdrInfo = it.Data().Headers();
        for(DLIterC<HeaderInfoC> hit(hdrInfo);hit;hit++) {
          target = hit->Name(); // Set current target.
          context.Push(ContextC(*hit));
          StringC outName = FilenameC(CurFilename()).NameComponent();
          FilenameC fn = MakeFilename(outName);
          if(!fn.Exists())
            Build(fn);
          context.DelTop();
        }
      }
      
      return true;
    }
    
    cerr << "Unknown file object : '" << fileObject << "'\n";
    return false;
  }

}
