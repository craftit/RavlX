// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlImageProc
//! file="Ravl/Image/Processing/Edges/doEdgeDet.cc"
#include "Ravl/config.h"
#include "Ravl/Image/Image.hh"
#include "Ravl/DP/SequenceIO.hh"
#include "Ravl/DP/Compose.hh"
#include "Ravl/DP/ThreadPipe.hh"
#include "Ravl/DP/Method2Proc.hh"
#include "Ravl/DP/Multiplex.hh"
#include "Ravl/DP/MTIOConnect.hh" 

#include "Ravl/IO.hh"
#include "Ravl/Option.hh"
#include "Ravl/Image/EdgeDeriche.hh"
#include "Ravl/Image/SqrComposition.hh"
#include "Ravl/Image/Edgel.hh"
#include "Ravl/Image/ImageConv.hh"
#include "Ravl/Image/EdgeDetector.hh"
#include "Ravl/Image/SubSample.hh"
#include "Ravl/OS/Date.hh"
#include "Ravl/Image/Deinterlace.hh"

using namespace RavlN;
using namespace RavlImageN;


ImageC<ByteT> DoDeinterlace(const ImageC<ByteT> &img) {
  return DeinterlaceSubsample(img);
}

ImageC<RealT> SquareComp(const Tuple2C<ImageC<RealT>,ImageC<RealT> > &dat) {
  static SqrCompositionC sc;
  ImageC<RealT> out;
  sc.Apply(dat.Data1(),dat.Data2(),out);
  return out;
}

ImageC<RealT> SquareComp(const ImageC<TFVectorC<RealT,2> > &dat) {
  static SqrCompositionC sc;
  ImageC<RealT> out;
  sc.Apply(dat,out);
  return out;
}

class EdgeOverlayC {
public:
  EdgeOverlayC(EdgeDetectorC &nEdgeDet)
    : edgeDet(nEdgeDet)
  {}
  //: Overlay.
  
  ImageC<RealT> OverlayEdges(const ImageC<RealT> &img) {
    SArray1dC<EdgelC> edges = edgeDet.PApply(img);
    ImageC<RealT> overlayImg(img);
    for(SArray1dIterC<EdgelC> it(edges);it;it++)
      overlayImg[it->At()] = 255;
    return overlayImg;
  }
  
  EdgeDetectorC edgeDet;
};


int main(int argc,char **argv) {
    
  // Parse options.
  
  OptionC option(argc,argv,true);
  StringC   inType = option.String("it","","Input file type. ");
  StringC   outType = option.String("ot","","Output file type. ");
  bool seq = option.Boolean("seq","Assume input is a sequence. ");
  bool verb = option.Boolean("v","Verbose. ");
  //bool eightCon = option.Boolean("u",false,"Use eight contectivity. ");
  bool rawEdgeImg = option.Boolean("rei",false,"Show raw edge image.");
  RealT hystLower = option.Real("hl",4.0,"Lower hysteresis threshold. ");
  RealT hystUpper = option.Real("hu",8.0,"Upper hysteresis threshold. ");
  RealT alpha     = option.Real("a", 2.0,"\"alpha\" for Deriche. ");
  RealT omega     = option.Real("w", 0.001,"\"omega\" for Deriche. ");
  IntT threads = option.Int("th",2,"Number of threads to use in processing. ");
  StringC overlay = option.String("ol","","Overlay edges on input image. ");
  bool overlayBlank = option.Boolean("bol","Use a blank overlay image");
  bool useDeriche = !option.Boolean("s","Use Sobel filter. ");
  bool deinterlace = option.Boolean("d","Deinterlace images");
  StringC inFile = option.String("","in.pgm","Input filename");
  StringC outFile = option.String("","","Output filename");
  
  option.Check();
  
  // Setup edge detector.
  EdgeDetectorC edgeDet(useDeriche,hystLower,hystUpper);
  edgeDet.SetDeriche(3.0, 0.002);
  
  EdgeDericheC edgeDeriche(omega, alpha);
  if(!seq) { // 
    ImageC<RealT> input;
    if(!Load(inFile,input,inType,verb)) {
      cerr << "Failed to open input '" << inFile << "' \n";
      return 1;      
    }
    if(verb)
      cerr << "Processing... \n";
    
    if(rawEdgeImg) {
      ImageC<RealT> mag;
      ImageC<TFVectorC<RealT,2> > inImg;
      edgeDeriche.Apply(input,inImg);
      mag = SquareComp(inImg);
      if(!Save(outFile,mag,outType,verb)) {
	cerr << "Failed to open output '" << outFile << "' \n";
	return 1;
      }
      return 0; // Done.
    }
    
    SArray1dC<EdgelC> output;
    edgeDet.Apply(input,output);
    
    if(verb)
      cerr << "Writing data. \n";

    if(!overlay.IsEmpty()) {
      ImageC<RealT> overlayImg(input.Copy());
      if(overlayBlank)
        overlayImg.Fill(0);
      for(SArray1dIterC<EdgelC> it(output);it;it++)
	overlayImg[it->At()] = 255;
      if(!Save(overlay,overlayImg)) {
	cerr << "Failed to save overlay image '" << overlay << "' \n";
	return 1;
      }
    }
    if(!outFile.IsEmpty()) {
      if(!Save(outFile,output,outType,verb)) {
	cerr << "Failed to open output '" << outFile << "' \n";
	return 1;
      }
    }
  } else {
    // Setup IO.
    
    DPIPortC<ImageC<ByteT> > in;
    if(!OpenISequence(in,inFile,inType,verb)) {
      cerr << "Failed to open input '" << inFile << "' \n";
      return 1;
    }
    //IPBWSubSampleC subSampleProc(subSampl,subSampl);
    
    if(rawEdgeImg) {
      DPOPortC<ImageC<RealT> > out;
      if(!OpenOSequence(out,outFile,outType,verb)) {
	cerr << "Failed to open output '" << outFile << "' \n";
	return 1;
      }
      
      DPEventC done = in >>= DPMultiplex(threads,DPThread(RavlImageN::ByteImageCT2DoubleImageCT >> Process(edgeDeriche,&EdgeDericheC::EdgeMagnitude))) >>= out;
      if(verb)
	cerr << "Running... \n";
      done.Wait();
      return 0;
    }
    DPOPortC<SArray1dC<EdgelC > > out;
    if(!outFile.IsEmpty()) {
      if(!OpenOSequence(out,outFile,outType,verb)) {
	cerr << "Failed to open output '" << outFile << "' \n";
	return 1;
      }
    }
    DPOPortC<ImageC<RealT> > outOverlay;
    if(!overlay.IsEmpty()) {
      if(!OpenOSequence(outOverlay,overlay,"",verb)) {
	cerr << "Failed to open output '" << overlay << "' \n";
	return 1;
      }
    }
    // Do some processing.
    if(!out.IsValid() && !outOverlay.IsValid()) {
      cerr << "No output specified. ";
      return 1;
    }
    if(deinterlace) {
      in = in >> &DoDeinterlace;
    }
    if(verb)
      cerr << "Processing... \n";
    //subSampleProc >> 
    DPEventC done;
    if(!outOverlay.IsValid()) {
      done = in >>= DPMultiplex(threads,DPThread(RavlImageN::ByteImageCT2DoubleImageCT >> Process(edgeDet,&EdgeDetectorC::PApply))) >>= out;
    } else {
      EdgeOverlayC edgeOverlay(edgeDet);
      done = in >>= DPMultiplex(threads,DPThread(RavlImageN::ByteImageCT2DoubleImageCT >> Process(edgeOverlay,&EdgeOverlayC::OverlayEdges))) >>= outOverlay;
    }
    
    done.Wait();
  }
  
  if(verb)
    cerr << "Done. \n";
  return 0;
}


// help the compiler 
#if RAVL_COMPILER_MIPSPRO 
#include "Ravl/Image/DrawFrame.hh"
static DPIPortBodyC<SArray1dC<EdgelC > > fix_sgi_bug1;
static DPOPortBodyC<SArray1dC<EdgelC > > fix_sgi_bug2;

// this is crude but seems to be needed to resolve the templated 
// function DrawFrame 
namespace {
  void fix_sgi_func (void) 
  { 
    Array2dC<double> fix_sgi_bug3 ; 
    double           fix_sgi_bug4 ;
    IndexRange2dC    fix_sgi_bug5 ;
    DrawFrame<double> ( fix_sgi_bug3, fix_sgi_bug4, fix_sgi_bug5 ) ; 
  }
};



#endif
