// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2006, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id: aamLocaliseFace.cc 5572 2006-06-09 16:09:48Z sevynaej $"
//! lib=RavlAAM
//! file="Ravl/CompVision/ActiveAppearanceModels/aamLocaliseFace.hh"
//! docentry="Ravl.API.Images.AAM"
//! userlevel="Normal"
//! author="Jean-Yves Guillemaut"

#include "Ravl/Image/AAMActiveAppearanceModel.hh"
#include "Ravl/Image/AAMAppearanceModel.hh"

#include "Ravl/Option.hh"
#include "Ravl/Array2dIter2.hh"
#include "Ravl/IO.hh"
#include "Ravl/DP/SequenceIO.hh"

using namespace RavlN;
using namespace RavlImageN;

int main (int argc, char** argv)
{
  OptionC opts(argc,argv);
  StringC vidIn = opts.String("i","@V4L2:/dev/video0#1", "Video source.");
  StringC aamFile = opts.String("aam","aam.abs","Active appearance model. ");
  bool displayFace = opts.Boolean("df",true,"Display fitted face ");
  float resetThreshold = opts.Real("t",1.0,"Reset model threshold ");
  opts.Check();
  
  DPIPortC<ImageC<ByteT> > imgStream;

  if(!OpenISequence(imgStream,vidIn)) {
    std::cerr << "Failed to open video stream. '" << vidIn.chars() << "' \n";
    return 1;
  }

  // Get first image so we know the size.
  ImageC<ByteT> img;
  if(!imgStream.Get(img)) {
    std::cerr << "Failed to read image from source \n";
    return 1;
  }

  AAMActiveAppearanceModelC aam;
  
  if(!RavlN::Load(aamFile,aam)) {
    std::cerr << "Failed to load active appearance model '" << aamFile << "' ";
    return 1;
  }
  
  VectorC faceParam(aam.Dimensions());
  faceParam.Fill(0);
  
  AAMAppearanceModelC am = aam.AppearanceModel();
  for(UIntT i = 0;i < am.FixedMean().Size();i++)
    faceParam[i] = am.FixedMean()[i];

  faceParam[0] = img.Rows()/2;
  faceParam[1] = img.Cols()/2;
  faceParam[2] = 40;
  VectorC defaultParam = faceParam.Copy();

  std::cerr << " default:" << defaultParam << "\n";

  while(1) {
    if(!imgStream.Get(img))
      break;
    RealT residual;
    am.MakePlausible(faceParam,2.6);
    faceParam = aam.RefinePose(img,faceParam,residual);
    std::cerr << "Residual=" << residual << "\n";
    if(residual > resetThreshold)
      faceParam = defaultParam.Copy();
    
    if(displayFace) {
      ImageC<ByteT> rendImg = img.Copy();
      IndexRange2dC rng = rendImg.Frame();
      AAMAppearanceC appear = am.Synthesize(faceParam,1.0);
      rng.ClipBy(appear.Image().Frame());
      for(Array2dIter2C<ByteT,ByteT> it(rendImg,appear.Image(),rng);it;it++) {
        if(it.Data2() != 0)
          it.Data1() = it.Data2();
      }
      
      RavlN::Save("@X:Overlay",rendImg);
    }
    //RavlN::Save("@X:Input",img);
    
  }
  
  return 0;
}
