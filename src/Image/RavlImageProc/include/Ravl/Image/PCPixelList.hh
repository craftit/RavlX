// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_PCPIXELLIST_HEADER
#define RAVLIMAGE_PCPIXELLIST_HEADER 1
/////////////////////////////////////////////////////////
//! docentry="Ravl.API.Images.Lines"
//! file="Ravl/Image/Processing/Lines/PCPixelList.hh"
//! lib=RavlImageProc
//! author="Charles Galambos"
//! rcsid="$Id$"
//! date="07/01/1999"

#include "Ravl/Image/PCPixel.hh"
#include "Ravl/SArray1d.hh"
#include "Ravl/DLIter.hh"

namespace RavlImageN {
#if RAVL_VISUALCPP_NAMESPACE_BUG
  using namespace RavlN;
  using RavlN::DListC;
  using RavlN::DLIterC;
  using RavlN::IntT;
  using RavlN::RealT;
  using RavlN::SArray1dC;
  using RavlN::Point2dC;
  using RavlN::ByteT;
#endif

  template<class PixelT> class ImageC;
  
  //! userlevel=Normal
  //: List of PCPixels.
  // BIG OBJECT
  
  class PCPixelListC 
    : public DLIterC<PCIndex2dC> 
  {
  public:
    inline PCPixelListC()
    {}
    //: Default constructor.
    // NB. The list will be invalid!
    
    inline PCPixelListC(bool withList);
    //: Construct with new list.
    
    inline PCPixelListC(DListC<PCIndex2dC> &iter)
      : DLIterC<PCIndex2dC>(iter)
    {}
    //: Constructor.
    
    IntT TrimLine(RealT MaxDist,RealT &DistEst);
    // Trim points that are too far appart to code effiecently  and 
    // estimate average distance between points.  Starting from the
    // mid-point of the line.
    // Return: Number of points left.
    
    inline IntT TrimLine(RealT MaxDist);
    // Trim points that are too far appart to code effiecently.
    // Return: Number of points left.   Starting from the
    // mid-point of the line.
    // Return: Number of points left.
    
    IntT TrimLongest(RealT MaxDist,RealT &DistEst);
    // Trim points that are too far appart to code effiecently  and 
    // estimate average distance between points. But use the longest
    // segment of pixels without a gap. 
    // Return: Number of pixels in segment.
    
    inline IntT TrimLongest(RealT MaxDist);
    // Trim points that are too far appart to code effiecently  and 
    // estimate average distance between points. But use the longest
    // segment of pixels without a gap. 
    // Return: Number of pixels in segment.
    
    SArray1dC<Point2dC> PointList() const;
    //: Convert PCIndex2dC list to an array of 2d measurment points for fitting.
    // This returns a list of pixel locations as real points.
    
    static bool WriteGFHeader(ostream &out);
    //: Write out appropriate GF header.
    
    bool WriteGFPnts(ostream &out) const;
    //: Write points in GF format.
    // First number of points N, followed by N co-ordinate pairs.
    
    bool RemoveFromImage(ImageC<ByteT> &img);
    //: Remove points in list from the image.
    // Set the to zero.
    
    IntT Size() const 
    { return List().Size(); }
    //: Get number of points in list. (Slow)
    
    bool IsEmpty() const { 
      if(!IsValid())
	return true;
      return List().IsEmpty(); 
    }
    //: Is list infact empty ?
    
    void InsFirst(const PCIndex2dC &px)
    { const_cast<DListC<PCIndex2dC> &>(List()).InsFirst(px); }
    //: Insert new pixel at begining of list.
    
    void InsLast(const PCIndex2dC &px) 
    { const_cast<DListC<PCIndex2dC> &>(List()).InsLast(px); }
    //: Insert new pixel at end of list.
    
    DLIterC<PCIndex2dC> &Iter() { return *this; }
    //: Access the list.
    
    bool IsOk() { return IsElm(); }
    //: Is list valid ?
    
  };
  
  //////////////////////////////////////////////
  
  inline 
  PCPixelListC::PCPixelListC(bool withBod) {
    if(withBod) {
      DListC<PCIndex2dC> tmp;
      DLIterC<PCIndex2dC>::operator=(tmp);
    }
  }
  
  inline 
  IntT PCPixelListC::TrimLine(RealT MaxDist) {
    RealT Dummy;
    return TrimLine(MaxDist,Dummy);
  }
  
  inline 
  IntT PCPixelListC::TrimLongest(RealT MaxDist) {
    RealT Dummy;
    return TrimLongest(MaxDist,Dummy);
  }
}

#endif
