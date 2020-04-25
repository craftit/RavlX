#include "Ravl/Plot/GnuPlot3d.hh"
#include "Ravl/IO.hh"
#include "Ravl/Option.hh"
#include "Ravl/RandomGauss.hh"

using namespace RavlN;
using namespace RavlGUIN;
using namespace RavlImageN;

int main(int nargs, char **argv) {

  //: parse options
  OptionC opt(nargs, argv);
  opt.Check();


  GnuPlot3dC plot("Big Test", "apples", "bananas", "pears");

  for (UIntT i = 0; i < 1000; i++) {
    plot.AddPoint(RandomGauss(), RandomGauss(), RandomGauss());
  }

  for (UIntT i = 0; i < 1000; i++) {
    plot.AddPoint("Plot 1", RandomGauss() + 10.0, RandomGauss() - 5.0, RandomGauss() + 13.0);
  }

  for (UIntT i = 0; i < 1000; i++) {
    plot.AddPoint("Plot 2", RandomGauss() - 10.0, RandomGauss() - 5.0, RandomGauss() - 10.0);
  }

  // Perform a plot. This should display plot in GnuPlot window
  plot.Plot();

  // Get plot as image
  ImageC<ByteRGBValueC> image = plot.Image(500, 500);
  Save("@X", image);

  return 0;
}
