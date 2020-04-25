// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_TREE_HEADER
#define RAVLGUI_TREE_HEADER 1
//////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/GUI/GTK/Tree.hh"
//! lib=RavlGUI
//! author="Charles Galambos"
//! date="23/09/1999"
//! example=exTree.cc
//! docentry="Ravl.API.Graphics.GTK.Control"

#include "Ravl/GUI/Widget.hh"
#include "Ravl/GUI/OneChild.hh"

namespace RavlGUIN {
  class TreeC;
  class TreeItemC;
  class TreeBodyC;
  
  
  ///////////////////////////////////////////////////////////////////
  
  //! userlevel=Develop
  //: Tree body
  
  class TreeBodyC
    : public WidgetBodyC
  {
  public:
    TreeBodyC(bool rootOfTree);
    //: Constructor.
    
    virtual ~TreeBodyC() { }
    //: Have virtual methods - need virtual destructor
    
    void AddChild(const TreeItemC &child);
    //: Add child to tree.
    // Thread safe.
    
    void DelChild(const TreeItemC &child);
    //: Add child to tree.
    // Thread safe.
    
    bool GUIAddChild(TreeItemC &child);
    //: Add child to tree.
    // GUI thread only.
    
    bool GUIDelChild(TreeItemC &child);
    //: Add child to tree.
    // GUI thread only.
    
    bool IsRoot() const
    { return root; }
    //: Is root of tree ?
    
    bool IsExpanded() const;
    //: Is node exanded.
    // NB. Always true of root.
    
    bool IsVisible() const;
    //: Is node visible ?
    // True if all nodes up to root are expanded.
    
    Signal0C &TreeSignal(const char *);
    //: Get handle for named signal in Tree.
    
  protected:
    
    bool GUIAddChildWidget(TreeItemC &child);
    //: Add child to tree, the gtk end of things..
    
    virtual bool Create();
    //: Create the widget.
    
    virtual void Destroy();
    //: Undo all references.
    
    DListC<TreeItemC> children; // List of child tree items. 
    
    bool root;  // Root of tree.
    
    friend class TreeC;
  };
  
  //! userlevel=Obsolete
  //: Tree 
  // OBSOLETE! The gtk widget this relies on doesn't work.
  
  class TreeC
    : public WidgetC
  {
  public:
    TreeC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    TreeC(bool rootOfTree)
      : WidgetC(*new TreeBodyC(rootOfTree))
    {}
    //: Construct tree root.
    
  protected:
    TreeC(TreeBodyC &bod)
      : WidgetC(bod)
    {}
    //: Body constructor.
    
    TreeBodyC &Body()
    { return static_cast<TreeBodyC &>(WidgetC::Body()); }
    //: Access.
    
    const TreeBodyC &Body() const
    { return static_cast<const TreeBodyC &>(WidgetC::Body()); }
    //: Access.
    
  public:
    Signal0C &TreeSignal(const char *nm)
    { return Body().TreeSignal(nm); }
    //: Get handle for named signal in Tree.
    
    bool GUIAddChild(TreeItemC &child)
    { return Body().GUIAddChild(child); }
    //: Add child to tree.
    // GUI thread only.
    
    bool GUIDelChild(TreeItemC &child)
    { return Body().GUIDelChild(child); }
    //: Add child to tree.
    // GUI thread only.
    
    void AddChild(const TreeItemC &child)
    { Body().AddChild(child); }
    //: Add child to tree.
    // Thread safe, after .Show() is called!
    
    void DelChild(const TreeItemC &child)
    { Body().DelChild(child); }
    //: Add child to tree.
    // Thread safe.
    
    inline TreeItemC AddChild(const WidgetC &child);
    //: Add child widget to tree.
    // Thread safe, after .Show() is called!
    
    bool IsExpanded() const
    { return Body().IsExpanded(); }
    //: Is node exanded.
    // NB. Always true of root.
    
    bool IsVisible() const
    { return Body().IsVisible(); }
    //: Is node visible ?
    // True if all nodes up to root are expanded.
    
    friend class TreeBodyC;
  };
  
  ////////////////////////////////////////////////////////////////////
  
  //! userlevel=Develop
  //: Leaf of tree.
  
  class TreeItemBodyC
    : public OneChildBodyC
  {
  public:
    TreeItemBodyC()
    {}
    //: Default constructor
    
    TreeItemBodyC(const WidgetC &nchild)
      : OneChildBodyC(nchild)
    {}
    //: Constuctor.
    
    TreeItemBodyC(const StringC &text);
    //: Constuctor.
    
    TreeItemBodyC(const StringC &text,const TreeC &subTree);
    //: Subtree Constuctor.
    
  protected:
    
    virtual bool Create();
    //: Create the widget.
    
    void AddSubTree();
    //: Add sub tree.
    
    TreeC subTree;
    
    friend class TreeItemC;
  };
  
  //! userlevel=Normal
  //: Leaf of tree.
  
  class TreeItemC
    : public OneChildC
  {
  public:
    TreeItemC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    TreeItemC(const StringC &text)
      : OneChildC(*new TreeItemBodyC(text))
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    TreeItemC(const char *text)
      : OneChildC(*new TreeItemBodyC(StringC(text)))
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    TreeItemC(const WidgetC &child)
      : OneChildC(dynamic_cast<const TreeItemBodyC *>(BodyPtr(child)))
    {}
    //: Base class constructor.
    
    TreeItemC(const WidgetC &child,bool)
      : OneChildC(*new TreeItemBodyC(child))
    {}
    //: Sub widget...
    
    TreeItemC(const StringC &text,const TreeC &subTree)
      : OneChildC(*new TreeItemBodyC(text,subTree))
    {}
    //: Subtree Constuctor.
    
  protected:
    TreeItemC(TreeItemBodyC &bod)
      : OneChildC(bod)
    {}
    //: Body constructor.
    
    
    TreeItemBodyC &Body()
    { return static_cast<TreeItemBodyC &>(WidgetC::Body()); }
    //: Access.
    
    const TreeItemBodyC &Body() const
    { return static_cast<const TreeItemBodyC &>(WidgetC::Body()); }
    //: Access.
    
    void AddSubTree()
    { Body().AddSubTree(); }
    //: Add sub tree.
  public:
    
    friend class TreeBodyC;
  };
  
  
  inline 
  TreeItemC TreeC::AddChild(const WidgetC &child) {
    TreeItemC ret(const_cast<WidgetC &> (child));
    Body().AddChild(ret);
    return ret;
  }

}
#endif
