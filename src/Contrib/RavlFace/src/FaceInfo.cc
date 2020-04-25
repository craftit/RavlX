// Copyright (C) 2003, Omniperception Ltd.
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlFace
//! file="Ravl.Contrib.Face/FaceInfo.cc"

#include "Ravl/Face/FaceInfo.hh"
#include "Ravl/DLIter.hh"
#include "Ravl/HashIter.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/DP/FileFormatXMLStream.hh"
#include "Ravl/RLog.hh"

namespace RavlN {
  namespace FaceN {

    using namespace RavlN;

    FaceInfoBodyC::FaceInfoBodyC(const StringC & actualId, const StringC & faceId, const StringC & origImage) :
        m_actualId(actualId), m_faceId(faceId), m_origImage(origImage), m_date(0.0)
    {
      m_pose = "unknown";
      m_glasses = "unknown";
      m_spectrum = "unknown";
      m_detectionScore = 0.;
    }


    FaceInfoBodyC::FaceInfoBodyC(XMLIStreamC& xml) :
        m_date(0.0)
    {
      m_detectionScore = 0.;

      StringC strType("");
      // Read tags until we get a FaceInfo
      while (xml && xml.ReadTag(strType) != XMLBeginTag && strType != "faceinfo");
      if (!xml)
        return;


      //: Get the attributes of the tag
      xml.GetAttrib("image", m_origImage);
      xml.GetAttrib("id", m_actualId);
      xml.GetAttrib("faceid", m_faceId);

      //: The next tag is the actual id
      while (xml && xml.PeekTag(strType) != XMLEndTag) {
        if (strType == "image") {
          xml.ReadTag(strType);
          xml >> m_origImage;
          m_origImage = m_origImage.TopAndTail();
          xml.ReadTag(strType);
        } else if (strType == "FeatureSet") {
          xml >> m_featureSet;
        } else if (strType == "pose") {
          xml.ReadTag(strType);
          xml >> m_pose;
          m_pose = m_pose.TopAndTail();
          xml.ReadTag(strType);
        } else if (strType == "spectrum") {
          xml.ReadTag(strType);
          xml >> m_spectrum;
          m_spectrum = m_spectrum.TopAndTail();
          xml.ReadTag(strType);
        } else if (strType == "glasses") {
          xml.ReadTag(strType);
          xml >> m_glasses;
          m_glasses = m_glasses.TopAndTail();
          xml.ReadTag(strType);
        } else if (strType == "date") {
          xml.ReadTag(strType);
          StringC odbcDate;
          xml >> odbcDate;
          odbcDate = odbcDate.TopAndTail();
          m_date = DateC::FromODBCString(odbcDate);
          xml.ReadTag(strType);
        } else if (strType == "score") {
          xml.ReadTag(strType);
          xml >> m_detectionScore;
          //cout << "detection score:" << detectionScore << endl;
          xml.ReadTag(strType);
        } else {
          xml.ReadTag(strType);
          xml.SkipElement();
          xml.ReadTag(strType);
        }
      }
      //: read the end tag of enrol
      xml.ReadTag(strType);
      //cout << "end tag:" << strType << endl;


#ifdef VISUAL_CPP
      m_origImage.gsub("/vol/db", "h:/db");
#endif

      return;
    }

    XMLOStreamC &FaceInfoBodyC::SaveXML(XMLOStreamC &xml) const
    {

      // Set auto-indent mode
      xml.AutoIndent();

      xml.StartTag("faceinfo");
      xml.SetAttrib("faceid", m_faceId);
      xml.SetAttrib("id", m_actualId);
      xml.StartContents();

      if (!m_origImage.IsEmpty()) {
        xml.StartTag("image");
        xml << m_origImage;
        xml.EndTag();
      }

      //: Save feature set if there
      if (m_featureSet.IsValid()) {
        int no_features = 0;
        for (HashIterC < IntT, ImagePointFeatureC > it(m_featureSet.FeatureIterator()); it; it++)
          no_features++;
        if (no_features != 0)
          xml << m_featureSet;
      }

      //: Pose
      if (!m_pose.IsEmpty()) {
        xml.StartTag("pose");
        xml << m_pose;
        xml.EndTag();
      }

      //: Spectrum
      if (!m_spectrum.IsEmpty()) {
        xml.StartTag("spectrum");
        xml << m_spectrum;
        xml.EndTag();
      }

      //: Glasses
      if (!m_glasses.IsEmpty()) {
        xml.StartTag("glasses");
        xml << m_glasses;
        xml.EndTag();
      }

      //date
      if (m_date.USeconds() != DateC(0.0).USeconds()) {
        xml << m_date;
      }

      // detection score
      if (m_detectionScore != 0) {
        xml.StartTag("score");
        xml << m_detectionScore;
        xml.EndTag();
      }

      //: End faceinfo tag
      xml.EndTag();

      return xml;
    }

    XMLIStreamC &operator>>(XMLIStreamC &xml, FaceInfoC &data)
    {
      data = FaceInfoC(xml);
      return xml;
    }

    XMLOStreamC &operator<<(XMLOStreamC &xml, const FaceInfoC& data)
    {
      return data.SaveXML(xml);
    }

    void InitFaceInfoFormat()
    {
    }

    static TypeNameC typeNameFaceInfoC(typeid(FaceInfoC), "FaceN::FaceInfoC");

    FileFormatXMLStreamC<FaceInfoC> FileFormatXMLStream_FaceInfoC;

  }
}

