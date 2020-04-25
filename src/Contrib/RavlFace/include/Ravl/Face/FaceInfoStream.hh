// Copyright (C) 2003, Omniperception Ltd.
// file-header-ends-here
#ifndef RAVLN_FACEN_FACEINFOSTREAM_HEADER
#define RAVLN_FACEN_FACEINFOSTREAM_HEADER 1
//! lib=RavlFace
//! docentry="Ravl.API.Applications.Image.Face"
//! file="Ravl/Contrib/Face/Xml/FaceInfoStream.hh"
//! author="Kieron J Messer"
//! rcsid="$Id$"

#include "Ravl/PatternRec/SampleStream.hh"
#include "Ravl/DList.hh"
#include "Ravl/DLIter.hh"
#include "Ravl/Face/FaceInfoDb.hh"

namespace RavlN {
  namespace FaceN {
    
    using namespace RavlN;
    using namespace FaceN;
    
    //! userlevel=Develop
    //: Get a stream of FaceInfo from FaceInfoDb they are sorted by client and image
    
    class FaceInfoStreamBodyC : public DPISPortBodyC<FaceInfoC>
    {
    public:
      FaceInfoStreamBodyC(const FaceInfoDbC & db, bool markedUp);
      //: Default constructor.

      FaceInfoStreamBodyC(const StringC & xmlFileName);
      //: Construct from a cdb file

      bool Rewind();
      //: Rewind to the beginning

      virtual bool Get(FaceInfoC & ret);
      //: Get next piece of data.

      UIntT NoFaces() const
      {
        return noFaces;
      }

      UIntT NoClients() const
      {
        return noClients;
      }


    protected:
      UIntT m_formatId; // The input format we are using

      DListC<FaceInfoC> faces;
      //: the list of all sorted face infos

      DLIterC<FaceInfoC> iter;
      //: the iterator through the file names

      UIntT noFaces;
      //: Number of faces in list

      UIntT noClients;
      //: Number of clients in stream

      bool markedUp;
      //: Only consider images which have been marked up

      StringC m_cacheDir;
      //: Store a cache of stuff in here

      XMLIStreamC xml;

    };
    
    //! userlevel=Normal
    //: Get a stream of faces from XML files
    
    class FaceInfoStreamC : public SampleStreamC<FaceInfoC>
    {
    protected:
      FaceInfoStreamC(FaceInfoStreamBodyC &bod) :
          DPEntityC(bod)
      {
      }
      //: Body constructor.

      FaceInfoStreamBodyC & Body()
      {
        return dynamic_cast<FaceInfoStreamBodyC &>(DPEntityC::Body());
      }
      //: Access body.

      const FaceInfoStreamBodyC & Body() const
      {
        return dynamic_cast<const FaceInfoStreamBodyC &>(DPEntityC::Body());
      }
      //: Access body.

    public:
      FaceInfoStreamC(const FaceInfoDbC & db, bool markedUp = true) :
          DPEntityC(*new FaceInfoStreamBodyC(db, markedUp))
      {
      }
      //: Constructor.

      FaceInfoStreamC(const StringC & xmlFilename) :
          DPEntityC(*new FaceInfoStreamBodyC(xmlFilename))
      {
      }
      //: Constructor.

      bool Rewind()
      {
        return Body().Rewind();
      }
      //: Rewind to the beginning

      UIntT NoFaces() const
      {
        return Body().NoFaces();
      }
      //: The number of faces in the stream

      UIntT NoClients() const
      {
        return Body().NoClients();
      }
      //: The number of seperate clients in the stream



    };

  }
}

#endif
