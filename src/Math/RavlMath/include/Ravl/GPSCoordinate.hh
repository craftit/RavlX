// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_GPSCOORDINATE_HEADER
#define RAVL_GPSCOORDINATE_HEADER 1
//! lib=RavlCore

#include "Ravl/BinStream.hh"
#include "Ravl/Point3d.hh"
#include "Ravl/Vector3d.hh"
#include "Ravl/String.hh"

namespace RavlN {
  
  //! GPS Coordinate information.
  // This class uses the GRS80 Ellipsoid.
  
  // Latitude , Longitude, Height.
  // Angles are in degree's
  
  class GPSCoordinateC 
    : public Point3dC
  {
  public:
    //! Default constructor.
    GPSCoordinateC()
     : m_horizontalError(0.1),
       m_verticalError(0.15)
    {}
    
    //! Constructor.
    //! Note: The default standard deviation for height and position of 10 cm and 15cm is rather
    //! generous. 
    GPSCoordinateC(RealT latitude,
                   RealT longitude,
                   RealT height = 0,
                   FloatT verticalError = 0.1,
                   FloatT horizontalError = 0.15
                   )
      : RavlN::Point3dC(latitude,longitude,height),
        m_horizontalError(horizontalError),
        m_verticalError(verticalError)
    {}
    
    //! Constructor.
    //! Note: The default standard deviation for height and position of 10 cm and 15cm is rather
    //! generous. 
    GPSCoordinateC(const RavlN::Point3dC &val,
                   FloatT verticalError = 0.1,
                   FloatT horizontalError = 0.15
                   )
      : RavlN::Point3dC(val),
        m_horizontalError(horizontalError),
        m_verticalError(verticalError)
    {}
    
    //! Convert from coordinates in text form.
    // The string must have at least 2 comma separated values for
    // latitude and longitude followed by height. If height is omitted
    // it is assumed to be zero.
    // Form: latitude,longitude,[height]
    // Longitude may be either expressed as decimal angles, or in degree's minutes seconds.
    GPSCoordinateC(const RavlN::StringC &text);
    
    // Convert a gps co-orindate from text form to a GPS class.
    // Returns false if text is not correctly formatted
    static bool Text2GPS(const RavlN::StringC &text,GPSCoordinateC &gps);
    
    //! Convert a Cartesian position to gps.
    static GPSCoordinateC Cartesian2GPS(const Point3dC &position,RealT precision = 1e-12);

    // Print as text in degree's minutes seconds N/S E/W
    RavlN::StringC TextDMS() const;
    
    //! Convert GPS coordinate into global Cartesian coordinates.
    RavlN::Point3dC Cartesian() const;
    
    //! Access local vertical  direction
    //! Of gravity.
    RavlN::Vector3dC Vertical() const;

    //! Interpolate between two GPS positions.
    //! Fraction is between 0.0 and 1.0, where 0 is p1, and 1 is p2.
    //! This currently just does a linear interpolation of all the gps parameters.
    static bool BilinearInterpolate(RealT fraction,const GPSCoordinateC &p1,const GPSCoordinateC &p2,GPSCoordinateC &position);

    //! Access latitude in degree's
    const RealT &Latitude() const
    { return (*this)[0]; }
    
    //! Access longitude in degree's
    const RealT &Longitude() const
    { return (*this)[1]; }
    
    //! Height in meters
    const RealT &Height() const
    { return (*this)[2]; }

    //! Access latitude in degree's
    RealT &Latitude()
    { return (*this)[0]; }
    
    //! Access longitude in degree's
    RealT &Longitude()
    { return (*this)[1]; }
    
    //! Height in meters
    RealT &Height()
    { return (*this)[2]; }

    //!< Standard deviation in position values in meters
    const FloatT &HorizontalErrorBounds() const
    { return m_horizontalError; }
    
    //!< Standard deviation in height values in meters
    const FloatT &VerticalErrorBounds() const
    { return m_verticalError; }
    
    //!< Set error bounds on GPS position in meters
    void SetErrorBounds(FloatT horizontalErrorBounds,FloatT verticalErrorBounds)
    {
      m_horizontalError = horizontalErrorBounds;
      m_verticalError = verticalErrorBounds;
    }
    
    //! Compute the differential 
    bool Differential(RavlN::Vector3dC &diffLat,RavlN::Vector3dC &diffLong,RavlN::Vector3dC &diffHeight) const;
    
  protected:
    //! Normalise GPS coordinates.
    Point3dC Normalise() const;
    
    FloatT m_horizontalError; //!< Standard deviation in position values.
    FloatT m_verticalError;   //!< Standard deviation in hight values.
    
    enum AngleFormT {
      AF_None,
      AF_NS,
      AF_EW
    };
    
    // Convert a angle in degree's to text Degree's minutes seconds.
    static RavlN::StringC AngleToTextDMS(RealT value);
    
    // Convert a angle in text Degree's minutes seconds, or decimal notation
    static bool TextDMSToAngle(const RavlN::StringC &strValue,RealT &value,AngleFormT af);

    //! Test if a string contains N or S
    static bool UsesNS(const RavlN::StringC &text);
    
    //! Test if a string contains E or W
    static bool UsesEW(const RavlN::StringC &text);
    
    static const RealT m_e2;
    static const RealT m_a;
    static const RealT m_b;
  };

  //! Load from binary stream
  RavlN::BinIStreamC &operator>>(RavlN::BinIStreamC &strm,GPSCoordinateC &gpsCoord);
  
  //! Save to binary stream
  RavlN::BinOStreamC &operator<<(RavlN::BinOStreamC &strm,const GPSCoordinateC &gpsCoord);
  
}

#endif

