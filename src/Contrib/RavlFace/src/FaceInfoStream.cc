// Copyright (C) 2003, Omniperception Ltd.
// file-header-ends-here
//! author="Kieron J Messer"
//! rcsid="$Id$"
//! lib=RavlFace

#include "Ravl/Face/FaceInfoStream.hh"
#include "Ravl/IO.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/StringList.hh"

namespace RavlN {
  namespace FaceN {

    using namespace RavlN;

    FaceInfoStreamBodyC::FaceInfoStreamBodyC(const FaceInfoDbC & db, bool markedUp) :
        m_formatId(0), xml("")
    {
      //: The first thing to do is make sure all the client images are next to each other
      RCHashC < StringC, DListC<FaceInfoC> > sort = db.Sort(false);

      //: Next make a simple list
      for (HashIterC < StringC, DListC<FaceInfoC> > it(sort); it; it++) {
        for (DLIterC < FaceInfoC > faceIt(it.Data()); faceIt; faceIt++) {
          //: Only add face if it has a mark up and asked to only add marked up images
          if (faceIt.Data().FeatureSet().IsValid() && markedUp) {
            faces.InsLast(*faceIt);
          }
          //: Else if we do not care if they have to be marked up
          else if (!markedUp) {
            faces.InsLast(*faceIt);
          }
        }
      }

      //: How many clients do we have etc...
      noFaces = faces.Size();
      noClients = sort.Size();

      //: Set the root dir
      //root = db.Root();

      //: Finally make the iter
      iter = DLIterC < FaceInfoC > (faces);
    }
    
    FaceInfoStreamBodyC::FaceInfoStreamBodyC(const StringC & xmlFilename) :
        m_formatId(1), xml(xmlFilename)
    {
      cerr << "Performing quick scan of xml file...." << endl;
      noFaces = 0;
      FaceInfoC face;
      RCHashC<StringC, bool> clients;
      while (Get(face)) {
        noFaces++;
        clients.Insert(face.ActualId(), true);
      }
      noClients = clients.Size();
      Rewind();
    }
    
    bool FaceInfoStreamBodyC::Rewind()
    {
      if (m_formatId == 0) {
        iter.Nth(0);
        return true;
      } else {
        xml.is().clear();
        xml.seekg(0);
      }
      return true;
    }
    //: Seek to element.

    bool FaceInfoStreamBodyC::Get(FaceInfoC & ret)
    {
      
      if (m_formatId == 0) {
        if (iter) {
          ret = iter.Data();
          iter++;
          return true;
        }
        return false;
      } else {

        StringC strType;

        if (xml.IsEndOfStream())
          return false;

        while (xml && xml.PeekTag(strType) != XMLEndTag) {

          if (strType == "faceinfo") {
            xml >> ret;
            return true;
          }

          else if (strType == "protocol") {
            ProtocolC prot;
            xml >> prot;
          }

          else {
            // the first faceInfoDb tag
            xml.ReadTag(strType);
          }
        }
        xml.ReadTag(strType); // the end faceInfoDb tag

        return false;
      }
    }
    static TypeNameC typeNameFaceInfoStreamC(typeid(FaceInfoStreamC), "FaceInfoStreamC");

  }
}
