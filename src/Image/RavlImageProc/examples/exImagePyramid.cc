// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2005, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#include "Ravl/Image/ImagePyramid.hh"
//! lib=RavlImageProc

using namespace RavlN;
using namespace RavlImageN;

int main() 
{
  ImageC<ByteT> img(100,100);
  for (Array2dIterC<ByteT> i(img); i; ++i)
    *i = i.Index()[0] + i.Index()[1];

  std::cout << "\nPyramid without image subsampling:\n";
  ImagePyramidC<ByteT> pyramid(img, Sqrt(2), 4, false);
  for (CollectionIterC<Tuple3C<RealT,RealT,ImageC<ByteT> > > l(pyramid.Images()); l; ++l)
    std::cout << "Frame: " << l->Data3().Frame() << endl
         << "Pixel scale: " << l->Data2() << endl
         << "Filter scale: " << l->Data1() << std::endl;

  std::cout << "\nPyramid including image subsampling:\n";
  // Also different indexing style
  pyramid = ImagePyramidC<ByteT>(img, Sqrt(2), 4, true);
  for (IntT l=0; l<pyramid.Images().Size(); ++l) {
    Tuple3C<RealT,RealT,ImageC<ByteT> > img(pyramid.Images()[l]);
    std::cout << "Frame: " << img.Data3().Frame() << endl
         << "Pixel scale: " << img.Data2() << endl
         << "Filter scale: " << img.Data1() << std::endl;
  }
  std::cout << "\nMulti-octave pyramid (SIFT-style):\n";
  // create the octaves using subsampling:
  pyramid = ImagePyramidC<ByteT>(img, 3, true);
  for (IntT level=0; level<pyramid.Images().Size(); ++level) {
    // create the images in between (without subsampling)
    ImageC<ByteT> octaveImg = pyramid.Images()[level].Data3();
    ImagePyramidC<ByteT> subPyramid(octaveImg, Pow(2.0,1.0/8.0), 8, false);
    for (CollectionIterC<Tuple3C<RealT,RealT,ImageC<ByteT> > > l(subPyramid.Images()); l; ++l)
      std::cout << "Frame: " << l->Data3().Frame() << endl
           << "Pixel scale: " << l->Data2() * Pow(2.0, level) << endl
           << "Filter scale: " << l->Data1() * Pow(2.0, level) << std::endl;
  }
  
  return 0;
}
