#include "Ravl/DP/SequenceIO.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/RealRGBValue.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/Image/DeinterlaceStream.hh"
#include "Ravl/OS/Date.hh"
#include "Ravl/OS/CharIO.hh"
#include "Ravl/Image/Font.hh"
#include "Ravl/SysLog.hh"
#include "Ravl/Option.hh"
#include <stdio.h>

using namespace RavlN;
using namespace RavlImageN;

int main (int argc, char* argv[])
{
  RavlN::OptionC opts(argc,argv);
  opts.Comment("Runs either continuously (-r option) or using keystrokes (default)."
               "Valid keys are:"
               "    <- -> (L/R arrow keys) - increment / decrement field"
               "    q                      - quit");
  bool waitForKey = !opts.Boolean("r","Run file continuously. (Default: brows with keys)");
  StringC inFile = opts.String("","","Input file");
  StringC outFile = opts.String("","@X","Output file");
  bool verbose = opts.Boolean("v", "Adds field no. to display");
  opts.Check();

  DPISPortC<ImageC<ByteRGBValueC> > in;
  if(!OpenISequence(in, inFile)) {
    RavlError("Failed to open input '%s' ",inFile.c_str());
    exit(-2);
  }
  DPOPortC<ImageC<ByteRGBValueC> > out;
  if(!OpenOSequence(out, outFile)) {
    RavlError("Failed to open output '%s' ",outFile.c_str());
    exit (-3);
  }

  DeinterlaceStreamC<ByteRGBValueC> di(in);
  ImageC<ByteRGBValueC> im;
  int i(di.Start());
  if (waitForKey) {
    UInt64T c(0x484f1b);
    SizeT oldi = di.Start();
    while (c != 'q') {
      switch (c) {
      case 0x435b1b: ++i; break;
      case 0x445b1b: --i; break;
      case 0x484f1b: i=di.Start(); break;
      }
      if (di.GetAt((UIntT)i, im)) {
        if (verbose) {
          DrawText<ByteRGBValueC>(im, ByteRGBValueC(255,0,0), Index2dC(15,15), StringC(i));
        }
        out.Put(im);
        oldi = i;
      }
      else  i = oldi;
      c = GetKeypress();
    }
  }
  else {
    while (di.Get(im)) {
      if (verbose)
        DrawText<ByteRGBValueC>(im, ByteRGBValueC(255,0,0), Index2dC(15,15), StringC(i++));
      out.Put(im);
      Sleep(0.02);
      ++i;
    }
  }
}

