// Copyright (C) 2003, Omniperception Ltd.
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlFace
//! file="Ravl.Contrib.Face/Expert.cc"

#include "Ravl/Face/Expert.hh"
#include "Ravl/DLIter.hh"
#include "Ravl/HashIter.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/DP/FileFormatXMLStream.hh"

namespace RavlN {
  namespace FaceN {

    using namespace RavlN;

    ExpertBodyC::ExpertBodyC(const StringC & exp, RealT meas) :
        type(exp), measure(meas)
    {
    }

    ExpertBodyC::ExpertBodyC(XMLIStreamC& xml)
    {
      StringC strType("");
      // Read tags until we get a Enrol
      while (xml && xml.ReadTag(strType) != XMLBeginTag && strType != "expert")
        ;
      if (!xml)
        return;
      xml.GetAttrib("type", type);
      StringC value;
      xml >> value;
      value = value.TopAndTail();
      measure = value.RealValue();
      //: read the end tag of enrol
      xml.ReadTag(strType);
      return;
    }

    XMLOStreamC &ExpertBodyC::SaveXML(XMLOStreamC &xml) const
    {

      // Set auto-indent mode
      xml.AutoIndent();

      // Start tag
      xml.StartTag("expert");
      xml.SetAttrib("type", type);

      xml.StartContents();

      //: Lets output the score
      xml << (StringC) measure;

      // Last Tag
      xml.EndTag();

      return xml;
    }

    XMLIStreamC &operator>>(XMLIStreamC &xml, ExpertC &data)
    {
      data = ExpertC(xml);
      return xml;
    }

    XMLOStreamC &operator<<(XMLOStreamC &xml, const ExpertC& data)
    {
      return data.SaveXML(xml);
    }

    void InitExpertFormat()
    {
    }
    
    static TypeNameC typeNameExpertC(typeid(ExpertC), "FaceN::ExpertC");
    
    FileFormatXMLStreamC<ExpertC> FileFormatXMLStream_ExpertC;
  
  }
}

