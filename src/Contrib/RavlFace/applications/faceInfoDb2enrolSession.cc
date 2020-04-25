// Copyright (C) 2003, Omniperception Ltd.
// file-header-ends-here
// Copyright (C) 2002, Omniperception Ltd.
//! rcsid="$Id: modifyXml.cc 5236 2005-01-28 11:11:46Z kieron $"
//! docentry="Ravl.API.Applications.Image.Face"
//! lib=RavlFace
//! file="Ravl/Contrib/Face/Xml/modifyXml.cc"

#include "Ravl/Option.hh"
#include "Ravl/IO.hh"
#include "Ravl/Face/FaceInfoDb.hh"
#include "Ravl/Face/EnrolSession.hh"

using namespace RavlN::FaceN;

//! userlevel=User
//: Allows you to modify XML files

int main(int argc, char **argv)
{  
  OptionC   opt(argc,argv);
  DListC<StringC> FaceInfoDbFiles  = opt.List("db", "input FaceInfoDb files");
  StringC   EnrolSessionFile        = opt.String("enrol", "", "enrol session file");
  opt.Check();

  FaceInfoDbC db(FaceInfoDbFiles);
  EnrolSessionC enrolSession;
  for(HashIterC<StringC, DListC<FaceInfoC> >it(db.Sort(true));it;it++) {
    EnrolC enrol(it.Key());
    for(DLIterC<FaceInfoC>faceIt(it.Data());faceIt;faceIt++)
      enrol.AddFaceId(faceIt.Data().FaceId());
    enrolSession.Insert(it.Key(), enrol);
  }
  cerr << "no clients: " << enrolSession.NoClients() << endl;

  if(!Save(EnrolSessionFile, enrolSession, "", true))
    RavlIssueError("trouble saving enrol session");
  return 0;
}

