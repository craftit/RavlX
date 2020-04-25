// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlCore

#include "Ravl/GPSCoordinate.hh"
#include "Ravl/StringList.hh"
#include "Ravl/SysLog.hh"

namespace RavlN {
  using RavlN::RealT;
  using RavlN::StringC;  
  using RavlN::Sqr;
  using RavlN::Sqrt;
  using RavlN::Floor;
  using RavlN::IntT;
  using RavlN::Abs;
  using RavlN::Sin;
  using RavlN::Cos;
  using RavlN::Pow;
  using RavlN::UIntT;
  using RavlN::Vector3dC;

#if 1
  // The following are the parameters for the GRS80 Ellipsoid used in GPS
  
  const RealT GPSCoordinateC::m_e2 = 0.00669438002290;
  const RealT GPSCoordinateC::m_a = 6378137.0000;
  const RealT GPSCoordinateC::m_b = 6356752.3141;
#else
  
  // AIRY 1830 for testing only.
  const RealT GPSCoordinateC::m_e2 = 6.6705397616e-3;
  
  const RealT GPSCoordinateC::m_a = 6377563.396;
  const RealT GPSCoordinateC::m_b = 6356256.910 ;
#endif
  
  GPSCoordinateC::GPSCoordinateC(const RavlN::StringC &text) {
    Text2GPS(text,*this);    
  }
  

  //! Convert a cartesian position to gps.
  
  GPSCoordinateC GPSCoordinateC::Cartesian2GPS(const Point3dC &position,RealT precision) {
    const RealT x = position[0];
    const RealT y = position[1];
    const RealT z = position[2];
    
    RealT lambda = atan2(y,x);
    RealT p = Sqrt(Sqr(x) + Sqr(y));
    
    RealT theta = atan(z/(p * (1.0-m_e2)));
    UIntT iterLimit = 10;
    
    while(iterLimit-- > 0) {
      //std::cout << "Theta=" << (RavlConstN::rad2deg * theta) << "\n";
      
      RealT sinTheta = Sin(theta);
      RealT v = m_a / Sqrt(1-m_e2*Sqr(sinTheta));
      
      RealT oldTheta = theta;
      
      theta = atan((z+m_e2*v*sinTheta)/p);
      
      if(RavlN::Abs(oldTheta - theta) < precision)
        break;
      
    }
    
    RealT sinTheta = Sin(theta);
    RealT v = m_a / Sqrt(1-m_e2*Sqr(sinTheta));
    
    RealT height = (p / Cos(theta)) - v;
    
    return GPSCoordinateC(RavlConstN::rad2deg * theta,RavlConstN::rad2deg * lambda,height);
  }
  
  bool GPSCoordinateC::UsesNS(const StringC &coord) 
  { return coord.contains('N') || coord.contains('S') || coord.contains('n') || coord.contains('s'); }
  
  bool GPSCoordinateC::UsesEW(const StringC &coord) 
  { return coord.contains('E') || coord.contains('W') || coord.contains('e') || coord.contains('w'); }
  
  // Convert a gps coordinate from text form to a GPS class.
  // Returns false if text is not correctly formatted
  
  bool GPSCoordinateC::Text2GPS(const StringC &text,GPSCoordinateC &gps) {
    RavlN::StringListC strComps(text,",");
    if(strComps.Size() != 2 && strComps.Size() != 3) {
      //RavlError("Unexpected number of components (%zu) in GPS coordinates [%s] ",(size_t) strComps.Size(),text.chars());

      return false;
    }
    
    RealT degLat = RavlConstN::nanReal;
    RealT degLong = RavlConstN::nanReal;
    
    StringC coord1 = strComps.First();
    StringC coord2 = strComps.Nth(1);
    
    bool c1useNS = UsesNS(coord1);
    bool c1useEW = UsesEW(coord1);
    
    bool c2useNS = UsesNS(coord2);
    bool c2useEW = UsesEW(coord2);
    
    bool useNSEW = c1useNS || c1useEW || c2useNS || c2useEW;
    
    if(!useNSEW) {
      if(!TextDMSToAngle(coord1,degLat,AF_None)) {
        return false;
      }
      if(!TextDMSToAngle(coord2,degLong,AF_None)) {
        return false;
      }      
    } else {
      if((c1useNS && c1useEW)) {
        //RavlError("Failed to parse GPS coordinates, confusing use of NS & EW designators. Angle 1=[%s]",coord1.chars());
        return false;
      }
      if((c2useNS && c2useEW)) {
        //RavlError("Failed to parse GPS coordinates, confusing use of NS & EW designators. Angle 2=[%s]",coord2.chars());
        return false;
      }
      
      if(c1useNS) {
        if(!TextDMSToAngle(coord1,degLat,AF_NS))
          return false;
        if(!TextDMSToAngle(coord2,degLong,AF_EW))
          return false;
      } else {
        if(!TextDMSToAngle(coord1,degLong,AF_EW))
          return false;
        if(!TextDMSToAngle(coord2,degLat,AF_NS))
          return false;
      }
    }
    
    // Sort out hight.
    RealT height = 0;
    if(strComps.Size() == 3) {
      StringC coord3 = strComps.Last();
      
      height = coord3.RealValue();
    }
    
    gps = GPSCoordinateC(degLat,degLong,height);
    return true;
  }

  
  //! Normalise GPS coordinates.
  
  RavlN::Point3dC GPSCoordinateC::Normalise() const {
    // Normalise
    RealT rLat = (*this)[0];
    RealT rLong = (*this)[1];
    if(rLat > RavlConstN::pi)
      rLat -= Floor(rLat/360) * 360;
    
    if(rLat < -RavlConstN::pi)
      rLat += Floor(-rLat/360) * 360;
    
    if(rLong > RavlConstN::pi)
      rLong -= Floor(rLong/360) * 360;
    
    if(rLong < -RavlConstN::pi)
      rLong += Floor(-rLong/360) * 360;
    
    return RavlN::Point3dC(rLat,rLong,(*this)[2]);
  }
  
  
  
  // Print as text in degree's minutes seconds.
  
  StringC GPSCoordinateC::TextDMS() const {
    Point3dC val = Normalise();
    
    StringC ret = AngleToTextDMS(RavlN::Abs(val[0]));
    if(Latitude() > 0) {
      ret += " N, ";
    } else {
      ret += " S, ";
    }
    
    ret += AngleToTextDMS(RavlN::Abs(val[1]));
    if(Longitude() > 0) {
      ret += " E, ";
    } else {
      ret += " W, ";
    }
    
    StringC elev;
    elev.form("%3.2fm",val[2]);
    ret += elev;
    return ret;
  }
  
  StringC GPSCoordinateC::AngleToTextDMS(RealT value) {
    IntT degrees = Floor(value);
    RealT tmp = (value - (RealT) degrees) * 60;
    IntT minutes = Floor(tmp);
    RealT seconds = (tmp - (RealT) minutes) * 60;
    StringC ret;
    ret.form("%d %d'%2.4f\"",degrees,minutes,seconds);
    
    return ret;
  }
  
  
  // Convert a angle in text Degree's minutes seconds or decimal notation
  
  bool GPSCoordinateC::TextDMSToAngle(const StringC &value,RealT &angle,AngleFormT af) {
    StringC rest = value.TopAndTail();
    //rDebug("Parsing angle [%s] ",value.chars());
    
    // Decide the format of the string.
    IntT degreesAt = rest.index(' ');
    IntT minutesAt = rest.index('\'');
    IntT secondsAt = rest.index('\"');
    //IntT desingator = -1;
    RealT tv = 0;
        
    // Is angle in decimal format ?
    
    if(minutesAt < 0 && secondsAt < 0) {
      angle = rest.RealValue();
    } else {
      StringC tmp;
      if(degreesAt >= 0) {
        tmp = rest.before(degreesAt);
        //rDebug("Degrees=[%s] ",tmp.chars());

        angle = tmp.RealValue();
        if(modf(angle,&tv) != 0) {
          RavlError("Fractional value in degree's gps coordinate. ");
          return false;
        }
      } else {
        if(minutesAt > 0 || secondsAt > 0) {
          RavlError("Minutes or seconds specified in GPS angle without integer component. ");
          return false;
        }
      }
      
      if(minutesAt >= 0) {
        tmp = rest.at(degreesAt+1,minutesAt - degreesAt - 1);
        //rDebug("Minutes=[%s] ",tmp.chars());
        RealT rv = tmp.RealValue();
        if(secondsAt >= 0) {
          if(modf(angle,&tv) != 0) {
            RavlError("Fractional value in minutes of gps coordinate.");
            return false;
          }
        }
        if(rv < 0) {
          RavlError("Negative minutes values in gps coordinate.");
          return false; 
        }
        angle += rv * (1.0/60.0);
        
      } else {
        if(secondsAt > 0) {
          RavlError("Seconds specified in GPS angle minutes component. [%s] ",rest.chars());
          return false;
        }
      }
      
      if(secondsAt >= 0) {
        tmp = rest.at(minutesAt+1,secondsAt - minutesAt -1);
        //rDebug("Seconds=[%s] ",tmp.chars());
        RealT rv = tmp.RealValue();
        if(rv < 0) {
          RavlError("Negative values specified in GPS angle seconds component [%s]. ",rest.chars());
          return false;
        }
        angle += rv * (1.0/(60.0*60.0));
      } else {
        tmp = rest.after(minutesAt+1);
        // Any digits after the minute marker ?
        if(tmp.contains('1') ||
           tmp.contains('2') ||
           tmp.contains('3') ||
           tmp.contains('4') ||
           tmp.contains('5') ||
           tmp.contains('6') ||
           tmp.contains('7') ||
           tmp.contains('8') ||
           tmp.contains('9')) {
          RavlError("No seconds marker found, but digits after '. [%s]. ",rest.chars());
          return false;
        }
      }
    }
    
    bool haveNorth = rest.contains('N') || rest.contains('n');
    bool haveSouth = rest.contains('S') || rest.contains('s');
    bool haveEast = rest.contains('E') || rest.contains('e');
    bool haveWest = rest.contains('W') || rest.contains('w');
    
    switch(af) {
    case AF_None:
      if(haveNorth || haveSouth || haveEast || haveWest) {
        RavlError("Unexpected designator in [%s] ",value.chars());
        return false;
      }
      break;
      
    case AF_NS:
      if(haveEast || haveWest || (haveNorth && haveSouth)) {
        RavlError("Inconsistent designator in [%s] ",value.chars());
        return false;
      }
      if(angle < 0) {
        RavlError("Negative angle used in gps coordinate with N/S designator. ");
        return false;
      }
      
      if(haveSouth)
        angle *= -1;
      break;
      
    case AF_EW:
      if(haveNorth || haveSouth || (haveEast && haveWest)) {
        RavlError("Inconsistant designator in [%s] ",value.chars());
        return false;
      }
      if(angle < 0) {
        RavlError("Negative angle used in gps coordinate with E/W designator. ");
        return false;
      }
      if(haveWest)
        angle *= -1;      
      break;
    }
    
    return true;
  }

  
  //! Convert GPS coordinate into global Cartesian coordinates.
  
  RavlN::Point3dC GPSCoordinateC::Cartesian() const {
    RealT latRad = RavlConstN::deg2rad * (*this)[0]; 
    RealT longRad = RavlConstN::deg2rad * (*this)[1];
    
    RealT sinTheta = Sin(latRad);
    RealT cosTheta = Cos(latRad);
    RealT sinLambda = Sin(longRad);
    RealT cosLambda = Cos(longRad);
    
    RealT H = (*this)[2];
    RealT v = m_a / Sqrt(1-m_e2*Sqr(sinTheta));
    
    Point3dC ret;
    RealT vPlusH = (v + H);
    ret[0] = vPlusH * cosTheta * cosLambda; // X
    ret[1] = vPlusH * cosTheta * sinLambda; // Y
    ret[2] = ((1-m_e2) * v + H) * sinTheta; // Z
    
    return ret;
  }
  
  //! Access local vertical direction
  
  RavlN::Vector3dC GPSCoordinateC::Vertical() const {
    
    RealT latRad = RavlConstN::deg2rad * (*this)[0];
    RealT longRad = RavlConstN::deg2rad * (*this)[1];
    
    RealT sinTheta = sin(latRad);
    RealT cosTheta = cos(latRad);
    RealT sinLambda = sin(longRad);
    RealT cosLambda = cos(longRad);
    
    Point3dC ret;
    ret[0] = cosTheta * cosLambda;
    ret[1] = cosTheta * sinLambda;
    ret[2] = sinTheta;
    
    return ret;
    
  }

  //! Interpolate between two GPS positions.
  //! Fraction is between 0.0 and 1.0, where 0 is p1, and 1 is p2.

  bool GPSCoordinateC::BilinearInterpolate(RealT fraction,
                                           const GPSCoordinateC &p1,
                                           const GPSCoordinateC &p2,
                                           GPSCoordinateC &position)
  {
    RealT mf = 1.0 - fraction;
    // FIXME: This isn't ideal.  We really want to find the shortest path over a sphere,
    // but it'll do for now.
    position = GPSCoordinateC(p1.Latitude() * mf + p2.Latitude() * fraction,
                              p1.Longitude() * mf + p2.Longitude() * fraction,
                              p1.Height() * mf + p2.Height() * fraction,
                              p1.HorizontalErrorBounds() * mf + p2.HorizontalErrorBounds() * fraction,
                              p1.VerticalErrorBounds() * mf + p2.VerticalErrorBounds() * fraction
                              );
    return true;
  }


  //! Compute the differential 
  
  bool GPSCoordinateC::Differential(Vector3dC &diffLat,Vector3dC &diffLong,Vector3dC &diffHeight) const {
    RealT latRad = RavlConstN::deg2rad * (*this)[0];
    RealT longRad = RavlConstN::deg2rad * (*this)[1];
    
    RealT sinTheta = sin(latRad);
    RealT cosTheta = cos(latRad);
    RealT sinLambda = sin(longRad);
    RealT cosLambda = cos(longRad);
    
    RealT H = (*this)[2];

    RealT q3 = Sqrt(1-m_e2*Sqr(sinTheta));
    
    {
      RealT q1 = m_a*m_e2*Sqr(cosTheta)*sinTheta/Pow((1-m_e2*Sqr(sinTheta)),(3.0/2.0));
      RealT q2 = sinTheta*(H+m_a/q3);
      
      diffLat[0] = q1*cosLambda-q2*cosLambda;
      diffLat[1] = q1*sinLambda-q2*sinLambda;
      diffLat[2] = 
        cosTheta*(H+m_a*(1-m_e2)/q3)+
        m_a*(1-m_e2)*m_e2*cosTheta*Sqr(sinTheta)/Pow((1-m_e2*Sqr(sinTheta)),(3.0/2.0));
    }
    
    {
      RealT q4 = cosTheta*(H+m_a/q3);
      
      diffLong[0] = -q4*sinLambda;
      diffLong[1] =  q4*cosLambda;
      diffLong[2] =  0;
    }
    
    
    {
      // Height.
      diffHeight[0] = cosTheta * cosLambda;
      diffHeight[1] = cosTheta * sinLambda;
      diffHeight[2] = sinTheta;
    }
    
    
    return true;
  }

  RavlN::BinIStreamC &operator>>(RavlN::BinIStreamC &strm,GPSCoordinateC &gpsCoord) {
    strm >> (RavlN::Point3dC&)gpsCoord;
    FloatT errHorz,errVert;
    strm >> errHorz >> errVert;
    gpsCoord.SetErrorBounds(errHorz,errVert);
    return strm;
  }
  
  RavlN::BinOStreamC &operator<<(RavlN::BinOStreamC &strm,const GPSCoordinateC &gpsCoord) {
    strm << (const RavlN::Point3dC&)gpsCoord;
    FloatT errHorz = gpsCoord.HorizontalErrorBounds();
    FloatT errVert = gpsCoord.VerticalErrorBounds();
    strm << errHorz << errVert;
    return strm;
  }


}
