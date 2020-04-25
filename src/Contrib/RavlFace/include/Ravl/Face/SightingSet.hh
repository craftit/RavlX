// Copyright (C) 2003, Omniperception Ltd.
// file-header-ends-here
#ifndef RAVLN_FACEN_SightingSet_HEADER
#define RAVLN_FACEN_SightingSet_HEADER 1
//! rcsid="$Id$"
//! lib=RavlFace
//! docentry="Ravl.API.Applications.Image.Face"
//! file="Ravl/Contrib/Face/Xml/SightingSet.hh"
//! author="Kieron J Messer"

#include "Ravl/RefCounter.hh"
#include "Ravl/XMLStream.hh"
#include "Ravl/DArray1d.hh"
#include "Ravl/Face/Sighting.hh"

namespace RavlN {
  namespace FaceN {

    //! userlevel="Normal"
    //: A set of sightings

    class SightingSetC : public DArray1dC<SightingC>
    {

    public:

      SightingSetC();
      //: Constructor

      // Delete some face ids from the sighting set
      bool DeleteFaceIds(const DListC<StringC> & faceIds);

    protected:

      SightingSetC(XMLIStreamC& xml);
      //: Constructor from XML stream

      XMLOStreamC& SaveXML(XMLOStreamC &xml) const;
      //: Save to xml stream

    public:

    protected:

      friend XMLIStreamC &operator>>(XMLIStreamC &xml, SightingSetC &data);
      friend XMLOStreamC &operator<<(XMLOStreamC &xml, const SightingSetC &data);

    };

    XMLIStreamC &operator>>(XMLIStreamC &xml, SightingSetC &data);
    //: XML Input
    // DO NOT USE DIRECTLY unless you're using this inside another XML loader and you know what you're doing.
    // Use Load() instead.

    XMLOStreamC &operator<<(XMLOStreamC &xml, const SightingSetC &data);
//: XML Output
// DO NOT USE DIRECTLY unless you're using this inside another XML output func and you know what you're doing.
// Use Save() instead.

  }
}

#endif
