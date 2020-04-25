// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_PIXMAPSEARCH_HEADER 
#define RAVLIMAGE_PIXMAPSEARCH_HEADER 1
/////////////////////////////////////////////////////////
//! date="13/2/1997"
//! author="Charles Galambos"
//! docentry="Ravl.API.Images.Lines"
//! file="Ravl/Image/Processing/Lines/PixelMapSearch.hh"
//! lib=RavlImageProc
//! rcsid="$Id$"

#include "Ravl/HSet.hh"
#include "Ravl/StdConst.hh"
#include "Ravl/Image/PixelMap.hh"
#include "Ravl/Image/PCPixelList.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/QInt.hh"
#include "Ravl/Vector2d.hh"
#include "Ravl/StdMath.hh"

namespace RavlImageN {

  //! userlevel=Develop
  //: Pixel map search base class.
  
  class PixelMapSearchBaseC {
  public:
    inline PixelMapSearchBaseC(PixelMapC &Map,RealT CWidth,RealT nMinParam,RealT nMaxParam,const ImageC<ByteT> &ninimg);
    //: Constructor.
    
    inline RealT ParamPos() const { return Where; }
    //: Get position in linear space.
    
  protected:
    
    const RealT CWidth;// Width of corridor.
    const RealT CWidth2;
    
    RealT Where;       // Where in the model we are.
    RealT MaxParam;    // Maximum value of parameter.
    RealT MinParam;    // Minimum value of parameter.
    
    PixelMapC &Map;   // Map of pixels.
    HSetC<Index2dC> Done;  // Bins that have been checked.
    DListC<PCIndex2dC> Ready; // Points that have been covered. in increasing parameter value.
    Index2dC LastSet;  // Centre of last set of 4 bins.
    
    ImageC<ByteT> inimg;
    
    void MergeReady(DListC<PCIndex2dC> &aLst,const DListC<PCIndex2dC> &Ready);
    //: Merge new points into ready list.
    
    inline BinListC<Point2dC,Index2dC,Index2dC> &MapLocal() { return Map.local; }
    //: Access bins in map.
  };
  
  
  ///////////////////////////////////
  
  //! userlevel=Normal
  //: Pixel map search base class.
  
  template <class CurveT>
  class PixelMapSearchC 
    : public PixelMapSearchBaseC 
  {
  public:
    inline PixelMapSearchC(PixelMapC &Map,
			   RealT CWidth,
			   CurveT Curve,
			   RealT nMinParam,RealT nMaxParam,const  ImageC<ByteT> &ninimg);
    //: Constructor.
    
    Point2dC NewPoints(Point2dC CurAt,DListC<PCIndex2dC> &NewPnts);
    //: Get new points.
    
    Point2dC NextPoint(Point2dC From,bool PosDir,RealT &NewPar);
    //: Find next point on grid.
    
    PCPixelListC Collect(Point2dC Start); 
    //: Returns closest pixels to the 'Start' point
    
  protected:
    inline void IncWhere();
    //: Increment where
    
    DListC<PCIndex2dC> NewPoints(Point2dC At);
    
    bool NextValid();
    
  private:
    CurveT Curve; // Object model.  
  };
  
  ///////////////////////////////
  
  inline PixelMapSearchBaseC::PixelMapSearchBaseC(PixelMapC &nMap,RealT nCWidth,RealT nMinParam,RealT nMaxParam,const  ImageC<ByteT> &ninimg) 
    : CWidth(nCWidth),
      CWidth2(nCWidth * nCWidth),
      Where(0),
      MaxParam(nMaxParam),
      MinParam(nMinParam),
      Map(nMap),
      Done(),
      inimg(ninimg)
  {}
  
  ////////////////////////////////////////////////////////
  
  template <class CurveT>
  inline 
  PixelMapSearchC<CurveT>::PixelMapSearchC(PixelMapC &Map,RealT CWidth,CurveT nCurve,
					   RealT nMinParam,RealT nMaxParam,const  ImageC<ByteT> &ninimg)
    : PixelMapSearchBaseC(Map,CWidth,nMinParam,nMaxParam,ninimg),
      Curve(nCurve)
  {}
  
  template <class CurveT>
  Point2dC PixelMapSearchC<CurveT>::NewPoints(Point2dC CurAt,DListC<PCIndex2dC> &NewPnts)  {
    Point2dC MAt = (CurAt/Map.BinSize()) - Vector2dC(0.5,0.5);
    // Find nearest grid.
    Index2dC IndAt((IndexC) (QFloor(MAt[0])),(IndexC) (QFloor(MAt[1])));
    Point2dC Center = (Point2dC(IndAt) * Map.BinSize()) + Map.BinSize();
    IntT dx,dy;
    //cerr << "NewPoints: " << Center << "\n";
    for(dx = 0;dx < 2;dx++) {
      for(dy = 0;dy < 2;dy++) {
	Index2dC TInd(IndAt.Row() + dx,IndAt.Col() + dy);
	if(Done.Insert(TInd)) { // If new bin, check contents
	  //ONLFCDEBUG(DBInt.DrawRect(Map.BinSize() * TInd,((Map.BinSize() *TInd) +Map.BinSize())-Vector2dC(1,1),false,1));
	  for(DLIterC<Index2dC> pIt(MapLocal().Bin(TInd));
	      pIt.IsElm();
	      pIt.Next()) {
	    if(inimg[pIt.Data()] == 0)
	      continue;
	    Point2dC sample(pIt.Data()); // Point of measurement.
	    RealT param = Curve.Closest(sample);   // Find paramiter of closest point on curve
	    Point2dC onCurve = Curve.Point(param); // Find position of closest point on curve.
	    RealT dist = onCurve.SqrEuclidDistance(sample); // Distance from ideal curve.	
	    //ONLFCDEBUG(DBInt.Draw(pIt.Data()));
	    if(dist > CWidth2) { // Within corridor ?	    
	      //cerr << "PixelMapSearchC<CurveT>::NewPoints(),   Sample: " << sample << " Param:" << Param << " Dist:" << Dist << " OnCurve:" << OnCurve << "\n";
	      //ONLFCDEBUG(DBInt.Draw(pIt.Data(),1));
	    } else {
	      //cerr << "PixelMapSearchC<CurveT>::NewPoints(), * Sample: " << sample << " Param:" << Param << " Dist:" << Dist << " OnCurve:" << OnCurve << "\n";
	      //ONLFCDEBUG(DBInt.Draw(pIt.Data(),3));
	      NewPnts.InsLast(PCIndex2dC(pIt.Data(),onCurve,param));
	    }
	  }
	}
      }
    }
    //ONLFCDEBUG(DBInt.Wait());
    //return NewPnts;
    return Center;
  }
  
  //////////////////////////////////////
  // Find next point.
  
  template <class CurveT>
  Point2dC PixelMapSearchC<CurveT>::NextPoint(Point2dC From,bool PosDir,RealT &NewPar)  {
    //const RealT CheckX[] = { -1, -1, -1, 0,  0,  1,  1,  1 };
    //const RealT CheckY[] = {  1,  0, -1, 1, -1,  1,  0, -1 };
    const RealT CheckX[] = { -1, 0,  0, 1 };
    const RealT CheckY[] = {  0, 1, -1, 0 };
    Point2dC Ret(0,0),BinSize = Map.BinSize();
    Point2dC Newun;
    RealT NNPar = RavlConstN::nanReal;
    //RealT MinDist =RavlConstN::maxReal;
    RealT MinOff =  RavlConstN::maxReal;
    for(IntT i = 0;i < 4;i++) {
      Newun[0] = CheckX[i] * BinSize[0] + From[0];
      Newun[1] = CheckY[i] * BinSize[1] + From[1];
      RealT NP = Curve.Closest(Newun);
      RealT PDist = (NP - NewPar);
      //cerr << "Consider :" << NP << " D:" << PDist << "\n";
      if(PosDir) {
	if(PDist <= 0)
	  continue;
      } else {
	if(PDist >= 0)
	  continue;
      }
      Point2dC OnCurve = Curve.Point(NP);
      RealT Dist = fabs(OnCurve.SqrEuclidDistance(Newun));
      //cerr << "Dist: " << Dist << " CW2:" << CWidth2 << " " << CWidth <<  "\n";
      //if(Dist > CWidth2)
      //continue;
      PDist = Abs(PDist);
      //if(PDist < MinDist || (PDist <= MinDist && Dist < MinOff)) 
      if(Dist < MinOff) {
	//MinDist = PDist;
	Ret = Newun;
	NNPar = NP;
	MinOff = Dist;
      }
    }
    NewPar = NNPar;
    return Ret;
  }

  template <class CurveT>
  PCPixelListC PixelMapSearchC<CurveT>::Collect(Point2dC Start)  {
    Ready.Empty();
    PCPixelListC Mid(Ready);
    DListC<PCIndex2dC> Latest;  
    Point2dC Center,CurAt;
    bool MidSet = false;
  //cerr << "PixelMapSearchC<CurveT>::Collect(); " << Start << "\n";
    // Find first point.
    Center = NewPoints(Start,Latest);
    //  Center += Map.BinSize()/2;
    MergeReady(Latest,Ready);
    
    RealT CVal = RavlConstN::maxReal;
    RealT StPar = Curve.Closest(Start); // Get paramiter for start pixel.
    PCPixelListC It(Ready);
    for(It.First();It.IsElm();It.Next()) {
      RealT NParam = It.Data().P();
      RealT TmpV = fabs(StPar - NParam);
      if(TmpV < CVal) {
	Mid = It;
	MidSet = true;
	CVal = TmpV;
      }
      if(NParam > StPar)
	break; // Must of passed closest.
    }
    //cerr << "PixelMapSearchC<CurveT>::Collect(); Forward \n";
    // Go forward.
    CurAt = Center;
    RealT StartNP = Curve.Closest(Start);
    RealT NP = StartNP;
    while(1) {
      CurAt = NextPoint(CurAt,true,NP);
      //cerr << "f " << NP << " : " << CurAt << "\n";
      if(IsNan(NP)) 
	break;  // No point found.
      if(NP > MaxParam)
	break;
      NewPoints(CurAt,Latest);
      MergeReady(Latest,Ready);
    }
    if(!MidSet)
      Mid.First(); // Set to first.
    //cerr << "PixelMapSearchC<CurveT>::Collect(); Backward \n";
    // Go backward.
    CurAt = Center;
    NP = StartNP;
    while(1) {
      CurAt = NextPoint(CurAt,false,NP);
      //cerr << "b " << NP << " : " << CurAt << "\n";
      //cerr << NP << "\n";
      if(IsNan(NP)) 
	break;  // No point found.
      if(NP < MinParam)
	break;
      NewPoints(CurAt,Latest);
      MergeReady(Latest,Ready);
    }
    //cerr << "PixelMapSearchC<CurveT>::Collect(); Done " << Start << "\n";
    return Mid;
  } 
}


#endif
