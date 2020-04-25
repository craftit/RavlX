// Copyright (C) 2003, Omniperception Ltd.
// file-header-ends-here
#ifndef RAVL_IMAGE_EXPERIMENT_HEADER
#define RAVL_IMAGE_EXPERIMENT_HEADER 1
//! rcsid="$Id$"
//! lib=RavlFace
//! docentry="Ravl.API.Applications.Image.Face"
//! file="Ravl/Contrib/Face/Xml/Experiment.hh"
//! author="Kieron J Messer"

#include "Ravl/RefCounter.hh"
#include "Ravl/XMLStream.hh"
#include "Ravl/DList.hh"
#include "Ravl/Face/ResultsInfo.hh"

namespace RavlN {
  namespace FaceN {

    using namespace RavlN;

    //! userlevel=Normal
    //: A DListC of ResultsInfoC with XML streams

    class ExperimentC : public DListC<ResultsInfoC>
    {

    public:

      ExperimentC();
      //: Constructor

    protected:

      ExperimentC(XMLIStreamC& xml);
      //: Constructor from XML stream

      XMLOStreamC& SaveXML(XMLOStreamC &xml) const;
      //: Save to xml stream

    public:

      friend XMLIStreamC &operator>>(XMLIStreamC &xml, ExperimentC &data);
      friend XMLOStreamC &operator<<(XMLOStreamC &xml, const ExperimentC &data);

    };

    XMLIStreamC &operator>>(XMLIStreamC &xml, ExperimentC &data);
    //: XML Input
    // DO NOT USE DIRECTLY unless you're using this inside another XML loader and you know what you're doing.
    // Use Load() instead.

    XMLOStreamC &operator<<(XMLOStreamC &xml, const ExperimentC &data);
    //: XML Output
    // DO NOT USE DIRECTLY unless you're using this inside another XML output func and you know what you're doing.
    // Use Save() instead.

    void InitExperimentFormat();
  
  }
}

#endif
