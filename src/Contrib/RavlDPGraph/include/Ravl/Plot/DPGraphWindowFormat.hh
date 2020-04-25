// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLPLOT_DPWINDOWFORMAT_HEADER
#define RAVLPLOT_DPWINDOWFORMAT_HEADER 1
//////////////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlDPGraph
//! file="Ravl/Contrib/Plotting/DPGraph/DPGraphWindowFormat.hh"
//! author="Charles Galambos"
//! docentry="Ravl.API.Graphics.Plotting.DPGraph"

#include "Ravl/DP/FileFormat.hh"
#include "Ravl/Array1d.hh"
#include "Ravl/Point2d.hh"

namespace RavlPlotN {
  using namespace RavlN;
  
  class DPGraphWindowFormatBodyC
    : public FileFormatBodyC
  {
  public:
    DPGraphWindowFormatBodyC();
    //: Default constructor.
    
    virtual const type_info &ProbeSave(const StringC &filename,const type_info &obj_type,bool forceFormat) const;
    //: Probe for Save.
    
    virtual DPOPortBaseC CreateOutput(const StringC &filename,const type_info &obj_type) const;
    //: Create a output port for saving.
    // Will create an Invalid port if not supported.
    
    virtual const type_info &DefaultType() const
    { return typeid(Array1dC<Point2dC>); }
    //: Get prefered IO type.
    
    virtual bool IsStream() const { return true; }
    //: Test if format is a fully streamable.
    // i.e. check if you can read/write more than object object.
    
  };

  class DPGraphWindowFormatC
    : public FileFormatBaseC
  {
  public:
    DPGraphWindowFormatC()
      : FileFormatBaseC(*new DPGraphWindowFormatBodyC())
    {}
    //: Default constructor.
  };

}

#endif
