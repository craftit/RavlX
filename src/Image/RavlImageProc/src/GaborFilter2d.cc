// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! author="Kieron Messer"
//! date="17/10/2000"
//! rcsid="$Id$"
//! lib=RavlImageProc
//! file="Ravl/Image/Processing/Filters/GaborFilter2d.cc"

#include "Ravl/Image/GaborFilter2d.hh"
#include "Ravl/Array2dIter3.hh"
#include "Ravl/Array2dIter2.hh"
#include "Ravl/Math.hh"

namespace RavlImageN {
  
  static RealT defaultAngles[4] = {0.0, 45.0, 90.0, 135.0};
  static RealT defaultScales[2] = {0.125, 0.35};
  
  //: Constructor.
  
  GaborFilterBank2dBodyC::GaborFilterBank2dBodyC(Index2dC size)
    : filterSize(0,0)
  { 
    angleArr = SArray1dC<RealT>(defaultAngles,4,false);
    scaleArr = SArray1dC<RealT>(defaultScales,2,false);
    SetupFilters(size);
  }
  
  //: Constructor.
  
  GaborFilterBank2dBodyC::GaborFilterBank2dBodyC(const SArray1dC<RealT> &angles,const SArray1dC<RealT> &scales,Index2dC size)
    : filterSize(0,0),
      angleArr(angles),
      scaleArr(scales)
  { SetupFilters(size); }
  
  //: Constructor.
  
  GaborFilterBank2dBodyC::GaborFilterBank2dBodyC()
    : filterSize(0,0)
  {
    angleArr = SArray1dC<RealT>(defaultAngles,4,false);
    scaleArr = SArray1dC<RealT>(defaultScales,2,false);
  }
  
  //: Setup filters for given image size.
  
  void GaborFilterBank2dBodyC::SetupFilters(Index2dC imgSize) {    
    if(imgSize[0] == 0 || imgSize[1] == 0)
      return ;
    filterSize = imgSize;
    forward = FFT2dC(imgSize,false);
    backward = FFT2dC(imgSize,true);
    MakeGaborFilters(imgSize.Row().V(),imgSize.Col().V());
  }
  
  
  void GaborFilterBank2dBodyC::MakeGaborFilters(int ymax, int xmax) {
    double delta_x, delta_y;
    double PIE, temp1, temp2, temp3, temp4, temp5;
    double u, v, udash, vdash, centre_u, centre_v;
    int i, j;
    unsigned int k, l;
    
    filter = Array1dC<ImageC<RealT> >(angleArr.Size() * scaleArr.Size());
    
    PIE = 4.0*atan(1.0);
    IntT num = 0;
    
    for(k=0;k < scaleArr.Size();k++) {
      for(l=0;l < angleArr.Size();l++) {
	double Btheta = 45.0;
	double Bu = 1.0;
	double  theta0 = angleArr[l];
 	double  f0 = scaleArr[k];
	ImageC<RealT> newFilter(ymax, xmax);
	filter[num] = newFilter;
	
	/*Have to transfer to radians*/
	Btheta = Btheta * (PIE/180.);
	theta0 = theta0 * (PIE/180.);
	
	temp1 = Sqrt((Log(2)/2));
	delta_x = (1/(f0*PIE)) * temp1 * (((Pow(2,Bu) +1)/(Pow(2,Bu)-1)));
	temp2 = (1/(f0*PIE*tan((Btheta/2.0))));
	delta_y = temp2 * temp1;
	
	delta_x = Pow(delta_x,2);
	delta_y = Pow(delta_y,2);
	
	temp3 = -2.0 * Pow(PIE,2);
	
	centre_u = ((double)xmax/2.0)+1.0;
	centre_v = ((double)ymax/2.0)+1.0;
      
	for(i=0;i<ymax;i++) {
	  v = ((double)i - centre_v)/(double)ymax;
	  for(j=0;j<xmax;j++) {
	    u = ((double)j- centre_u)/(double)xmax;
	    udash = u*Cos(theta0) + v * Sin(theta0);
	    vdash = -u * Sin(theta0) + v * Cos(theta0);
	    temp4 = Pow((udash-f0),2) * delta_x;
	    temp5 = Pow(vdash,2) * delta_y;
	    newFilter[i][j] = Exp((temp3 *(temp4 + temp5)));
	  }
	}
	num++;
      }
    }
  }
  
  //: Apply filters to a real image.
  
  SArray1dC<ImageC<RealT> > GaborFilterBank2dBodyC::Apply(const ImageC<RealT> &rim) {
    if(rim.Frame().Size() != filterSize)
      SetupFilters(rim.Frame().Size());
    
    //: Lets apply the forward FFT to input image
    ImageC<ComplexC> trans = forward.Apply(rim);
    
    //: Next apply the filters in turn to the FT image
    SArray1dC<ImageC<RealT> > out(filter.Size());
    ImageC<ComplexC> cop(rim.Frame());
    
    for(UIntT d=0;d<filter.Size();d++) {
      for(Array2dIter3C<ComplexC,RealT,ComplexC> it(cop,filter[d],trans);it;it++)
	it.Data1() = it.Data2() * it.Data3();
      
      //: now we need to do reverse fft and compute power spectrum
      ImageC<ComplexC> fted = backward.Apply(cop);
      out[d] = ImageC<RealT>(fted.Frame());
      for(Array2dIter2C<RealT,ComplexC> it(out[d],fted);it;it++)
	it.Data1() = it.Data2().Mag();    
    }
  
    return out;

  }
  
}
