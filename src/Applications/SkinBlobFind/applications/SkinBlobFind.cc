// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! file="Ravl/Applications/SkinBlobFind/SkinBlobFind.cc"
//! author="Joel Mitchelson"
//! docentry="Ravl.Applications.Image"
//
//: Locate skin colour regions in an image sequence
//
//Skin blobs are located in normalised rgb colour space. Requires an example image of skin colour.  The algorithm makes the (incorrect) assumption of independence of normalised rgb components, but gives reasonable results.  Only the <i>N</i> largest connected skin-coloured blobs are located.  For each frame in a video sequence, output is of the form:
//<br>
//<br>
//<i>r0</i> <i>c0</i> <i>a0</i><br>
//.<br>
//.<br>
//.<br>
//<i>rN</i> <i>cN</i> <i>aN</i><br>
//<br>
//where (<i>ri</i>, <i>ci</i>) is the image co-ordinate of the centre of the <i>i</i>th largest blob, and <i>ai</i> is its area in pixels.

#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/Image/ByteRGBAValue.hh"
#include "Ravl/Image/ChromaThreshold.hh"
#include "Ravl/Image/Erode.hh"
#include "Ravl/Image/Dilate.hh"
#include "Ravl/Image/ConnectedComponents.hh"
#include "Ravl/Vector3d.hh"
#include "Ravl/Option.hh"
#include "Ravl/IO.hh"
#include "Ravl/OS/Date.hh"
#include "Ravl/DP/SequenceIO.hh"
  
using namespace RavlImageN;

int main(int argc, char** argv)
{
  OptionC opt(argc,argv);
  StringC strSkin = opt.String("skin", "skin.ppm", "example image of skin colour");
  StringC strInput = opt.String("input", "image.%05d.ppm", "image file");
  RealT tol = opt.Real("tol", 5.0, "skin colour tolerance");
  UIntT nlargest = (UIntT)opt.Int("nlargest", 3, "take the n largest blobs only");
  bool cleanup = opt.Boolean("cleanup", false, "morphological cleanup");
  UIntT maxframes = (UIntT)opt.Int("maxframes", -1, "maximum frames (-1 means no limit)");
  opt.Check();

  // load skin example image
  ImageC<ByteRGBValueC> skin;
  if (!Load(strSkin, skin))

  {
    std::cerr << "skin image load failed" << std::endl;
    return 0;
  }

  // load input sequence
  DPISPortC< ImageC<ByteRGBAValueC> > port_image;
  if (!OpenISequence(port_image, strInput))
  {
    std::cerr << "couldn't open input sequence" << std::endl;
    return 0;
  }

  // image processing classes
  ChromaThresholdRGBC thresh(skin, tol, 0.1);
  ConnectedComponentsC<ByteT>  comp(true);

  SArray1dC<Vector3dC>  comp_last;

  // track each frame
  for (UIntT iframe = 0; iframe < maxframes; iframe++)
  {
    ImageC<ByteRGBAValueC> image;
    if (!port_image.Get(image))
    {
      std::cerr << "finishing after " << iframe << " frames" << std::endl;
      break;
    }

    // record start time
    DateC date0(true);

    // skin colour threshold
    ImageC<ByteT> image_thresh(image.Rows(), image.Cols());
    thresh.Apply(image_thresh, image);

    ImageC<ByteT> image_sil;

    if (cleanup)
    {
      // morphological image cleanup - removes pixel noise in skin detection
      
      // morphology kernel, centred at 0
      ByteT byte_kernel[3*3] =
      {
	0,255,0,
	255,255,255,
	0,255,0
      };
      ImageC<ByteT> kernel(3,3, BufferC<ByteT>(3*3, byte_kernel));
      kernel.ShiftIndexes1(1);
      kernel.ShiftIndexes2(1);

      // erode
      ImageC<ByteT> image_erode(image.Rows(),image.Cols());
      image_erode.Fill(0);
      BinaryErode(image_thresh, kernel, image_erode, (ByteT)255);

      // dilate
      image_sil = ImageC<ByteT> (image.Rows(),image.Cols());
      image_sil.Fill(0);
      BinaryDilate(image_erode, kernel, image_sil, (ByteT)255);
    }
    else
    {
      // no cleanup - use threshold image directly
      // - this is much faster
      image_sil = image_thresh;
    }


    DateC date1(true);
    
    // connected components
    Tuple2C<ImageC<UIntT>, UIntT> component = comp.Apply(image_sil);
  
    // array of 3-vectors for size and position of components
    SArray1dC< Vector3dC > stats(component.Data2());
    stats.Fill(Vector3dC(0,0,0));
    
    // sum size and position for each component
    UIntT count(0);
    for (Array2dIterC<UIntT> icomp(component.Data1()); icomp; icomp++, count++)
    {
      if (*icomp)
      {
	RavlAssert(*icomp < stats.Size());
	stats[*icomp] += Vector3dC(count / image.Cols(), count % image.Cols(), 1.0);
      }
    }

    // take n largest components
    SArray1dC< Vector3dC > comp_best(nlargest);
    comp_best.Fill(Vector3dC(0,0,0));
    for (UIntT istat = 0; istat < stats.Size(); istat++)
    {
      // use num pixels to find com of blob
      stats[istat][0] /= stats[istat][2];
      stats[istat][1] /= stats[istat][2];

      // do largest search
      for (UIntT ifound = 0; ifound < nlargest; ifound++)
      {
	if (stats[istat][2] > comp_best[ifound][2])
	{
	  UIntT ireplace = ifound;
	  while (++ifound < nlargest)
	    comp_best[ifound] = comp_best[ifound-1];
	  comp_best[ireplace] = stats[istat];
	}
      }
    }

    // ouptut results
    for (UIntT iout = 0; iout < nlargest; iout++)
      std::cout << comp_best[iout] << std::endl;


    DateC date2(true);
    std::cerr << "Connected components time: " << (date2-date1).Double() << std::endl;
    std::cerr << "Total frame time: " << (date2-date0).Double() << std::endl;  
  }
  return 0;
}
