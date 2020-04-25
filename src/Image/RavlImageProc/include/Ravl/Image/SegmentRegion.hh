// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_SEGMENTREGION_HEADER
#define RAVLIMAGE_SEGMENTREGION_HEADER
//////////////////////////////////////////////////////////////
//! docentry="Ravl.API.Images.Segmentation"
//! userlevel=Develop
//! file="Ravl/Image/Processing/Segmentation/SegmentRegion.hh"
//! rcsid="$Id$"
//! lib=RavlImageProc
//! example=exSegmentation.cc
//! author="Ratna Rambaruth"
//! date="12/06/1998"

#include "Ravl/RCHandleV.hh"
#include "Ravl/Image/Image.hh"

namespace RavlImageN {
  using namespace RavlN;
  
  template<class StatT> class RegionSetC;
  
  //! userlevel=Develop
  //: Virtual segmentation class (provides interface for real classes)
  
  template<class PixelT, class StatT>
  class SegmentRegionBodyC 
    : public RCBodyVC 
  { 
  public: 
    SegmentRegionBodyC() {}
    //: Default constructor.
    
    virtual RegionSetC<StatT> Apply(const ImageC<PixelT> &in) {
      cout << "This function cannot be used\n";
      return RegionSetC<StatT>();
    }
    //: Perform segmentation on "in"
  };
  
  //! userlevel=Develop
  //: Handle class for a basic segmentation class
  
  template<class PixelT, class StatT>
  class SegmentRegionC 
    : public RCHandleC<SegmentRegionBodyC<PixelT,StatT> > 
  {
  public:
    SegmentRegionC() 
      : RCHandleC<SegmentRegionBodyC<PixelT,StatT> > (*new SegmentRegionBodyC<PixelT,StatT>())
    {}
    //: Default constructor
    
    inline RegionSetC<StatT> Apply(const ImageC<PixelT> &in) 
    { return Body().Apply(in); }
    //: Perform segmentation on "in"
    
  protected:
    SegmentRegionC(SegmentRegionC<PixelT,StatT> &bod)
      : RCHandleC<SegmentRegionBodyC<PixelT,StatT> >(bod)
    {}
    //: Constructor from the body class

    SegmentRegionC(SegmentRegionBodyC<PixelT,StatT> &bod)
      : RCHandleC<SegmentRegionBodyC<PixelT,StatT> >((SegmentRegionBodyC<PixelT,StatT> &)bod)
    {}
    //: Constructor from the body class
    
    SegmentRegionBodyC<PixelT,StatT> &Body()
    { return RCHandleC<SegmentRegionBodyC<PixelT,StatT> >::Body(); }
    //: Access body
    
    const SegmentRegionBodyC<PixelT,StatT> &Body() const
    { return RCHandleC<SegmentRegionBodyC<PixelT,StatT> >::Body(); }
    //: Access body
    
  };

}
#endif




