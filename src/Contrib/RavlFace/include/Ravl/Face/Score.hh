// Copyright (C) 2003, Omniperception Ltd.
// file-header-ends-here
#ifndef RAVLN_FACEN_SCORE_HEADER
#define RAVLN_FACEN_SCORE_HEADER 1
//! rcsid="$Id$"
//! lib=RavlFace
//! docentry="Ravl.API.Applications.Image.Face"
//! file="Ravl/Contrib/Face/Xml/Score.hh"
//! author="Kieron J Messer"

#include "Ravl/RefCounter.hh"
#include "Ravl/XMLStream.hh"
#include "Ravl/String.hh"
#include "Ravl/DList.hh"
#include "Ravl/Face/Expert.hh"

namespace RavlN {
  namespace FaceN {
    
    //! userlevel=Develop
    //: A score obtained on a client or impostor claim
    
    class ScoreBodyC : public RCBodyC
    {

    public:

      ScoreBodyC(const StringC & claimedId, const StringC & actualId, const Int64T & key);
      //: Construct from claimed id, actual id

      ScoreBodyC(const StringC & claimedId, const StringC & actualId, const StringC & metric, RealT measure, const Int64T & key);
      //: Construct from claimed id, actual id, metric and measure

    protected:

      ScoreBodyC(XMLIStreamC& xml);
      //: Constructor from XML stream

      XMLOStreamC& SaveXML(XMLOStreamC &xml) const;
      //: Save to xml stream

    public:
      Int64T Key(void) const
      {
        return key;
      }
      //: The key of the claim

      Int64T & Key(void)
      {
        return key;
      }
      //: The key of the claim

      StringC ClaimedId(void) const
      {
        return claimedId;
      }
      //: The claimed id

      StringC ActualId(void) const
      {
        return actualId;
      }
      //: The actual id

      DListC<ExpertC> Experts(void) const
      {
        return experts;
      }
      //: The experts

      DListC<ExpertC>& Experts(void)
      {
        return experts;
      }
      //: The experts

      RealT Measure(const StringC & expert) const;
      //: Get the value of a metric for an expert

      bool ClientClaim() const
      {
        return claimedId == actualId;
      }
      //: Is is a client claim or impostor attack

    protected:
      Int64T key;
      //: The key of the claim

      StringC claimedId;
      //: The unique id of the person

      StringC actualId;
      //: The actual id of the person

      DListC<ExpertC> experts;
      //: The experts involved in the claim

      friend class ScoreC;

    };
    
    //! userlevel="Normal"
    //: A score obtained on a client or impostor claim
    class ScoreC : public RCHandleC<ScoreBodyC>
    {

    public:

      ScoreC()
      {
      }
      //: Default constructor
      // Creates invalid handle

      explicit ScoreC(const StringC & claimedId, const StringC & actualId, const Int64T & key = -1) :
          RCHandleC<ScoreBodyC>(*new ScoreBodyC(claimedId, actualId, key))
      {
      }
      //: Construct from claimed id, actual id

      explicit ScoreC(const StringC & claimedId, const StringC & actualId, const StringC & metric, RealT value, const Int64T & key = -1) :
          RCHandleC<ScoreBodyC>(*new ScoreBodyC(claimedId, actualId, metric, value, key))
      {
      }
      //: Construct from claimed id, actual id, the type of metric being used and the value of metric

    protected:

      ScoreC(XMLIStreamC& xml) :
          RCHandleC<ScoreBodyC>(*new ScoreBodyC(xml))
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

      ScoreC(ScoreBodyC &bod) :
          RCHandleC<ScoreBodyC>(bod)
      {
      }
      //: Body constructor.

      ScoreBodyC &Body()
      {
        return RCHandleC<ScoreBodyC>::Body();
      }
      //: Access body.

      const ScoreBodyC &Body() const
      {
        return RCHandleC<ScoreBodyC>::Body();
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

      StringC ClaimedId(void) const
      {
        return Body().ClaimedId();
      }
      //: The claimed id

      StringC ActualId(void) const
      {
        return Body().ActualId();
      }
      //: The actual id

      DListC<ExpertC> Experts(void) const
      {
        return Body().Experts();
      }
      //: The experts

      DListC<ExpertC> & Experts(void)
      {
        return Body().Experts();
      }
      //: The experts

      RealT Measure(const StringC & expert) const
      {
        return Body().Measure(expert);
      }
      //: Get the value of a metric for an expert

      bool ClientClaim() const
      {
        return Body().ClientClaim();
      }
      //: Is is a client claim or impostor attack

      friend XMLIStreamC &operator>>(XMLIStreamC &xml, ScoreC &data);
      friend XMLOStreamC &operator<<(XMLOStreamC &xml, const ScoreC &data);

    };
    
    XMLIStreamC &operator>>(XMLIStreamC &xml, ScoreC &data);
    //: XML Input
    // DO NOT USE DIRECTLY unless you're using this inside another XML loader and you know what you're doing.
    // Use Load() instead.
    
    XMLOStreamC &operator<<(XMLOStreamC &xml, const ScoreC &data);
  //: XML Output
  // DO NOT USE DIRECTLY unless you're using this inside another XML output func and you know what you're doing.
  // Use Save() instead.

  }
}

#endif
