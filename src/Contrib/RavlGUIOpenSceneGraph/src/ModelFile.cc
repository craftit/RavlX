// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/Contrib/OpenSceneGraph/ModelFile.cc"
//! lib=RavlGUIOpenSceneGraph
//! author = "Warren Moore"

#include "Ravl/OpenSceneGraph/ModelFile.hh"
#include "Ravl/XMLFactoryRegister.hh"
#include <osgDB/ReadFile>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlOSGN
{

  using namespace osg;

  ModelFileC::ModelFileC(const std::string &filename)
  {
    BuildNode(filename);
  }

  //: XML Factory constructor
  
  ModelFileC::ModelFileC(const XMLFactoryContextC &factory)
  {
    std::string filename = factory.AttributeString("filename","");
    BuildNode(filename);
    NodeC::Setup(factory);
  }

  ModelFileC::~ModelFileC()
  {
  }
  
  //: Build the node
  bool ModelFileC::BuildNode(const std::string &filename)
  {
    if(filename.empty())
      return false;
    m_node = osgDB::readNodeFile(filename);
    return m_node;
  }
  
  //: Zero owners reached.
  
  void ModelFileC::ZeroOwners()
  {
    NodeC::ZeroOwners();
  }

  void LinkModelFile()
  {}
  
  static RavlN::XMLFactoryRegisterConvertC<ModelFileC,NodeC> g_registerXMLFactoryModelFile("RavlOSGN::ModelFileC");
  
}
