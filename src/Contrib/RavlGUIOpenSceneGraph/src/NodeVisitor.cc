
#include "Ravl/OpenSceneGraph/NodeVisitor.hh"
#include "Ravl/SysLog.hh"

#include <osg/Transform>
#include <osg/PositionAttitudeTransform>

namespace RavlOSGN
{


  bool NodeVisitorC::Visit(NodeC &node)
  {
    return true;
  }

  //! Used to filter nodes.
  void NodeVisitorC::apply(osg::Node& node) {
    NodeC::RefT theNode;
    //RavlDebug("Visiting '%s' ",node.className());
    if(NodeC::GetNode(&node,theNode) && theNode.IsValid())
      Visit(*theNode);

    //visitor.traverse(mode);
    osg::NodeVisitor::apply(node);
    //node.accept(*this);
  }

  // -------------------------------------------------------------

  //: Visit all the child nodes derived from NodeC of this one

  void NodeC::Accept(NodeVisitorC &visitor)
  {

    //visitor.traverse(*m_node);
    m_node->accept(visitor);
  }



}
