// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlClassWizard
//! file="Ravl/Applications/ClassWizard/ClassWizard.cc"

#include "Ravl/SourceTools/ClassWizard.hh"
#include "Ravl/SourceTools/SourceCodeManager.hh"
#include "Ravl/CxxDoc/CxxElements.hh"
#include "Ravl/CallMethods.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {

  //: Constructor.
  
  ClassWizardBodyC::ClassWizardBodyC(const StringC &nrootDir,const StringC &nlocalDir,bool nverbose,bool nwriteStubs,bool ndryRun) 
    : rootDir(nrootDir),
      localDir(nlocalDir),
      parseTree(true),
      verbose(nverbose),
      writeStubs(nwriteStubs),
      dryRun(ndryRun)
  { 
    FilenameC fn(nrootDir);
    if(!fn.IsDirectory()) {
      ONDEBUG(cerr << "ClassWizardBodyC::ClassWizardBodyC, Root is not a directory. \n");
      if(!fn.Exists()) 
        throw ExceptionOperationFailedC("Can't find root file");
      FilenameC newRootDir = fn.PathComponent();
      editList += fn.NameComponent();
      if(newRootDir.IsEmpty())
        newRootDir = ".";
      if(!newRootDir.IsDirectory()) 
        throw ExceptionOperationFailedC("Failed to find root directory.");
      rootDir = newRootDir;
    }
    parseTree.SetRootFilename(rootDir); 
  }
  
  //: Dump the parse tree.
  
  void ClassWizardBodyC::DumpParseTree(ostream &out) {
    parseTree.Dump(out);
  }
  
  //: Gather information about the source code.
  
  bool ClassWizardBodyC::GatherInfo() {
    SourceCodeManagerC scm(rootDir);
    ClassWizardC me(*this);
    scm.ForAllDirs(Trigger(me,&ClassWizardC::GatherDir,StringC(),DefsMkFileC()));
    return true;
  }
  
  //: Gather info from file.
  
  bool ClassWizardBodyC::GatherFile(StringC &filename) {
    //cerr << "ClassWizardBodyC::GatherFile(), " << filename << "\n";
    parseTree.Parse(filename);
    return true;
  }
  
  //: Gather info from a directory.
  
  bool ClassWizardBodyC::GatherDir(StringC &dirname,DefsMkFileC &defsmk) {
    ONDEBUG(cerr << "ClassWizardBodyC::GatherDir(), " << dirname << "\n");
    StringListC hdrfiles = defsmk.Value("HEADERS");
    if(writeStubs) {
      StringListC srcfiles = defsmk.Value("SOURCES");
      hdrfiles.MoveLast(srcfiles);
    }
    for(DLIterC<StringC> it(hdrfiles);it;it++) {
      FilenameC sfile = dirname + '/' + *it;
      if(!sfile.Exists())
	cerr << "WARNING: Can't find file '" << sfile << "'\n";
      GatherFile(sfile);
    }
    return true;
  }
  
  //: Go through and update local source files.
  
  bool ClassWizardBodyC::ApplyWizard() {
    if(verbose)
      cerr << "Gathering class information. \n";
    
    if(!GatherInfo())
      return false;
    
    if(verbose)
      cerr << "Resolving refrences. \n";
    
    parseTree.Resolve();

    if(verbose)
      cerr << "Scanning for big objects. \n";
    
    ScopeC scope(parseTree.Data());
    //scope.Dump(cout);
    
    ScanScope(scope);
    if(!dryRun) {
      if(verbose)
	cerr << "Saving edits. \n";
      for(HashIterC<StringC,TextFileC> it(textFiles);it;it++) {
	if(!modifiedPrefix.IsEmpty()) {
          if (modifiedPrefix == "-")
            it.Data().SetFilename(modifiedPrefix);
          else
            it.Data().SetFilename(modifiedPrefix + it.Key());
        }
	if(!it.Data().Save()) {
	  cerr << "ERROR: Failed to save '" << it.Data().Filename() << "'\n";
	}
      }
    }
    return true;
  }
  
  //: Find public section in classObj
  
  IntT ClassWizardBodyC::FindPublicSection(ObjectC &obj) {
    //cerr << "ClassWizardBodyC::FindPublicSection() of " << obj.Name() << " called. \n";
    ClassC classObj(obj);
    for(DLIterC<ObjectC> it(classObj.List());it;it++) {
      if(!MarkerC::IsA(*it))
	continue;
      MarkerC marker(*it);
      //cerr << "Marker=" << marker.Name() << " at " << marker.EndLineno() << "\n";
      if(marker.Name() == "public")
	return marker.EndLineno();
    }
    return -1;
  }
  
  //: Append comment to text buffer.
  
  bool ClassWizardBodyC::WriteComment(SourceCursorC &sc,ObjectC &obj,bool markAsAuto) {
    if(!obj.Comment().Header().IsEmpty()) {
      StringListC lines(obj.Comment().Header(),"\n"); // Split up into seperate lines.
      for(DLIterC<StringC> it(lines);it;it++)
	sc += StringC("//:") + *it;
    }
    if(!obj.Comment().Text().IsEmpty()) {
      StringListC lines(obj.Comment().Text(),"\n"); // Split up into sperate lines.
      for(DLIterC<StringC> it(lines);it;it++)
	sc += StringC("//") + *it;
    }
    for(HashIterC<StringC,StringC> it(obj.Comment().Locals());it;it++) {
      if(it.Data().contains("\n")) {
	StringListC slist(it.Data(),"\n");
	for(DLIterC<StringC> lit(slist);lit;lit++)
	  sc += StringC("//!") + it.Key() + ':' + lit.Data();
      } else {
	sc += StringC("//!") + it.Key() + ':' + it.Data();
      }
    }
    if(markAsAuto)
      sc += "//!cwiz:author";
    return true;
  }
  
  //: Write the handle method for the given method.
  
  bool ClassWizardBodyC::WriteHandleMethod(SourceCursorC &sc,ObjectC &obj) {
    RCHashC<StringC,ObjectC> templSub;
    
    sc += obj.FullName(templSub,descGen);
    MethodC method(obj);
    StringC funcArgs;
    DLIterC<DataTypeC> it(method.Args());
    if(it) {
      funcArgs += it->Alias();
      for(it++;it;it++)
	funcArgs += StringC(",") + it->Alias();
    }
    StringC retType = method.ReturnType().FullName(templSub,descGen);
    if(retType != "void")
      sc += StringC("{ return Body().") + obj.Var("BaseName") +"(" + funcArgs + "); }";
    else
      sc += StringC("{ Body().") + obj.Var("BaseName") +"(" + funcArgs + "); }";
    WriteComment(sc,obj);
    return true;
  }
  
  //: Write stubs file.
  
  bool ClassWizardBodyC::WriteStubsFile(ObjectC &obj) {
    TextFileC txt(true);
    ClassC bodyClass(obj);
    RCHashC<StringC,ObjectC> templSub;
    SourceCursorC sc(txt,0);
    ObjectC scope = obj.GetScope();
    if(scope.IsValid())
      sc += "namespace " + scope.Name() + " {";
    else
      sc += "namespace ??? {";
    sc.AddIndent(1);
    for(DLIterC<ObjectC> it(bodyClass.List());it;it++) {
      if(!MethodC::IsA(*it)) // Only interested in methods here
	continue;
      //cerr << "Type=" << it->Var("dectype") << "\n";
      if(it->Var("dectype") != "prototype")
	continue; // Only generate stubs for prototypes.
      MethodC method(*it);
      sc += "//:" + method.Comment().Header(); 
      sc += "";
      sc += method.DefinitionName(bodyClass.Name() + "::") + "{";
      sc += "}";
      sc += "";
    }
    sc.AddIndent(-1);
    sc += "}";
    StringC lfn = modifiedPrefix + "Stub_" + obj.Name() + ".cc";
    txt.SetFilename(lfn);
    txt.Save();
    return true;
  }
  
  
  //: Write the handle constructor.
  
  bool ClassWizardBodyC::WriteHandleConstructor(SourceCursorC &sc,ObjectC &obj,const StringC &handleBaseClass) {
    ONDEBUG(cerr << "ClassWizardBodyC::WriteHandleConstructor(). \n");
    if(obj.Name().contains('~')) {
      cerr << "ClassWizardBodyC::WriteHandleConstructor(), Template method is a destructor!. \n";
      return false;
    }
    RCHashC<StringC,ObjectC> templSub;
    MethodC method(obj);
    StringC funcArgs;
    DLIterC<DataTypeC> it(method.Args());
    if(it) {
      StringC argName = it->Alias();
      if(argName.contains("=")) // Got a default argument for the function
	argName = argName.before("=").TopAndTail(); // Remove default args.
      funcArgs += argName;
      for(it++;it;it++) {
	argName = it->Alias();
	if(argName.contains("=")) // Got a default argument for the function
	  argName = argName.before("=").TopAndTail(); // Remove default args.
	funcArgs += StringC(",") + argName;
      }
    }
    StringC fullName= obj.FullName(templSub,descGen);
    StringC baseName=obj.Var("BaseName");
    StringC className = baseName.Copy();
    className.gsub("BodyC","C");
    fullName.gsub(baseName,className);
    
    ONDEBUG(cerr << "baseName=" << baseName << " className=" << className << "\n");
    
    sc += fullName;
    sc.AddIndent(1);
    sc += StringC(": ") + handleBaseClass + "(*new " + baseName + "(" + funcArgs + "))";
    sc.AddIndent(-1);
    sc += "{}";
    WriteComment(sc,obj);
    return true;
  }
  
  //: Generate text for handle class.
  
  bool ClassWizardBodyC::WriteHandleClass(SourceCursorC &sc,ObjectC &bodyObj,const StringC &className) {
    TextBufferC textBuff(true);
    
    ClassC bodyClass(bodyObj);
    // Assume we're in the right namespace.
    
    StringC bodyClassName = bodyObj.Name();
    //ONDEBUG(err << "ClassWizardBodyC::WriteHandleClass() Body=" <<  << " Classname=" << className << " Body=" << bodyClassName << "\n");
    sc += "//! userlevel=Normal";
    if(bodyObj.Comment().Header().IsEmpty())
      sc += StringC("//: Handle for ") + bodyClass.Name();
    WriteComment(sc,bodyObj);
    sc += "";
    sc += StringC("class ") + className;
    
    // Work out class inheritance.
    // What does body class inherit from ?
    DListC<StringC> baseClasses;
    for(DLIterC<ObjectC> it(bodyClass.Uses());it;it++) {
      StringC iname = it->Var("classname");
      //cerr << "Inherit=" << iname << "\n";
      if(iname == "RCBodyC" || iname == "RCBodyVC" || iname == "RavlN::RCBodyC" || iname == "RavlN::RCBodyVC") {
	baseClasses.InsLast(StringC("RCHandleC<") + bodyObj.Name() + StringC(">"));
	continue;
      }
      if(!iname.matches("BodyC",-1, false))
	continue; // Ingore inheritance from non-big objects.
      StringC handleName = iname.before("BodyC") + "C";
      baseClasses.InsLast(handleName);
    }
    
    //: Write inheritance info into class.
    sc.AddIndent(1);
    bool gotOne = false;
    for(DLIterC<StringC> it(baseClasses);it;it++) {
      if(!gotOne) {
	sc += StringC(": public ") + *it;
	gotOne = true;
      } else
	sc += StringC(", public ") + *it;
    }
    sc.AddIndent(-1);
    sc += "{";
    sc += "public:";
    sc.AddIndent(1);
    
    StringC mainBaseClass;
    if(baseClasses.IsEmpty())
      mainBaseClass = StringC("(Unknown)");
    else
      mainBaseClass = baseClasses.First();
    
    StringC rootBaseClass = mainBaseClass;
    
    for(InheritIterC it(bodyClass,SAPublic,true);it;it++) {
      if(!MethodC::IsA(*it)) // Only interested in methods here
	continue;
      if(it->Var("access") != "public") // Private ?
	continue;
      if(it->Var("constructor") == "true") {
        if(it->Name().contains('~')) // Ingore destructors.
          continue;
        WriteHandleConstructor(sc,*it,mainBaseClass);
      } else 
        WriteHandleMethod(sc,*it);
      sc += "";
    }
    sc.AddIndent(-1);
    sc += "protected:\n";
    sc.AddIndent(1);
    
    //: Write body constructor.
    sc += className + "(" + bodyClassName + " &bod)";
    sc += StringC(" : ") + mainBaseClass + "(bod)";
    sc += "{}";
    sc += "//: Body constructor. ";
    sc += "";
    
    //: Write body access.
    sc += bodyObj.Name() + "& Body()";
    sc += StringC("{ return static_cast<") + bodyObj.Name() + " &>(" + rootBaseClass + "::Body()); }" ;
    sc += "//: Body Access. ";
    sc += "";
    
    //: Write const body access.
    sc += StringC("const ") + bodyObj.Name() + "& Body() const" ;
    sc += StringC("{ return static_cast<const ") + bodyObj.Name() + " &>(" + rootBaseClass + "::Body()); }" ;
    sc += "//: Body Access. ";
    sc += "";

    //: Close class.
    
    sc.AddIndent(-1);
    sc += "};";
    
    return true;
  }
  
  //: Get a text file for editing.
  
  TextFileC ClassWizardBodyC::TextFile(const StringC &filename) {
    TextFileC ret;
    if(textFiles.Lookup(filename,ret))
      return ret;
    FilenameC fn(filename);
    if(!fn.Exists()) {
      cerr << "ERROR: File '" << filename << "' not found. \n";
      return ret;
    }
    ret = TextFileC(filename);
    textFiles[filename] = ret;
    return ret;
  }
  
  //: Apply to a directory.
  
  bool ClassWizardBodyC::ApplyClass(ScopeC &scope,ObjectC &bodyObj) {
    StringC handleClassname = bodyObj.Name().before("BodyC") + 'C';
    ONDEBUG(cerr << "ClassWizardBodyC::ApplyClass(), Scope=" << scope.Name() << " Handle=" << handleClassname << " Body=" << bodyObj.Name() << "\n");
    ONDEBUG(cerr << "ClassWizardBodyC::ApplyClass(), File='" << scope.Var("filename") << "'\n");
    ObjectC rawHandleObj = scope.Lookup(handleClassname);
    ClassC bodyClass(bodyObj);
    
    // Check this is a file we're allowed to edit.
    {
      StringC classFile = bodyObj.Var("filename");
      if(!editList.IsEmpty() && !editList.IsMember(classFile))
        return true;
    }
    
    if(!rawHandleObj.IsValid()) {
      ONDEBUG(cerr << "Failed to find handle class '" << handleClassname << "'\n");
      StringC localFile = bodyObj.Var("filename");
      IntT insertPoint = bodyObj.EndLineno() + 1;
      
      // Write one.
      cerr << "Adding class " << handleClassname << " to " << localFile << " at " << insertPoint << "\n";
      TextFileC txt = TextFile(localFile);
      SourceCursorC sc(txt,1);
      if(!sc.FindLine(insertPoint)) {
	cerr << "ERROR: Failed to find insert point " << insertPoint  << ", skipping edit. \n";
	return false;
      }
      sc += "";
      WriteHandleClass(sc,bodyObj,handleClassname);
      return true;
    }
    if(!ClassC::IsA(rawHandleObj)) {
      cerr <<"WARNING: Unexpected handle type '" << rawHandleObj.TypeName() << "'. \n";
      return false;
    } 
    ClassC handleObj = rawHandleObj;
    ONDEBUG(cerr << "EndLine = " << handleObj.EndLineno() << " " << handleObj.StartLineno()<< "\n");
    
    if(writeStubs) {
      WriteStubsFile(bodyObj);
    }
    
    // Look through handle for cruft that can be removed.
    
    for(InheritIterC it(handleObj,SAPublic,true);it;it++) {
      if(!MethodC::IsA(*it)) // Only interested in methods here
	continue;
      if(it->Var("access") != "public")
	continue;
      if(verbose)
	cerr << "Checking method " << it->Name() << "\n";
      //it->Dump(cerr);
      if(it->Comment().Locals()["author"].TopAndTail() != "cwiz")
	continue; // Only modify methods we wrote
      bool isConstructor = false;
      StringC bodyName = it->Name();
      if(it->Var("constructor") == "true") {
	isConstructor = true;
	MethodC constructor = GenerateBodyConstructor(*it);
	bodyName = constructor.Name();
	//cerr << "Bodyname=" << bodyName << "\n";
      }
      ObjectC bodyMethod = bodyClass.Lookup(bodyName);
      if(bodyMethod.IsValid()) {
	//cerr << " found body " << bodyMethod.Name() << "\n";
	continue; // Its fine.
      }
      StringC localFile = bodyObj.Var("filename");
      
      // Can't find corresponding body method, must have been deleted.
      TextFileC txt = TextFile(localFile);
      if(!txt.IsValid())
	continue; // Couldn't find file.
      // Work out what exactly to delete.
      IntT firstLine = it->StartLineno();
      IntT lastLine = it->EndLineno();
      if(it->Comment().EndLine() > lastLine)
	lastLine = it->Comment().EndLine();
      SourceCursorC sc(txt,2);
      // Check for blank line after method to remove.
      if(sc.FindLine(lastLine+1) && sc.IsBlank()) 
	lastLine++;
      cerr << "Deleteing method " << it->Name() << " from " << localFile << " at lines " << firstLine << " to " << lastLine << "\n";
      // Do the dirty deed.
      txt.Delete(firstLine,lastLine);
    }
    
    // Now we've got both the handle and the body classes, lets look for public methods that aren't in the handle.
    // Note we're only iterating through the public part of the local scope, .
    
    StringC mainBaseClass = BaseClassName(handleObj);
    
    int endOfLastHandle=-1;
    for(InheritIterC it(bodyObj,SAPublic,true);it;it++) {
      if(!MethodC::IsA(*it)) // Only interested in methods here
	continue;
      ONDEBUG(cerr << "Found method '" << it->Name() << "' \n");
      bool isConstructor = false;
      if(it->Var("access") != "public")
	continue;
      StringC handleName = it->Name();
      if(it->Var("constructor") == "true") {
        if(it->Name().contains('~')) // Ingore destructors.
          continue; 
	isConstructor = true;
	MethodC constructor = GenerateHandleConstructor(*it);
	handleName = constructor.Name();
      }
      
      ObjectC handleMethod;
      handleObj.LookupI(handleName,handleMethod);
      
      // Does method exist ?
      
      if(!handleMethod.IsValid()) {
	//cerr << "Failed to find handle method. \n";
	if(endOfLastHandle < 0) {
	  // Need to find were to write it....
	  endOfLastHandle = FindPublicSection(handleObj);
	  if(endOfLastHandle < 0) {
	    cerr << "WARNING: Don't know where to write handle method " << handleName << " in class " << handleObj.Name() << ". skipping. \n";
	    continue;
	  }
	}
	StringC localFile = bodyObj.Var("filename");
        
	cerr << "Adding method " << it->Name() << " to " << localFile << " at " << (endOfLastHandle+1) << "\n";
	TextFileC txt = TextFile(localFile);
	SourceCursorC sc(txt,2);
	if(!sc.FindLine(endOfLastHandle+1)) {
	  cerr << "ERROR: Failed to find insert point " << endOfLastHandle << ", skipping edit. \n";
	  continue;
	}
	sc += "";
	if(isConstructor) 
          WriteHandleConstructor(sc,*it,mainBaseClass);
        else
	  WriteHandleMethod(sc,*it);
	continue;
      }
      
      // FIXME: Are the comments right ?
      
      // Make a note of where we are.
      
      endOfLastHandle = handleMethod.Comment().EndLine();
      if(endOfLastHandle < 0)
	endOfLastHandle = handleMethod.EndLineno();
      
      //cerr << "EndLine = " << handleMethod.EndLineno() << " " << handleMethod.StartLineno()<< " Comment=" << handleMethod.Comment().StartLine() << " "<<handleMethod.Comment().EndLine() << " \n";
    }
    return true;
  }
  
  //: Scan scope
  
  bool ClassWizardBodyC::ScanScope(ScopeC &scope) {
    ONDEBUG(cerr << "ClassWizardBodyC::ScanScope(), Scan scope " << scope.Name() << "\n");
    for(DLIterC<ObjectC> it(scope.List());it;it++) {
      if(ClassC::IsA(*it)) { // Got a class ?
	if(it->Name().matches("BodyC",-1,false)) {
	  ONDEBUG(cerr << "Found body class '" << it->Name() << "\n");
	  ApplyClass(scope,*it);
	}
	continue; // Don't look inside class scopes.
      }
      if(ScopeC::IsA(*it)) {
	ScopeC subScope(*it);
	if(!ScanScope(subScope))
	  return false;
      }
    }
    
    return true;
  }

  //: Generate a handle constructor prototype from its body counterpart
  
  MethodC ClassWizardBodyC::GenerateHandleConstructor(ObjectC &bodyConstructor) {
    MethodC ret;
    MethodC orginal(bodyConstructor);
    ret = orginal.Copy();
    StringC baseName = orginal.Var("BaseName").before("BodyC",-1) + "C";
    ret.Rename(baseName);
    //ret.
    return ret;
  }

  //: Generate a body constructor prototype from its handle counterpart
  
  MethodC ClassWizardBodyC::GenerateBodyConstructor(ObjectC &bodyConstructor) {
    MethodC ret;
    MethodC orginal(bodyConstructor);
    ret = orginal.Copy();
    StringC baseName = orginal.Var("BaseName").before("C",-1) + "BodyC";
    ret.Rename(baseName);
    //ret.
    return ret;    
  }

  //: Find name of base class.
  
  StringC ClassWizardBodyC::BaseClassName(ObjectC &aclass) {
    RCHashC<StringC,ObjectC> templSub;
    ClassC theclass(aclass);
    if(theclass.Uses().IsEmpty())
      return StringC();
    return theclass.Uses().First().FullName(templSub,descGen,100); //Var("classname");
  }

  //: Generate a list of base class names.
  
  DListC<StringC> ClassWizardBodyC::BaseClassNames(ObjectC &aclass) {
    RCHashC<StringC,ObjectC> templSub;
    ClassC theclass(aclass);
    DListC<StringC> baseClasses;
    for(DLIterC<ObjectC> it(theclass.Uses());it;it++)
      baseClasses.InsLast(it->FullName(templSub,descGen,100)); // Var("classname")
    return baseClasses;
  }
  
}
  
