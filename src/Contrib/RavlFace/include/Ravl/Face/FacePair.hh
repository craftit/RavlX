// Copyright (C) 2003, Omniperception Ltd.
// file-header-ends-here
#ifndef RAVLN_FACEN_FacePair_HEADER
#define RAVLN_FACEN_FacePair_HEADER 1
//! rcsid="$Id$"
//! lib=RavlFace
//! docentry="Ravl.API.Applications.Image.Face"
//! file="Ravl/Contrib/Face/Xml/FacePair.hh"
//! author="Kieron J Messer"

#include "Ravl/RefCounter.hh"
#include "Ravl/XMLStream.hh"
#include "Ravl/DList.hh"
#include "Ravl/Tuple2.hh"
#include "Ravl/String.hh" 

namespace RavlN {
  namespace FaceN {

    //! userlevel=Develop
    //: A list of images used to FacePair a client

    class FacePairBodyC : public RCBodyC
    {

    public:

      FacePairBodyC()
      {
      }

      FacePairBodyC(const StringC & actualId, const StringC & faceId1, const StringC & faceId2);
      //: Construct from id of subject to FacePair

    protected:

      FacePairBodyC(XMLIStreamC& xml);
      //: Constructor from XML stream

      XMLOStreamC& SaveXML(XMLOStreamC &xml) const;
      //: Save to xml stream

    public:

      //! Get the actual id of the pair of faces
      const StringC & ActualId(void) const
      {
        return m_actualId;
      }

      //! Get face id of first face
      const StringC & FaceId1(void) const
      {
        return m_faceId1;
      }

      //! Get face id of second fce
      const StringC & FaceId2(void) const
      {
        return m_faceId2;
      }

    protected:

      StringC m_actualId; //!< Actual id of faces
      StringC m_faceId1; //!< Id of first face
      StringC m_faceId2; //!< Id of second face

      friend class FacePairC;

    };

    //! userlevel="Normal"
    //: A list of images used to FacePair a client
    
    class FacePairC : public RCHandleC<FacePairBodyC>
    {

    public:

      FacePairC()
      {
      }
      //: Default constructor
      // Creates invalid handle

      explicit FacePairC(const StringC & actualId, const StringC & faceId1, const StringC & faceId2) :
          RCHandleC<FacePairBodyC>(*new FacePairBodyC(actualId, faceId1, faceId2))
      {
      }
      //: Construct with ID of person to be FacePairled

    protected:

      FacePairC(XMLIStreamC& xml) :
          RCHandleC<FacePairBodyC>(*new FacePairBodyC(xml))
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

      FacePairC(FacePairBodyC &bod) :
          RCHandleC<FacePairBodyC>(bod)
      {
      }
      //: Body constructor.

      FacePairBodyC &Body()
      {
        return RCHandleC<FacePairBodyC>::Body();
      }
      //: Access body.

      const FacePairBodyC &Body() const
      {
        return RCHandleC<FacePairBodyC>::Body();
      }
      //: Access body (const)

    public:

      //! Get face id of first face
      const StringC & ActualId(void) const
      {
        return Body().ActualId();
      }

      //! Get face id of first face
      const StringC & FaceId1(void) const
      {
        return Body().FaceId1();
      }

      //! Get face id of second face
      const StringC & FaceId2(void) const
      {
        return Body().FaceId2();
      }

      friend XMLIStreamC &operator>>(XMLIStreamC &xml, FacePairC &data);
      friend XMLOStreamC &operator<<(XMLOStreamC &xml, const FacePairC &data);

    };

    XMLIStreamC &operator>>(XMLIStreamC &xml, FacePairC &data);
    //: XML Input
    // DO NOT USE DIRECTLY unless you're using this inside another XML loader and you know what you're doing.
    // Use Load() instead.
    
    XMLOStreamC &operator<<(XMLOStreamC &xml, const FacePairC &data);
    //: XML Output
    // DO NOT USE DIRECTLY unless you're using this inside another XML output func and you know what you're doing.
    // Use Save() instead.
    
    inline std::istream &operator>>(std::istream &strm, FacePairC &obj)
    {
      RavlAssertMsg(0, "Not implemented. ");
      return strm;
    }
    //: Load from a stream.
    // Uses virtual constructor.
    
    inline std::ostream &operator<<(std::ostream &out, const FacePairC &obj)
    {
      RavlAssertMsg(0, "Not implemented. ");
      return out;
    }
    //: Save to a stream.
    // Uses virtual constructor.
    
    inline RavlN::BinIStreamC &operator>>(RavlN::BinIStreamC &strm, FacePairC &obj)
    {
      RavlAssertMsg(0, "Not implemented. ");
      return strm;
    }
    //: Load from a binary stream.
    // Uses virtual constructor.
    
    inline RavlN::BinOStreamC &operator<<(RavlN::BinOStreamC &out, const FacePairC &obj)
    {
      RavlAssertMsg(0, "Not implemented. ");
      return out;
    }
//: Save to a stream.
// Uses virtual constructor.
  
  }
}

#endif
