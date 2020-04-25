// Copyright (C) 2003, Omniperception Ltd.
// file-header-ends-here
#ifndef RAVLN_FACEN_SCORESESSION_HEADER
#define RAVLN_FACEN_SCORESESSION_HEADER 1
//! rcsid="$Id$"
//! lib=RavlFace
//! docentry="Ravl.API.Applications.Image.Face"
//! file="Ravl/Contrib/Face/Xml/ScoreSession.hh"
//! author="Kieron J Messer"

#include "Ravl/RefCounter.hh"
#include "Ravl/XMLStream.hh"
#include "Ravl/DList.hh"
#include "Ravl/Face/Score.hh"

namespace RavlN {
  namespace FaceN {

    //! userlevel="Normal"
    //: A DListC of ScoreC with XML streams

    class ScoreSessionC : public DListC<ScoreC>
    {

    public:

      ScoreSessionC();
      //: Constructor

    protected:

      ScoreSessionC(XMLIStreamC& xml);
      //: Constructor from XML stream

      XMLOStreamC& SaveXML(XMLOStreamC &xml) const;
      //: Save to xml stream

    public:
      bool Sort(bool ascending = false);
      //: Sort the score session

      ScoreSessionC FalseAcceptances(const RealT & threshold) const;
      //: Return the false acceptances at given threshold

      ScoreSessionC FalseRejections(const RealT & threshold) const;
      //: Return the false rejections at given threshold

      friend XMLIStreamC &operator>>(XMLIStreamC &xml, ScoreSessionC &data);
      friend XMLOStreamC &operator<<(XMLOStreamC &xml, const ScoreSessionC &data);

    };

    XMLIStreamC &operator>>(XMLIStreamC &xml, ScoreSessionC &data);
    //: XML Input
    // DO NOT USE DIRECTLY unless you're using this inside another XML loader and you know what you're doing.
    // Use Load() instead.
    
    XMLOStreamC &operator<<(XMLOStreamC &xml, const ScoreSessionC &data);
  //: XML Output
  // DO NOT USE DIRECTLY unless you're using this inside another XML output func and you know what you're doing.
  // Use Save() instead.

  }
}

#endif
