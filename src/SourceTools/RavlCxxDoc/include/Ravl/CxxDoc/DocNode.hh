// This file is part of CxxDoc, The RAVL C++ Documentation tool 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU General 
// Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
#ifndef RAVLCXXDOC_DOCNODE_HEADER
#define RAVLCXXDOC_DOCNODE_HEADER 1
////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/SourceTools/CxxDoc/DocNode.hh"
//! lib=RavlCxxDoc
//! userlevel=Normal
//! docentry="Ravl.API.Source Tools.CxxDoc.Internal"
//! author="Charles Galambos"
//! date="17/08/2000"

#include "Ravl/String.hh"
#include "Ravl/RefCounter.hh"
#include "Ravl/DList.hh"
#include "Ravl/Hash.hh"
#include "Ravl/CxxDoc/Object.hh"

namespace RavlCxxDocN {
  using namespace RavlN;
  
  class DocNodeC;
  
  //! userlevel=Develop
  //: Body for a doc node.
  
  class DocNodeBodyC 
    : public ObjectListBodyC
  {
  public:
    DocNodeBodyC(const StringC &nm,
		 const StringC &position = StringC(),
		 const StringC &userlvl = "Default",
		 const StringC &brief = StringC(),
		 const StringC &docName = StringC(),
		 const StringC &nodeType = StringC("class"),
		 bool aplaceHolder = false);
    //: Default constructor.
    
    DocNodeC AddNode(const DListC<StringC> &id,bool create = true,const StringC &path = StringC());
    //: Find/Create node corresponding to the the address including and after 'it'.
    
    DocNodeC FindChild(const StringC &name,bool doadd = false,const StringC &path = StringC());
    //: Find the named child.
    // if doadd is true, add the new DocNode. 
    // NB. this may fail if node exists, but is not a DocNode.

    StringC &DocFilename()
    { return docFilename; }
    //: Filename for info on this node.
    // This may be an empty string.
    
    StringC &Userlevel()
    { return userlevel; }
    //: User level for this node.

    bool SetUserlevel(const StringC &value);
    //: Set the user level of this node.

    bool SetDetailComment(const StringC &value);
    //: Set the detailed comment of the node.
    
    bool SetBriefComment(const StringC &value);
    //: Set the brief comment of the node.

    bool UpdateVars(const HashC<StringC,StringC> &value);
    //: Update node's variables.
    
    bool OrderChildren(const DListC<StringC> &children);
    //: Put children in given order.
    
    virtual const char *TypeName() const { 
      if(placeHolder)
	return "docleaf";
      return "docnode"; 
    }
    //: Get name of object type.
    
    virtual StringC ActualPath() const
    { return position; }
    //: Resolve data references correctly.
    
  protected:
    StringC docFilename; // Filename for info on this node.
    StringC userlevel;
    StringC position;
    StringC nodeType;
    bool placeHolder; // Is this a placeholder for another page?
    friend class DocNodeC;
  };
  
  //! userlevel=Normal
  //: Doc node.
  
  class DocNodeC 
    : public ObjectListC
  {
  public:
    DocNodeC()
    {}
    //: Default constructor.
    // Creates an invalid handle.

    DocNodeC(ObjectC &obj)
      : ObjectListC(dynamic_cast<DocNodeBodyC *>(BodyPtr(obj)))
    {}
    //: Base class constructor.
    // Node if the base class is of a different type an invalid handle
    // is created.
    
    DocNodeC(const StringC &nm,
	     const StringC &position = StringC(),
	     const StringC &userlvl = "Default",
	     const StringC &brief = StringC(),
	     const StringC &docName = StringC(),
	     const StringC &nodeType = StringC("class"),
	     bool aplaceHolder = false
	     )
      : ObjectListC(*new DocNodeBodyC(nm,position,userlvl,brief,docName,nodeType,aplaceHolder))
    {}
    //: Constructor.
    
  protected:
    DocNodeC(DocNodeBodyC &bod)
      : ObjectListC(bod)
    {}
    //: Body constructor.
    
    DocNodeC(const DocNodeBodyC *bod)
      : ObjectListC(bod)
    {}
    //: Body constructor.
    
    DocNodeBodyC &Body() 
    { return dynamic_cast<DocNodeBodyC &>(ObjectC::Body()); }
    //: Access body.
    
    const DocNodeBodyC &Body() const
    { return dynamic_cast<const DocNodeBodyC &>(ObjectC::Body()); }
    //: Access body.
    
  public:    
    DocNodeC AddNode(const DListC<StringC> &id,bool create = true,const StringC &path = StringC())
    { return Body().AddNode(id,create,path); }
    //: Find/Create node corresponding to the the address including and after 'it'.
    
    DocNodeC FindChild(const StringC &id,bool doadd = false,const StringC &path = StringC())
    { return Body().FindChild(id,doadd,path); }
    //: Find the named child.
    // if doadd is true, add the new DocNode. 
    // NB. this may fail if node exists, but is not a DocNode.
    
    bool SetUserlevel(const StringC &value)
    { return Body().SetUserlevel(value); }
    //: Set the user level of this node.

    bool SetDetailComment(const StringC &value)
    { return Body().SetDetailComment(value); }
    //: Set the detailed comment of the node.
    
    bool SetBriefComment(const StringC &value)
    { return Body().SetBriefComment(value); }
    //: Set the brief comment of the node.
    
    bool UpdateVars(const HashC<StringC,StringC> &value)
    { return Body().UpdateVars(value); }
    //: Update node's variables.
    
    bool OrderChildren(const DListC<StringC> &children)
    { return Body().OrderChildren(children); }
    //: Put children in given order.
    
    friend class DocNodeBodyC;
  };
  
}
#endif
