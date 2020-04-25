/* 
 * File:   GnuPlot3d.hh
 * Author: kieron
 *
 */

#ifndef RAVL_OBJECT_GnuPlot3d_HH
#define	RAVL_OBJECT_GnuPlot3d_HH

#include "Ravl/SmartPtr.hh"
#include "Ravl/DList.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/PointSet3d.hh"
#include "Ravl/Point3d.hh"
#include "Ravl/RCHash.hh"

//! docentry="Ravl.API.Graphics.Plotting.GnuPlot"

namespace RavlGUIN {

  using RavlN::RealT;
  using RavlN::StringC;
  using RavlN::Point3dC;
  using RavlN::RCHashC;
  using RavlImageN::ImageC;
  using RavlImageN::ByteRGBValueC;


  //! Do some plotting in 3D using GNUPLOT

  class GnuPlot3dC: public RavlN::RCHashC<RavlN::StringC, RavlN::DListC<RavlN::Point3dC> > {
    public:
      //! Default constructor.
      GnuPlot3dC() {}

      //! Constructor
      GnuPlot3dC(const RavlN::StringC & title, const RavlN::StringC & xlabel,
                 const RavlN::StringC & ylabel, const RavlN::StringC & zlabel);

      //! Add a point to the "default" plot
      bool AddPoint(const RavlN::Point3dC & point3d) {
        return AddPoint("default", point3d);
      }

      //! Add a point to the "default" plot
      bool AddPoint(RealT x, RealT y, RealT z) {
        return AddPoint("default", x, y, z);
      }

      //! Add a point to the plot
      bool AddPoint(const StringC & plotName, const Point3dC & point3d);

      //! Add a point to the plot
      bool AddPoint(const StringC & plotName, RealT x, RealT y, RealT z) {
        return AddPoint(plotName, Point3dC(x, y, z));
      }

      //! Perform the plot and return an image
      bool Plot(const StringC & outfile = "") const;

      //! Perform plot and get image
      ImageC<ByteRGBValueC> Image(RavlN::UIntT rows = 500, RavlN::UIntT cols = 500);


    protected:
      RavlN::StringC m_terminal;
      RavlN::StringC m_title;
      RavlN::StringC m_xlabel;
      RavlN::StringC m_ylabel;
      RavlN::StringC m_zlabel;

  };

}

#endif	/* _GNUPLOT3D_HH */

