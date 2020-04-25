// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_CRACKCODE_HEADER
#define RAVL_CRACKCODE_HEADER 1
//////////////////////////////////////////////////////////////////////////
//! file="Ravl/Math/Geometry/Euclidean/Boundary/CrackCode.hh"
//! lib=RavlMath
//! userlevel=Normal
//! author="Radek Marik"
//! date="26.10.1992"
//! docentry="Ravl.API.Images.Boundaries;Ravl.API.Math.Geometry.2D.Boundary"
//! rcsid="$Id$"

#include "Ravl/Types.hh"
#include "Ravl/Index2d.hh"
#include "Ravl/Stream.hh"
#include "Ravl/BinStream.hh"

namespace RavlN {
  
  //: Crack code 
  // Symbol names of crack code, ordered counter-clockwise.
  
  enum  CrackCodeT {
    CR_DOWN  = 0,
    CR_RIGHT = 1,
    CR_UP    = 2,
    CR_LEFT  = 3,
    CR_NODIR = 4
  };
  
  //: Relative crack code 
  // Symbol names of crack code, ordered counter-clockwise.
  
  enum  RelativeCrackCodeT {
    CR_AHEAD     = 0,
    CR_TO_LEFT   = 1,
    CR_BACK      = 2,
    CR_TO_RIGHT  = 3,
    CR_RNODIR    = 4
  };
  
  Index2dC CrackStep(const Index2dC &pixel,CrackCodeT crackCode);
  Index2dC CrackDirection(CrackCodeT crackCode);
  
  //:-
  
  //! userlevel=Normal
  //: Crack code or Freeman code 
  
  class CrackCodeC {
  public:
    CrackCodeC()
    {}
    //: Default constructor.
    // Direction is undefined.
    
    CrackCodeC(const IntT i);
    //: Constructs object from an integer 'i'.
    
    CrackCodeC(CrackCodeT cc)
      : crackCode(cc)
    {}
    //: Constructs and set value to be 'cc'.
    
    inline CrackCodeT Code() const
    { return crackCode; }
    //: Returns the crack code.
    
    RelativeCrackCodeT Relative(const CrackCodeC & cc) const {
      int rcode = (int) cc.Code() - (int) Code();
      if(rcode < 0) rcode += 4;
      else rcode %= 4;
      return static_cast<RelativeCrackCodeT>(rcode); 
    }
    //: Get relative crack code of direction 'cc' relative to this one.
    
    inline bool operator==(const CrackCodeC & cc) const
    { return cc.Code() == Code(); }
    //: Equality test
    
    inline bool operator!=(const CrackCodeC & cc) const
    { return (bool)(cc.Code() != Code()); }
    //: Returns true if the object content is not equal to 'cc'.
    
    inline const CrackCodeC & operator+=(const CrackCodeC & cc)  {
      IntT result = crackCode + cc.crackCode;
      crackCode = (CrackCodeT)(result % 4);
      return(*this);
    }
    //: Add a relative crack code.
    // The crack code 'cc' is taken as a relative crack code. The relative
    // crack code is added to this crack code.

    inline const CrackCodeC & operator-=(const CrackCodeC & cc) { 
      IntT result = crackCode - cc.crackCode + 4;
      crackCode = (CrackCodeT)(result % 4);
      return(*this);
    }
    //: Subtract a relative crack code.
    // The crack code 'cc' is taken as a relative crack code. 
    // The relative crack code is subtracted from this crack code.
    
    const CrackCodeC &operator=(const CrackCodeC & cc) {
      crackCode=cc.crackCode;
      return *this;
    }
    //: Assignment.
    
    const CrackCodeC &operator=(const CrackCodeT & cc) {
      crackCode=cc;
      return *this;
    }
    //: Assignment.
    
    // ----------   turning -----------------------

    inline CrackCodeC & TurnClock()
    { crackCode = clockWiseTurn[crackCode]; return *this; }
    //: Turns the crack code clockwise.
    // This is an in-place operation.
    
    inline CrackCodeC & TurnCClock()
    { crackCode = cClockWiseTurn[crackCode]; return *this; }
    //: Turns the crack code counterclockwise.
    // This is an in-place operation.
    
    inline CrackCodeC & TurnBack()
    { crackCode = backTurn[crackCode]; return *this; }
    //: Turns the crack code backward.
    // This is an in-place operation.
    
    Index2dC Next(const Index2dC &pixel) const {
      return Index2dC(pixel.Row() + offsetRow[crackCode],
		      pixel.Col() + offsetCol[crackCode]);
    }
    //: Get pixel in the direction of the crack code.
    
  protected:
    static const CrackCodeT clockWiseTurn[5];
    static const CrackCodeT cClockWiseTurn[5];
    static const CrackCodeT backTurn[5];
    static const IndexC offsetRow[5];
    static const IndexC offsetCol[5];
    
    CrackCodeT crackCode;  // The code.
    friend Index2dC CrackStep(const Index2dC & ,CrackCodeT );
    friend Index2dC CrackDirection(CrackCodeT );
  };
  
  inline ostream &operator<<(ostream &strm,const CrackCodeC &cc) {
    strm << (int) cc.Code();
    return strm;
  }
  //! userlevel=Normal
  //: Write to a stream.
  
  inline istream &operator>>(istream &strm,CrackCodeC &cc) {
    int v;
    strm >> v;
    cc = (CrackCodeT) v;
    return strm;
  }
  //! userlevel=Normal
  //: Read from a stream.
  
  inline BinOStreamC &operator<<(BinOStreamC &strm,const CrackCodeC &cc) {
    strm << (char) cc.Code();
    return strm;
  }
  //! userlevel=Normal
  //: Write to a bin stream.
  
  inline BinIStreamC &operator>>(BinIStreamC &strm,CrackCodeC &cc) {
    char v;
    strm >> v;
    cc = (CrackCodeT) v;
    return strm;
  }
  //! userlevel=Normal
  //: Read from a bin stream.
  
  inline
  Index2dC CrackStep(const Index2dC &pixel,CrackCodeT crackCode) {
    switch (crackCode) {
    case CR_DOWN : return Index2dC(pixel.Row()+1,pixel.Col());
    case CR_RIGHT: return Index2dC(pixel.Row(),pixel.Col()+1);
    case CR_UP   : return Index2dC(pixel.Row()-1,pixel.Col());
    case CR_LEFT : return Index2dC(pixel.Row(),pixel.Col()-1);
    case CR_NODIR: break;
    }
    return pixel;
  }
  //: Step one pixel in the direction of the crack code.
  
  inline
  Index2dC CrackDirection(CrackCodeT crackCode) 
  { return Index2dC(CrackCodeC::offsetRow[crackCode],CrackCodeC::offsetCol[crackCode]); }
  //: Direction in the form of an offset for a crack code.
  
}
#endif
