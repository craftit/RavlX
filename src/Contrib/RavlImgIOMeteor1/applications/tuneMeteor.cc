// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlImgIOMeteor1
//! file="Ravl/Contrib/Meteor1/tuneMeteor.cc"
//! author="Charles Galambos"
//! docentry="Ravl.API.Images.Video.Video IO.Meteor1"
//! userlevel=Normal

#include "Ravl/Image/ImgIOMeteor1.hh"
#include "Ravl/Option.hh"
#include "Ravl/DP/FileFormatIO.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/GUI/Window.hh"
#include "Ravl/GUI/Canvas.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/GUI/LBox.hh"
#include "Ravl/GUI/Label.hh"
#include "Ravl/GUI/Slider.hh"
#include "Ravl/OS/Date.hh"
#include "Ravl/Image/ImageConv.hh"

using namespace RavlN;
using namespace RavlImageN;
using namespace RavlGUIN;

extern void InitDPImageIO(void);

bool SetHue(RealT &val,DPIImageMeteor1C<ByteRGBValueC> &obj) {
  obj.SetParam(DPIImageBaseMeteor1BodyC::VP_HUE,(int) val);
  return true;
}

bool SetChromaGain(RealT &val,DPIImageMeteor1C<ByteRGBValueC> &obj) {
  obj.SetParam(DPIImageBaseMeteor1BodyC::VP_CHROMA_GAIN,(int) val);
  return true;
}

bool SetBrightness(RealT &val,DPIImageMeteor1C<ByteRGBValueC> &obj) {
  obj.SetParam(DPIImageBaseMeteor1BodyC::VP_BRIGHTNESS,(int) val);
  return true;
}

bool SetChromaSat(RealT &val,DPIImageMeteor1C<ByteRGBValueC> &obj) {
  obj.SetParam(DPIImageBaseMeteor1BodyC::VP_CHROMA_SAT,(int) val);
  return true;
}

bool SetContrast(RealT &val,DPIImageMeteor1C<ByteRGBValueC> &obj) {
  obj.SetParam(DPIImageBaseMeteor1BodyC::VP_CONTRAST,(int) val);
  return true;
}


int main(int nargs,char *args[])
{
  Manager.Init(nargs,args); // Initalise GUI manager, let it read args.
  OptionC option(nargs,args,true);
  bool onlyOne = option.Boolean("one",false,"Only capture 1 image. ");
  RealT delay = option.Real("d",0.2,"Delay between capturing frames. \n");
  option.Check();
  
  DPIImageMeteor1C<ByteRGBValueC> imgIn("/dev/meteor0");

  if(!imgIn.IsGetReady()) {
    cerr << "Failed to open grabber. \n";
    return 1;
  }
  
  CanvasC draw(768,576); // Make a canvas to draw into.  
  WindowC win(768,576,"tuneMeteor"); // Make a new window called hello.
  win.Add(VBox(draw + 
	       HBox(LabelC("Hue       ") + SliderH((RealT) imgIn.GetParam(DPIImageBaseMeteor1BodyC::VP_HUE),
						   0.0,255.0,1.0,&SetHue,imgIn)) +
	       HBox(LabelC("ChromaGain") + SliderH(imgIn.GetParam(DPIImageBaseMeteor1BodyC::VP_CHROMA_GAIN),
						   0,255,1,&SetChromaGain,imgIn)) +
	       HBox(LabelC("ChromaSat ") + SliderH(imgIn.GetParam(DPIImageBaseMeteor1BodyC::VP_CHROMA_SAT),
						   0,255,1,&SetChromaSat,imgIn)) +	
	       HBox(LabelC("Brighness ") + SliderH(imgIn.GetParam(DPIImageBaseMeteor1BodyC::VP_BRIGHTNESS),
						   0,255,1,&SetBrightness,imgIn)) +
	       HBox(LabelC("Contrast  ") + SliderH(imgIn.GetParam(DPIImageBaseMeteor1BodyC::VP_CONTRAST)
						   ,0,255,1,&SetContrast,imgIn))
	       ));   // Put draw area inside a window.
  win.Show();                    // Display this lot on the screen.
  Manager.Execute();            // Start X thread.
  
  cerr << "Capture inital image. \n";
  ImageC<ByteRGBValueC> testImg;
  if(!imgIn.Get(testImg)) {
    cerr << "Failed to get image. \n";
  }
  draw.DrawImage(testImg);
  cerr << "Starting loop. \n";
  if(!onlyOne) {
    while(1) {
      cerr << "Capture image. \n";
      if(!imgIn.Get(testImg)) {
	cerr << "Failed to get image. \n";
      continue;
      }
      draw.DrawImage(testImg);
      Sleep(delay);
    }
  }
  cerr << "Waiting for manage to quit. \n";
  Manager.Wait();
  return 0;
}

