// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Base/Index2d.cc"

#include "Ravl/Index2d.hh"
#include "Ravl/Assert.hh"
#include "Ravl/IndexRange2d.hh"
#include "Ravl/Stream.hh"
#include "Ravl/Math.hh"

namespace RavlN {
  Index2dC relNeighPixel[9]= {Index2dC( 0,  1),
			      Index2dC( 1,  1),
			      Index2dC( 1,  0),
			      Index2dC( 1, -1),
			      Index2dC( 0, -1),
			      Index2dC(-1, -1),
			      Index2dC(-1,  0),
			      Index2dC(-1,  1),
			      Index2dC( 0,  0)
  };

  const NeighbourOrderT reverseNeighPixel[9] = {
    NEIGH_LEFT      , /* 0 */
    NEIGH_UP_LEFT   , /* 1 */
    NEIGH_UP        , /* 2 */
    NEIGH_UP_RIGHT  , /* 3 */
    NEIGH_RIGHT     , /* 4 */
    NEIGH_DOWN_RIGHT, /* 5 */
    NEIGH_DOWN      , /* 6 */
    NEIGH_DOWN_LEFT , /* 7 */
    NEIGH_CENTER      /* 8 */
  };

  static IndexC Neighbours[3][3] = {{ IndexC(5), IndexC(6), IndexC(7)},
			     { IndexC(4), IndexC(8), IndexC(0)},
			     { IndexC(3), IndexC(2), IndexC(1)}};

  bool Index2dC::IsInside(const IndexRange2dC & range) const
  { return range.RowRange().Contains(Row()) && range.ColRange().Contains(Col()); }

  IndexC Index2dC::NeighbourOrder() const {
    RavlAssertMsg(IsRelNeighbour8(),"pxl is not a neighbouring pixel");
    return Neighbours[Row().V()+1][Col().V()+1];
  }

  IndexC Index2dC::NeighbourOrder(const Index2dC & pxl) const
  { return Index2dC(pxl - (*this)).NeighbourOrder(); }

  Index2dC Index2dC::Neighbour(NeighbourOrderT neighOrder) const {
    switch(neighOrder) {
    case NEIGH_RIGHT:      return Index2dC(Row()  , Col()+1);
    case NEIGH_DOWN_RIGHT: return Index2dC(Row()+1, Col()+1);
    case NEIGH_DOWN:       return Index2dC(Row()+1, Col()  );
    case NEIGH_DOWN_LEFT:  return Index2dC(Row()+1, Col()-1);
    case NEIGH_LEFT:       return Index2dC(Row()  , Col()-1);
    case NEIGH_UP_LEFT:    return Index2dC(Row()-1, Col()-1);
    case NEIGH_UP:         return Index2dC(Row()-1, Col()  );
    case NEIGH_UP_RIGHT:   return Index2dC(Row()-1, Col()+1);
    case NEIGH_CENTER:     return Index2dC(Row()  , Col()  );
    };
    return Index2dC(-1000,-1000);
  }

}
