// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2011-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/DrawPolygon.hh"
#include "Ravl/Image/DrawLine.hh"
#include "Ravl/Image/EdgeDetector.hh"
#include "Ravl/Image/EdgeDetect.hh"
#include "Ravl/Polygon2d.hh"
#include "Ravl/Point2d.hh"
#include "Ravl/DList.hh"
#include "Ravl/Image/ImageConv.hh"
#include "Ravl/IO.hh"
//! lib=RavlImageProc

using namespace RavlN;
using namespace RavlImageN;

int testEdgeDet1();
int testEdgeDet2();
int testEdgeDet3();
int testEdgeLink();

int main(int nargs,char **argv) {
  int ln;
  if((ln = testEdgeDet1()) != 0) {
    cerr << "test failed on line " << ln << "\n";
    return 1;
  }
  if((ln = testEdgeDet2()) != 0) {
    cerr << "test failed on line " << ln << "\n";
    return 1;
  }
  if((ln = testEdgeDet3()) != 0) {
    cerr << "test failed on line " << ln << "\n";
    return 1;
  }
  if((ln = testEdgeLink()) != 0) {
    cerr << "test failed on line " << ln << "\n";
    return 1;
  }
  
  return 0;
}

int testEdgeDet1() {
  // set up triangles in image
  ImageC<RealT> img(20,20);
  img.Fill(0.0);
  DListC<Point2dC> pts;
  ((pts += Point2dC(0,5)) += Point2dC(10,5)) += Point2dC(0,15);
  Polygon2dC triangle(pts);
  DrawPolygon(img, 50.0, triangle, true);
  pts.Empty();
  ((pts += Point2dC(16,5)) += Point2dC(6,15)) += Point2dC(16,15);
  triangle = Polygon2dC(pts);
  DrawPolygon(img, 50.0, triangle, true);
  DrawLine(img, 25.0, Index2dC(15,6), Index2dC(7,14));
  DrawLine(img, 25.0, Index2dC(15,15), Index2dC(6,15));

  // run old Deriche edge detector
  {
  EdgeDetectorC det(true, 4, 8);
  DListC<SArray1dC<EdgelC> > edges;
  det.Apply(img, edges);
  cout << "Edges 1:\n" << edges << endl;
  ImageC<bool> edgeImg;
  det.Apply(DoubleImageCT2ByteImageCT(img), edgeImg);
  UIntT tot(0);
  for (Array2dIterC<bool> i(edgeImg); i; ++i) {
    if (*i) ++tot;
  }
  cout << "tot: " << tot << endl;
  if (tot != 56) return __LINE__;
  //Save("@X", edgeImg);
  }

  // run new Deriche edge detector
  {
  EdgeDetectC det;
  det.SetHysteresis(4,8);
  det.Apply(img);
  DListC<SArray1dC<EdgelC> > edges = det.EdgeArrayList();
  cout << "Edges 2:\n" << edges << endl;
  ImageC<bool> edgeImg = det.EdgeImg();
  UIntT tot(0);
  for (Array2dIterC<bool> i(edgeImg); i; ++i) {
    if (*i) ++tot;
  }
  cout << "tot: " << tot << endl;
  if (tot != 56) return __LINE__;
  //Save("@X", edgeImg);
  }
  return 0;
}


int testEdgeDet2() {
  // set up byte & real images with gradually increasing gradient
  ImageC<ByteT> imgB(20,20);
  for (IndexC r=0; r<imgB.Rows(); ++r) {
    if (r<imgB.Rows()/2) {
      for (IndexC c=0; c<imgB.Cols(); ++c)  imgB[r][c] = 128-c;
    }
    else if (r==imgB.Rows()/2) {
      for (IndexC c=0; c<imgB.Cols(); ++c)  imgB[r][c] = 128;
    }
    else {
      for (IndexC c=0; c<imgB.Cols(); ++c)  imgB[r][c] = 128+c;
    }
  }
  ImageC<RealT> imgR = ByteImageCT2DoubleImageCT(imgB);

  // run Deriche edge detector
  EdgeDetectorC det(true, 8, 4);

  cout << "Apply() methods in order, ignoring ones with edge map args (methods #1 & #8):\n";

  EdgeLinkC edgeMap;  
  if (!det.Apply(imgR, edgeMap)) return __LINE__;
  cout << "EdgeLinkC list size: " << edgeMap.ListEdges().Size()
       << "\n  uses #1\n";

  SArray1dC<EdgelC> edgeArray;
  if (!det.Apply(imgR, edgeArray)) return __LINE__;
  cout << "SArray1dC<EdgelC> size: " << edgeArray.Size()
       << "\n  uses #1\n";

  edgeArray = det.PApply(imgR);
  cout << "SArray1dC<EdgelC> size: " << edgeArray.Size()
       << "\n  uses #3\n";

  DListC<EdgelC> edgeList;
  if (!det.Apply(imgR, edgeList)) return __LINE__;
  cout << "DListC<EdgelC> size: " << edgeList.Size()
       << "\n  uses #1\n";

  edgeList = det.LApply(imgR);
  cout << "DListC<EdgelC> size: " << edgeList.Size()
       << "\n  uses #5\n";

  DListC<SArray1dC<EdgelC> > edgeArrayList;
  if (!det.Apply(imgR, edgeArrayList)) return __LINE__;
  if (edgeArrayList.Size() != 1) return __LINE__;
  cout << "DListC<SArray1dC<EdgelC> > size: " << edgeArrayList.First().Size()
       << "\n  uses #1\n";

  if (!det.Apply(imgB, edgeArrayList)) return __LINE__;
  if (edgeArrayList.Size() != 1) return __LINE__;
  if (edgeArrayList.First().Size() != 7) return __LINE__;
  for (IntT i=0; i<edgeArrayList.First().Size(); ++i) {
    if (edgeArrayList.First()[i].At()[0] != imgB.Rows()/2) return __LINE__;
    if (edgeArrayList.First()[i].At()[1] != i+11)  return __LINE__;
  }
  cout << "DListC<SArray1dC<EdgelC> > size: " << edgeArrayList.First().Size()
       << "\n  uses #8\n";

  DListC<DListC<EdgelC> > edgeListList;
  if (!det.Apply(imgB, edgeListList)) return __LINE__;
  if (edgeListList.Size() != 1) return __LINE__;
  cout << "DListC<DListC<EdgelC> > size: " << edgeListList.First().Size()
       << "\n  uses #8\n";
  return 0;
}


int testEdgeDet3() {
  // Check order of hysteresis threshold params is what it says on the tin (i.e. wrong way round).
  ImageC<RealT> im(10,10);
  im.Fill(0.0);
  for (IntT i=0; i<10; ++i) {
    im[4][i] = i;
    im[8][i] = i*2;
  }
  // Should just pick up row 8
  EdgeLinkC el = HysteresisThreshold(im,12,5);
  DListC<DListC<Index2dC> > edges = el.LinkEdges();
  cout << "\n\nStrongest edges: " << edges;
  if (edges.Size() != 1) return __LINE__;
  // Should  pick up rows 4 & 8
  el = HysteresisThreshold(im,7,3);
  edges = el.LinkEdges();
  cout << "More edges: " << edges;
  if (edges.Size() != 2) return __LINE__;

  return 0;
}


int testEdgeLink() {
  // set up edgel image
  ImageC<RealT> img(20,20);
  img.Fill(0.0);
  for (IntT i=5; i<=15; ++i)
    img[10][i] = (RealT)i;

  EdgeLinkC linkImg = HysteresisThreshold(img, 10.5, 12);
  cout << "LinkEdges:\n" << linkImg.LinkEdges() << endl;
  cout << "ListEdges:\n" << linkImg.ListEdges() << endl; // <<< BROKEN

  return 0;
}

