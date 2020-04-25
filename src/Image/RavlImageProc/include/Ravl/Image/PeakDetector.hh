// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_PEAKDETECTOR_HEADER
#define RAVLIMAGE_PEAKDETECTOR_HEADER 1
//! rcsid="$Id$"
//! lib=RavlImageProc
//! userlevel=Normal
//! author="Charles Galambos"
//! docentry="Ravl.API.Images.Misc"
//! file="Ravl/Image/Processing/Tools/PeakDetector.hh"

#include "Ravl/Array2d.hh"
#include "Ravl/Index2d.hh"
#include "Ravl/Point2d.hh"
#include "Ravl/Vector2d.hh"

namespace RavlImageN {
  using namespace RavlN;
  
  template<class DataT>
  inline 
  bool PeakDetect3(const Array2dC<DataT> &img,const Index2dC &pos) {
    const DataT *rt = &(img[pos]);
    const DataT &cent = rt[0];
    if(rt[-1] >= cent || rt[1] >= cent)
      return false;
    rt = &(img[pos[0]-1][pos[1]]);
    if(rt[-1] >= cent || rt[0] >= cent || rt[1] >= cent)
      return false;
    rt = &(img[pos[0]+1][pos[1]]);
    return (rt[-1] < cent && rt[0] < cent && rt[1] < cent);
  }
  //: Test if position 'pos' is the largest value in a 3 by 3 area.
  // Is is the users responsibility to ensure that all pixels around 'pos'
  // are in the image.

  template<class DataT>
  inline
  bool PeakDetect5(const Array2dC<DataT> &img,const Index2dC &pos) {
    const DataT *rt = &(img[pos]);
    const DataT &cent = rt[0];
    IntT cc = pos[1].V();
    IntT cr = pos[0].V();
    // Check the middle row first as we already have a pointer to it.
    if(rt[-2] >= cent || rt[-1] >= cent || rt[1] >= cent || rt[2] >= cent)
      return false;
    
    rt = &(img[cr-1][cc]);
    if(rt[-2] >= cent || rt[-1] >= cent || rt[0] >= cent || rt[1] >= cent || rt[2] >= cent)
      return false;

    rt = &(img[cr+1][cc]);
    if(rt[-2] >= cent || rt[-1] >= cent || rt[0] >= cent || rt[1] >= cent || rt[2] >= cent)
      return false;

    rt = &(img[cr-2][cc]);
    if(rt[-1] >= cent || rt[0] >= cent || rt[1] >= cent)
      return false;
        
    rt = &(img[cr+2][cc]);
    if(rt[-1] >= cent || rt[0] >= cent || rt[1] >= cent)
      return false;
    return true;
  }
  //: Test if position 'pos' is the largest value in a 5 by 5 area.
  // Is is the users responsibility to ensure that all pixels around 'pos'
  // are in the image.  The corners of the area are not mask to bring
  // the area checked closer to a circle.

  template<class DataT>
  inline
  bool PeakDetect7(const Array2dC<DataT> &img,const Index2dC &pos) {
    const DataT *rt = &(img[pos]);
    const DataT &cent = rt[0];
    IntT cc = pos[1].V();
    IntT cr = pos[0].V();
    // Check the middle row first as we already have a pointer to it.
    if(rt[-3] >= cent || rt[-2] >= cent || rt[-1] >= cent || rt[1] >= cent || rt[2] >= cent || rt[3] >= cent)
      return false;
    
    rt = &(img[cr-1][cc]);
    if(rt[-3] >= cent || rt[-2] >= cent || rt[-1] >= cent || rt[0] >= cent || rt[1] >= cent || rt[2] >= cent || rt[3] >= cent)
      return false;
    
    rt = &(img[cr+1][cc]);
    if(rt[-3] >= cent || rt[-2] >= cent || rt[-1] >= cent || rt[0] >= cent || rt[1] >= cent || rt[2] >= cent || rt[3] >= cent)
      return false;
    
    rt = &(img[cr-2][cc]);
    if(rt[-2] >= cent || rt[-1] >= cent || rt[0] >= cent || rt[1] >= cent || rt[2] >= cent)
      return false;
    
    rt = &(img[cr+2][cc]);
    if(rt[-2] >= cent || rt[-1] >= cent || rt[0] >= cent || rt[1] >= cent || rt[2] >= cent)
      return false;
    
    rt = &(img[cr-3][cc]);
    if(rt[-1] >= cent || rt[0] >= cent || rt[1] >= cent)
      return false;

    rt = &(img[cr+3][cc]);
    if(rt[-1] >= cent || rt[0] >= cent || rt[1] >= cent)
      return false;
    return true;
  }
  //: Test if position 'pos' is the largest value in a 7 by 7 area.
  // Is is the users responsibility to ensure that all pixels around 'pos'
  // are in the image.  The corners of the area are not mask to bring
  // the area checked closer to a circle.



  template<class DataT>
  Point2dC LocatePeakSubPixel(const Array2dC<DataT> &img,const Index2dC &pos,RealT pof) {
    // apply geometric fitting in image-proportional coordinates.
    if(!img.Frame().Erode().Contains(pos))
      return pos;
    
    const DataT *rt = &(img[pos[0]-1][pos[1]]);
    RealT spp = Pow((RealT) rt[-1],pof);
    RealT spc = Pow((RealT) rt[ 0],pof);
    RealT spn = Pow((RealT) rt[ 1],pof);
    
    rt = &(img[pos]);
    RealT scp = Pow((RealT) rt[-1],pof);
    RealT scc = Pow((RealT) rt[ 0],pof);
    RealT scn = Pow((RealT) rt[ 1],pof);
    
    rt = &(img[pos[0]+1][pos[1]]);
    RealT snp = pow((RealT) rt[-1],pof);
    RealT snc = Pow((RealT) rt[ 0],pof);
    RealT snn = Pow((RealT) rt[ 1],pof);
    
    // Use least-squares to fit quadratic to local corner strengths.
    RealT Pxx = (spp - 2.0*spc + spn + scp - 2.0*scc + scn + snp - 2.0*snc + snn)/3.0;
    RealT Pxy = (spp - spn - snp + snn)/4.0;
    RealT Pyy = (spp + spc + spn - 2.0*scp - 2.0*scc - 2.0*scn + snp + snc + snn)/3.0;
    RealT Px = (- spp - scp - snp + spn + scn + snn)/6.0;
    RealT Py = (- spp - spc - spn + snp + snc + snn)/6.0;
    RealT det = Pxy*Pxy - Pxx*Pyy;
    
    if(det == 0)
      return Point2dC(pos);
    
    // calculate sub-pixel corrections to the corner position.
    Vector2dC corr((Pyy*Px - Pxy*Py)/det,(Pxx*Py - Pxy*Px)/det);
    
    // pull the corrections inside the pixel.
    if (corr[0] > 0.5) 
     corr[0]=0.5; 
    if (corr[0] < -0.5) 
      corr[0]= -0.5;
    if (corr[1] > 0.5) 
      corr[1]=0.5; 
    if (corr[1] < -0.5) 
      corr[1]= -0.5;
    return Point2dC(pos) + corr; 
  }
  //: Locate peak with sub-pixel precision.
  // Fits a quadratic to the peak and works out the center. The position of the
  // peak is returned. 'img' should contain values surrounding the center of
  // the peak at 'pos'.

  template<class DataT>
  Point2dC LocatePeakSubPixel(const Array2dC<DataT> &img,const Index2dC &pos) {
    // apply geometric fitting in image-proportional coordinates.
    if(!img.Frame().Erode().Contains(pos))
      return pos;
    
    const DataT *rt = &(img[pos[0]-1][pos[1]]);
    RealT spp = (RealT) rt[-1];
    RealT spc = (RealT) rt[ 0];
    RealT spn = (RealT) rt[ 1];
    
    rt = &(img[pos]);
    RealT scp = (RealT) rt[-1];
    RealT scc = (RealT) rt[ 0];
    RealT scn = (RealT) rt[ 1];
    
    rt = &(img[pos[0]+1][pos[1]]);
    RealT snp = (RealT) rt[-1];
    RealT snc = (RealT) rt[ 0];
    RealT snn = (RealT) rt[ 1];
    
    // Use least-squares to fit quadratic to local corner strengths.
    RealT Pxx = (spp - 2.0*spc + spn + scp - 2.0*scc + scn + snp - 2.0*snc + snn)/3.0;
    RealT Pxy = (spp - spn - snp + snn)/4.0;
    RealT Pyy = (spp + spc + spn - 2.0*scp - 2.0*scc - 2.0*scn + snp + snc + snn)/3.0;
    RealT Px = (- spp - scp - snp + spn + scn + snn)/6.0;
    RealT Py = (- spp - spc - spn + snp + snc + snn)/6.0;
    RealT det = Pxy*Pxy - Pxx*Pyy;
    
    if(det == 0)
      return Point2dC(pos);
    
    // calculate sub-pixel corrections to the corner position.
    Vector2dC corr((Pyy*Px - Pxy*Py)/det,(Pxx*Py - Pxy*Px)/det);
    
    // pull the corrections inside the pixel.
    if (corr[0] > 0.5) 
     corr[0]=0.5; 
    if (corr[0] < -0.5) 
      corr[0]= -0.5;
    if (corr[1] > 0.5) 
      corr[1]=0.5; 
    if (corr[1] < -0.5) 
      corr[1]= -0.5;
    return Point2dC(pos) + corr; 
  }
  //: Locate peak with sub-pixel precision.
  // Fits a quadratic to the peak and works out the center. The position of the
  // peak is returned. 'img' should contain values surrounding the center of
  // the peak at 'pos'.
}



#endif
