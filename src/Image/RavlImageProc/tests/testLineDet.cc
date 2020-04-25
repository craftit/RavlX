// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlImageProc
//! file="Ravl/Image/Processing/Lines/testLineDet.cc"

#include "Ravl/Image/PPHT.hh"
#include "Ravl/Polygon2d.hh"
#include "Ravl/Image/DrawPolygon.hh"
#include "Ravl/Image/EdgeDetector.hh"
#include "Ravl/IO.hh"
#include "Ravl/CDLIter.hh"
#include "Ravl/Angle.hh"

using namespace RavlImageN;
using namespace RavlConstN;

int testLineSegment();
int testPPHT();


int main(int nargs,char **argv) {
  int ln;
  if((ln = testLineSegment()) != 0) {
    cerr << "test failed on line " << ln << "\n";
    return 1;
  }
  
  if((ln = testPPHT()) != 0) {
    cerr << "test failed on line " << ln << "\n";
    return 1;
  }
  
  return 0;
}

int testLineSegment() {
  SArray1dC<Point2dC> points(20);
  for(int i = 0;i < (int) points.Size();i++) {
    points[i] = Point2dC((RealT) 100 - i,(RealT) 10.0 + i *0.5);
  }
  Curve2dLineSegmentC aline(points); // Fit line to remaining points.

  cerr << "Normal=" << aline.Normal() << "\n";
  cerr << "Start=" << aline.StartPnt() << " End=" << aline.EndPnt() << "\n";
  cerr << "Real =" << points[0] << " End=" << points[points.Size()-1] << "\n";
  
  if(aline.StartPnt().SqrEuclidDistance(points[0]) > 0.0001)   return __LINE__;
  if(aline.EndPnt().SqrEuclidDistance(points[points.Size()-1]) > 0.0001)   return __LINE__;
  return 0;
}


#define ANGDIFF(x,y,j) Abs(AngleC(x.Angle(), (j?2.0:1.0)*pi).Diff(AngleC(y.Angle(), (j?2.0:1.0)*pi)))

int testPPHT() {
  
  // create image containing solid irregular polygon
  ImageC<RealT> img(100,100);
  img.Fill(0.0);
  DListC<Point2dC> pts;
  ((((((pts += Point2dC(20,20)) += Point2dC(30,30)) += Point2dC(20,50))
     += Point2dC(40,60)) += Point2dC(70,40)) += Point2dC(55,15));
  Polygon2dC poly(pts);
  DrawPolygon(img, 255.0, poly, true);
  // create set of line segments for polygon
  SArray1dC<LinePP2dC> polyLines(pts.Size());
  IntT i=0;
  for (ConstDLIterC<Point2dC> p(pts); p; ++p) {
    polyLines[i++] = LinePP2dC(*p, p.NextCrcData());
  }
  //cerr << polyLines;

  // find edges using PPHT
  EdgeDetectorC edgeDet(true);
  DListC<EdgelC> inList = edgeDet.LApply(img);
  for (RealT a=0.001; a<0.5; a*=10)  for (IntT j=0; j<=1; ++j) {
    PPHoughTransformC pphtProc;
    pphtProc.SetDirectedLines((bool)j);
    pphtProc.SetAngleResolution(a);
    PCPixMappingC<Curve2dLineSegmentC> ht = pphtProc.Apply(inList);
    if (ht.Size() != pts.Size())  return __LINE__;
    
    // find match between PPHT lines and polylines
    DListC<LinePP2dC> lines;
    for (DLIterC<PCPixMapC<Curve2dLineSegmentC> >  it(ht);it;it++) {
      LinePP2dC line(it->Curve().StartPnt(),it->Curve().EndPnt());
      //cerr << line << endl;
      bool found(false);
      for (IntT i=0; i<polyLines.Size(); ++i) {
        if ((ANGDIFF(polyLines[i],line,j) < 0.04) &&
            (polyLines[i].Distance(line.MidPoint()) < 1.0) &&
            (Abs(polyLines[i].Length() - line.Length()) < 4.0)) {
          found = true;
          /*
          cerr << "Found match for hough line " << line << " with polygon line\n"
               << "a = " << a << "; j = " << j
               << "\n "<<ANGDIFF(polyLines[i],line,j) <<", "
               << polyLines[i].Distance(line.MidPoint()) <<", "
               << polyLines[i].Length() <<" " << line.Length() << endl;
          */
          break;
        }
      }
      if (!found) {
        cerr << "Could not match hough line " << line << " with polygon lines\n"
             << "a = " << a << "; j = " << j << endl;
        for (IntT i=0; i<polyLines.Size(); ++i) {
          cout << " "<<ANGDIFF(polyLines[i],line,j) <<", "
               << polyLines[i].Distance(line.MidPoint()) <<", "
               << polyLines[i].Length() <<" " << line.Length() << endl;
        }
        return __LINE__;
      }
    }
  }

  return 0;
}

