// Copyright (C) 2003, Omniperception Ltd.
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlFace
//! file="Ravl.Contrib.Face/FacePair.cc"

#include "Ravl/Face/FacePair.hh"
#include "Ravl/DLIter.hh"
#include "Ravl/HashIter.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/DP/FileFormatXMLStream.hh"

namespace RavlN {
  namespace FaceN {

    FacePairBodyC::FacePairBodyC(const StringC & actualId, const StringC & faceId1, const StringC & faceId2) :
        m_actualId(actualId), m_faceId1(faceId1), m_faceId2(faceId2)
    {
    }

    FacePairBodyC::FacePairBodyC(XMLIStreamC& xml)
    {
      StringC strType("");
      // Read tags until we get a FacePair
      while (xml && xml.ReadTag(strType) != XMLBeginTag && strType != "FacePair")
        ;
      if (!xml)
        return;

      xml.GetAttrib("actualId", m_actualId);
      xml.GetAttrib("faceId1", m_faceId1);
      xml.GetAttrib("faceId2", m_faceId2);

      //: read the end tag of FacePair
      xml.ReadTag(strType);
      return;
    }
    
    XMLOStreamC &FacePairBodyC::SaveXML(XMLOStreamC &xml) const
    {

      // Set auto-indent mode
      xml.AutoIndent();

      // Start tag
      xml.StartTag("FacePair");

      xml.SetAttrib("actualId", m_actualId);
      xml.SetAttrib("faceId1", m_faceId1);
      xml.SetAttrib("faceId2", m_faceId2);

      // Write child tags
      xml.StartContents();

      //: End Tag
      xml.EndTag();
      
      return xml;
    }

    XMLIStreamC &operator>>(XMLIStreamC &xml, FacePairC &data)
    {
      data = FacePairC(xml);
      return xml;
    }

    XMLOStreamC &operator<<(XMLOStreamC &xml, const FacePairC& data)
    {
      return data.SaveXML(xml);
    }

    void InitFacePairFormat()
    {
    }

    static TypeNameC typeNameFacePairC(typeid(FacePairC), "FaceN::FacePairC");

    FileFormatXMLStreamC<FacePairC> FileFormatXMLStream_FacePairC;
  
  }
}

