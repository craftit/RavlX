// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2005, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_AAMSAMPLESTREAM_HEADER
#define RAVLIMAGE_AAMSAMPLESTREAM_HEADER 1
//! rcsid="$Id$"
//! lib=RavlAAM
//! docentry="Ravl.API.Images.AAM"
//! author="Charles Galambos"
//! file="Ravl/CompVision/ActiveAppearanceModels/AAMSampleStream.hh"
//! example = aamBuildActiveAppearanceModel.cc

#include "Ravl/DP/SPort.hh"
#include "Ravl/Vector.hh"
#include "Ravl/Tuple2.hh"
#include "Ravl/Image/AAMActiveAppearanceModel.hh"
#include "Ravl/PatternRec/SampleVector.hh"
#include "Ravl/PatternRec/SampleIter.hh"
#include "Ravl/PatternRec/SampleStream.hh"
#include "Ravl/Image/GaussConvolve2d.hh"
#include "Ravl/Image/AAMAppearanceUtil.hh"
#include "Ravl/String.hh"
#include "Ravl/DList.hh"
#include "Ravl/DLIter.hh"

namespace RavlImageN {

  using namespace RavlN;
  using namespace RavlImageN;

  //! userlevel=Develop
  //: AAM Sample body.

  class AAMSampleStreamBodyC
    : public DPISPortBodyC<Tuple2C<VectorC,VectorC> >
  {
  public:
    AAMSampleStreamBodyC(const AAMAppearanceModelC &nam,
                         const GaussConvolve2dC<RealT> &smooth,
                         const DListC<StringC> &fileList,
                         const StringC &dir,
                         const StringC &mirrorFile,
                         const UIntT incrSize,
                         bool ignoreSuspect = true
                         );
    //: Constructor.

    AAMSampleStreamBodyC(const AAMAppearanceModelC &am,
                         const GaussConvolve2dC<RealT> &smooth,
                         SampleStreamC<AAMAppearanceC> &sample,
                         const UIntT incrSize);
    //: Constructor from sample stream

    virtual bool Seek(UIntT off);
    //: Seek to location in stream.

    virtual UIntT Size() const;
    //: Find the total size of the stream. (assuming it starts from 0)

    virtual Tuple2C<VectorC,VectorC> Get();
    //: Get next piece of data.

    virtual bool Get(Tuple2C<VectorC,VectorC> &buff);
    //: Try and get next piece of data.

    virtual bool IsGetEOS() const;
    //: Has the End Of Stream been reached?
    // true = yes.

  protected:
    AAMAppearanceModelC m_am;  // Statistical model of appearance.
    GaussConvolve2dC<RealT> m_smooth;  // Gaussian convolver for smoothing.
    SampleStreamC<AAMAppearanceC> m_sample;  // Stream of appearance samples
    ImageC<RealT> m_image;  // Current image frame.
    VectorC m_trueVec;  // True parameter vector.
    VectorC m_deltaVec;  // Displacement on parameter vector.
    UIntT m_samplesPerFrame;  // Number of displacements per appearance.
    UIntT m_sampleNo;  // Index of displacement for the current appearance.
    UIntT m_incrSize;  // Half number of displacements for each parameter when perturbing the model.
    bool m_done;  // Have all files been processed?
    HashC<IntT,IntT> m_typeMap;
    HashC<StringC,IntT> m_namedTypeMap;
    bool m_useTypeId;
  };

  //! userlevel=Advanced
  //: AAM Sample.

  class AAMSampleStreamC
    : public SampleStream2C<VectorC,VectorC >
  {
  public:
    AAMSampleStreamC(const AAMAppearanceModelC &am,
                     const GaussConvolve2dC<RealT> &smooth,
                     const DListC<StringC> &fileList,
                     const StringC &dir,
                     const StringC &mirrorFile,
                     const UIntT incrSize,
                     bool ignoreSuspect = true
                     )
      : DPEntityC(*new AAMSampleStreamBodyC(am,smooth,fileList,dir,mirrorFile,incrSize,ignoreSuspect))
    {}
    //: Default constructor.
    // Creates an invalid handle.

    AAMSampleStreamC(const AAMAppearanceModelC &am,
                     const GaussConvolve2dC<RealT> &smooth,
                     SampleStreamC<AAMAppearanceC> &sample,
                     const UIntT incrSize)
      : DPEntityC(*new AAMSampleStreamBodyC(am, smooth, sample, incrSize))
    {}
    //: Constructor from sample stream

  };

}



#endif
