// Copyright (C) 2003, Omniperception Ltd.
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlFace
//! file="Ravl/Contrib/Face/Xml/SightingSet.cc"

#include "Ravl/Face/SightingSet.hh"
#include "Ravl/DArray1dIter.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/DP/FileFormatXMLStream.hh"
#include "Ravl/Exception.hh"
#include "Ravl/SysLog.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  namespace FaceN {

    SightingSetC::SightingSetC()
    {
    }
    //: Constructor

    SightingSetC::SightingSetC(XMLIStreamC& xml)
    {
      StringC strType("");

      // Read tags until we get a SightingSet
      while (xml && xml.ReadTag(strType) != XMLBeginTag && strType != "SightingSet")
        ;
      if (!xml)
        return;

      // Read features
      while (xml && xml.PeekTag(strType) != XMLEndTag) {
        if (strType == "Sighting") {
          SightingC sighting;
          xml >> sighting;
          Append(sighting);
        } else {
          // Skip this set of tags
          xml.ReadTag(strType);
          xml.ReadTag(strType);
        }
      }
      xml.ReadTag(strType); // the end enrol session tag
      return;
    }

    XMLOStreamC &SightingSetC::SaveXML(XMLOStreamC &xml) const
    {

      // Set auto-indent mode
      xml.AutoIndent();

      // Start tag
      xml << XMLStartTag("SightingSet") << XMLContent;

      for (DArray1dIterC<SightingC> it(*this); it; it++) {
        xml << *it;
      }

      // End Tag
      xml << XMLEndTag;

      return xml;
    }

    // Delete some face ids from the sighting set
    bool SightingSetC::DeleteFaceIds(const DListC<StringC> & faceIds)
    {
      bool allFound = true;
      for (DLIterC<StringC> faceIt(faceIds); faceIt; faceIt++) {
        bool found = false;

        for (DArray1dIterC<SightingC> it(*this); it; it++) {
          if (it.Data().DeleteFaceId(*faceIt)) {
            found = true;
            break;
          }
        } // end sighting it

        // If we didn't find it in the sightings then flag an error
        if (!found) {
          allFound = false;
          RavlWarning("Failed to delete face %s from sighting set!", faceIt.Data().data());
        }

      } // end faceids it

      return allFound;
    }

    XMLIStreamC &operator>>(XMLIStreamC &xml, SightingSetC &data)
    {
      data = SightingSetC(xml);
      return xml;
    }

    XMLOStreamC &operator<<(XMLOStreamC &xml, const SightingSetC& data)
    {
      return data.SaveXML(xml);
    }

    void InitSightingSetFormat()
    {
    }

    static TypeNameC typeNameSightingSetC(typeid(SightingSetC), "FaceN::SightingSetC");

    FileFormatXMLStreamC<SightingSetC> FileFormatXMLStream_SightingSetC;

  }
}
