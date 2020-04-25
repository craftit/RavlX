// Copyright (C) 2003, Omniperception Ltd.
// file-header-ends-here
#ifndef RAVLN_FACEN_Claim_HEADER
#define RAVLN_FACEN_Claim_HEADER 1
//! rcsid="$Id$"
//! lib=RavlFace
//! docentry="Ravl.API.Applications.Image.Face"
//! file="Ravl/Contrib/Face/Xml/Claim.hh"
//! author="Kieron J Messer"

#include "Ravl/RefCounter.hh"
#include "Ravl/XMLStream.hh"
#include "Ravl/DList.hh"
#include "Ravl/String.hh"

namespace RavlN {
  namespace FaceN {
    
    //! userlevel=Develop
    //: All the info used for a client claim
    
    class ClaimBodyC : public RCBodyC
    {

    public:
      ClaimBodyC(const StringC & claimedId, const StringC & actualId, const Int64T & key);
      //: Construct from claimed id and actual id.  The key should be unique for a claim within a Claim Session

    protected:
      ClaimBodyC(XMLIStreamC& xml);
      //: Constructor from XML stream

      XMLOStreamC& SaveXML(XMLOStreamC &xml) const;
      //: Save to xml stream

    public:

      Int64T Key(void) const
      {
        return m_key;
      }
      //: The key of the claim

      Int64T & Key(void)
      {
        return m_key;
      }
      //: The key of the claim

      const StringC & ClaimedId(void) const
      {
        return m_claimedId;
      }
      //: The claimed id

      StringC & ClaimedId(void)
      {
        return m_claimedId;
      }
      //: The claimed id

      const StringC & ActualId(void) const
      {
        return m_actualId;
      }
      //: The actual id of subject

      StringC & ActualId(void)
      {
        return m_actualId;
      }
      //: The actual id of subject

      const DListC<StringC> & FaceIds() const
      {
        return m_faceIds;
      }
      //: Which faces are to be used in claim

      DListC<StringC> & FaceIds()
      {
        return m_faceIds;
      }
      //: Which faces are to be used in claim

      bool AddFaceId(const StringC & faceid);
      //: Add a new face to the set

      bool AddFaceIds(const DListC<StringC> & faceids);
      //: Add new faces to the set

      const StringC & Result() const
      {
        return m_result;
      }
      //: The result of the claim

      StringC & Result()
      {
        return m_result;
      }
      //: The result of the claim

    protected:

      Int64T m_key;
      //: The unique key

      StringC m_claimedId;
      //: The claimed id of the person

      StringC m_actualId;
      //: The actual id of the person

      StringC m_result;
      //: The result

      DListC<StringC> m_faceIds;
      //: The list of images used to verify this claim

      friend class ClaimC;

    };
    
    //! userlevel="Normal"
    //: The information required for a client claim
    
    class ClaimC : public RCHandleC<ClaimBodyC>
    {

    public:

      ClaimC()
      {
      }
      //: Default constructor
      // Creates invalid handle

      explicit ClaimC(const StringC & claimedId) :
          RCHandleC<ClaimBodyC>(*new ClaimBodyC(claimedId, claimedId.Copy(), (Int64T) -1))
      {
      }
      //: The claimed id (the actual id is set to the same), i.e. true claim

      explicit ClaimC(const StringC & claimedId, const StringC & actualId, const Int64T & key = -1) :
          RCHandleC<ClaimBodyC>(*new ClaimBodyC(claimedId, actualId, key))
      {
      }
      //: Construct from a claimed id and an actual id

    protected:

      ClaimC(XMLIStreamC& xml) :
          RCHandleC<ClaimBodyC>(*new ClaimBodyC(xml))
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

      ClaimC(ClaimBodyC &bod) :
          RCHandleC<ClaimBodyC>(bod)
      {
      }
      //: Body constructor.

      ClaimBodyC &Body()
      {
        return RCHandleC<ClaimBodyC>::Body();
      }
      //: Access body.

      const ClaimBodyC &Body() const
      {
        return RCHandleC<ClaimBodyC>::Body();
      }
      //: Access body (const)

    public:
      Int64T Key(void) const
      {
        return Body().Key();
      }
      //: The key of the claim

      Int64T & Key(void)
      {
        return Body().Key();
      }
      //: The key of the claim

      const StringC & ClaimedId(void) const
      {
        return Body().ClaimedId();
      }
      //: The claimed id

      StringC & ClaimedId(void)
      {
        return Body().ClaimedId();
      }
      //: The claimed id

      const StringC & ActualId(void) const
      {
        return Body().ActualId();
      }
      //: The actual id

      StringC & ActualId(void)
      {
        return Body().ActualId();
      }
      //: The actual id

      const DListC<StringC> & FaceIds() const
      {
        return Body().FaceIds();
      }
      //: Get list of face infos for claim

      DListC<StringC> & FaceIds()
      {
        return Body().FaceIds();
      }
      //: Get list of face infos for claim

      bool AddFaceId(const StringC & faceid)
      {
        return Body().AddFaceId(faceid);
      }
      //: Add a new face to the claim

      bool AddFaceIds(const DListC<StringC> & faceids)
      {
        return Body().AddFaceIds(faceids);
      }
      //: Add a new face to the claim

      const StringC & Result() const
      {
        return Body().Result();
      }
      //: The result of the claim

      StringC & Result()
      {
        return Body().Result();
      }
      //: The result of the claim

      friend XMLIStreamC &operator>>(XMLIStreamC &xml, ClaimC &data);
      friend XMLOStreamC &operator<<(XMLOStreamC &xml, const ClaimC &data);

    };
    
    XMLIStreamC &operator>>(XMLIStreamC &xml, ClaimC &data);
    //: XML Input
    // DO NOT USE DIRECTLY unless you're using this inside another XML loader and you know what you're doing.
    // Use Load() instead.
    
    XMLOStreamC &operator<<(XMLOStreamC &xml, const ClaimC &data);
  //: XML Output
  // DO NOT USE DIRECTLY unless you're using this inside another XML output func and you know what you're doing.
  // Use Save() instead.

  }
}

#endif
