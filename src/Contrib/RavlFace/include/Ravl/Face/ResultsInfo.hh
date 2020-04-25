// Copyright (C) 2003, Omniperception Ltd.
// file-header-ends-here
#ifndef RAVLN_FACEN_RESULTSINFO_HEADER
#define RAVLN_FACEN_RESULTSINFO_HEADER 1
//! rcsid="$Id$"
//! lib=RavlFace
//! docentry="Ravl.API.Applications.Image.Face"
//! file="Ravl/Contrib/Face/Experiment/ResultsInfo.hh"
//! author="Kieron J Messer"

#include "Ravl/RefCounter.hh"
#include "Ravl/XMLStream.hh"
#include "Ravl/String.hh"

namespace RavlN {
  namespace FaceN {

    //! userlevel=Normal
    //: XML format for FA and FR errors

    class ResultsInfoC
    {

    public:
      ResultsInfoC()
      {
      }

      ResultsInfoC(RealT fa, RealT fr, RealT threshold);
      //: Construct from FA and FR rates obtained with a given threshold

    protected:

      ResultsInfoC(XMLIStreamC& xml);
      //: Constructor from XML stream

      XMLOStreamC& SaveXML(XMLOStreamC &xml) const;
      //: Save to xml stream

    public:
      StringC & Description()
      {
        return desc;
      }
      //: Change description of how results obtained

      RealT FA() const
      {
        return fa;
      }
      //: Look at fa rate

      RealT FR() const
      {
        return fr;
      }
      //: Look at fr rate

      RealT Threshold() const
      {
        return threshold;
      }
      //: Look at the threshold used

      RealT HTER() const
      {
        return (fa + fr) / 2.0;
      }

      RealT TER() const
      {
        return (fa + fr);
      }

    protected:
      StringC desc;
      //: a description of the test

      RealT fa;
      //: false acceptance

      RealT fr;
      //: false rejection

      RealT threshold;
      //: the value of the threshold used

    public:
      friend XMLIStreamC &operator>>(XMLIStreamC &xml, ResultsInfoC &data);
      friend XMLOStreamC &operator<<(XMLOStreamC &xml, const ResultsInfoC &data);

    };
    
    XMLIStreamC &operator>>(XMLIStreamC &xml, ResultsInfoC &data);
    //: XML Input
    // DO NOT USE DIRECTLY unless you're using this inside another XML loader and you know what you're doing.
    // Use Load() instead.
    
    XMLOStreamC &operator<<(XMLOStreamC &xml, const ResultsInfoC &data);
  //: XML Output
  // DO NOT USE DIRECTLY unless you're using this inside another XML output func and you know what you're doing.
  // Use Save() instead.

  }
}

#endif
