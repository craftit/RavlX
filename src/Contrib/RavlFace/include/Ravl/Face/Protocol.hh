// Copyright (C) 2003, Omniperception Ltd.
// file-header-ends-here
#ifndef RAVLN_FACEN_PROTOCOL_HEADER
#define RAVLN_FACEN_PROTOCOL_HEADER 1
//! rcsid="$Id$"
//! lib=RavlFace
//! docentry="Ravl.API.Applications.Image.Face"
//! file="Ravl/Contrib/Face/Xml/Protocol.hh"
//! author="Kieron J Messer"

#include "Ravl/RefCounter.hh"
#include "Ravl/XMLStream.hh"
#include "Ravl/DList.hh"
#include "Ravl/Tuple2.hh"
#include "Ravl/String.hh" 
#include "Ravl/OS/Filename.hh"

namespace RavlN {
  namespace FaceN {
    
    //! userlevel=Develop
    //: A protocol to perform an experiment
    
    class ProtocolBodyC : public RCBodyC
    {

    public:

      ProtocolBodyC()
      {
      }

      ProtocolBodyC(const StringC & name);
      //: name of the protocol

    protected:

      ProtocolBodyC(XMLIStreamC& xml);
      //: Constructor from XML stream

      XMLOStreamC& SaveXML(XMLOStreamC &xml) const;
      //: Save to xml stream

    public:

      const StringC& Name(void) const
      {
        return name;
      }
      //: The protocol name

      StringC & Name(void)
      {
        return name;
      }
      //: The protocol name

      const StringC& Type(void) const
      {
        return type;
      }
      //: The protocol type

      StringC & Type(void)
      {
        return type;
      }
      //: The protocol type

      const FilenameC& ModelFile(void) const
      {
        return modelFile;
      }
      //: The model file

      FilenameC& ModelFile(void)
      {
        return modelFile;
      }
      //: The model file

      const DListC<FilenameC>& EnrolFile(void) const
      {
        return enrolFile;
      }
      //: The enrol file

      DListC<FilenameC> & EnrolFile(void)
      {
        return enrolFile;
      }
      //: The enrol file

      const DListC<FilenameC>& EvalFile(void) const
      {
        return evalFile;
      }
      //: The eval file

      DListC<FilenameC> & EvalFile(void)
      {
        return evalFile;
      }
      //: The eval file

      const DListC<FilenameC>& TestFile(void) const
      {
        return testFile;
      }
      //: The test file

      DListC<FilenameC> & TestFile(void)
      {
        return testFile;
      }
      //: The test file

    protected:

      StringC name;
      //: The protocol name

      StringC type;
      //: The protocol type

      FilenameC modelFile;
      //: The model file

      DListC<FilenameC> enrolFile;
      //: The enrol file

      DListC<FilenameC> evalFile;
      //: The evaluation file

      DListC<FilenameC> testFile;
      //: The test file

      friend class ProtocolC;
    };
    
    //! userlevel="Normal"
    //: A list of images used to enrol a client
    
    class ProtocolC : public RCHandleC<ProtocolBodyC>
    {

    public:

      ProtocolC()
      {
      }
      //: Default constructor
      // Creates invalid handle

      explicit ProtocolC(const StringC & name) :
          RCHandleC<ProtocolBodyC>(*new ProtocolBodyC(name))
      {
      }
      //: Construct with ID of person to be enrolled

    protected:

      ProtocolC(XMLIStreamC& xml) :
          RCHandleC<ProtocolBodyC>(*new ProtocolBodyC(xml))
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

      ProtocolC(ProtocolBodyC &bod) :
          RCHandleC<ProtocolBodyC>(bod)
      {
      }
      //: Body constructor.

      ProtocolBodyC &Body()
      {
        return RCHandleC<ProtocolBodyC>::Body();
      }
      //: Access body.

      const ProtocolBodyC &Body() const
      {
        return RCHandleC<ProtocolBodyC>::Body();
      }
      //: Access body (const)

    public:

      const StringC& Name(void) const
      {
        return Body().Name();
      }
      //: The protocol name

      StringC & Name(void)
      {
        return Body().Name();
      }
      //: The protocol type

      const StringC& Type(void) const
      {
        return Body().Type();
      }
      //: The protocol type

      StringC & Type(void)
      {
        return Body().Type();
      }
      //: The protocol type

      const FilenameC& ModelFile(void) const
      {
        return Body().ModelFile();
      }
      //: The model file

      FilenameC& ModelFile(void)
      {
        return Body().ModelFile();
      }
      //: The model file

      const DListC<FilenameC>& EnrolFile(void) const
      {
        return Body().EnrolFile();
      }
      //: The enrol file

      DListC<FilenameC> & EnrolFile(void)
      {
        return Body().EnrolFile();
      }
      //: The enrol file

      const DListC<FilenameC> Evalfile(void) const
      {
        return Body().EvalFile();
      }
      //: The eval file

      DListC<FilenameC>& EvalFile(void)
      {
        return Body().EvalFile();
      }
      //: The eval file

      const DListC<FilenameC>& TestFile(void) const
      {
        return Body().TestFile();
      }
      //: The test file

      DListC<FilenameC>& TestFile(void)
      {
        return Body().TestFile();
      }
      //: The test file

      friend XMLIStreamC &operator>>(XMLIStreamC &xml, ProtocolC &data);
      friend XMLOStreamC &operator<<(XMLOStreamC &xml, const ProtocolC &data);

    };
    
    XMLIStreamC &operator>>(XMLIStreamC &xml, ProtocolC &data);
    //: XML Input
    // DO NOT USE DIRECTLY unless you're using this inside another XML loader and you know what you're doing.
    // Use Load() instead.
    
    XMLOStreamC &operator<<(XMLOStreamC &xml, const ProtocolC &data);
  //: XML Output
  // DO NOT USE DIRECTLY unless you're using this inside another XML output func and you know what you're doing.
  // Use Save() instead.

  }
}

#endif
