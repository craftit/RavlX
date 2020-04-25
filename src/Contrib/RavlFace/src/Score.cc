// Copyright (C) 2003, Omniperception Ltd.
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlFace
//! file="Ravl.Contrib.Face/Score.cc"

#include "Ravl/Face/Score.hh"
#include "Ravl/DLIter.hh"
#include "Ravl/HashIter.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/DP/FileFormatXMLStream.hh"
#include "Ravl/CDLIter.hh"

namespace RavlN {
  namespace FaceN {

    ScoreBodyC::ScoreBodyC(const StringC & cid, const StringC & aid, const Int64T & k) :
        key(k), claimedId(cid), actualId(aid)
    {
    }
    
    ScoreBodyC::ScoreBodyC(const StringC & cid, const StringC & aid, const StringC & metric, RealT value, const Int64T & k) :
        key(k), claimedId(cid), actualId(aid)
    {
      ExpertC expert(metric, value);
      experts.InsLast(expert);
    }

    ScoreBodyC::ScoreBodyC(XMLIStreamC& xml)
    {
      StringC strType("");
      // Read tags until we get a Enrol
      while (xml && xml.ReadTag(strType) != XMLBeginTag && strType != "score")
        ;
      if (!xml)
        return;
      xml.GetAttrib("key", key);
      xml.GetAttrib("actual_id", actualId);
      xml.GetAttrib("claimed_id", claimedId);
      UIntT exp;
      xml.GetAttrib("experts", exp);
      experts.Empty();
      for (UIntT i = 0; i < exp; i++) {
        ExpertC expert;
        xml >> expert;
        experts.InsLast(expert);
      }
      //: read the end tag of enrol
      xml.ReadTag(strType);
      return;
    }

    XMLOStreamC &ScoreBodyC::SaveXML(XMLOStreamC &xml) const
    {

      // Set auto-indent mode
      xml.AutoIndent();

      // Start tag
      xml.StartTag("score");
      xml.SetAttrib("key", key);
      xml.SetAttrib("claimed_id", claimedId);
      xml.SetAttrib("actual_id", actualId);
      xml.SetAttrib("experts", experts.Size());

      xml.StartContents();

      //: Lets output the score
      for (DLIterC < ExpertC > it(experts); it; it++)
        xml << *it;

      // Last Tag
      xml.EndTag();

      return xml;
    }

    RealT ScoreBodyC::Measure(const StringC & expert) const
    {
      for (ConstDLIterC < ExpertC > it(experts); it; it++) {
        if (it.Data().Expert() == expert)
          return it.Data().Measure();
      }
      return 0.0;
    }

    XMLIStreamC &operator>>(XMLIStreamC &xml, ScoreC &data)
    {
      data = ScoreC(xml);
      return xml;
    }

    XMLOStreamC &operator<<(XMLOStreamC &xml, const ScoreC& data)
    {
      return data.SaveXML(xml);
    }

    void InitScoreFormat()
    {
    }

    static TypeNameC typeNameScoreC(typeid(ScoreC), "FaceN::ScoreC");

    FileFormatXMLStreamC<ScoreC> FileFormatXMLStream_ScoreC;
  
  }
}

