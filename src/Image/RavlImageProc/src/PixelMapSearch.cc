// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////
//! date="13/2/1997"
//! author="Charles Galambos"
//! rcsid="$Id$"
//! lib=RavlImageProc
//! file="Ravl/Image/Processing/Lines/PixelMapSearch.cc"

#include "Ravl/Image/PixelMapSearch.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlImageN {
  
  static bool PixelMapSearchBasePntCompair(const PCIndex2dC & el1, const PCIndex2dC & el2) 
  { return el1.P() > el2.P(); }
  
  void PixelMapSearchBaseC::MergeReady(DListC<PCIndex2dC> &aLst,const DListC<PCIndex2dC> &aReady)  {
    DListC<PCIndex2dC> Ready(aReady);
    aLst.MergeSort(PixelMapSearchBasePntCompair);
#if DODEBUG
    {
      cerr << "PixelMapSearchBaseC::MergeReady() aLst: ";
      for(DLIterC<PCIndex2dC> It(aLst);It.IsElm();It.Next()) {
	cerr << It.Data().P() << " ";
      }
      cerr << "\n";
    }
#endif
    DLIterC<PCIndex2dC> ItN(aLst);
    DLIterC<PCIndex2dC> ItR(Ready);
    if(ItN.IsElm() && ItR.IsElm()) {
      while(1) {
	if(ItR.Data().P() > ItN.Data().P()) {
	  ItR.Next();
	  if(!ItR.IsElm())
	    break;
	} else {
	  ItR.MoveBef(ItN);
	  ItN.Next();
	  if(!ItN.IsElm())
	    break;
	}
      }
    }
    //  Ready.MoveLast(ItN);
    Ready.MoveLast(aLst);
#if DODEBUG
    {
      cerr << "PixelMapSearchBaseC::MergeReady() Ready: ";
      for(DLIterC<PCIndex2dC> It(Ready);It.IsElm();It.Next()) {
	cerr << It.Data().P() << " ";
      }
      cerr << "\n";
    }
#endif
  }

}
