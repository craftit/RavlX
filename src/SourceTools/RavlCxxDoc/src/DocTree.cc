// This file is part of CxxDoc, The RAVL C++ Documentation tool 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU General 
// Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
///////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlCxxDoc
//! file="Ravl/SourceTools/CxxDoc/DocTree.cc"

#include "Ravl/CxxDoc/DocTree.hh"
#include "Ravl/StringList.hh"
#include "Ravl/Stream.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/OS/Directory.hh"
#include "Ravl/Text/TextFile.hh"
#include "Ravl/Text/TextCursor.hh"
#include "Ravl/SourceTools/SourceFile.hh"

#define DODEBUG 0
#if DODEBUG 
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlCxxDocN {

  //: Constructor.
  
  DocTreeBodyC::DocTreeBodyC(const StringC &nProjName)
    : projName(nProjName),
      root("root","root")
    {
    }
  
  //: Read a docentries file.
  // This creates the appropriate doc node structure.
  
  bool DocTreeBodyC::ReadDocentries(const StringC &fn) {
    ONDEBUG(std::cerr << "DocTreeBodyC::ReadDocentries() Reading file '" << fn << "'\n");
    IStreamC is(fn);
    if(!is) {
      ONDEBUG(std::cerr << "DocTreeBodyC::ReadDocentries() :");
      std::cerr << " ERROR:Failed to read docentries file '" << fn << "' \n";
      return false;
    }
    while(is) {
      StringC line;
      readline(is,line);
      //ONDEBUG(std::cerr <<"Readline:" << line << "\n");
      StringListC strList(line,true);
      DLIterC<StringC> it(strList);
      if(!it) continue; // Skip empty lines.
      StringC name = *it;
      it.Next(); if(!it) { std::cerr << "WARNING: Incomplete line in docentry. \n"; continue; }    
      StringC docFilename = *it;
      it.Next(); if(!it) { std::cerr << "WARNING: Incomplete line in docentry. \n"; continue; }    
      StringC userlevel = *it;
      it.Next(); if(!it) { std::cerr << "WARNING: Incomplete line in docentry. \n"; continue; }    
      StringC brief = *it;
      it.Next(); if(!it) { std::cerr << "WARNING: Incomplete line in docentry. \n"; continue; }    
      StringC position = *it;
      
      StringListC locations(position,";");
      DLIterC<StringC> pit(locations);
      for(;pit;pit++) {
	cerr << "Adding node '" << name << "' to :'" << *pit << "' from '" << position << "'\n";
	StringListC path(*pit,".");
	DocNodeC parent = root.AddNode(path); // Find parent node.
	// Add child.
	parent.Append(DocNodeC(name,*pit,userlevel,brief,docFilename,StringC("docnode"),true));
      }
    }
    //ONDEBUG(root.Dump(std::cerr));
    return true;
  }


  //: Read a EHT file.
  // Puts information into the appropriate node.
  
  bool DocTreeBodyC::ReadEHT(const StringC &fn) {
    ONDEBUG(std::cerr << "Reading EHT '" << fn << "' \n");
    IStreamC is(fn);
    if(!is) {
      ONDEBUG(std::cerr << "DocTreeBodyC::ReadEHT(), ");
      std::cerr << "Failed to open file '" << fn << "' \n";
      return false;
    }
    StringC brief;
    StringC detail;
    bool lastBrief = false;
    StringC line;
    HashC<StringC,StringC> vars;
    StringC docentry;
    StringC userlevel;
    
    while(is) {
      if(readline(is,line) < 1)
	continue; // Skip empty lines.
      if(lastBrief) {
	if(line[0] != ':') {
	  detail += line;
	  detail += '\n';
	  break;
	}
      }
      switch(line[0]) 
	{
	case ':': // Brief comment.
	  lastBrief = true;
	  brief += line.after(1);
	  continue;
	case '!': // Variable.
	  {
	    line = line.after(0);
	    IntT eat = line.index('=');
	    if(eat < 0) {
	      std::cerr << "Malformed variable in EHT file '" << fn << "' \n";
	      continue;
	    }
	    StringC name = line.before(eat);
	    StringC value = line.after(eat);
	    name = name.TopAndTail();
	    value = value.TopAndTail();
	    if(name == "userlevel") {
	      userlevel=value;
	      continue;
	    }
	    if(value.length() > 1) { 
	      // Quoted value ?
	      if(value.firstchar() == '"' && value.lastchar() == '"') {
		value = value.after(0);
		value = value.before('"',-1);
	      }
	    }
	    vars[name] = value;
	  }
	  continue;
	case '<': // Possible html ? 
	default:
	  // Some other rubbish, ignore it.
	  break;
	}
    }
    // Read detail comment.
    while(is) {
      if(readline(is,line,'\n',0) == 0)
	break; 
      detail += line;
    }
    
    // Insert node
    
    brief = brief.TopAndTail(); // Get rid of nasty white space.
    if(vars.IsElm("docentry"))
      docentry = vars["docentry"];
    else {
      StringC afn(fn);
      docentry = afn.after(filenameSeperator,-1);
      docentry = docentry.before('.',-1); // Just get text before last '.'
      docentry.gsub("_"," "); // Turn underscores into spaces.
    }
    
    StringListC locations(docentry,";");
    DLIterC<StringC> pit(locations);
    DListC<StringC> oc;
    if(vars.IsElm("children"))
      oc = StringListC(vars["children"],";");
    
    for(;pit;pit++) {
      ONDEBUG(std::cerr << "Updating node '" << *pit << "'\n");
      StringListC path(*pit,".");
      if(path.IsEmpty())
	path.InsFirst(projName);
      if(path.First() != projName)
	path.InsFirst(projName);
      DocNodeC node = root.AddNode(path); // Find parent node.
      node.UpdateVars(vars);
      if(userlevel!="")
	node.SetUserlevel(userlevel);
      if(detail != "")
	node.SetDetailComment(detail);
      if(brief != "")
	node.SetBriefComment(brief);
      if(!oc.IsEmpty())
	node.OrderChildren(oc);
    }
    
    return true;
  }

  //: Read a HTML file.
  // Puts information into the appropriate node.
  
  bool DocTreeBodyC::ReadHTML(const StringC &fn) {
    ONDEBUG(std::cerr << "DocTreeBodyC::ReadHTML(), Called for '" << fn << "' \n");
    TextFileC tf(true);
    if(!tf.Load(fn)) 
      return false;
    StringC brief,children,docentry,detail,author,userlevel("Default");
    HashC<StringC,StringC> vars;
    
    TextCursorC it(tf);
    ONDEBUG(std::cerr << "Extracting variables. \n");
    while(it.IsElm()) {
      if(!it.SkipTo("<!--! "))
	break; // Not found.
      StringC varLine = it.ClipTo("-->");
      int ind = varLine.index('=');
      if(ind < 0)
	continue;
      StringC varName = varLine.before(ind).TopAndTail();
      if(varName == "")
	continue; // No name found.
      StringC value = varLine.after(ind).TopAndTail();
      if(value.length() > 1) {
	// Quoted value ?
	if(value.firstchar() == '"' && value.lastchar() == '"') {
	  value = value.after(0);
	  value = value.before('"',-1);
	}
      }
      vars[varName] = value;
      if(varName == "author") {
	author = value;
	continue;
      }
      if(varName == "userlevel") {
	userlevel = value;
	continue;
      }
      if(varName == "docentry") {
	docentry = value;
	continue;
      }
      if(varName == "children") {
	children = value;
	continue;
      }
    }
    
    ONDEBUG(std::cerr << "Extracting content. \n");
    it.First();
    it.SkipTo("<head>");
    if(brief == "") {
      if(it.SkipTo("<title>"))
	brief = it.ClipTo("</title>").TopAndTail();
    }
    it.SkipTo("</head>");
    if(it.SkipTo("<body>"))
      detail = it.ClipTo("</body>",true);
    
    if(docentry == "") // Default to file name without extention for docentry.
      docentry = StringC(StringC(fn).before('.',-1)).after('/',-1);
    
    ONDEBUG(std::cerr << "Adding node. '"<< docentry <<"' Brief:'"  << brief << "'\n");
    
    StringListC locations(docentry,";");    
    DLIterC<StringC> pit(locations);
    DListC<StringC> oc;
    if(children != "")
      oc = StringListC(children,";");
    for(;pit;pit++) {
      ONDEBUG(std::cerr << "Updating node '" << *pit << "'\n");
      StringC pathnm = pit->Copy();
      StringListC path(pathnm,".");
      if(path.IsEmpty())
	path.InsFirst(projName);
      if(path.First() != projName)
	path.InsFirst(projName);
      DocNodeC node = root.AddNode(path,true); // Find parent node.
      node.UpdateVars(vars);
      if(userlevel!="")
	node.SetUserlevel(userlevel);
      if(detail != "")
	node.SetDetailComment(detail);
      if(brief != "")
	node.SetBriefComment(brief);
      if(!oc.IsEmpty())
	node.OrderChildren(oc);
    }    
    
    return true;
  }

  //: Read a set of EHT files from a directory.
  
  bool DocTreeBodyC::ReadEHTSet(const StringC &fn) {
    FilenameC adir(fn);
    if(adir.IsDirectory()) {
      DirectoryC dir(adir);
      DListC<StringC> fl = dir.List();
      for(DLIterC<StringC> it(fl);it;it++) {
	FilenameC efn(adir + filenameSeperator + *it);
	ONDEBUG(std::cerr << "DocTreeBodyC::ReadEHTSet(), Found file '" << efn << "' \n");
	if(!efn.IsRegular())
	  continue;
	StringC aext = efn.Extension();
	if(aext == "eht") {
	  ReadEHT(efn);
	  continue;
	}
	if(aext == "html" || aext == "htm") {
	  ReadHTML(efn);
	  continue;
	}
	cerr << "WARNING: Don't know now to process EHT file '" << efn <<"' (Ext:" << aext <<")\n";
      }
      return true;
    }
    ONDEBUG(std::cerr << "DocTreeBodyC::ReadEHTSet() '" << fn << "' isn't a directory. \n");
    return ReadEHT(adir); // Try reading as a single file.
  }

  //: Insert a documentation leaf into the tree.
  
  bool DocTreeBodyC::InsertDocLeaf(DListC<StringC> &path,const StringC &xpos,
				   const StringC &nm,
				   const StringC &userlevel,
				   const StringC &brief,
				   const StringC &docFilename,
				   const StringC &nodeType) {
    
    StringC pos = xpos;
    if(!path.IsEmpty()) {
      if(path.First() != projName) {
	path.InsFirst(projName);
	pos = projName + '.' + xpos;
      }
    } else {
      path.InsFirst(projName);
      pos = projName + '.' + xpos;
    }
    DocNodeC parent = root.AddNode(path); // Find parent node.
    ONDEBUG(std::cerr << "DocTreeBodyC::InsertDocLeaf(), Adding leaf '" << pos << "' to node '" << parent.Name() << "' \n");
    // Add child.
    parent.Append(DocNodeC(nm,pos,userlevel,brief,docFilename,nodeType,true));
    return true;
  }

  //: Insert a documentation leaf into the tree.
  
  bool DocTreeBodyC::InsertDocLeaf(const StringC &path,const StringC &nm,const StringC &userlevel,const StringC &brief,const StringC &docFilename,const StringC &nodeType) {
    StringListC places(path,";");
    for(DLIterC<StringC> it(places);it;it++) {
      StringListC npath(*it,".");
      if(!InsertDocLeaf(npath,path,nm,userlevel,brief,docFilename,nodeType))
	return false;
    }
    return true;
  }
  

}
