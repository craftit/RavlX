// Copyright (C) 2003, Omniperception Ltd.
// file-header-ends-here
#ifndef RAVLN_FACEN_EnrolSession_HEADER
#define RAVLN_FACEN_EnrolSession_HEADER 1
//! rcsid="$Id$"
//! lib=RavlFace
//! docentry="Ravl.API.Applications.Image.Face"
//! file="Ravl/Contrib/Face/Xml/EnrolSession.hh"
//! author="Kieron J Messer"

#include "Ravl/RefCounter.hh"
#include "Ravl/XMLStream.hh"
#include "Ravl/RCHash.hh"
#include "Ravl/Face/Enrol.hh"

namespace RavlN {
  namespace FaceN {

    //! userlevel="Normal"
    //: A collection of enrollments
    
    class EnrolSessionC : public RCHashC<StringC, EnrolC>
    {

    public:

      EnrolSessionC();

      //: Constructor

    protected:

      EnrolSessionC(XMLIStreamC& xml);
      //: Constructor from XML stream

      XMLOStreamC& SaveXML(XMLOStreamC &xml) const;
      //: Save to xml stream

    public:
      UIntT NoClients() const
      {
        return Size();
      }
      //: The number of clients

      DListC<StringC> AllIds() const;
      //: Get a list of all the ids held in the session

    protected:

      friend XMLIStreamC &operator>>(XMLIStreamC &xml, EnrolSessionC &data);
      friend XMLOStreamC &operator<<(XMLOStreamC &xml, const EnrolSessionC &data);

    };
    
    XMLIStreamC &operator>>(XMLIStreamC &xml, EnrolSessionC &data);
    //: XML Input
    // DO NOT USE DIRECTLY unless you're using this inside another XML loader and you know what you're doing.
    // Use Load() instead.
    
    XMLOStreamC &operator<<(XMLOStreamC &xml, const EnrolSessionC &data);
  //: XML Output
  // DO NOT USE DIRECTLY unless you're using this inside another XML output func and you know what you're doing.
  // Use Save() instead.

  }
}

#endif
