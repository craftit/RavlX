// Copyright (C) 2003, Omniperception Ltd.
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlFace

#include "Ravl/Face/ResultsInfo.hh"
#include "Ravl/StringList.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/DP/FileFormatXMLStream.hh"

namespace RavlN {
  namespace FaceN {
    
    ResultsInfoC::ResultsInfoC(RealT in_fa, RealT in_fr, RealT in_threshold) :
        fa(in_fa), fr(in_fr), threshold(in_threshold)
    {
    }
    
    ResultsInfoC::ResultsInfoC(XMLIStreamC& xml)
    {
      StringC strType("");
      // Read tags until we get a ResultsInfo
      while (xml && xml.ReadTag(strType) != XMLBeginTag && strType != "results")
        ;
      if (!xml)
        return;

      xml.GetAttrib("desc", desc);
      StringC thres;
      xml.GetAttrib("threshold", thres);
      thres = thres.TopAndTail();
      threshold = thres.RealValue();

      while (xml && xml.PeekTag(strType) != XMLEndTag) {
        if (strType == "fa") {
          StringC value;
          xml.ReadTag(strType);
          xml >> value;
          value = value.TopAndTail();
          fa = value.RealValue();
          xml.ReadTag(strType);
        } else if (strType == "fr") {
          StringC value;
          xml.ReadTag(strType);
          xml >> value;
          value = value.TopAndTail();
          fr = value.RealValue();
          xml.ReadTag(strType);
        } else if (strType == "hter") {
          StringC value;
          xml.ReadTag(strType);
          xml >> value;
          xml.ReadTag(strType);
        } else {
          xml.ReadTag(strType);
          xml.ReadTag(strType);
        }
      }
      //: read the end tag of markup
      xml.ReadTag(strType);
      return;
    }

    XMLOStreamC &ResultsInfoC::SaveXML(XMLOStreamC &xml) const
    {

      // Set auto-indent mode
      xml.AutoIndent();

      // Start tag
      xml.StartTag("results");
      if (!desc.IsEmpty())
        xml.SetAttrib("desc", desc);
      xml.SetAttrib("threshold", threshold);

      // Write child tags
      xml.StartContents();

      //: Claimed ID
      xml.StartTag("fa");
      xml << (StringC) fa;
      xml.EndTag();

      xml.StartTag("fr");
      xml << (StringC) fr;
      xml.EndTag();

      xml.StartTag("hter");
      xml << (StringC)((fr + fa) / 2.0);
      xml.EndTag();

      //: End Tag
      xml.EndTag();
      
      return xml;
    }

    XMLIStreamC &operator>>(XMLIStreamC &xml, ResultsInfoC &data)
    {
      data = ResultsInfoC(xml);
      return xml;
    }

    XMLOStreamC &operator<<(XMLOStreamC &xml, const ResultsInfoC& data)
    {
      return data.SaveXML(xml);
    }

    void InitResultsInfoFormat()
    {
    }

    static TypeNameC typeNameResultsInfoC(typeid(ResultsInfoC), "FaceN::ResultsInfoC");
    
    FileFormatXMLStreamC<ResultsInfoC> FileFormatXMLStream_ResultsInfoC;
  }
}

