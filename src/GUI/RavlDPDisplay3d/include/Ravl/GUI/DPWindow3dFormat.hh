// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2008-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_DPWINDOW3DFORMAT_HEADER
#define RAVLGUI_DPWINDOW3DFORMAT_HEADER 1
//////////////////////////////////////////////////////////////////////////////
//! rcsid="$Id: DPWindowFormat.hh 5342 2006-02-02 15:37:51Z hickson $"
//! lib=RavlDPDisplay3d
//! file="Ravl/GUI/DPDisplay/DPWindowFormat.hh"
//! author="Charles Galambos"
//! docentry="Ravl.API.Graphics.3D Data Display"

#include "Ravl/DP/FileFormat.hh"
#include "Ravl/GUI/DObject3D.hh"

namespace RavlGUIN {
  
  
  class DPWindow3dFormatBodyC
    : public FileFormatBodyC
  {
  public:
    DPWindow3dFormatBodyC();
    //: Default constructor.
    
    virtual const type_info &ProbeSave(const StringC &filename,const type_info &obj_type,bool forceFormat) const;
    //: Probe for Save.
    
    virtual DPOPortBaseC CreateOutput(const StringC &filename,const type_info &obj_type) const;
    //: Create a output port for saving.
    // Will create an Invalid port if not supported.
    
    virtual const type_info &DefaultType() const
    { return typeid(DObject3DC); }
    //: Get prefered IO type.
    
    virtual bool IsStream() const { return true; }
    //: Test if format is a fully streamable.
    // i.e. check if you can read/write more than object object.
    
  };

  class DPWindow3dFormatC
    : public FileFormatBaseC
  {
  public:
    DPWindow3dFormatC()
      : FileFormatBaseC(*new DPWindow3dFormatBodyC())
    {}
    //: Default constructor.
  };

}

#endif
