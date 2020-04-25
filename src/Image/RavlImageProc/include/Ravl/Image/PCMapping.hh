// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_PCMAPPING_HEADER
#define RAVLIMAGE_PCMAPPING_HEADER 1
////////////////////////////////////////////////////////////
//! docentry="Ravl.API.Images.Lines"
//! file="Ravl/Image/Processing/Lines/PCMapping.hh"
//! lib=RavlImageProc
//! author="Charles Galambos"
//! rcsid="$Id$"
//! date="07/01/1999"

#include "Ravl/Tuple2.hh"
#include "Ravl/Image/PCPixelList.hh"
#include "Ravl/DLIter.hh"
#include "Ravl/SArray1d.hh"

#ifndef __sgi__
#include <typeinfo>
#else
#include <typeinfo.h>
#endif

namespace RavlImageN {

  //! userlevel=Normal
  //: Simple mapping between a curve and some pixels.
  
  template<class CurveT>
  class PCPixMapC {
  public:
    PCPixMapC()
    {}
    //: Default constructor.
    // The curve will be in an undefined state, and the pixel list
    // will be empty.
  
    PCPixMapC(const CurveT &cur,const PCPixelListC &px = PCPixelListC(true),SArray1dC<IntT> threshData = SArray1dC<IntT>())
      : curve(cur),
	pixs(px),
	values(threshData)
    {}
    //: Constructor.
    
    CurveT &Curve() { return curve; }
    //: Access curve.
    
    const CurveT &Curve() const { return curve; }
    //: Access curve.
    
    PCPixelListC &PixList() { return pixs; }
    //: Access pixel list.
    
    const PCPixelListC &PixList() const { return pixs; }
    //: Access pixel list.
  
    void InsLast(Index2dC pix) {
      RealT p = curve.Closest(pix);
      pixs.InsLast(PCIndex2dC(pix,curve.Point(p),p));
    }
    //: Add new pixel to end of list.
    
    SArray1dC<IntT> &Thresh()
    { return values; }
    //: Access threshold used in finding this line.
    
    SArray1dC<IntT> Thresh() const
    { return values; }
    //: Access threshold used in finding this line.
    
  protected:
    CurveT curve;
    PCPixelListC pixs;
    SArray1dC<IntT> values;
  };
  
  template<class CurveT>
  ostream &operator<<(ostream &strm,const PCPixMapC<CurveT> &data) {
    RavlAssert(0); // Not implemented.
    return strm;
  }
  
  template<class CurveT>
  istream &operator>>(istream &strm,PCPixMapC<CurveT> &data) {
    RavlAssert(0); // Not implemented.
    return strm;
  }
  
  
  //! userlevel=Normal
  //: Mappings from a curve to a set of pixels.
  
  template<class CurveT>
  class PCPixMappingC 
    : public DListC<PCPixMapC<CurveT> >
  {
  public:
    PCPixMappingC();
    //: Default constructor.
    
    inline PCPixMappingC(const DListC<PCPixMapC<CurveT> > &oth);
    //: Constructor.
    
    bool WriteGF(ostream &out) const;
    //: Write in GF format.
    
  };
  
  template<class CurveT>
  ostream &operator<<(ostream &strm,const PCPixMappingC<CurveT> &data) {
    strm << ((const DListC<PCPixMapC<CurveT> > &) data);
    return strm;
  }
  
  template<class CurveT>
  istream &operator>>(istream &strm,PCPixMappingC<CurveT> &data) {
    strm >> ((DListC<PCPixMapC<CurveT> > &) data);
    return strm;
  }
  
  ////////////////////////////////////////////
  
  template<class CurveT>
  PCPixMappingC<CurveT>::PCPixMappingC()
  {}

  template<class CurveT>
  inline 
  PCPixMappingC<CurveT>::PCPixMappingC(const DListC<PCPixMapC<CurveT> > &oth)
    : DListC<PCPixMapC<CurveT> >(oth)
  {}
  
  template<class CurveT>
  bool 
  PCPixMappingC<CurveT>::WriteGF(ostream &out) const {
    out << "@Set " << typeid(CurveT).name() << endl;
    out << "Format idEdgePixels ";
    CurveT::WriteGFHeader(out); // Write curve header.
    out << endl;
    IntT id = 1;
    DLIterC<PCPixMapC<CurveT> > it(*this);
    for(;it;it++) {
      out << id++ << " ";
      it.Data().Curve().WriteGF(out);
      out << endl;
    }
    out << "@\n";
    out << "@Set EdgePixels\n";  
    out << "Format ";
    PCPixelListC::WriteGFHeader(out);
    out << endl;
    for(it.First();it;it++) {
      it.Data().PixList().WriteGFPnts(out);
      out << endl;
    }
    out << "@\n";
    return true;
  }
}
#endif
