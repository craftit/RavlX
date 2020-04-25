// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! file="Ravl/Image/Processing/Tracking/PointTracker.cc"
//! lib=RavlImageProc

#define RAVL_USE_CORRELATION 1

#include "Ravl/Image/PointTracker.hh"
#include "Ravl/Array2dIter2.hh"
#include "Ravl/Image/Matching.hh"
#include "Ravl/IO.hh"
#include "Ravl/SquareIter.hh"
#include "Ravl/Image/PeakDetector.hh"
#include "Ravl/Image/CornerDetectorSusan.hh"
#include "Ravl/Image/CornerDetectorHarris.hh"
#include "Ravl/Image/MatchPatch.hh"

#include "Ravl/Image/MatchNormalisedCorrelation.hh"

#include "Ravl/Sums1d2.hh"

#define DODEBUG 0
#if DODEBUG 
#include "Ravl/Image/DrawCross.hh"
#include "Ravl/Image/DrawFrame.hh"
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlImageN {
  
  
  //: Constructor.
  
  PointTrackerC::PointTrackerC(int cthreshold,int cwidth,RealT nmthreshold,int nmwidth,int nlifeTime,int nsearchSize,int nnewFreq,bool nUseNormCorrelation)
    : idAlloc(1),
      newFreq(nnewFreq),
      frameCount(1),
      //cornerDet(CornerDetectorHarrisC(cthreshold,cwidth)),
      cornerDet(CornerDetectorSusanC(cthreshold)),
      mwidth(nmwidth),
      mthreshold(nmthreshold),
      lifeTime(nlifeTime),
      searchSize(nsearchSize),
      useNormCorrelation(nUseNormCorrelation)
  {}
  
  
  //: Returns a list of tracks.
  
  RCHashC<UIntT,PointTrackC> PointTrackerC::Apply(const ImageC<ByteT> &img) {
    Update(img);
    return GetTracks();
  }
  
  //: Create a list of current tracks.
  
  RCHashC<UIntT,PointTrackC> PointTrackerC::GetTracks() const {
    RCHashC<UIntT,PointTrackC> ret;
    for(DLIterC<PointTrackModelC> itt(tracks);itt;itt++) {
      if(itt->Frame() != frameCount)
	ret[itt->ID()] = PointTrackC(itt->ID(),itt->Location(),0);
      else
	ret[itt->ID()] = PointTrackC(itt->ID(),itt->Location(),itt->Confidence());
    }
    return ret;    
  }
  
  //: Init tracks
  void PointTrackerC::AddTracks(const ImageC<ByteT> &img, ImageC<ByteT>* debugimg) {
    
    // Detect corners
    DListC<CornerC> cl;
    cl = cornerDet.Apply(img);
    
    // Drop corners that are too close to existing tracks
    for(DLIterC<PointTrackModelC> itt(tracks);itt;itt++) {
      for(DLIterC<CornerC> it(cl);it;it++) {
	// Is corner too close to one already being tracked ? 
	Point2dC cat = it->Location();
	if(itt->Location().SqrEuclidDistance(cat) < Sqr(mwidth)) {
	  ONDEBUG(DrawFrame(*debugimg,(ByteT) 0,IndexRange2dC(it->Location(),mwidth,mwidth)));
	  it.Del(); // Remove it.
	}
      }
    }
    
    // Make tracks for good corners
    RealT removeThresh = Sqr(mwidth) * mthreshold;
    for(DLIterC<CornerC> it(cl);it;it++) {
      IndexRange2dC fr(it->Location(),mwidth,mwidth);
      if(!img.Frame().Contains(fr))
         continue;
      Array2dC<ByteT> templ(img,fr,Index2dC(-mwidth/2,-mwidth/2));
      
#if 0
      // Check variance of window.
      Sums1d2C tsums;
      for(Array2dIterC<ByteT> sit(templ);sit;sit++)
	tsums += *sit;
      RealT var = tsums.Variance();
      std::cerr << "Patch var=" << var << "\n";
      if(var < 1000)
	continue;
#endif
      tracks.InsLast(PointTrackModelC(idAlloc++,it->Location(),frameCount,templ,removeThresh));
      ONDEBUG(DrawFrame(*debugimg,(ByteT) 255,IndexRange2dC(it->Location(),mwidth,mwidth)));
    }
  }
  
  //: Update tracks.
  IntT PointTrackerC::Update(const ImageC<ByteT> &img) {
    ONDEBUG(ImageC<ByteT> timg(img.Copy()));
    //ONDEBUG(ImageC<ByteT> timg(img.Frame()); timg.Fill(0));
    
    MatchNormalisedCorrelationC normCorr;
    RealT removeThresh;
    if(useNormCorrelation) {
      normCorr = MatchNormalisedCorrelationC(img);
      removeThresh = mthreshold;
    } else {
      removeThresh = Sqr(mwidth) * mthreshold;
    }
    
    frameCount++; 
    for(DLIterC<PointTrackModelC> itt(tracks);itt;itt++) {
      if(!itt->IsLive()) { // Has something else flagged the point as unneeded ?
	itt.Del();
	continue;
      }
      Point2dC lookAtp =itt->EstimateLocation(frameCount);
      Index2dC lookAt(Round(lookAtp[0]),Round(lookAtp[1]));
      //cerr << "Vel=" << itt->Velocity() << "\n";
      if(!img.Frame().Contains(lookAt)) { // Has point left the image ?
	itt->SetLive(false);
	itt.Del();
	continue;
      }
      Point2dC at;
      IntT score = 100000000;
      if(useNormCorrelation) {
	IndexRange2dC searchArea(lookAt,searchSize,searchSize);
	searchArea.ClipBy(img.Frame());
#if 0
	SearchMinAbsDifference(itt->Template(),img,searchArea,at,score);
#else
	Index2dC iat;
	RealT rscore;
	normCorr.Search(itt->Template(),searchArea,rscore,iat);
	//cerr << rscore << "  " << (1.0/rscore) << "\n";
	score = (IntT) (1.0/rscore);
	at = iat;
#endif
      } else {
	SearchMinAbsDifferenceCreep(itt->Template(),img,lookAt,at,score,searchSize);
      }
      
      //cerr <<"Score=" << score << " At=" << at << " Thresh=" << removeThresh << "\n";
      if(score > removeThresh) {
	// Lost track of the object.
	if(itt->Frame() < (frameCount - lifeTime)) {
	  itt->SetLive(false);
	  itt.Del();  // Haven't seen it for a while, it can go!
	}
	ONDEBUG(DrawCross(timg,(ByteT) 0,itt->Location()));
	continue;
      }
      itt->Update(at,frameCount,score);
      ONDEBUG(DrawCross(timg,(ByteT) 255,at));
    }
    RavlAssert(newFreq != 0);
    // If we're on the right frame, add new tracks
    if((frameCount-1) % newFreq == 0)
#if DODEBUG
      AddTracks(img,&timg);
#else
      AddTracks(img,NULL);
#endif
    
    ONDEBUG(Save("@X:Track",timg));
    return 1;
  }


  
}
