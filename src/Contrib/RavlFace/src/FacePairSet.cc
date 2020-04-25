// Copyright (C) 2003, Omniperception Ltd.
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlFace
//! file="Ravl/Contrib/Face/Xml/FacePairSet.cc"

#include "Ravl/Face/FacePairSet.hh"
#include "Ravl/DArray1dIter.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/DP/FileFormatXMLStream.hh"
#include "Ravl/Exception.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  namespace FaceN {

    FacePairSetC::FacePairSetC()
    {
    }
    //: Constructor

    FacePairSetC::FacePairSetC(XMLIStreamC& xml)
    {
      StringC strType("");

      // Read tags until we get a FacePairSet
      while (xml && xml.ReadTag(strType) != XMLBeginTag && strType != "FacePairSet")
        ;
      if (!xml)
        return;

      // Read features
      while (xml && xml.PeekTag(strType) != XMLEndTag) {
        if (strType == "FacePair") {
          FacePairC facePair;
          xml >> facePair;

          if (!IsElm(facePair.ActualId())) {
            Insert(facePair.ActualId(), DListC<FacePairC>());
          }
          (*this)[facePair.ActualId()].InsLast(facePair);

        } else {
          // Skip this set of tags
          xml.ReadTag(strType);
          xml.ReadTag(strType);
        }
      }
      xml.ReadTag(strType); // the end enrol session tag
      return;
    }

    XMLOStreamC &FacePairSetC::SaveXML(XMLOStreamC &xml) const
    {

      // Set auto-indent mode
      xml.AutoIndent();

      // Start tag
      xml << XMLStartTag("FacePairSet") << XMLContent;

      for (HashIterC < StringC, DListC<FacePairC> > it(*this); it; it++) {
        for (DLIterC < FacePairC > facePairIt(*it); facePairIt; facePairIt++) {
          xml << *facePairIt;
        }
      }

      // End Tag
      xml << XMLEndTag;

      return xml;
    }

    //! Add a face pair
    bool FacePairSetC::Add(const FacePairC & facePair)
    {
      if (!IsElm(facePair.ActualId())) {
        Insert(facePair.ActualId(), DListC<FacePairC>());
      }
      (*this)[facePair.ActualId()].InsLast(facePair);
      return true;
    }

    XMLIStreamC &operator>>(XMLIStreamC &xml, FacePairSetC &data)
    {
      data = FacePairSetC(xml);
      return xml;
    }

    XMLOStreamC &operator<<(XMLOStreamC &xml, const FacePairSetC& data)
    {
      return data.SaveXML(xml);
    }

    void InitFacePairSetFormat()
    {
    }

    static TypeNameC typeNameFacePairSetC(typeid(FacePairSetC), "FaceN::FacePairSetC");

    FileFormatXMLStreamC<FacePairSetC> FileFormatXMLStream_FacePairSetC;

  }
}
