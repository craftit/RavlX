// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_HOMOMORPHICFILTER_HEADER
#define RAVLIMAGE_HOMOMORPHICFILTER_HEADER 1
//! userlevel=Normal
//! author="Charles Galambos"
//! docentry="Ravl.API.Images.Filtering"
//! lib=RavlImageProc
//! file="Ravl/Image/Processing/Filters/Misc/HomomorphicFilter.hh"

#include "Ravl/Image/Image.hh"
#include "Ravl/FFT2d.hh"
#include "Ravl/Cache.hh"
#include "Ravl/Tuple3.hh"
#include "Ravl/Threads/RWLock.hh"

namespace RavlImageN {
  
  //! userlevel=Develop
  //: Homomorphic filter
  
  class HomomorphicFilterBodyC 
    : public RCBodyC
  {
  public:
    HomomorphicFilterBodyC(RealT nSigma = 10,RealT nDepth = 0.5,const Index2dC &size = Index2dC(0,0));
    //: Constructor.
    
    ImageC<RealT> Apply(const ImageC<RealT> &img);
    //: Apply filter to image.

    ImageC<float> Apply(const ImageC<float> &img);
    //: Apply filter to image.

    RealT Sigma() const
    { return sigma; }
    //: Access current sigma.
    
    RealT Depth() const
    { return depth; }
    //: Access current depth.
    
  protected:
    bool InitFilter(const Index2dC &size,Tuple3C<FFT2dC,FFT2dC, ImageC<RealT> > &filter);
    //: Setup the filter. This does no locking.
    
    bool UseFilter(const Index2dC &size,Tuple3C<FFT2dC,FFT2dC, ImageC<RealT> > &filter);
    //: Initialise a filter.

    RealT sigma;
    RealT depth;
    
    CacheC<Index2dC, Tuple3C<FFT2dC,FFT2dC, ImageC<RealT> > > m_cache;
    RWLockC m_lock; //!< A lock
    
  };
  
  //! userlevel=Normal
  //: Homomorphic filter
  
  class HomomorphicFilterC 
    : public RCHandleC<HomomorphicFilterBodyC>
  {
  public:
    HomomorphicFilterC()
    {}
    //: Default constructor.
    
    HomomorphicFilterC(RealT sigma,RealT depth = 0.5,const Index2dC &size = Index2dC(0,0))
      : RCHandleC<HomomorphicFilterBodyC>(*new HomomorphicFilterBodyC(sigma,depth,size))
    {}
    //: Constructor.
    // depth controls the strength of the effect of the filter on the image 
    // from 0 with no effect to 1 complete removal of some components.
    // sigma controls the width of the filter.
    
    ImageC<RealT> Apply(const ImageC<RealT> &img)
    { return Body().Apply(img); }
    //: Apply filter to image.

    ImageC<float> Apply(const ImageC<float> &img)
    { return Body().Apply(img); }
    //: Apply filter to image.

    RealT Sigma() const
    { return Body().Sigma(); }
    //: Access current sigma.
    
    RealT Depth() const
    { return Body().Depth(); }
    //: Access current depth.
  };
  
  
  std::ostream &operator<<(std::ostream &s,const HomomorphicFilterC &hist);
  //: Write to a stream.
  
  std::istream &operator>>(std::istream &s,HomomorphicFilterC &hist);
  //: Read from a stream.

  BinOStreamC &operator<<(BinOStreamC &s,const HomomorphicFilterC &hist);
  //: Write to a binary stream.
  
  BinIStreamC &operator>>(BinIStreamC &s,HomomorphicFilterC &hist);
  //: Read from a binary stream.
  
  
}

#endif
