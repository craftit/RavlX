#include "Ravl/FFT2d.hh"
#include "Ravl/Option.hh"
#include "Ravl/IO.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/Image/Image.hh"
//#include "Ravl/GUI/DPDisplayImage.hh"
#include "Ravl/Array2dIter.hh"

using namespace RavlN;
using namespace RavlImageN;

int main(int argc, char* argv[]) {
  OptionC opt(argc, argv);
  opt.Comment("This example displays the log amplitude of the 2D FFT of an image.");
  FilenameC imgFile = opt.String("", "", "I/p image file name");
  opt.CompulsoryArgs(1);
  opt.Check();
  ImageC<RealT> im;
  RavlAlwaysAssertMsg(Load(imgFile, im), "Failed to load image " + imgFile);
  FFT2dC fft(im.Rows(), im.Cols());
  ImageC<RealT> spectrum = FFT2dC::Mag(fft.RotateFreq(fft.Apply(im)));
  for (Array2dIterC<RealT> i(spectrum); i; ++i) *i = Log(*i);
  Save("@X:FFT", spectrum);
  exit(0);
}
