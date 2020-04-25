// Copyright (C) 2003, Omniperception Ltd.
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlFace
//! file="Ravl.Contrib.Face/Enrol.cc"

#include "Ravl/Face/Enrol.hh"
#include "Ravl/DLIter.hh"
#include "Ravl/HashIter.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/DP/FileFormatXMLStream.hh"

namespace RavlN {
  namespace FaceN {

    using namespace RavlN;

    EnrolBodyC::EnrolBodyC(const StringC & actualId) :
        m_actualId(actualId)
    {
    }
    
    EnrolBodyC::EnrolBodyC(XMLIStreamC& xml)
    {
      StringC strType("");
      // Read tags until we get a Enrol
      while (xml && xml.ReadTag(strType) != XMLBeginTag && strType != "enrol")
        ;
      if (!xml)
        return;

      xml.GetAttrib("id", m_actualId);

      //: The next tag is the actual id
      while (xml && xml.PeekTag(strType) != XMLEndTag) {
        if (strType == "faceid") {
          StringC fc;
          xml.ReadTag(strType);
          xml >> fc;
          fc = fc.TopAndTail();
          AddFaceId(fc);
          xml.ReadTag(strType);
        } else {
          xml.ReadTag(strType);
          xml.ReadTag(strType);
        }
      }
      //: read the end tag of enrol
      xml.ReadTag(strType);
      return;
    }

    bool EnrolBodyC::AddFaceId(const StringC & faceId)
    {
      m_faceIds.InsLast(faceId);
      return true;
    }

    XMLOStreamC &EnrolBodyC::SaveXML(XMLOStreamC &xml) const
    {

      // Set auto-indent mode
      xml.AutoIndent();

      // Start tag
      xml.StartTag("enrol");

      xml.SetAttrib("id", m_actualId);

      // Write child tags
      xml.StartContents();

      //: All the paths to the images
      for (DLIterC < StringC > it(m_faceIds); it; it++) {
        xml.StartTag("faceid");
        xml << *it;
        xml.EndTag();
      }

      //: End Tag
      xml.EndTag();
      
      return xml;
    }

    XMLIStreamC &operator>>(XMLIStreamC &xml, EnrolC &data)
    {
      data = EnrolC(xml);
      return xml;
    }

    XMLOStreamC &operator<<(XMLOStreamC &xml, const EnrolC& data)
    {
      return data.SaveXML(xml);
    }
    
    void InitEnrolFormat()
    {
    }
    
    static TypeNameC typeNameEnrolC(typeid(EnrolC), "FaceN::EnrolC");

    FileFormatXMLStreamC<EnrolC> FileFormatXMLStream_EnrolC;
  
  }
}

