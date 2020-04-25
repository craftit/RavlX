// This file is part of CxxDoc, The RAVL C++ Documentation tool 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU General 
// Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlCxxDoc
//! file="Ravl/SourceTools/CxxDoc/DocNode.cc"

#include "Ravl/CxxDoc/DocNode.hh"
#include "Ravl/DLIter.hh"
#include "Ravl/CDLIter.hh"
#include "Ravl/HashIter.hh"
#include "Ravl/HSet.hh"

#define DODEBUG 0

#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlCxxDocN {
  
  static HSetC<StringC> UserLevels() {
    HSetC<StringC> ret;
    ret += "Basic";
    ret += "Obsolete";
    ret += "Normal";
    ret += "Advanced";
    ret += "Develop";
    ret += "Default";
    return ret;
  }
  
  static HSetC<StringC> validUserlevels = UserLevels();
  
  //// DocNodeBodyC ////////////////////////////////
  
  //: Default constructor.
  
  DocNodeBodyC::DocNodeBodyC(const StringC &nm,
			     const StringC &nposition ,
			     const StringC &userlvl,
			     const StringC &brief,
			     const StringC &docName,
			     const StringC &nnodeType,
			     bool aplaceHolder)
    : ObjectListBodyC(nm),
      docFilename(docName),
      userlevel(userlvl),
      position(nposition),
      nodeType(nnodeType),
      placeHolder(aplaceHolder)
  {
    ONDEBUG(std::cerr << "DocNodeBodyC::DocNodeBodyC(), Creating node :'" << nm << "' DocName:'" << docName << "' Userlevel:'" << userlvl << "' \n");
    Comment().Header() = brief;
    SetVar("brief",brief);
    if(validUserlevels[userlvl])
      SetVar("userlevel",userlvl);
    else {
      std::cerr << "WARNING: Invalid userlevel detected '" << userlvl << "' in node :'" << nm << "' DocName:'" << docName << "' \n";
      SetVar("userlevel","Default");
    }
    SetVar("docentry",nposition);
    SetVar("docNodeType",nodeType);
    if(docFilename != "")
      SetVar("content",docFilename);
  }
  
  //: Set the user level of this node.
  
  bool DocNodeBodyC::SetUserlevel(const StringC &value) {
    userlevel=value;
    SetVar("userlevel",value);
    return true;
  }
  
  //: Set the detailed comment of the node.
  
  bool DocNodeBodyC::SetDetailComment(const StringC &value) {
    Comment().Text() = value;
    SetVar("detail",value);
    return true;
  }
  
  //: Set the brief comment of the node.
  
  bool DocNodeBodyC::SetBriefComment(const StringC &value) {
    Comment().Header() = value;
    SetVar("brief",value);
    return true;
  }
  
  //: Update node's variables.
  
  bool DocNodeBodyC::UpdateVars(const HashC<StringC,StringC> &value) {
    comment.Vars().Add(value,true);
    return true;
  }
  
  //: Find/Create node corresponding to the the address including and after 'it'.
  
  DocNodeC DocNodeBodyC::AddNode(const DListC<StringC> &id,bool create,const StringC &path) {
    StringC newPath = path;
    DocNodeC place = DocNodeC(*this);
    for(ConstDLIterC<StringC> it(id);it;it++) {
      StringC tmp = it->Copy();
      tmp.gsub("_"," ");
      newPath = newPath.Copy();
      if(newPath.length() > 0)
	newPath += '.';
      newPath += tmp;
      place = place.FindChild(tmp,create,newPath);
      if(!place.IsValid()) {
	ONDEBUG(std::cerr << "DocNodeBodyC::AddNode(), Failed to create node. \n");
	return DocNodeC();
      }
    }
    return place;
   }
  
  //: Find the named child.
  
  DocNodeC DocNodeBodyC::FindChild(const StringC &aname,bool doadd,const StringC &path) {
    ObjectC child = Find(aname);
    if(!child.IsValid()) {
      if(!doadd) 
	return DocNodeC();
      DocNodeC newnode = DocNodeC(aname,path);
      Append(newnode);
      return newnode;
    }
    return DocNodeC(child);
  }

  //: Put children in given order.
  
  bool DocNodeBodyC::OrderChildren(const DListC<StringC> &children) {
    DListC<ObjectC> newlist;
    for(DLIterC<StringC> it(children);it;it++) {
      bool found = false;
      for(DLIterC<ObjectC> sit(list);sit;sit++) {
	if(sit->Name() == *it) {
	  newlist.MoveLast(sit); // Move item to back of new list.
	  found = true;
	  break;
	}
      }
      if(!found) { // Need to add a new node ?
	DocNodeC newnode = DocNodeC(*it,position + '.' + *it);
	newlist.InsLast(newnode);
      }
    }
    list.MoveFirst(newlist) ; // Put ordered items to the front of the list.
    return true;
  }
  
}
