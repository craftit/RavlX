// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_SEGMENTATION_HEADER
#define RAVLIMAGE_SEGMENTATION_HEADER 1
/////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Images.Segmentation"
//! lib=RavlImageProc
//! file="Ravl/Image/Processing/Segmentation/Segmentation.hh"
//! author="Charles Galambos"

#include "Ravl/Array2dSqr2Iter2.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/Image/ByteYUVValue.hh"
#include "Ravl/SArray1d.hh"
#include "Ravl/RefCounter.hh"
#include "Ravl/HSet.hh"
#include "Ravl/IntC.hh"
#include "Ravl/Tuple2.hh"
#include "Ravl/Moments2d2.hh"

namespace RavlImageN {
  
  //! userlevel=Develop
  //: Segmentation map.
  
  class SegmentationBodyC 
    : public RCBodyVC
  {
  public:
    SegmentationBodyC()
      : labels(0)
    {}
    //: Default constructor.
    
    SegmentationBodyC(ImageC<UIntT> nsegmap,UIntT nlabels)
      : segmap(nsegmap),
	labels(nlabels)
    {}
    //: Constructor.
    
    SegmentationBodyC(const ImageC<IntT> &nsegmap);
    //: Construct from an IntT image.
    // Negative values will be labeled as region 0.
    
    SArray1dC<HSetC<UIntT> > Adjacency(bool biDir = false);
    //: Generate a table of 4 connected region adjacencies.
    // For each region, a set of adjacent regions is 
    // generated.   If biDir is false, only adjacenies from
    // regions with a smaller id to those with a larger ID are
    // generated, otherwise both direction are registered. 
    
    SArray1dC<HashC<UIntT,UIntC> > BoundaryLength(bool biDir = false);
    //: Generate a table of boundary lengths with 4 connected adjacent regions
    // if biDir is false only adjacenies from regions with a smaller id to those 
    // with a larger ID are generated
    
    SArray1dC<UIntT> LocalBoundaryLength();
    //: Generate a table of the length of the 4 connected boundary for each region
    // Note, boundary pixels at the edge of the image are NOT counted.
    
    UIntT RemoveSmallComponents(IntT thrSize);
    //: Remove small components from map, label them as 0.
    
    UIntT CompressAndRelabel(SArray1dC<UIntT> &newLabs);
    //: Compress newlabs and re-label segmentation.
    // this correctly resolves multilevel mappings.
    // Note: newLabs will be changed to contain a mapping
    // from the original labels to their new values.
    
    SArray1dC<Tuple2C<IndexRange2dC,UIntT> > BoundsAndArea() const;
    //: Compute the bounding box and area of each region in the segmentation.
    
    SArray1dC<UIntT> Areas() const;
    //: Compute the areas of all the segmented regions.
    
    SArray1dC<UIntT> RedoArea(SArray1dC<UIntT> area,SArray1dC<UIntT> map);
    //: recompute the areas from the original areas and a mapping.
    
    SArray1dC<UIntT> IdentityLabel();  
    //: Make an array of labels mapping to themselves.
    // This is useful for making merge tables which can
    // the be passed to CompressAndRelabel.
    
    template<class PixelT,class CmpT>
    UIntT MergeComponents(ImageC<PixelT> &dat,UIntT thrSize,RealT maxDist,CmpT &cmp,IntT iter = 1);
    //: Merge simlar components smaller than 'thrSize'.
    // This just looks for the difference between adjacent pixels from different regions.
    // FIXME :- It maybe better to look at the average difference.
    
    ImageC<UIntT> &SegMap()
    { return segmap; }
    //: Access segmentation map.
    
    UIntT Labels()
    { return labels; }
    //: Access number of labels.
    
    SArray1dC<Moments2d2C> ComputeMoments(bool ignoreZero = false);
    //: Compute moments for each of the segmented regions.
    // if ignoreZero is true, region labeled 0 is ignored.
    
    ImageC<ByteT> ByteImage() const;
    //: Returns the segmentation map in the form of a ByteImageC
    // Note: if there are more than 255 labels in the image, some may be used twice.
    
    ImageC<ByteRGBValueC> RandomImage() const;
    //: Returns the segmentation map in the form of a colour random image; this means that segmentation maps with more than 255 labels can be saved to disk
    
    ImageC<ByteYUVValueC> RandomTaintImage(ByteT max=100) const;
    //: Returns the segmentation map in the form of a colour random image.
    // The Y channel is left blank (e.g., for displaying the original data).
    // The labels in the U and V channels are in the range 0 to 'max'.
    
  protected:
    UIntT RelabelTable(SArray1dC<UIntT> &labelTable, UIntT currentMaxLabel);
    //: Compress labels.
    
    ImageC<UIntT> segmap; // Segmentation map.
    UIntT labels;         // Number of labels in map.
  };
  
  //! userlevel=Normal
  //: Segmentation map.
  // Provides operations on an (existing) segmentation map in which each segment is labelled with a unique identifier
  
  class SegmentationC 
    : public RCHandleC<SegmentationBodyC>
  {
  public:
    SegmentationC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    SegmentationC(ImageC<UIntT> nsegmap,UIntT nlabels)
      : RCHandleC<SegmentationBodyC>(*new SegmentationBodyC(nsegmap,nlabels))
    {}
    //: Constructor.
    
    SegmentationC(const Tuple2C<ImageC<UIntT>,UIntT> &tup)
      : RCHandleC<SegmentationBodyC>(*new SegmentationBodyC(tup.Data1(),tup.Data2()))
    {}
    //: Constructor, using the connected components image generated by ConnectedComponentsC.
    
    SegmentationC(const ImageC<IntT> &nsegmap)
      : RCHandleC<SegmentationBodyC>(*new SegmentationBodyC(nsegmap))
    {}      
    //: Construct from an IntT image.
    // Negative values will be labeled as region 0.
    
  protected:
    SegmentationC(SegmentationBodyC &bod)
      : RCHandleC<SegmentationBodyC>(bod)
    {}
    //: Body constructor.
    
    SegmentationBodyC &Body()
    { return RCHandleC<SegmentationBodyC>::Body(); }
    //: Access body.
    
    const SegmentationBodyC &Body() const
    { return RCHandleC<SegmentationBodyC>::Body(); }
    //: Access body.
    
  public:
    
    SArray1dC<HSetC<UIntT> > Adjacency(bool biDir = true)
    { return Body().Adjacency(biDir); }
    //: Generate a table of region adjacencies.
    // For each region, a set of adjacent regions is 
    // generated.   If biDir is false, only adjacenies in
    // one direction are generated, otherwise both directions
    // are registered.
    
    SArray1dC<HashC<UIntT,UIntC> > BoundaryLength()
    { return Body().BoundaryLength(); }
    //: Generate a table of region adjacencies boundary lengths.
    // only adjacenies from regions with a smaller id to those 
    // with a larger ID are generated
    
    UIntT RemoveSmallComponents(IntT thrSize)
    { return Body().RemoveSmallComponents(thrSize); }
    //: Remove small components from map, label them as 0.
    // Returns the number of labels in the new segmentation.
    
    SArray1dC<UIntT> Areas() const
    { return Body().Areas(); }
    //: Compute the areas of all the segmented regions.
    
    SArray1dC<Tuple2C<IndexRange2dC,UIntT> > BoundsAndArea() const
    { return Body().BoundsAndArea(); }
    //: Compute the bounding box and area of each region in the segmentation.
    
    template<class PixelT,class CmpT>
    UIntT MergeComponents(ImageC<PixelT> &dat,UIntT thrSize,RealT maxDist,CmpT &cmp,IntT iter = 1)
    { return Body().MergeComponents(dat,thrSize,maxDist,cmp,iter); }
    //: Merge regions smaller than <code>thrSize</code> into neighbouring regions of sufficiently similar value
    // It compares pixel pairs across the boundary to establish the similarity of the regions, comparing the value returned by <code>cmp</code> with <code>maxDist</code>.<br>
    // It does not detect if all suitable candidates have been merged; it just goes through the process <code>iter</code> times.<br>
    // The argument <code>cmp</code> should be an instance of a simple user-specified class containing the <code>operator()</code> method, as in this example:
    //<pre>
    //class CmpPixelsC { 
    //  public:
    //    RealT operator() (const RealRGBValueC &arg1,const RealRGBValueC &arg2) {
    //      return arg1.SqrEuclidDistance(arg2);
    //    }
    //};</pre>
    
    ImageC<UIntT> &SegMap()
    { return Body().SegMap(); }
    //: Access segmentation map.
    
    UIntT Labels()
    { return Body().Labels(); }
    //: Access number of labels in the segmentation.
    
    SArray1dC<Moments2d2C> ComputeMoments(bool ignoreZero = false)
    { return Body().ComputeMoments(ignoreZero); }
    //: Compute moments for each of the segmented regions.
    // if ignoreZero is true, region labeled 0 is ignored.

    inline ImageC<ByteT> ByteImage() const
    { return Body().ByteImage(); }
    //: Returns the segmentation map in the form of a ImageC<ByteT>
    
    inline  ImageC<ByteRGBValueC> RandomImage() const
    { return Body().RandomImage(); }
    //: Returns the segmentation map in the form of a colour random image
    
    inline  ImageC<ByteYUVValueC> RandomTaintImage(ByteT max=100) const
    { return Body().RandomTaintImage(max); }
    //: Returns the segmentation map in the form of a colour random image. 
    // The Y channel is left blank (e.g., for displaying the original data). 
    // The labels in the U and V channels are in the range 0 to 'max'.
  };

  //: Merge similar components smaller than 'thrSize'.
  
  template<class PixelT,class CmpT>
  UIntT SegmentationBodyC::MergeComponents(ImageC<PixelT> &dat,
					   UIntT thrSize,
					   RealT maxDist,
					   CmpT &cmp,
					   IntT iter) 
  {
    if(labels <= 1)
      return labels;
    SArray1dC<RealT> minDist(labels);  
    SArray1dC<UIntT> area = Areas();
    
    for(;iter > 0;iter--) {
      minDist.Fill(maxDist); // Fill with maximum merge threshold./
      SArray1dC<UIntT> minLab =  IdentityLabel();
      
      // Find closest neigbour of small regions.
      
      for(Array2dSqr2Iter2C<UIntT,PixelT> it(segmap,dat);it;) {
	// Do up.
	if(it.DataBR1() != it.DataTR1()) { // Are labels different ?
	  if(area[it.DataBR1()] < thrSize) {
	    RealT d = cmp(it.DataBR2(),it.DataTR2());
	    if(minDist[it.DataBR1()] > d) {
	      minDist[it.DataBR1()] = d;
	      minLab[it.DataBR1()] = it.DataTR1();
	    }
	  }
	  if(area[it.DataTR1()] < thrSize) {
	    RealT d = cmp(it.DataBR2(),it.DataTR2());
	    if(minDist[it.DataTR1()] > d) {
	      minDist[it.DataTR1()] = d;
	      minLab[it.DataTR1()] = it.DataBR1();
	    }
	  }
	}
	
	for(;it.Next();) { // The rest of the image row.
	  // Do up.
	  if(it.DataBR1() != it.DataTR1()) {  // Are labels different ?
	    if(area[it.DataBR1()] < thrSize) {
	      if(it.DataBR1() != it.DataTR1()) {
		RealT d = cmp(it.DataBR2(),it.DataTR2());
		if(minDist[it.DataBR1()] > d) {
		  minDist[it.DataBR1()] = d;
		  minLab[it.DataBR1()] = it.DataTR1();
		}
	    }
	    }
	    if(area[it.DataTR1()] < thrSize) { 
	      RealT d = cmp(it.DataBR2(),it.DataTR2());
	      if(minDist[it.DataTR1()] > d) {
		minDist[it.DataTR1()] = d;
		minLab[it.DataTR1()] = it.DataBR1();
	      }
	    }
	  }
	  
	  // Do back.
	  if(it.DataBR1() != it.DataBL1()) { // Are labels different ?
	    if(area[it.DataBR1()] < thrSize) { 
	      RealT d = cmp(it.DataBR2(),it.DataBL2());
	      if(minDist[it.DataBR1()] > d) {
		minDist[it.DataBR1()] = d;
		minLab[it.DataBR1()] = it.DataBL1();
	      }
	    }
	    if(area[it.DataBL1()] < thrSize) {
	      RealT d = cmp(it.DataBR2(),it.DataBL2());
	      if(minDist[it.DataBL1()] > d) {
		minDist[it.DataBL1()] = d;
		minLab[it.DataBL1()] = it.DataBR1();
	      }
	    }
	  }
	}
      }
      CompressAndRelabel(minLab);
      if(iter > 1)
	area = RedoArea(area,minLab);
    }
    return labels;
  }
}
#endif
