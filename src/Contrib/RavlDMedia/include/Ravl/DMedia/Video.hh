// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_DMEDIAVIDEO_HEADER
#define RAVLIMAGE_DMEDIAVIDEO_HEADER 1
///////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Contrib/DMedia/Video.hh"
//! lib=RavlDMedia
//! userlevel=Default
//! docentry="Ravl.API.Images.Video.Video IO.SGI DMedia"
//! author="Charles Galambos"
//! date="22/11/1999"

#include "Ravl/DMedia/NodePath.hh"

namespace RavlImageN
{

  //! userlevel=Develop
  //: Video buffer node body.
  
  class VLNodeVideoBodyC 
    : public VLNodeBodyC 
  {
  public:
    VLNodeVideoBodyC(const VLServerC &srv,int type,int number = VL_ANY);
    //: Constructor.
    // type  = VL_SRC,VL_DRN
    
  protected:
    virtual bool Init();
    //: Initalise componentes.
    
  };
  
  //! userlevel=Normal
  //: Video buffer node.
  
  class VLNodeVideoC 
    : public VLNodeC 
  {
  public:
    VLNodeVideoC()
      {}
    //: Default constructor.
    // Creates an invalid handle.
    
    VLNodeVideoC(const VLServerC &srv,int type,int number = VL_ANY)
      : VLNodeC(*new VLNodeVideoBodyC(srv,type,VL_ANY))
      {}
    //: Constructor.
    // type  = VL_SRC,VL_DRN
    
  protected:
    VLNodeVideoBodyC &Body()
      { return static_cast<VLNodeVideoBodyC &>(VLComponentC::Body()); }
    //: Access body.

    const VLNodeVideoBodyC &Body() const
      { return static_cast<const VLNodeVideoBodyC &>(VLComponentC::Body()); }
    //: Access body.
    
  public:
    
  };

}


#endif
