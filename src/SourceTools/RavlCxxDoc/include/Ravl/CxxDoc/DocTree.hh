// This file is part of CxxDoc, The RAVL C++ Documentation tool 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU General 
// Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
#ifndef RAVLCXXDOC_DOCTREE_HEADER
#define RAVLCXXDOC_DOCTREE_HEADER 1
////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/SourceTools/CxxDoc/DocTree.hh"
//! lib=RavlCxxDoc
//! userlevel=Normal
//! docentry="Ravl.API.Source Tools.CxxDoc.Internal"
//! author="Charles Galambos"
//! date="17/08/2000"


#include "Ravl/CxxDoc/DocNode.hh"

namespace RavlCxxDocN {
  
  //! userlevel=Develop
  //: Documentation tree body.
  
  class DocTreeBodyC 
    : public RCBodyC
  {
  public:
    DocTreeBodyC()
    {}
    //: Default constructor.

    DocTreeBodyC(const StringC &nProjName);
    //: Constructor.
    
    bool ReadDocentries(const StringC &fn);
    //: Read a docentries file.
    // This creates the appropriate doc node structure.

    bool ReadEHT(const StringC &fn);
    //: Read a EHT file.
    // Puts information into the appropriate node.

    bool ReadHTML(const StringC &fn);
    //: Read a HTML file.
    // Puts information into the appropriate node.
    
    bool ReadEHTSet(const StringC &dir);
    //: Read a set of EHT files from a directory.
    
    bool InsertDocLeaf(DListC<StringC> &node,const StringC &pos,const StringC &nm,const StringC &userlevel,const StringC &brief,const StringC &docFilename,const StringC &nodeType = StringC("class"));
    //: Insert a documentation leaf into the tree.
    // Note: If the path 'node' doesn't contain the project name 'projName' it will
    // be added both to the path and to 'pos' before its used.
    
    bool InsertDocLeaf(const StringC &node,const StringC &nm,const StringC &userlevel,const StringC &brief,const StringC &docFilename,const StringC &nodeType = StringC("class"));
    //: Insert a documentation leaf into the tree.
    
    DocNodeC &Root()
    { return root; }
    //: Root of docnode tree.
    
    void Dump(std::ostream &out) { 
      if(root.IsValid())
	root.Dump(out); 
      else
	out << "(NULL)\n";
    }
    //: Dump tree to std::ostream.
  protected:
    StringC projName;
    DocNodeC root; // Root node for documentation.
  };
  
  //! userlevel=Normal
  //: Documentation tree body.

  class DocTreeC 
    : public RCHandleC<DocTreeBodyC>
  {
  public:
    DocTreeC()
    {}
    //: Default constructor.
    // creates an invalid handle.

    DocTreeC(const StringC &projName)
      : RCHandleC<DocTreeBodyC>(*new DocTreeBodyC(projName))
    {}
    //: Constructor.
    
    bool ReadDocentries(const StringC &fn)
    { return Body().ReadDocentries(fn); }
    //: Read a docentries file.
    // This creates the appropriate doc node structure.

    bool ReadEHT(const StringC &fn)
    { return Body().ReadEHT(fn); }
    //: Read a EHT file.
    // Puts information into the appropriate node.
    
    bool ReadHTML(const StringC &fn)
    { return Body().ReadHTML(fn); }
    //: Read a html doc node file.
    // Puts information into the appropriate node.
    
    bool ReadEHTSet(const StringC &dir)
    { return Body().ReadEHTSet(dir); }
    //: Read a set of EHT files from a directory.
    
    bool InsertDocLeaf(const StringC &node,const StringC &nm,const StringC &userlevel,const StringC &brief,const StringC &docFilename,const StringC &nodeType = StringC("class"))
    { return Body().InsertDocLeaf(node,nm,userlevel,brief,docFilename,nodeType); }
    //: Insert a documentation leaf into the tree.
    
    bool InsertDocLeaf(DListC<StringC> &node,const StringC &pos,const StringC &nm,const StringC &userlevel,const StringC &brief,const StringC &docFilename,const StringC &nodeType = StringC("class"))
    { return Body().InsertDocLeaf(node,pos,nm,userlevel,brief,docFilename,nodeType); }
    //: Insert a documentation leaf into the tree.
    // Note: If the path 'node' doesn't contain the project name 'projName' it will
    // be added both to the path and to 'pos' before its used.
    
    DocNodeC &Root()
    { return Body().Root(); }
    //: Root of docnode tree.
    
    void Dump(std::ostream &out)
    { Body().Dump(out); }
    //: Dump tree to std::ostream.
    
  };
  
  
}

#endif
