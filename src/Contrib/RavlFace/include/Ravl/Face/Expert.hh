// Copyright (C) 2003, Omniperception Ltd.
// file-header-ends-here
#ifndef RAVLN_FACEN_EXPERT_HEADER
#define RAVLN_FACEN_EXPERT_HEADER 1
//! rcsid="$Id$"
//! lib=RavlFace
//! docentry="Ravl.API.Applications.Image.Face"
//! file="Ravl/Contrib/Face/Xml/Expert.hh"
//! author="Kieron J Messer"

#include "Ravl/RefCounter.hh"
#include "Ravl/XMLStream.hh"
#include "Ravl/String.hh"

namespace RavlN {
  namespace FaceN {
    
    //! userlevel=Develop
    //: A score obtained on a client or impostor claim
    
    class ExpertBodyC : public RCBodyC
    {

    public:

      ExpertBodyC(const StringC & expert, RealT measure);
      //: Expert used and score obtained

    protected:

      ExpertBodyC(XMLIStreamC& xml);
      //: Constructor from XML stream

      XMLOStreamC& SaveXML(XMLOStreamC &xml) const;
      //: Save to xml stream

    public:

      StringC Expert(void) const
      {
        return type;
      }
      //: The claimed id

      StringC & Expert(void)
      {
        return type;
      }
      //: The actual id

      RealT Measure(void) const
      {
        return measure;
      }
      //: The value of the metric for this claim

      RealT & Measure(void)
      {
        return measure;
      }
      //: The value of the metric for this claim

    protected:

      StringC type;
      //: The type of expert

      RealT measure;
      //: The actual value of the score

      friend class ExpertC;

    };
    
    //! userlevel="Normal"
    //: A score obtained on a client or impostor claim
    class ExpertC : public RCHandleC<ExpertBodyC>
    {

    public:

      ExpertC()
      {
      }
      //: Default constructor
      // Creates invalid handle

      explicit ExpertC(const StringC & type, RealT measure) :
          RCHandleC<ExpertBodyC>(*new ExpertBodyC(type, measure))
      {
      }
      //: Construct from claimed id, actual id, the type of metric being used and the value of metric

    protected:

      ExpertC(XMLIStreamC& xml) :
          RCHandleC<ExpertBodyC>(*new ExpertBodyC(xml))
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

      ExpertC(ExpertBodyC &bod) :
          RCHandleC<ExpertBodyC>(bod)
      {
      }
      //: Body constructor.

      ExpertBodyC &Body()
      {
        return RCHandleC<ExpertBodyC>::Body();
      }
      //: Access body.

      const ExpertBodyC &Body() const
      {
        return RCHandleC<ExpertBodyC>::Body();
      }
      //: Access body (const)

    public:

      StringC Expert(void) const
      {
        return Body().Expert();
      }
      //: The claimed id

      StringC & Expert(void)
      {
        return Body().Expert();
      }
      //: The actual id

      RealT Measure(void) const
      {
        return Body().Measure();
      }
      //: The value of the metric on this claim

      RealT & Measure(void)
      {
        return Body().Measure();
      }
      //: The value of the metric on this claim

      friend XMLIStreamC &operator>>(XMLIStreamC &xml, ExpertC &data);
      friend XMLOStreamC &operator<<(XMLOStreamC &xml, const ExpertC &data);

    };
    
    XMLIStreamC &operator>>(XMLIStreamC &xml, ExpertC &data);
    //: XML Input
    // DO NOT USE DIRECTLY unless you're using this inside another XML loader and you know what you're doing.
    // Use Load() instead.
    
    XMLOStreamC &operator<<(XMLOStreamC &xml, const ExpertC &data);
  //: XML Output
  // DO NOT USE DIRECTLY unless you're using this inside another XML output func and you know what you're doing.
  // Use Save() instead.

  }
}

#endif
