// Copyright (C) 2003, Omniperception Ltd.
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlFace
//! file="Ravl/Contrib/Face/Xml/ScoreSession.cc"

#include "Ravl/Face/ScoreSession.hh"
#include "Ravl/DLIter.hh"
#include "Ravl/HashIter.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/DP/FileFormatXMLStream.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  namespace FaceN {

    bool compare_ascend_score(const ScoreC & sc1, const ScoreC & sc2)
    {
      return sc1.Measure("0") <= sc2.Measure("0");
    }

    bool compare_descend_score(const ScoreC & sc1, const ScoreC & sc2)
    {
      return sc1.Measure("0") >= sc2.Measure("0");
    }

    using namespace RavlN;

    ScoreSessionC::ScoreSessionC()
    {
    }
    
    ScoreSessionC::ScoreSessionC(XMLIStreamC& xml)
    {
      StringC strType("");

      // Read tags until we get a EnrolSession
      while (xml && xml.ReadTag(strType) != XMLBeginTag && strType != "scoresession")
        ;
      if (!xml)
        return;

      // Read features
      while (xml && xml.PeekTag(strType) != XMLEndTag) {
        if (strType == "score") {
          ScoreC score;
          xml >> score;
          InsLast(score);
        } else {
          // Skip this set of tags
          xml.ReadTag(strType);
          xml.ReadTag(strType);
        }
      }
      xml.ReadTag(strType); // the end enrol session tag
      return;
    }

    XMLOStreamC &ScoreSessionC::SaveXML(XMLOStreamC &xml) const
    {

      // Set auto-indent mode
      xml.AutoIndent();

      // Start tag
      xml.StartTag("scoresession");

      // Write child tags
      xml.StartContents();
      for (DLIterC < ScoreC > it(*this); it; it++) {
        xml << *it;
        ;
      }

      // End Tag
      xml.EndTag();
      return xml;
    }
    
    bool ScoreSessionC::Sort(bool ascending)
    {
      if (ascending)
        MergeSort (compare_ascend_score);
      else
        MergeSort (compare_descend_score);
      return true;
    }

    ScoreSessionC ScoreSessionC::FalseAcceptances(const RealT & threshold) const
    {
      //: Get all the impostor attacks
      ScoreSessionC scoreSession;
      for (DLIterC < ScoreC > it(*this); it; it++) {
        if (it.Data().ClaimedId() != it.Data().ActualId()) {
          if (it.Data().Measure("0") > threshold)
            scoreSession.InsLast(*it);
        }
      }
      //: sort into highest scores first
      scoreSession.Sort(false);
      return scoreSession;
    }
    
    ScoreSessionC ScoreSessionC::FalseRejections(const RealT & threshold) const
    {
      //: Get all the genuine client claims
      ScoreSessionC scoreSession;
      for (DLIterC < ScoreC > it(*this); it; it++) {
        if (it.Data().ClaimedId() == it.Data().ActualId())
          if (it.Data().Measure("0") < threshold)
            scoreSession.InsLast(*it);
      }
      //: Sort into lowest scores first
      scoreSession.Sort(true);
      return scoreSession;
    }

    // XML streams
    XMLIStreamC &operator>>(XMLIStreamC &xml, ScoreSessionC &data)
    {
      data = ScoreSessionC(xml);
      return xml;
    }

    XMLOStreamC &operator<<(XMLOStreamC &xml, const ScoreSessionC& data)
    {
      return data.SaveXML(xml);
    }

    void InitScoreSessionFormat()
    {
    }

    static TypeNameC typeNameScoreSessionC(typeid(ScoreSessionC), "FaceN::ScoreSessionC");

    FileFormatXMLStreamC<ScoreSessionC> FileFormatXMLStream_ScoreSessionC;
  
  }
}

