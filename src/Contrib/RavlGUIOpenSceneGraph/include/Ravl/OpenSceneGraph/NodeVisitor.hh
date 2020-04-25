// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2012, Charles Galambos.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/Contrib/OpenSceneGraph/Node.hh"
//! lib=RavlGUIOpenSceneGraph
//! author = "Charles Galambos"
//! docentry = "Ravl.API.Graphics.OpenSceneGraph"
//! example = "exOpenSceneGraphWidget.cc"

#ifndef RAVLGUI_OPENSCENEGRAPHNODEVISITOR_HEADER
#define RAVLGUI_OPENSCENEGRAPHNODEVISITOR_HEADER 1

#include "Ravl/OpenSceneGraph/Node.hh"
#include <osg/NodeVisitor>


namespace RavlOSGN
{
  // Visitor base class.

  class NodeVisitorC
   : public osg::NodeVisitor
  {
  public:
     //! Constructor
    NodeVisitorC(TraversalMode tm=TRAVERSE_NONE)
     : osg::NodeVisitor(tm)
    {}

    //! Constructor
    NodeVisitorC(VisitorType type,TraversalMode tm=TRAVERSE_NONE)
     : osg::NodeVisitor(type,tm)
    {}


    //! Visitor method
    virtual bool Visit(NodeC &node);

    //! Used to filter nodes.
    virtual void apply(osg::Node& node);

  };
}

#endif
