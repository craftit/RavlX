// Copyright (C) 2003, Omniperception Ltd.
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlFace
//! file="Ravl/Contrib/Face/Xml/Experiment.cc"

#include "Ravl/Face/Experiment.hh"
#include "Ravl/DLIter.hh"
#include "Ravl/CDLIter.hh"
#include "Ravl/HashIter.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/DP/FileFormatXMLStream.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  namespace FaceN {

    using namespace RavlN;

    ExperimentC::ExperimentC()
    {
    }
    
    ExperimentC::ExperimentC(XMLIStreamC& xml)
    {
      StringC strType("");

      // Read tags until we get a EnrolSession
      while (xml && xml.ReadTag(strType) != XMLBeginTag && strType != "experiment")
        ;
      if (!xml)
        return;

      // Read features
      while (xml && xml.PeekTag(strType) != XMLEndTag) {
        if (strType == "results") {
          ResultsInfoC exp;
          xml >> exp;
          InsLast(exp);
        } else {
          // Skip this set of tags
          xml.ReadTag(strType);
          xml.ReadTag(strType);
        }
      }
      xml.ReadTag(strType); // the end claimsession tag
      return;

    }
    
    XMLOStreamC &ExperimentC::SaveXML(XMLOStreamC &xml) const
    {

      // Set auto-indent mode
      xml.AutoIndent();

      // Start tag
      xml.StartTag("experiment");

      // Write child tags
      xml.StartContents();
      for (DLIterC < ResultsInfoC > it(*this); it; it++) {
        xml << *it;
        ;
      }
      
      // End Tag
      xml.EndTag();
      return xml;
    }

    XMLIStreamC &operator>>(XMLIStreamC &xml, ExperimentC &data)
    {
      data = ExperimentC(xml);
      return xml;
    }

    XMLOStreamC &operator<<(XMLOStreamC &xml, const ExperimentC& data)
    {
      return data.SaveXML(xml);
    }

    void InitExperimentFormat()
    {
    }
    
    static TypeNameC typeNameExperimentC(typeid(ExperimentC), "FaceN::ExperimentC");

    FileFormatXMLStreamC<ExperimentC> FileFormatXMLStream_ExperimentC;
  }
}

