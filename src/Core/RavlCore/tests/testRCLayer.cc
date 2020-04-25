// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2005, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! docentry="Ravl.API.Core.Reference Counting"
//! rcsid="$Id$"
//! lib=RavlCore

#include "Ravl/RCLayer.hh"
#include "Ravl/Stream.hh"
#include "Ravl/SmartLayerPtr.hh"
#include "Ravl/SmartPtr.hh"

using namespace RavlN;

class RCLayerTestBodyC 
  : public RCLayerBodyC
{
public:
  RCLayerTestBodyC() {
    std::cerr << "RCLayerTestBodyC, Constructor.  \n";
    loopyHandle = RCLayerC<RCLayerTestBodyC>(*this,RCLH_CALLBACK);    
  }
  //: Default constructor.
  
  ~RCLayerTestBodyC() {
    std::cerr << "RCLayerTestBodyC, Destructor.  \n";
  }
  //: Destructor
  
  virtual void ZeroOwners() {
    std::cerr << "Hurra all the bosses have gone away. \n";
    loopyHandle.Invalidate();
  }
  //: Called when owner handles drop to zero.
  
protected:
  RCLayerC<RCLayerTestBodyC> loopyHandle;
};


int testSmartPtr();
int testRCLayer();

int main() {
  int lineno;
  if((lineno = testSmartPtr()) != 0) {
    std::cerr << "Test failed " << lineno << "\n";
    return 1;
  }
  if((lineno = testRCLayer()) != 0) {
    std::cerr << "Test failed " << lineno << "\n";
    return 1;
  }
  
  return 0;
}

static int g_classInstances = 0;
class AClassC
 : public RCBodyVC
{
public:
  AClassC()
  { g_classInstances++; }

  ~AClassC()
  { g_classInstances--; }
};

int testSmartPtr() {
  if(g_classInstances != 0) return __LINE__;
  SmartPtrC<AClassC> ptr1 = *new AClassC();
  if(ptr1->References() != 1) return __LINE__;
  if(g_classInstances != 1) return __LINE__;
  SmartPtrC<const AClassC> ptr2 = ptr1.BodyPtr();
  if(ptr1->References() != 2) return __LINE__;
  if(g_classInstances != 1) return __LINE__;
  ptr1.Invalidate();
  if(ptr2->References() != 1) return __LINE__;
  ptr2.Invalidate();
  if(g_classInstances != 0) return __LINE__;
  std::cout <<"SmartPtr test passed ok. \n";
  return 0;
}

int testRCLayer() {
  RCLayerC<RCLayerTestBodyC> layerH1(*new RCLayerTestBodyC(),RCLH_OWNER);
  RCLayerC<RCLayerTestBodyC> layerH2 = layerH1;
  std::cerr << "Handles=" << layerH2.OwnerHandles() << "\n";
  RCLayerC<RCLayerTestBodyC> layerH3 = RCLayerC<RCLayerTestBodyC>(layerH1,RCLH_OWNER);
  std::cerr << "Handles=" << layerH2.OwnerHandles() << "\n";
  layerH3 = layerH1;
  if(layerH3.OwnerHandles() != 3) return __LINE__;
  std::cerr << "Handles=" << layerH2.OwnerHandles() << " All=" << layerH2.References() << "\n";
  if(layerH2.References() != 4) return __LINE__;
  return 0;
}
