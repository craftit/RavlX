// Copyright (C) 2003, Omniperception Ltd.
// file-header-ends-here
#ifndef RAVLN_FACEN_FaceInfo_HEADER
#define RAVLN_FACEN_FaceInfo_HEADER 1
//! rcsid="$Id$"
//! lib=RavlFace
//! docentry="Ravl.API.Applications.Image.Face"
//! file="Contrib/Face/Xml/FaceInfo.hh"
//! author="Kieron J Messer"

#include "Ravl/RefCounter.hh"
#include "Ravl/XMLStream.hh"
#include "Ravl/String.hh"
#include "Ravl/Image/ImagePointFeatureSet.hh"
#include "Ravl/OS/Date.hh"

namespace RavlN {
  namespace FaceN {

    using RavlImageN::ImagePointFeatureSetC;
    using RavlImageN::ImagePointFeatureC;

    //! userlevel=Develop
    //: Information about a face for XML

    class FaceInfoBodyC : public RCBodyC
    {

    public:
      FaceInfoBodyC()
       : m_detectionScore(0)
      {
      }

      FaceInfoBodyC(const StringC & actualId, const StringC & faceId, const StringC & origImage);
      //: Construct from actual id and path to original image

    protected:

      FaceInfoBodyC(XMLIStreamC& xml);
      //: Constructor from XML stream

      XMLOStreamC& SaveXML(XMLOStreamC &xml) const;
      //: Save to xml stream

    public:

      const StringC & ActualId(void) const
      {
        return m_actualId;
      }
      //: The actual id of client

      StringC & ActualId(void)
      {
        return m_actualId;
      }
      //: Access the actual id of client

      const StringC & FaceId(void) const
      {
        return m_faceId;
      }
      //: The actual id of client

      StringC & FaceId(void)
      {
        return m_faceId;
      }
      //: Access the actual id of client

      const StringC & OrigImage(void) const
      {
        return m_origImage;
      }
      //: The path to the original image

      StringC & OrigImage(void)
      {
        return m_origImage;
      }
      //: Access the path to the original image

      const ImagePointFeatureSetC &FeatureSet(void) const
      {
        return m_featureSet;
      }
      //: The feature set on the original image

      ImagePointFeatureSetC & FeatureSet(void)
      {
        return m_featureSet;
      }
      //: Access the feature set on the image

      const StringC & Pose(void) const
      {
        return m_pose;
      }
      //: The pose of the face

      StringC & Pose(void)
      {
        return m_pose;
      }
      //: The pose of the face

      const StringC & Spectrum(void) const
      {
        return m_spectrum;
      }
      //: The spectrum of the image

      StringC & Spectrum(void)
      {
        return m_spectrum;
      }
      //: The spectrum of the image

      const StringC & Glasses(void) const
      {
        return m_glasses;
      }
      //: Is the subject wearing glasses

      StringC & Glasses(void)
      {
        return m_glasses;
      }
      //: Is the subject wearing glasses

      const DateC  & Date(void) const
      {
        return m_date;
      }
      //: What date was the image grabbed

      DateC & Date(void)
      {
        return m_date;
      }
      //: What date the images was grabbed

      RealT DetectionScore() const
      {
        return m_detectionScore;
      }
      //: Detection score

      RealT & DetectionScore()
      {
        return m_detectionScore;
      }
      //: Detection score

      unsigned int Hash(void) const
      {
        return m_faceId.Hash();
      }
      //: Create a hash value

    protected:

      StringC m_actualId;
      //: The unique id of the person

      StringC m_faceId;
      //: The id of the face

      StringC m_origImage;
      //: The original untouched image

      ImagePointFeatureSetC m_featureSet;
      //: Info about the feature set if there is one

      StringC m_glasses;
      //: Are they wearing glasses in the image

      StringC m_pose;
      //: What is the pose

      StringC m_spectrum;
      //: What is the spectrum: FlashIR, VisibleGrey, VisibleRGB

      DateC m_date;
      //: The data it was captured

      RealT m_detectionScore;
      //: Detection score

      friend class FaceInfoC;

    };

    //! userlevel="Normal"
    //: Information about a face for XML

    class FaceInfoC : public RCHandleC<FaceInfoBodyC>
    {
    public:
      FaceInfoC()
      {
      }
      //: Default constructor
      // Creates invalid handle

      explicit FaceInfoC(const StringC & actualId, const StringC & faceId, const StringC & origImage) :
          RCHandleC<FaceInfoBodyC>(*new FaceInfoBodyC(actualId, faceId, origImage))
      {
      }
      //: Construct from actual id and path to the original image
    protected:

      FaceInfoC(XMLIStreamC& xml) :
          RCHandleC<FaceInfoBodyC>(*new FaceInfoBodyC(xml))
      {
      }
      //: Constructor
      // Creates faceinfo from an XML stream

      XMLOStreamC& SaveXML(XMLOStreamC &xml) const
      {
        return Body().SaveXML(xml);
      }
      //: Save to xml stream

    protected:

      FaceInfoC(FaceInfoBodyC &bod) :
          RCHandleC<FaceInfoBodyC>(bod)
      {
      }
      //: Body constructor.

      FaceInfoBodyC &Body()
      {
        return RCHandleC<FaceInfoBodyC>::Body();
      }
      //: Access body.

      const FaceInfoBodyC &Body() const
      {
        return RCHandleC<FaceInfoBodyC>::Body();
      }
      //: Access body (const)

    public:
      const StringC  & ActualId(void) const
      {
        return Body().ActualId();
      }
      //: The actual id of the face

      StringC & ActualId(void)
      {
        return Body().ActualId();
      }
      //: Access the id of the face

      const StringC & FaceId(void) const
      {
        return Body().FaceId();
      }
      //: The unique id of the face

      StringC & FaceId(void)
      {
        return Body().FaceId();
      }
      //: Access the unique id of the face

      const StringC & OrigImage(void) const
      {
        return Body().OrigImage();
      }
      //: Path to the original image

      StringC & OrigImage(void)
      {
        return Body().OrigImage();
      }
      //: Access the path to the original image

      const ImagePointFeatureSetC & FeatureSet(void) const
      {
        return Body().FeatureSet();
      }
      //: The feature set on the original image

      ImagePointFeatureSetC & FeatureSet(void)
      {
        return Body().FeatureSet();
      }
      //: Access the feature set

      const StringC & Pose(void) const
      {
        return Body().Pose();
      }
      //: The pose of the face

      StringC & Pose(void)
      {
        return Body().Pose();
      }
      //: The pose of the face

      const StringC & Spectrum(void) const
      {
        return Body().Spectrum();
      }
      //: The spectrum of the image

      StringC & Spectrum(void)
      {
        return Body().Spectrum();
      }
      //: The spectrum of the image

      const StringC & Glasses(void) const
      {
        return Body().Glasses();
      }
      //: Is the subject wearing glasses

      StringC & Glasses(void)
      {
        return Body().Glasses();
      }
      //: Is the subject wearing glasses

      const DateC & Date(void) const
      {
        return Body().Date();
      }
      //: What date was the image grabbed

      DateC & Date(void)
      {
        return Body().Date();
      }
      //: What date the images was grabbed

      RealT DetectionScore() const
      {
        return Body().DetectionScore();
      }
      //: Detection score

      RealT & DetectionScore()
      {
        return Body().DetectionScore();
      }
      //: Detection score

      unsigned int Hash(void) const
      {
        return Body().Hash();
      }
      //: Create a hash value

      friend XMLIStreamC &operator>>(XMLIStreamC &xml, FaceInfoC &data);
      friend XMLOStreamC &operator<<(XMLOStreamC &xml, const FaceInfoC &data);

    };

    XMLIStreamC &operator>>(XMLIStreamC &xml, FaceInfoC &data);
    //: XML Input
    // DO NOT USE DIRECTLY unless you're using this inside another XML loader and you know what you're doing.
    // Use Load() instead.

    XMLOStreamC &operator<<(XMLOStreamC &xml, const FaceInfoC &data);
    //: XML Output
    // DO NOT USE DIRECTLY unless you're using this inside another XML output func and you know what you're doing.
    // Use Save() instead.

    inline std::istream &operator>>(std::istream &strm, FaceInfoC &obj)
    {
      RavlAssertMsg(0, "Not implemented. ");
      return strm;
    }
    //: Load from a stream.
    // Uses virtual constructor.

    inline std::ostream &operator<<(std::ostream &out, const FaceInfoC &obj)
    {
      RavlAssertMsg(0, "Not implemented. ");
      return out;
    }
    //: Save to a stream.
    // Uses virtual constructor.

    inline RavlN::BinIStreamC &operator>>(RavlN::BinIStreamC &strm, FaceInfoC &obj)
    {
      RavlAssertMsg(0, "Not implemented. ");
      return strm;
    }
    //: Load from a binary stream.
    // Uses virtual constructor.

    inline RavlN::BinOStreamC &operator<<(RavlN::BinOStreamC &out, const FaceInfoC &obj)
    {
      RavlAssertMsg(0, "Not implemented. ");
      return out;
    }
//: Save to a stream.
// Uses virtual constructor.

  }
}

#endif
