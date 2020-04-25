// Copyright (C) 2003, Omniperception Ltd.
// file-header-ends-here
#ifndef RAVLN_FACEN_FacePairSet_HEADER
#define RAVLN_FACEN_FacePairSet_HEADER 1
//! rcsid="$Id$"
//! lib=RavlFace
//! docentry="Ravl.API.Applications.Image.Face"
//! file="Ravl/Contrib/Face/Xml/FacePairSet.hh"
//! author="Kieron J Messer"

#include "Ravl/RefCounter.hh"
#include "Ravl/XMLStream.hh"
#include "Ravl/RCHash.hh"
#include "Ravl/DList.hh"
#include "Ravl/Face/FacePair.hh"

namespace RavlN {
  namespace FaceN {

    //! userlevel="Normal"
    //: A set of sightings

    class FacePairSetC : public RCHashC<StringC, DListC<FacePairC> >
    {

    public:

      FacePairSetC();
      //: Constructor

      //! Add a face pair
      bool Add(const FacePairC & facePair);

    protected:

      FacePairSetC(XMLIStreamC& xml);
      //: Constructor from XML stream

      XMLOStreamC& SaveXML(XMLOStreamC &xml) const;
      //: Save to xml stream

    public:

    protected:

      friend XMLIStreamC &operator>>(XMLIStreamC &xml, FacePairSetC &data);
      friend XMLOStreamC &operator<<(XMLOStreamC &xml, const FacePairSetC &data);

    };

    XMLIStreamC &operator>>(XMLIStreamC &xml, FacePairSetC &data);
    //: XML Input
    // DO NOT USE DIRECTLY unless you're using this inside another XML loader and you know what you're doing.
    // Use Load() instead.

    XMLOStreamC &operator<<(XMLOStreamC &xml, const FacePairSetC &data);
//: XML Output
// DO NOT USE DIRECTLY unless you're using this inside another XML output func and you know what you're doing.
// Use Save() instead.

  }
}

#endif
