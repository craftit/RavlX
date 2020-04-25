// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! author="Charles Galambos"
//! lib=RavlMath
//! file="Ravl/Math/Sequence/SquareIter.cc"

#include "Ravl/SquareIter.hh"
#include "Ravl/Stream.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  
  bool SquareIterC::Next() {
    ONDEBUG(std::cerr << "SquareIterC::Next(), State:" << state << "\n");
    switch(state) {
    case 1:  // From Center.
      if(maxSize <= 1) 
	break;
      at = centre.UpN(); 
      state++; 
      size = 1;
      end = at.Col().V() + 1;
      return true;
    case 2: // Along top to the right.
      at.Col()++;
      if(at.Col() < end) 
	return true;
      state++;
      end = at.Row().V() + (size * 2);
      return true;
    case 3: // Down the left side.
      at.Row()++;
      if(at.Row() < end)
	return true;
      state++;
      end = at.Col().V() - (size * 2);
      return true;
    case 4: // Back along the bottom.
      at.Col()--;
      if(at.Col() > end)
	return true;
      state++;
      end = at.Row().V() - ((size * 2)+1);
      return true;
    case 5: // Up the righ hand side.
      at.Row()--;
      if(at.Row() > end)
	return true;
      size++;
      if(size >= maxSize)
	break; // Done!
      at = centre;
      at.Row() -= size;
      at.Col() -= size;
      end = at.Col().V() + (size * 2);
      state = 2;
      return true;
    default:
      RavlIssueError("SquareIterC::Next(), ERROR: Illegal state. ");
      break;
    }
    state = 0;
    return false;
  }
  
}
