// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlImageProc
//! file="Ravl/Image/Processing/Lines/PCPixelList.cc"

#include "Ravl/Image/PCPixelList.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/SArray1dIter.hh"

#define DODEBUG 0
#if DODEBUG 
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlImageN {

  /////////////////////////////
  // Remove points in list from the image.
  // Set the to zero.
  
  bool PCPixelListC::RemoveFromImage(ImageC<ByteT> &img) {
    DLIterC<PCIndex2dC> It(*this);
    for(It.First();It.IsElm();It.Next())
      img[It.Data()] = 0;
    return true;
  }
  

  ////////////////////////////
  // Trim points that are too far appart to code effiecently  and 
  // estimate average distance between points.
  
  IntT PCPixelListC::TrimLine(RealT MaxDist,RealT &DistEst) {
    DLIterC<PCIndex2dC> It(*this);
    RealT TotDist = 0;
    IntT N = 0;
    if(!It.IsElm()) {
      if(List().Size() != 0) {
	cout << "PCPixelListC::TrimLine(), Bad List !!!!! \n";
      }
      DistEst = 1;
      return 0;
    }
    N++; // There's one!
    // Forward.
    PCIndex2dC *cur,*last = &It.Data();
    It.Next();
    for(;It.IsElm();It.Next()) {
      cur = &It.Data();
      RealT Dist = last->ClosestPnt().EuclidDistance(cur->ClosestPnt());
      if(Dist > MaxDist) {
	while(It.IsElm())
	  It.DelMoveNext();
	// Cut
	break;
      }
      last = cur;
      N++;
      TotDist += Dist;
    }
    // Backward
    It = *this;
    last = &It.Data();
    It.Prev();
    for(;It.IsElm();It.Prev()) {
      cur = &It.Data();
      RealT Dist = last->ClosestPnt().EuclidDistance(cur->ClosestPnt());
      if(Dist > MaxDist) {
	while(It.IsElm())
	  It.Del();
	// Cut
	break;
      }
      last = cur;
      N++;
      TotDist += Dist;
    }
    DistEst = (RealT) TotDist / N;
    return N;
  }
  
  ///////////////////////////////////////
  // Trim points that are too far appart to code effiecently  and 
  // estimate average distance between points. But use the longest
  // segment of pixels without a gap. 
  // Return: Number of pixels in segment.
  
  IntT PCPixelListC::TrimLongest(RealT maxDist,RealT &distEst) {
    ONDEBUG(cerr << "PCPixelListC::TrimLongest(), maxDist=" << maxDist << "\n");
    DLIterC<PCIndex2dC> it(*this);
    IntT longest  = 0,curLen = 1; 
    RealT totDist = 0;
    RealT longDist = 0;
    it.First();
    RavlAssert(it);
    PCIndex2dC *cur,*last = &it.Data();
    DLIterC<PCIndex2dC> lStart(it),lEnd,CStart(it);
    for(it++;it;it++) {
      cur = &it.Data();
      RealT dist = last->ClosestPnt().EuclidDistance(cur->ClosestPnt());
      //cerr << "Pixel " << curLen << " dist=" << dist << " @ " << (void*)cur << "\n";
      if(dist > maxDist) { // Is the distance between pixels larger than the maximum allowed ?
	if(curLen > longest) { // Is this the best line segment found so far ?
	  longest = curLen;   // Save pixel count.
	  longDist = totDist; // Save distance total
	  lStart = CStart;   // Save start
	  RavlAssert(it);
	  lEnd = it;         // Save end.
	}
	CStart = it; // Save the start if this segment.
	curLen = 0;  // Reset length counter for current segment.
	totDist = 0; // Reset total length of lines.
      }
      curLen++;
      totDist += dist;
      last = cur;
    }
    if(curLen > longest) {
      //cerr << "Last seg. \n";
      longest = curLen;
      longDist = totDist;
      lStart = CStart;
      lEnd = it;
    }
    if(longest == 0) return 0; // No segment meets the requirments.
    if(lEnd)
      lEnd.InclusiveTail(); // Dump end of list.
    lStart.Head(); // Dump head of list.
    First();       // Might as well point to the first element. 
    ONDEBUG(cerr << "PCPixelListC::TrimLongest(), Total=" << List().Size() << " longest=" << longest<< " lEnd=" << (IntT) (lEnd)<< "\n");
    //cerr << "Tail at @ " << ((void *) &(List().Last())) << " \n";
    ONDEBUG(RavlAssert(lst.Size() == (UIntT) longest));
    distEst = (RealT) longDist / longest;
    return longest;
  }
  
  ////////////////////////////////
  // Turn list into an array.
  
  SArray1dC<Point2dC> PCPixelListC::PointList() const {
    SArray1dC<Point2dC> ret(Size());
    DLIterC<PCIndex2dC> it(*this);
    SArray1dIterC<Point2dC> ait(ret);
    for(it.First();it;it++,ait++) 
      *ait = *it;
    return ret;
  }
  
  ///////////////////////////////
  //: Write out appropriate GF header.
  
  bool PCPixelListC::WriteGFHeader(ostream &out) {
    out << "pointset ";
    return true;
  }
  
  
  ///////////////////////////////
  // Write points in GF format.
  
  bool PCPixelListC::WriteGFPnts(ostream &out) const {
    IntT lsize = Size();
    out << lsize << " ";
    DLIterC<PCIndex2dC> It(*this);
    for(It.First();It.IsElm();It.Next()) 
      out << It.Data().Col() << " " << It.Data().Row() << " ";
    return true;
  }

}
