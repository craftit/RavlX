// Copyright (C) 2003, Omniperception Ltd.
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlFace
//! file="Ravl/Contrib/Xml/Claim.cc"

#include "Ravl/Face/Claim.hh"
#include "Ravl/DLIter.hh"
#include "Ravl/CDLIter.hh"
#include "Ravl/HashIter.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/DP/FileFormatXMLStream.hh"

namespace RavlN {
  
  namespace FaceN {

    using namespace RavlN;

    ClaimBodyC::ClaimBodyC(const StringC & claimedId, const StringC & actualId, const Int64T & key) :
        m_key(key), m_claimedId(claimedId), m_actualId(actualId)
    {
    }
    
    ClaimBodyC::ClaimBodyC(XMLIStreamC& xml)
    {
      StringC strType("");
      // Read tags until we get a Enrol
      while (xml && xml.ReadTag(strType) != XMLBeginTag && strType != "claim")
        ;
      if (!xml)
        return;

      xml.GetAttrib("key", m_key);
      xml.GetAttrib("claimed_id", m_claimedId);
      xml.GetAttrib("actual_id", m_actualId);
      xml.GetAttrib("result", m_result);

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

    bool ClaimBodyC::AddFaceId(const StringC & faceId)
    {
      m_faceIds.InsLast(faceId);
      return true;
    }

    bool ClaimBodyC::AddFaceIds(const DListC<StringC> & faceIds)
    {
      for (ConstDLIterC < StringC > it(faceIds); it; it++)
        if (!AddFaceId(*it))
          return false;
      return true;
    }

    XMLOStreamC &ClaimBodyC::SaveXML(XMLOStreamC &xml) const
    {

      // Set auto-indent mode
      xml.AutoIndent();

      // Start tag
      xml.StartTag("claim");

      //: Key
      xml.SetAttrib("key", m_key);

      //: Claimed ID
      xml.SetAttrib("claimed_id", m_claimedId);

      //: Actual ID
      xml.SetAttrib("actual_id", m_actualId);

      //: Actual ID
      xml.SetAttrib("result", m_result);

      // Write child tags
      xml.StartContents();

      //: And the images
      for (DLIterC < StringC > it(m_faceIds); it; it++) {
        xml.StartTag("faceid");
        xml << *it;
        xml.EndTag();
      }

      // End Tag
      xml.EndTag();
      
      return xml;
    }

    XMLIStreamC &operator>>(XMLIStreamC &xml, ClaimC &data)
    {
      data = ClaimC(xml);
      return xml;
    }

    XMLOStreamC &operator<<(XMLOStreamC &xml, const ClaimC& data)
    {
      return data.SaveXML(xml);
    }
    
    void InitClaimFormat()
    {
    }
    
    static TypeNameC typeNameClaimC(typeid(ClaimC), "FaceN::ClaimC");

    FileFormatXMLStreamC<ClaimC> FileFormatXMLStream_ClaimC;

  }
}

