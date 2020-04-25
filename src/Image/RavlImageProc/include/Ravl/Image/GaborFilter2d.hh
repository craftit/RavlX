// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_GABORFILTER_HEADER
#define RAVLIMAGE_GABORFILTER_HEADER 1
////////////////////////////////////////////////////////////////////////////
//! docentry="Ravl.API.Images.Filtering"
//! rcsid="$Id$"
//! author="Kieron Messer"
//! date="17/10/2000"
//! lib=RavlImageProc
//! example=exGaborFilter.cc
//! file="Ravl/Image/Processing/Filters/Gabor/GaborFilter2d.hh"

#include "Ravl/Array1d.hh"
#include "Ravl/SArray1d.hh"
#include "Ravl/SArray2dIter2.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/FFT2d.hh"
#include "Ravl/RefCounter.hh"

namespace RavlImageN {
  
  //! userlevel=Develop
  //: Gabor filters.
  
  class GaborFilterBank2dBodyC 
    : public RCBodyC
  {
  public:
    GaborFilterBank2dBodyC(Index2dC size);
    //: Constructor.
    // Uses default filter bank: <br>
    // Angles = 0.0, 45.0, 90.0, 135.0 <br>
    // Scales = 0.125, 0.35 
    
    GaborFilterBank2dBodyC(const SArray1dC<RealT> &angles,const SArray1dC<RealT> &scales,Index2dC size = Index2dC(0,0));
    //: Constructor.
    
    GaborFilterBank2dBodyC();
    //: Constructor.
    
    Array1dC<ImageC<RealT> > &Filters()
    { return filter; }
    //: Access filters.
    
    void SetupFilters(Index2dC imgSize);
    //: Setup filters for given image size.
    
    SArray1dC<ImageC<RealT> > Apply(const ImageC<RealT> &rim);
    //: Apply filters to a real image.
    
    void ConvToReal(const ImageC<RealT> &im,ImageC<RealT> &rim)
    { rim = im; }
    //: Image converter.
    
    template<typename PixelT>
    void ConvToReal(const ImageC<PixelT> &im,ImageC<RealT> &rim) { 
      rim = ImageC<RealT>(im.Size());
      for(SArray2dIter2C<RealT,PixelT> it(rim,im);it;it++)
	it.Data1() = it.Data2();
    }
    //: Image converter.
    
    const SArray1dC<RealT> &Angles() const
    { return angleArr; }
    //: Access filter angles.
    
    const SArray1dC<RealT> &Scales() const
    { return angleArr; }
    //: Access filter scales.
    
    void Angles(const SArray1dC<RealT> &angles)  { 
      angleArr = angles; 
      filterSize = Index2dC(0,0);
    }
    //: Access filter angles.
    
    void Scales(const SArray1dC<RealT> &scales) { 
      scaleArr = scales; 
      filterSize = Index2dC(0,0);
    }
    //: Access filter scales.
    
  protected:
    void MakeGaborFilters(int ymax, int xmax);
    //: Generate Gabor filters.
    
    Array1dC<ImageC<RealT> > filter;
    //: the filters for the given parameters
    
    Index2dC filterSize;
    
    SArray1dC<RealT> angleArr;
    SArray1dC<RealT> scaleArr;
    
    
    FFT2dC forward;
    FFT2dC backward;

  };

  //! userlevel=Develop
  //: Gabor filters.
  // This class is kept for backwards compatibility.  Use <a href="RavlImageN.GaborComplexC.html"> GaborComplexC </a> instead.
  
  class GaborFilterBank2dC 
    : public RCHandleC<GaborFilterBank2dBodyC>
  {
  public:
    GaborFilterBank2dC(Index2dC size)
      : RCHandleC<GaborFilterBank2dBodyC>(*new GaborFilterBank2dBodyC(size))
    {}
    //: Constructor.
    
    GaborFilterBank2dC(const SArray1dC<RealT> &angles,const SArray1dC<RealT> &scales,Index2dC size = Index2dC(0,0))
      : RCHandleC<GaborFilterBank2dBodyC>(*new GaborFilterBank2dBodyC(angles,scales,size))
    {}
    //: Constructor.
    
  protected:
    GaborFilterBank2dC(GaborFilterBank2dBodyC &bod)
      : RCHandleC<GaborFilterBank2dBodyC>(bod)
    {}
    //: Body constructor
    
    GaborFilterBank2dBodyC &Body()
    { return RCHandleC<GaborFilterBank2dBodyC>::Body(); }
    //: Access body.
    
    const GaborFilterBank2dBodyC &Body() const
    { return RCHandleC<GaborFilterBank2dBodyC>::Body(); }
    //: Access body.
    
  public:
    Array1dC<ImageC<RealT> > &Filters()
    { return Body().Filters(); }
    //: Access filters.
    
    const SArray1dC<RealT> &Angles() const
    { return Body().Angles(); }
    //: Access filter angles.
    
    const SArray1dC<RealT> &Scales() const
    { return Body().Scales(); }
    //: Access filter scales.
    
    void Angles(const SArray1dC<RealT> &angles)
    { Body().Angles(angles); }
    //: Access filter angles.
    
    void Scales(const SArray1dC<RealT> &scales) 
    { Body().Scales(scales); }
    //: Access filter scales.
    
  };
  
  //! userlevel=Develop
  //: Body class for computing set of Gabor filters
  //
  // You should not use this class directly, but rather the handle class
  // GaborFilter2dC.
  //
  
  template <class PixelT>
  class GaborFilter2dBodyC
    : public GaborFilterBank2dBodyC
  {
  public:
    GaborFilter2dBodyC();
    //: Default constructor
    
    GaborFilter2dBodyC(Index2dC size)
      : GaborFilterBank2dBodyC(size)
    {}
    //: Constructor with image size.
    // Uses default filter bank: <br>
    // Angles = 0.0, 45.0, 90.0, 135.0 <br>
    // Scales = 0.125, 0.35 

    GaborFilter2dBodyC(Index2dC size,const SArray1dC<RealT> &angles,const SArray1dC<RealT> &scales)
      : GaborFilterBank2dBodyC(angles,scales,size)
    {}
    //: Constructor
    
    SArray1dC<ImageC<RealT> > Apply (const ImageC<PixelT> &im)   {
      ImageC<RealT> rim;
      ConvToReal(im,rim); // At some point this we will do fft from native image type.
      return GaborFilterBank2dBodyC::Apply(rim);    
    }    
    //: Apply Gabor filters to image 'im'.
    
  };
  
  ///////////////////////////////////////////////////


  // --------------------------------------------------------------------------
  // **********  IPGaborC  ********************************************
  // --------------------------------------------------------------------------

  //! userlevel=Develop
  //: Compute Gabor feature images
  // This class is kept for backwards compatibility.  Use <a href="RavlImageN.GaborComplexC.html"> GaborComplexC </a> instead.

  template <class PixelT>
  class GaborFilter2dC
    : public GaborFilterBank2dC
  {
  public:
    GaborFilter2dC()
      : GaborFilterBank2dC(*new GaborFilter2dBodyC<PixelT>())
    {}
    //: Constructor 
    // Uses default filter bank: <br>
    // Angles = 0.0, 45.0, 90.0, 135.0 <br>
    // Scales = 0.125, 0.35 
    
    GaborFilter2dC(Index2dC size)
      : GaborFilterBank2dC(*new GaborFilter2dBodyC<PixelT>(size))
    {}
    //: Constructor 
    // Uses default filter bank: <br>
    // Angles = 0.0, 45.0, 90.0, 135.0 <br>
    // Scales = 0.125, 0.35 
    
    GaborFilter2dC(const SArray1dC<RealT> &angles,const SArray1dC<RealT> &scales,Index2dC size = Index2dC(0,0))
      : GaborFilterBank2dC(*new GaborFilter2dBodyC<PixelT>(size, angles,scales))      
    {}
    //: Constructor.
    
  protected:
    GaborFilter2dBodyC<PixelT> &Body()
    { return static_cast<GaborFilter2dBodyC<PixelT> &>(GaborFilterBank2dC::Body()); }
    //: Access body.

    const GaborFilter2dBodyC<PixelT> &Body() const
    { return static_cast<const GaborFilter2dBodyC<PixelT> &>(GaborFilterBank2dC::Body()); }
    //: Access body.
    
  public:
    SArray1dC<ImageC<RealT> > Apply (const ImageC<PixelT> &im)
    { return Body().Apply(im); }
    //: Apply Gabor filters to im.
    
  };
  
}


#endif
