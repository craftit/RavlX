// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2013, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Distance/DistanceCorrelation.cc"

#include "Ravl/PatternRec/DistanceCorrelation.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/XMLFactoryRegister.hh"

namespace RavlN {

  DistanceCorrelationBodyC::DistanceCorrelationBodyC(bool normalise)
   : m_normalise(normalise)
  {}

  //: XMLFactoryC constructor.

  DistanceCorrelationBodyC::DistanceCorrelationBodyC(const XMLFactoryContextC &factory)
    : DistanceBodyC(factory),
      m_normalise(factory.AttributeBool("normalise",true))
  {}

  //: Load from stream.
  
  DistanceCorrelationBodyC::DistanceCorrelationBodyC(std::istream &strm)
    : DistanceBodyC(strm),
      m_normalise(true)
  {
    strm >> m_normalise;
  }
  
  //: Load from binary stream.
  
  DistanceCorrelationBodyC::DistanceCorrelationBodyC(BinIStreamC &strm)
    : DistanceBodyC(strm),
      m_normalise(true)
  {
    ByteT version = 0;
    strm >> version;
    if(version != 1)
      throw ExceptionUnexpectedVersionInStreamC("DistanceCorrelationBodyC");
    strm >> m_normalise;
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool DistanceCorrelationBodyC::Save (std::ostream &out) const
  {
    DistanceBodyC::Save(out);
    out << " " << m_normalise;
    return true;
  }
  
  //: Writes object to stream, can be loaded using constructor

  bool DistanceCorrelationBodyC::Save (BinOStreamC &out) const
  {
    DistanceBodyC::Save(out);
    ByteT version = 1;
    out << version << m_normalise;
    return true;
  }
  
  //: Measure the distance from d1 to d2.
  
  RealT DistanceCorrelationBodyC::Measure(const VectorC &d1,const VectorC &d2) const {
    if(m_normalise) {
      return 1 - d1.Dot(d2) / (d1.Modulus() * d2.Modulus());
    } else {
      return 1 - d1.Dot(d2);
    }
  }
  
  //: Measure the magnitude of d1.
  
  RealT DistanceCorrelationBodyC::Magnitude(const VectorC &x) const {
    RavlIssueError("Not supported");
    return 1;
  }

  //: Calculates Jacobian matrix at X
  
  MatrixC DistanceCorrelationBodyC::Jacobian (const VectorC &X) const {
    RavlIssueError("Not supported");
    return MatrixC();
  }
  
  //////////////////////////////////////////////////////////////////////
  
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(DistanceCorrelationBodyC,DistanceCorrelationC,DistanceC);

  RavlN::XMLFactoryRegisterHandleConvertC<DistanceCorrelationC, DistanceC> g_registerXMLFactoryDistanceCorrelation("RavlN::DistanceCorrelationC");
  
  extern void linkDistanceCorrelation()
  {}

}
