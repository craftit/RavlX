// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/Tree.cc"

#include "Ravl/config.h"

#define GTK_ENABLE_BROKEN 1

#include "Ravl/GUI/Tree.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/GUI/Label.hh"
#include "Ravl/DLIter.hh"
#include "Ravl/Threads/Signal2.hh"
#include "Ravl/GUI/SignalInfo.hh"
#include "Ravl/HashIter.hh"

#include <gtk/gtk.h>


#define DODEBUG	0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlGUIN {

  // TreeItem /////////////////////////////////////////////////////////////////
  
  //: Constuctor.
  
  TreeItemBodyC::TreeItemBodyC(const StringC &text)
    : OneChildBodyC(LabelC(text))
  {}
  
  //: Subtree Constuctor.
  
  TreeItemBodyC::TreeItemBodyC(const StringC &text,const TreeC &nsubTree)
    : OneChildBodyC(LabelC(text)),
      subTree(nsubTree)
  {}
  
  //: Create the widget.
  
  bool TreeItemBodyC::Create() {
    if(widget != 0)
      return true;
    widget = gtk_tree_item_new();
    
    if(Child().IsValid()) {
      Child().GUIShow();  // Create it and show it.
      gtk_misc_set_alignment (GTK_MISC (Child().Widget()), 0.0, 0.5);
      gtk_container_add (GTK_CONTAINER (widget), Child().Widget());
    } else {
      ONDEBUG(std::cerr << "TreeItemBodyC::Create(), Warning : No child object. \n");
    }
    
    ConnectSignals();
    return true;
  }
  
  //: Add sub tree.
  
  void TreeItemBodyC::AddSubTree() {
    // NB. We can't add the sub tree until after,
  // the item has been added to the parent tree 
    // So we leave that for later.
    if(!subTree.IsValid())  // Sort out substree.
      return ;
    subTree.GUIShow();
    RavlAssert(widget != 0);
    gtk_tree_item_set_subtree (GTK_TREE_ITEM(widget),subTree.Widget());
  }
  
  // Tree /////////////////////////////////////////////////////////////////
  
  
  //: Constructor.
  
  TreeBodyC::TreeBodyC(bool rootOfTree)
    : root(rootOfTree)
  {}
  
  //: Is node exanded.
  // NB. Always true of root.
  
  bool TreeBodyC::IsExpanded() const {
#if 0
    if(subTree == widget) // Root.
      return true;
    return GTK_TREE_ITEM(widget)->expanded;
#else
    return true;
#endif
  }
  
  //: Is node visible ?
  // True if all nodes up to root are expanded.
  
  bool TreeBodyC::IsVisible() const {
#if 1
    GtkWidget *parent = widget;
    while(parent != 0) {
      if(GTK_IS_TREE_ITEM(parent)) {
	if(!GTK_TREE_ITEM(parent)->expanded)
	  return false;
      } else {
	RavlAssert(GTK_IS_TREE(parent)); // Must be Tree or TreeItem
	if(GTK_IS_ROOT_TREE(parent))
	  return true; // Done!
      }
      parent = parent->parent;
    }
    // Not parented, not visible.
#endif
    return false;
  }
  
  //: Add child to tree.
  // Thread safe.
  
  void TreeBodyC::AddChild(const TreeItemC &child) {
    Manager.Queue(Trigger(TreeC(*this),&TreeC::GUIAddChild,const_cast<TreeItemC &>(child)));
  }
  
  //: Add child to tree.
  // GUI thread only.
  
  void TreeBodyC::DelChild(const TreeItemC &child) {
    Manager.Queue(Trigger(TreeC(*this),&TreeC::GUIDelChild,const_cast<TreeItemC &>(child)));
  }
  
  //: Del child from tree.
  // GUI thread only.
  
  bool TreeBodyC::GUIDelChild(TreeItemC &child)  {
    RavlAssert(child.IsValid());
    if(!children.Del(child)) {
      std::cerr << "TreeBodyC::GUIDelChild(), ERROR: Child not found. \n";
      return true; // Child 
    }
    if(child.Widget() == 0)  //  || subTree == 0
      return true;
    // Make sure tree isn't deleted if its the last child.
    gtk_widget_ref(widget);
    //GtkWidget *owner = GTK_TREE(widget)->tree_owner;
    gtk_container_remove(GTK_CONTAINER(widget),child.Widget());
    gtk_widget_unref (widget);
    return true;
  }
  
  //: Add child to tree, the gtk end of things..
  
  bool TreeBodyC::GUIAddChildWidget(TreeItemC &child) {
    if(Widget() == 0) 
      return true;
    
    child.GUIShow();
    RavlAssert(child.Widget() != 0);
    gtk_tree_append (GTK_TREE(widget), child.Widget());
    child.AddSubTree(); 
    return true;
  }
  
  //: Add child to tree.
  // GUI thread only.
  
  bool TreeBodyC::GUIAddChild(TreeItemC &child) {
    //cerr << "TreeBodyC::GUIAddChild(); \n";
    
    if(!child.IsValid()) {
      std::cerr << "TreeBodyC::GUIAddChild(); WARNING: Ask to add invalid child. \n";
      return true;
    }
    children.InsLast(child);
    if(widget != 0)
      GUIAddChildWidget(child);
    return true;
  }
  
  //: Create the widget.
  
  bool TreeBodyC::Create() {
    if(widget != 0)
      return true;
    
    widget = gtk_tree_new ();
    
    gtk_tree_set_view_mode (GTK_TREE(widget), GTK_TREE_VIEW_ITEM);
    gtk_tree_set_selection_mode (GTK_TREE(widget),GTK_SELECTION_MULTIPLE);
    
    for(DLIterC<TreeItemC> it(children);it.IsElm();it.Next()) 
      GUIAddChildWidget(it.Data());
    
    ConnectSignals();
    
    return true;
  }
  
  
  //: Undo all references.
  
  void TreeBodyC::Destroy() {
    for(DLIterC<TreeItemC> it(children);it.IsElm();it.Next()) 
      it.Data().Destroy();
    WidgetBodyC::Destroy();
  }
  
}
