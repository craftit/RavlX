// Copyright (C) 2003, Omniperception Ltd.
// file-header-ends-here
#ifndef RAVLN_FACEN_Sighting_HEADER
#define RAVLN_FACEN_Sighting_HEADER 1
//! rcsid="$Id$"
//! lib=RavlFace
//! docentry="Ravl.API.Applications.Image.Face"
//! file="Ravl/Contrib/Face/Xml/Sighting.hh"
//! author="Kieron J Messer"

#include "Ravl/RefCounter.hh"
#include "Ravl/XMLStream.hh"
#include "Ravl/DList.hh"
#include "Ravl/Tuple2.hh"
#include "Ravl/String.hh" 

namespace RavlN {
  namespace FaceN {

    using namespace RavlN;

    //! userlevel=Develop
    //: A list of images used to Sighting a client

    class SightingBodyC : public RCBodyC
    {

    public:

      SightingBodyC()
      {
      }

      SightingBodyC(const StringC & id);
      //: Construct from id of subject to Sighting

    protected:

      SightingBodyC(XMLIStreamC& xml);
      //: Constructor from XML stream

      XMLOStreamC& SaveXML(XMLOStreamC &xml) const;
      //: Save to xml stream

    public:

      StringC ActualId(void) const
      {
        return m_actualId;
      }
      //: Get the id of person to be Sightingled

      StringC & ActualId(void)
      {
        return m_actualId;
      }
      //: Get the id of person to be Sightingled

      DListC<StringC> FaceIds() const
      {
        return m_faceIds;
      }
      //: All the faces to be used in Sightinglment

      DListC<StringC>& FaceIds()
      {
        return m_faceIds;
      }
      //: All the faces to be used in Sightinglment

      bool AddFaceId(const StringC & faceid);
      //: Add a new face to be included in the Sighting

      bool DeleteFaceId(const StringC & faceId);
      //: Delete a face id from sighting, if it exists

    protected:

      StringC m_actualId;
      //: The unique id of the person

      DListC<StringC> m_faceIds;
      //: The list of images used for the Sightinglment session

      friend class SightingC;

    };

    //! userlevel="Normal"
    //: A list of images used to Sighting a client

    class SightingC : public RCHandleC<SightingBodyC>
    {

    public:

      SightingC()
      {
      }
      //: Default constructor
      // Creates invalid handle

      explicit SightingC(const StringC & id) :
          RCHandleC<SightingBodyC>(*new SightingBodyC(id))
      {
      }
      //: Construct with ID of person to be Sightingled

    protected:

      SightingC(XMLIStreamC& xml) :
          RCHandleC<SightingBodyC>(*new SightingBodyC(xml))
      {
      }
      //: Constructor
      // Creates feature set from an XML stream

      XMLOStreamC& SaveXML(XMLOStreamC &xml) const
      {
        return Body().SaveXML(xml);
      }
      //: Save to xml stream

    protected:

      SightingC(SightingBodyC &bod) :
          RCHandleC<SightingBodyC>(bod)
      {
      }
      //: Body constructor.

      SightingBodyC &Body()
      {
        return RCHandleC<SightingBodyC>::Body();
      }
      //: Access body.

      const SightingBodyC &Body() const
      {
        return RCHandleC<SightingBodyC>::Body();
      }
      //: Access body (const)

    public:

      StringC ActualId(void) const
      {
        return Body().ActualId();
      }
      //: Whats the ID of the person you are Sightingling

      StringC & ActualId(void)
      {
        return Body().ActualId();
      }
      //: Whats the ID of the person you are Sightingling

      DListC<StringC> FaceIds() const
      {
        return Body().FaceIds();
      }
      //: List all the faces that are to be included in Sighting

      DListC<StringC> & FaceIds()
      {
        return Body().FaceIds();
      }
      //: List all the faces that are to be included in Sighting

      bool AddFaceId(const StringC & faceid)
      {
        return Body().AddFaceId(faceid);
      }
      //: Add a new face to be included in the Sighting

      bool DeleteFaceId(const StringC & faceId) {
        return Body().DeleteFaceId(faceId);
      }
      //: Delete a face id from sighting, if it exists


      friend XMLIStreamC &operator>>(XMLIStreamC &xml, SightingC &data);
      friend XMLOStreamC &operator<<(XMLOStreamC &xml, const SightingC &data);

    };

    XMLIStreamC &operator>>(XMLIStreamC &xml, SightingC &data);
    //: XML Input
    // DO NOT USE DIRECTLY unless you're using this inside another XML loader and you know what you're doing.
    // Use Load() instead.

    XMLOStreamC &operator<<(XMLOStreamC &xml, const SightingC &data);
    //: XML Output
    // DO NOT USE DIRECTLY unless you're using this inside another XML output func and you know what you're doing.
    // Use Save() instead.

    inline std::istream &operator>>(std::istream &strm, SightingC &obj)
    {
      RavlAssertMsg(0, "Not implemented. ");
      return strm;
    }
    //: Load from a stream.
    // Uses virtual constructor.

    inline std::ostream &operator<<(std::ostream &out, const SightingC &obj)
    {
      RavlAssertMsg(0, "Not implemented. ");
      return out;
    }
    //: Save to a stream.
    // Uses virtual constructor.

    inline RavlN::BinIStreamC &operator>>(RavlN::BinIStreamC &strm, SightingC &obj)
    {
      RavlAssertMsg(0, "Not implemented. ");
      return strm;
    }
    //: Load from a binary stream.
    // Uses virtual constructor.

    inline RavlN::BinOStreamC &operator<<(RavlN::BinOStreamC &out, const SightingC &obj)
    {
      RavlAssertMsg(0, "Not implemented. ");
      return out;
    }
//: Save to a stream.
// Uses virtual constructor.

  }
}

#endif
