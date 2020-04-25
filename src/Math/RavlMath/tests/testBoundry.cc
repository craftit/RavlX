// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Geometry/Euclidean/Boundary/testBoundry.cc"
//! userlevel=Develop
//! author="Radek Marik"
//! docentry="Ravl.API.Images.Boundaries;Ravl.API.Math.Geometry.2D.Boundary"

#include "Ravl/Boundary.hh"
#include "Ravl/Stream.hh"
#include "Ravl/Array2d.hh"

using namespace RavlN;

#define DODEBUG	0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

int testCrackCode();
int testBoundry();
int testEdge();
int testMidPoint();
int testOrderEdges();

int main() {
  int ln;
  if((ln = testCrackCode()) != 0) {
    cerr << "Test failed at line " << ln << "\n";
    return 1;
  }
  if((ln = testEdge()) != 0) {
    cerr << "Test failed at line " << ln << "\n";
    return 1;
  }
  if((ln = testBoundry()) != 0) {
    cerr << "Test failed at line " << ln << "\n";
    return 1;
  }
  if((ln = testMidPoint()) != 0) {
    cerr << "Test failed at line " << ln << "\n";
    return 1;
  }
  if((ln = testOrderEdges()) != 0) {
    cerr << "Test failed at line " << ln << "\n";
    return 1;
  } 
  cerr << "Test passed. \n";
  return 0;
}

int testCrackCode() {

  CrackCodeC xcc(CR_UP);
  CrackCodeC ycc(CR_RIGHT);
  RelativeCrackCodeT rcc = xcc.Relative(ycc);
  if(rcc != CR_TO_RIGHT) return __LINE__;
  
  for(int i = 0;i < 4;i++) {
    CrackCodeC cc(i);
    int ip = i + 1;
    if(ip >= 4)
      ip -= 4;
    rcc = cc.Relative(cc);
    if(rcc != CR_AHEAD) return __LINE__;
    CrackCodeC ipcc(ip);
    rcc = cc.Relative(ipcc);
    cerr << "CodeLeft=" << (int)rcc << "\n";
    if(rcc != CR_TO_LEFT) return __LINE__;
    rcc = ipcc.Relative(cc);
    cerr << "CodeRight=" << (int)rcc << "\n";
    if(rcc != CR_TO_RIGHT) return __LINE__;
    
    int ipp = i + 2;
    if(ipp >= 4)
      ipp -= 4;;
    CrackCodeC ippcc(ipp);
    rcc = ippcc.Relative(cc);
    if(rcc != CR_BACK) return __LINE__;
    rcc = cc.Relative(ippcc);
    if(rcc != CR_BACK) return __LINE__;
  }
    
  return 0;
}

int testEdge() {
  Index2dC start(5,6);
  CrackC edge(start,CR_DOWN);
  Index2dC at = edge.RPixel();
  ONDEBUG(cerr << "iAt=" << at << " Start=" << start << "\n");
  if(at != (start + Index2dC(0,-1))) return __LINE__;
  if(start != edge.LPixel()) return __LINE__;
  // Go around a pixel clockwise.
  for(int i = 0;i < 5;i++) {
    edge.Begin() = edge.End();
    edge.TurnClock();
    ONDEBUG(cerr << "At=" << edge.RPixel() << " Code:" << edge.Code() << "\n");
    if(at == edge.LPixel()) return __LINE__;
  }
  // Go around a pixel counter clockwise.
  edge = CrackC(start,CR_DOWN);
  at = edge.LPixel();
  if(at != start) return __LINE__;
  ONDEBUG(cerr << "iAt=" << at << "\n");
  for(int i = 0;i < 5;i++) {
    edge.Begin() = edge.End();
    edge.TurnCClock();
    ONDEBUG(cerr << "At=" << edge.LPixel() << " Code:" << edge.Code() << "\n");
    if(at != edge.LPixel()) return __LINE__;
  }
  //                       DOWN          RIGHT         UP            LEFT           None
  Index2dC offsets[5] = { Index2dC(0,-1),Index2dC(1,0),Index2dC(0,1),Index2dC(-1,0),Index2dC(0,0) };
  for(int i = 0;i < 5;i++) {
    edge = CrackC(start,i);
    cerr << " " << i << " RPixel=" << edge.RPixel() << "\n";
    if(edge.LPixel() != start) return __LINE__;
    if(edge.RPixel() != (start + offsets[i])) return __LINE__; 
  }
  return 0;
}

int testBoundry() {
  IndexRange2dC rect(IndexRangeC(1,3),IndexRangeC(2,4));
  BoundaryC bnd(rect,true);
  //cout << "Bounds:\n " << bnd << "\n";
  if(bnd.Size() != 12) return __LINE__;
  ONDEBUG(cout << "Area=" << bnd.Area() << "\n");
  if(bnd.Area() != - (IntT) rect.Area()) return __LINE__;
  IndexRange2dC tmpbb = bnd.BoundingBox();
  if(tmpbb == rect) return __LINE__;
  bnd.BReverse();
  if(tmpbb != bnd.BoundingBox()) return __LINE__;
  if(bnd.Area() != - (IntT) rect.Area()) return __LINE__;  
  bnd.Invert();
  ONDEBUG(cout << "RArea=" << bnd.Area() << "\n");
  if(bnd.Area() != (IntT) rect.Area()) return __LINE__;
  
  IndexRange2dC bb = bnd.BoundingBox();
  ONDEBUG(cerr << "Bounding box=" << bb << " Inv=" << tmpbb << "\n");
  if(bb != rect) return __LINE__;
  
  return 0;
}

int testMidPoint() {
  for(int i =0;i < 5;i++) {
    BVertexC start(5,5);
    CrackC edge(start,(CrackCodeC) i);
    //ONDEBUG(cerr << " Left=" << edge.LPixel() << " Right=" << edge.RPixel() << "\n");
    Point2dC m1 = ((Point2dC(edge.RPixel()) + Point2dC(edge.LPixel())) / 2) + Point2dC(0.5,0.5);
    //ONDEBUG(cerr << " Mid=" << edge.MidPoint() << " m1=" << m1 << " diff=" << (m1 - edge.MidPoint()) << "\n");
    if((m1 - edge.MidPoint()).SumOfSqr() > 0.00001) return __LINE__;
  }
  return 0;
}


int testOrderEdges() {
  cerr << "testOrderEdges() Called. \n";
  
  Array2dC<IntT> emask(5,5);
  
  emask.Fill(0);
  emask[1][1] = 1;
  emask[1][2] = 1;
  emask[1][3] = 1;
  emask[2][1] = 1;
  emask[3][1] = 1;
  emask[2][3] = 1;
  emask[3][2] = 1;
  
  BoundaryC bnds(emask,true);
  DListC<BoundaryC> lst = bnds.OrderEdges();
  
  // cerr << "Lst.Size()=" << lst.Size() << "\n";
  // cerr << "Lst.First().Size()=" << lst.First() << "\n";
  // cerr << "Lst.Last() =" << lst.Last() << "\n";
  
  if(lst.Size() != 2) return __LINE__;
  if((lst.First().Size() + lst.Last().Size()) != 16) return __LINE__;
  
  // Check its not a fluke, try a different orientation.
  
  emask.Fill(0);
  emask[1][2] = 1;
  emask[1][3] = 1;
  emask[2][1] = 1;
  emask[3][1] = 1;
  emask[2][3] = 1;
  emask[3][2] = 1;
  emask[3][3] = 1;
  
  BoundaryC bnds2(emask,true);
  lst = bnds2.OrderEdges();
  if(lst.Size() != 2) return __LINE__;
  if((lst.First().Size() + lst.Last().Size()) != 16) return __LINE__;
  
  BoundaryC bnds3;
  bnds3.InsLast(CrackC(BVertexC(2,2),1));
  bnds3.InsLast(CrackC(BVertexC(2,3),0));
  bnds3.InsLast(CrackC(BVertexC(3,3),3));
  lst = bnds3.OrderEdges();
  if(lst.Size() != 1) return __LINE__;
  if(lst.First().Size() != 3) return __LINE__;
  // cerr << "Lst.Size()=" << lst.Size() << "\n";
  // cerr << "Lst.First().Size()=" << lst.First() << "\n";

  BoundaryC bnds4;
  bnds4.InsLast(CrackC(BVertexC(2,2),1));
  //bnds4.InsLast(CrackC(BVertexC(2,3),0));
  bnds4.InsLast(CrackC(BVertexC(3,3),3));
  bnds4.InsLast(CrackC(BVertexC(3,2),2));
  lst = bnds4.OrderEdges();  
  if(lst.Size() != 1) return __LINE__;
  if(lst.First().Size() != 3) return __LINE__;
  //cerr << "Lst.Size()=" << lst.Size() << "\n";
  //cerr << "Lst.First().Size()=" << lst.First() << "\n";
  
  return 0;
}
