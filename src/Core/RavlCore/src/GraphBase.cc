// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Container/Graph/GraphBase.cc"

#include "Ravl/config.h"
#include "Ravl/GraphBase.hh"
#include "Ravl/GraphBaseLinearIter.hh"

#if RAVL_HAVE_ANSICPPHEADERS
#include <iostream>
#else
#include <iostream.h>
#endif

namespace RavlN {
  
  ///////////////////////
  // Test if a directed graph contains cycles.
  
  bool GraphBaseC::IsCyclic() {
    GraphBaseLinearIterC it(*this);
    if(it)
      while(++it) ;
    return it.IsCycle();
  }
  
  // ----------------------------------------------------------------------
  // ************ GraphEdgeRepC *******************************************
  // ----------------------------------------------------------------------
  
  void 
  GraphEdgeBaseC::Redirect() {
#if 1
    RavlAssertMsg(0,"GraphEdgeC::Redirect(), Not implemented. ");
#else
    GraphAdjIterBaseC s(SourceAdj());
    GraphAdjIterBaseC t(TargetAdj());
    if (Graph().Type() == GraphBaseBodyC::UNDIRECTED) {
      s.MoveAft(t);
      t.MoveAft(s);
    } else {
      Source().In().MoveBef(t);
      Target().Out().MoveBef(s);
    }  
    Data().source.SwapNodes(Data().target);  
#endif
  }
  
  // ----------------------------------------------------------------------
  // ************ GraphNodeRepC *******************************************
  // ----------------------------------------------------------------------
  
  // ----------------------------------------------------------------------
  // ************ GraphBaseBodyC **************************************************
  // ----------------------------------------------------------------------  
}

