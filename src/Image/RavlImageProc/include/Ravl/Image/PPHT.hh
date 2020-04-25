// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_PPHT_HEADER
#define RAVLIMAGE_PPHT_HEADER 1
//////////////////////////////////////////////////////
//! file="Ravl/Image/Processing/Lines/PPHT.hh"
//! userlevel=Normal
//! docentry="Ravl.API.Images.Lines"
//! lib=RavlImageProc
//! author="Charles Galambos"
//! rcsid="$Id$"
//! date="07/01/1999"
//! example="doPPHT.cc"

#include "Ravl/Image/Image.hh"
#include "Ravl/StdConst.hh"
#include "Ravl/Curve2dLineSegment.hh"
#include "Ravl/Image/PCPixelList.hh"
#include "Ravl/DP/Port.hh"
#include "Ravl/Collection.hh"
#include "Ravl/Image/PixelMap.hh"
#include "Ravl/Image/Edgel.hh"
#include "Ravl/Image/PCMapping.hh"
#include "Ravl/Array1d.hh"

#define SHTDEBUG 1

namespace RavlImageN {

  //! userlevel=Develop
  //: Line segment extraction engine.
  
  class PPHoughTransformBodyC
    : public RCBodyVC
  {
  public:

    //:----
    //: Constructors

    PPHoughTransformBodyC();
    //: Default constructor
    // Parameters have default values as denoted in <code>Setxxx()</code> methods below

    PPHoughTransformBodyC(bool Directed, RealT AngleRes, RealT AngRange,
                          bool UseSobol, RealT MaxPTime, RealT FalsePos);
    //: Constructor

    PPHoughTransformBodyC(Point2dC nRes, RealT fp, RealT angRange,
                          RealT maxPTime, bool useSobol, RealT dummy,
                          bool fullDir);
    //: Deprecated constructor
    //!deprecated: some arguments make no sense

    //:----
    //: Resetting default parameters

    void SetDirectedLines(bool Directed) 
    { fullDir = Directed; RecomputeParams(); }
    //: Set range of orientations for lines
    // true: range is 360&deg; (i.e. lines have a notional head and tail)<br>
    // false: range is 180&deg; (the default)

    void SetAngleResolution(RealT AngleRes)
    { radsPerBin = AngleRes; RecomputeParams(); }
    //: Set angular bin size of Hough transform (in radians)
    // Default: 0.01 rads

    void SetEdgelAngleRange(RealT AngRange)
    { edgelAngRange = AngRange; RecomputeParams(); }
    //: Set range of orientations relative to line that edgels can have
    // Default: &pi;/2

    void UseSobol(bool Use)
    { useSobol = Use; }
    //: Use Sobol sampling
    // Default: false

    void SetMaxTime(RealT Secs)
    { maxTime = Secs; }
    //: Set maximum processing time (in seconds)
    // -ve values denote no limit (the default)

    void SetFalsePostiveTrheshold(RealT Threshold)
    { falsep = 1.0-Threshold; }
    //: Set threshold for false +ves
    // Default: 10<sup>-6</sup>
    

    //:----
    
    bool PPHTStep(Index2dC pix);
    //: Do a single PPHT cycle starting from a pixel.
    
    bool PPHTTest(const Curve2dLineC &seg,Point2dC at,IntT lineAngle,IntT rho,IntT lMinLen = 0,RealT threshData = -1);
    //: Test this line hypothesis.
    // if the hypothesis is good, extract the line and return true.
    // 'at' should be a point on the line. <p>
    // lineAngle, is the expected angle of the line. (in accumulator co-ords)
    // lMinLen must be greater than minLength to have any effect.
    
    bool PPHTTest(const Curve2dLineSegmentC &seg);
    //: Test this line hypothesis.
    // if the hypothesis is good, extract the line and return true.
    
    bool Setup(const DListC<EdgelC > &dat);
    //: Setup PPHT with the data.
    
    inline void SetCorridor(RealT width) { cWidth = width; }
    //: Set width of corridor.
    
    inline void SetMaxGap(IntT maxgap) { maxGap = maxgap; }
    //: Set maximum gap in line.
    
    inline void SetMinLineLength(IntT Len) { minLength = Len; }
    //: Minimum line length.
    
    void SetDynamicThresh(RealT val) { dynthresh = val; }
    //: Set Dynamic threshold.
    
    void SetFalseP(RealT fpth) { falsep = fpth; }
    //: Set false positive thres.
    
    void SetAngleRange(IntT range) { angRange = range; }
    //: 0-255. 0-90 degrees.
    
    IntT CalcThresh(IntT votes);
    //: Calculate threshold for accumulator.
    
    IntT CalcThresh2(IntT votes);
    //: Calculate threshold for accumulator.
    
    void SetGradEstNeigh(IntT size) { gradEstNeigh = size; }
    //: Gradient estimate neighhood size.
    
#if 0
    void SaveAccum(StringC filename);
    //: Save accumulator.
#endif
    
    void SetImage(const  ImageC<ByteT> &apixMap) { pixMap = apixMap; }
    //: Set input image.
    
    void SetFeedback(bool sfb)
    { feedBack = sfb; }
    //: Enabled feed back.
    
    PCPixMappingC<Curve2dLineSegmentC> &Lines() { return lines; }
    //: List of found lines.
    
    inline bool WriteGF(ostream &out);
    //: Write GF file.
    
    PCPixMappingC<Curve2dLineSegmentC> Apply(const DListC<EdgelC > &dat);
    //: Apply operation.
    
    PCPixMappingC<Curve2dLineSegmentC> SHT(const DListC<EdgelC > &dat);
    //: Perform SHT.
    
    virtual RCBodyVC &Copy() const;
    //: Creat a copy of this object.
        
    void SetEarlyOut(const DPOPortC<Curve2dLineSegmentC> &to) { earlyOut = to; }
    //: Set early output of lines.
    
    DPOPortC<Curve2dLineSegmentC> &EarlyOut() { return earlyOut; }
    //: Early output of lines.
    
    void SetMagSort(bool val)
    { useMagSort = val; }
    //: Set magmitude sort.
  
    void SetDoSHT(bool val)
    { doSHT = val; }
    //: Do SHT, used for performance comparisons.
    
  protected:  
    
    void RecomputeParams();
    // Recompute initial parameters

    CollectionC<Index2dC> SetupApply(const DListC<EdgelC > &dat);
    //: Setup everthing ready for processing.
    
    IntT FilterPoints( ImageC<ByteT> &inimg,PCPixelListC &pnts);
    // Filter out points that are already used.
    // inimg must have 1 in each place.
    // Return: number of points left.
    
    IntT GradFilterPoints(const  ImageC<ByteT> &img,PCPixelListC &pnts,RealT lineAngle);
    //: Gradient filter points.
    
    void TrimOutUsed(const  ImageC<ByteT> &img,PCPixelListC &pnts);
    // Trim out points that have already been used.
    
    void GradTrimOutUsed(const  ImageC<ByteT> &img,PCPixelListC &pnts,IntT angle);
    // Trim out points that have already been used, and those that
    // don't match the gradient constraints.
    
    inline IntT MaxAccAngle() const
    { return accum.Range1().Max().V(); }
    //: Get max position in accum.
    
    inline IntT MinAccAngle() const
    { return accum.Range1().Min().V(); }
    //: Get max position in accum.
    
    bool Vote(Index2dC pix,Point2dC &peak,IntT &thresh);
    // Vote for a pixel, return true if it exceeds the threshold.
    
    bool UnVote(Index2dC pix);
    // Remove votes for a pixel.
    
    bool Vote(Index2dC pix,Point2dC &peak,RealT grad,RealT rng,IntT &thresh);
    //: Vote for a pixel with an angle.
    // return true if it exceeds the threshold.  
    //   grad 1-254 == 0-180 Degree.
    //   rnd  0-255 == 0-90  Degree.
    
    bool UnVote(Index2dC pix,RealT grad,RealT rng);
    //: Remove votes for a pixel, with an angle.
    // grad 1-254 == 0-180 Degree.
    // rnd  0-255 == 0-90  Degree.
    
    IntT GetThresh(IntT votes);
    //: Calculate threshold for accumulator.
    
    void Precompute();
    //: Pre compute sin and cos tables.
    //: and thresholds.
    
  private:
    bool useMagSort;
    RealT cWidth; // Corridor width.
    IntT maxGap;
    IntT minLength;
    IntT tvotes; // Total number of votes currently in accumulator.
    IntT allvotes; // Total number of votes done till now.
    RealT dynthresh;
    IntT gradEstNeigh; // Gradient estimation neighberhood size.
    IntT p;
    RealT falsep;
    RealT edgelAngRange; // Range of edgel angles to consider.
    RealT angRange; // derived from edgelAngRange
    RealT maxTime;  // Maximum time to take in line extraction.
    bool doSHT; 
    IntT arrSize;
    Array2dC<IntT> accum;      // Accumulator.
    Array1dC<IntT> angleCount; // Number of votes in each column.
    Array1dC<RealT> tCos;      // Cos.
    Array1dC<RealT> tSin;      // Sin.
    ImageC<ByteT> usedMap;        // Map of used pixels.
    ImageC<ByteT> pixMap;         // Pixel map.
    ImageC<RealT> gradimg;      // Gradient image.
    ImageC<RealT> gradacc;      // Gradient accuracy image.
    ImageC<UIntT> sobelImg;        // Image of sobel numbers.
    PCPixMappingC<Curve2dLineSegmentC> lines; // List of found lines.
    bool feedBack;                   // true= Use feedback processing.
    PCPixMappingC<Curve2dLineSegmentC> lastLines; // List of found lines.
    PixelMapC pmap;  
    int total; // Total pixels in current input.
    IntT cachv;
    IntT cachr;
    bool useSobol; // Use sobol sampling.
    bool fullDir;  // Use full direction, not gradient.
    DPOPortC<Curve2dLineSegmentC> earlyOut; // Anyone want line segments early ?
    RealT radsPerBin;  // Radians per bin. r/nb
    bool firstTime; // flag for precomputing trig funcs etc. only once
    
    SArray1dC<IntT> threshTab;
  };

  
  //////////////////////////////////
  
  inline 
  bool PPHoughTransformBodyC::WriteGF(ostream &out) { 
    lines.WriteGF(out);
    return true;
  }

  //! userlevel=Normal
  //: Progressive Probablistic Hough Transform.

  // <p>This algorithm extracts line segments from a set of edges, using the
  // algorithm described in <a href=
  // "http://www.sciencedirect.com/science/journal/10773142"> "Robust Detection
  // of Lines Using the Progressive Probabilistic Hough Transform", CVIU v.78-1, April 2000,
  // 119-137, J. Matas, C. Galambos, J. Kittler</a>.</p>

  // <p><b>N.B.:</b> In this algorithm, the accuracy of the line parameters is not limited by the resolution parameters (unlike a conventional Hough transform); instead these parameters determine how similar two lines can be and still be resolved.</p>
  
  class PPHoughTransformC
    : public RCHandleC<PPHoughTransformBodyC>
  {
  public:

    //:----
    //: Constructors

    PPHoughTransformC()
      : RCHandleC<PPHoughTransformBodyC>(*new PPHoughTransformBodyC())
    {}
    //: Default constructor.
    // Generates non-directed lines; otherwise default parameter values are as listed in the following constructor
    
    PPHoughTransformC(bool Directed, RealT AngleRes=0.01,RealT AngRange=RavlConstN::pi/2.0,
                      bool UseSobol=false, RealT MaxPTime=-1, RealT FalsePos=1.0e-6)
      : RCHandleC<PPHoughTransformBodyC>(*new PPHoughTransformBodyC(Directed,AngleRes,AngRange,UseSobol,MaxPTime,FalsePos))
    {}
    //: Constructor
    //!param: Directed - true: Directed lines are created (i.e. lines have a notional head and tail, and orientation can be in full is 360&deg; range).<br> false: Undirected lines are created (i.e. orientation is ambiguous modulo 180&deg;).
    //!param: AngleRes - Hough resolution in angular direction (in radians)
    //!param: AngRange - Range of orientations relative to line that edgels can have (in radians).
    //!param: UseSobol - Use Sobol sampling.
    //!param: MaxPTime - Stop processing after maxPTime seconds (-1 denotes no bound on time)
    //!param: Fp - False +ve threshold.

    //:----
    //: Resetting default and/or constructor parameters

    void SetDirectedLines(bool Directed) 
    { Body().SetDirectedLines(Directed); }
    //: Set range of orientations for lines
    // true: Directed lines are created (i.e. lines have a notional head and tail, and orientation can be in full is 360&deg; range).<br>
    // false (the default): Undirected lines are created (i.e. orientation is ambiguous modulo 180&deg;)

    void SetAngleResolution(RealT AngleRes)
    { Body().SetAngleResolution(AngleRes); }
    //: Set Hough resolution in angular direction (in radians)
    // Default: 0.01 rads

    void SetEdgelAngleRange(RealT AngRange)
    { Body().SetEdgelAngleRange(AngRange); }
    //: Set range of orientations relative to line that edgels can have (in radians)
    // Default: &pi;/2

    void UseSobol(bool Use)
    { Body().UseSobol(Use); }
    //: Use Sobol sampling
    // Default: false

    void SetMaxTime(RealT Secs)
    { Body().SetMaxTime(Secs); }
    //: Set maximum processing time (in seconds)
    // -ve values denote no limit (the default)

    void SetFalsePostiveTrheshold(RealT Threshold)
    { Body().SetFalsePostiveTrheshold(Threshold); }
    //: Set threshold for false +ves
    // Default: 10<sup>-6</sup>
    

    //:----
    //: Applying the transform

    PCPixMappingC<Curve2dLineSegmentC> Apply(const DListC<EdgelC > &dat)
    { return Body().Apply(dat); }
    //: Perform a probabilistic Hough transform.
    
    PCPixMappingC<Curve2dLineSegmentC> SHT(const DListC<EdgelC > &dat)
    { return Body().SHT(dat); }
    //: Perform a standard Hough Transform.

    //:----
    //: Other parameter setting
    
    void SetCorridor(RealT width)
    { Body().SetCorridor(width); }
    //: Set width of corridor.
    
    void SetMaxGap(IntT maxgap)
    { Body().SetMaxGap(maxgap); }
    //: Set maximum gap in line.
    
    void SetMinLineLength(IntT len)
    { Body().SetMinLineLength(len); }
    //: Minimum line length.
    
    void SetMagSort(bool val)
    { Body().SetMagSort(val); }
    //: Set magnitude sort flag.
    
    void SetDoSHT(bool sht)
    { Body().SetDoSHT(sht); }
    //: Do sht instread
    
  protected:
    
    PPHoughTransformBodyC &Body()
    { return RCHandleC<PPHoughTransformBodyC>::Body(); }
    //: Access body.
    
    const PPHoughTransformBodyC &Body() const
    { return RCHandleC<PPHoughTransformBodyC>::Body(); }
    //: Access body.
    
  public:

    PPHoughTransformC(Point2dC Res,RealT Fp = 0.999999,RealT AngRange = 90,RealT MaxPTime = -1,bool UseSobol = false,RealT dummy = 1,bool FullDir = false)
      : RCHandleC<PPHoughTransformBodyC>(*new PPHoughTransformBodyC(Res,Fp,AngRange,MaxPTime,UseSobol,dummy,FullDir))
    {}
    //: Constructor.
    //!deprecated: some arguments make no sense

    
  };

}

#endif
