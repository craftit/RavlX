// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2011, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/* 
 * File:   LayoutStack.hh
 * Author: charlesgalambos
 *
 * Created on April 25, 2011, 10:13 AM
 */
//! lib=RavlGUIOpenSceneGraph

#ifndef RAVLOSGN_LAYOUTSTACK_HH
#define	RAVLOSGN_LAYOUTSTACK_HH

#include "Ravl/OpenSceneGraph/Layout.hh"

namespace RavlOSGN {

  //: Base class for automatic layouts
  
  class LayoutStackC
   : public LayoutC
  {
  public:
    LayoutStackC(bool create);
    //: Constructor
    
    LayoutStackC(const XMLFactoryContextC &factory);
    //: XML factory constructor
    
  protected:
    virtual bool UpdateLayout();
    //: Update the layout
    
    virtual void DoCallback();
    //: Process a callback.
    
    bool Setup(const XMLFactoryContextC &factory);
    //: Do setup from factory
    
    virtual void ZeroOwners();
    //: Called when owner handles drop to zero.
    
    int m_stackAxis;
    bool m_invertAxis;
    float m_gap;
  };

}

#endif	/* LAYOUTSTACK_HH */

