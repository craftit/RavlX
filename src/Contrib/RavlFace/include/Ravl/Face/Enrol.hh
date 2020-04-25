// Copyright (C) 2003, Omniperception Ltd.
// file-header-ends-here
#ifndef RAVLN_FACEN_Enrol_HEADER
#define RAVLN_FACEN_Enrol_HEADER 1
//! rcsid="$Id$"
//! lib=RavlFace
//! docentry="Ravl.API.Applications.Image.Face"
//! file="Ravl/Contrib/Face/Xml/Enrol.hh"
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
    //: A list of faces used to enrol a client
    
    class EnrolBodyC : public RCBodyC
    {

    public:

      EnrolBodyC()
      {
      }

      EnrolBodyC(const StringC & actualId);
      //: Construct from id of subject to enrol

    protected:

      EnrolBodyC(XMLIStreamC& xml);
      //: Constructor from XML stream

      XMLOStreamC& SaveXML(XMLOStreamC &xml) const;
      //: Save to xml stream

    public:

      const StringC & ActualId(void) const
      {
        return m_actualId;
      }
      //: Get the id of person to be enrolled

      StringC & ActualId(void)
      {
        return m_actualId;
      }
      //: Get the id of person to be enrolled

      const DListC<StringC> & FaceIds() const
      {
        return m_faceIds;
      }
      //: All the faces to be used in enrollment

      DListC<StringC>& FaceIds()
      {
        return m_faceIds;
      }
      //: All the faces to be used in enrollment

      bool AddFaceId(const StringC & faceId);
      //: Add a new face to be included in the enrol

    protected:

      StringC m_actualId;
      //: The unique id of the person

      DListC<StringC> m_faceIds;
      //: The list of images used for the enrollment session

      friend class EnrolC;

    };
    
    //! userlevel="Normal"
    //: A list of images used to enrol a client

    class EnrolC : public RCHandleC<EnrolBodyC>
    {

    public:

      EnrolC()
      {
      }
      //: Default constructor
      // Creates invalid handle

      explicit EnrolC(const StringC & actualId) :
          RCHandleC<EnrolBodyC>(*new EnrolBodyC(actualId))
      {
      }
      //: Construct with ID of person to be enrolled

    protected:

      EnrolC(XMLIStreamC& xml) :
          RCHandleC<EnrolBodyC>(*new EnrolBodyC(xml))
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

      EnrolC(EnrolBodyC &bod) :
          RCHandleC<EnrolBodyC>(bod)
      {
      }
      //: Body constructor.

      EnrolBodyC &Body()
      {
        return RCHandleC<EnrolBodyC>::Body();
      }
      //: Access body.

      const EnrolBodyC &Body() const
      {
        return RCHandleC<EnrolBodyC>::Body();
      }
      //: Access body (const)

    public:

      const StringC & ActualId(void) const
      {
        return Body().ActualId();
      }
      //: Whats the ID of the person you are enrolling

      StringC & ActualId(void)
      {
        return Body().ActualId();
      }
      //: Whats the ID of the person you are enrolling

      const DListC<StringC> & FaceIds() const
      {
        return Body().FaceIds();
      }
      //: List all the faces that are to be included in enrol

      DListC<StringC> & FaceIds()
      {
        return Body().FaceIds();
      }
      //: List all the faces that are to be included in enrol

      bool AddFaceId(const StringC & faceid)
      {
        return Body().AddFaceId(faceid);
      }
      //: Add a new face to be included in the enrol

      friend XMLIStreamC &operator>>(XMLIStreamC &xml, EnrolC &data);
      friend XMLOStreamC &operator<<(XMLOStreamC &xml, const EnrolC &data);

    };
    
    XMLIStreamC &operator>>(XMLIStreamC &xml, EnrolC &data);
    //: XML Input
    // DO NOT USE DIRECTLY unless you're using this inside another XML loader and you know what you're doing.
    // Use Load() instead.

    XMLOStreamC &operator<<(XMLOStreamC &xml, const EnrolC &data);
    //: XML Output
    // DO NOT USE DIRECTLY unless you're using this inside another XML output func and you know what you're doing.
    // Use Save() instead.

    inline std::istream &operator>>(std::istream &strm, EnrolC &obj)
    {
      RavlAssertMsg(0, "Not implemented. ");
      return strm;
    }
    //: Load from a stream.
    // Uses virtual constructor.
    
    inline std::ostream &operator<<(std::ostream &out, const EnrolC &obj)
    {
      RavlAssertMsg(0, "Not implemented. ");
      return out;
    }
    //: Save to a stream.
    // Uses virtual constructor.
    
    inline RavlN::BinIStreamC &operator>>(RavlN::BinIStreamC &strm, EnrolC &obj)
    {
      RavlAssertMsg(0, "Not implemented. ");
      return strm;
    }
    //: Load from a binary stream.
    // Uses virtual constructor.
    
    inline RavlN::BinOStreamC &operator<<(RavlN::BinOStreamC &out, const EnrolC &obj)
    {
      RavlAssertMsg(0, "Not implemented. ");
      return out;
    }
  //: Save to a stream.
  // Uses virtual constructor.

  }
}

#endif
