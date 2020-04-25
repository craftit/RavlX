// Copyright (C) 2003, Omniperception Ltd.
// file-header-ends-here
//! lib=RavlFace
//! file="Ravl.Contrib.Face/FaceInfoDb.cc"

#include "Ravl/Face/FaceInfoDb.hh"
#include "Ravl/CDLIter.hh"
#include "Ravl/HashIter.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/DP/FileFormatXMLStream.hh"
#include "Ravl/IO.hh"
#include "Ravl/Exception.hh"
#include "Ravl/StringList.hh"
#include "Ravl/XMLFactoryRegister.hh"
#include "Ravl/RLog.hh"
#include "Ravl/StringList.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  namespace FaceN {

    bool compare_strings(const StringC & el1, const StringC & el2)
    {
      int v = compare(el1, el2);
      if (v > 0)
        return false;
      return true;
    }

    bool compare_faceinfo(const FaceInfoC & el1, const FaceInfoC & el2)
    {
      if (el1.Date() == el2.Date())
        return compare_strings(el1.FaceId(), el2.FaceId());
      return (el1.Date() >= el2.Date());
    }

    FaceInfoDbC::FaceInfoDbC()
    {
    }
    //: Constructor

    FaceInfoDbC::FaceInfoDbC(const StringC & dbName, const DListC<StringC> & imagePath, bool imageExists)
    {
      // And intialise in the normal way
      DListC<StringC> dbNames;
      dbNames.InsLast(dbName);
      init(dbNames, imagePath, imageExists);
    }

    FaceInfoDbC::FaceInfoDbC(DListC<StringC> & dbNames, const DListC<StringC> & imagePath, bool imageExists)
    {
      init(dbNames, imagePath, imageExists);
    }

    FaceInfoDbC::FaceInfoDbC(const XMLFactoryContextC &factory)
    {
      bool imageExists = factory.AttributeBool("imageExists", true);
      // Go through child entries.
      DListC<StringC> dbNames;
      for (RavlN::DLIterC<RavlN::XMLTreeC> it(factory.Children()); it; it++) {
        if (it->Name() == "XmlDb") {
          StringC dbName = it->Content();
          dbNames.InsLast(dbName);
        }
      }
      DListC<StringC> imagePath;
      init(dbNames, imagePath, imageExists);
    }

    void FaceInfoDbC::init(const DListC<StringC> & dbNames, const DListC<StringC> & imagePath, bool imageExists)
    {
      // Look up and see if we have an alias
      ExpandKnownDatabases2(dbNames);

      for (DLIterC<StringC> it(dbNames); it; it++) {

        // Load the XML file as is
        FaceInfoDbC db;
        if (!Load(*it, db)) {
          rWarning("Unable to load XML database: %s", it.Data().chars());
          continue;
        }
        rDebug("Loaded XML database '%s' with '%d' subjects and '%d' faces.", it.Data().chars(), db.NoClients(), db.NoFaces());

        // OK lets sort out which imagePath to use
        DListC<StringC> useImagePath = imagePath;
        if (imagePath.IsEmpty()) {
          // OK lets use the loaded db one
          useImagePath = db.ImagePath();
        }

        // Now lets go through the faces
        for (HashIterC<StringC, FaceInfoC> faceIt(db); faceIt; faceIt++) {

          // Do we want to check the image exists - not always
          if (imageExists) {

            FilenameC origImagePath(faceIt.Data().OrigImage());

            // If an imagePath has been defined we use that instead to pick up the images
            if (!useImagePath.IsEmpty()) {
              bool found = false;
              for (DLIterC<StringC> pathIt(useImagePath); pathIt; pathIt++) {
                FilenameC newImagePath = *pathIt + "/" + origImagePath;
                //RavlInfo("Checking %s", newImagePath.data());
                if (newImagePath.Exists()) {
                  faceIt.Data().OrigImage() = newImagePath;
                  Insert(faceIt.Key(), faceIt.Data());
                  found = true;
                  break;
                }
              }
              if (!found) {
                RavlWarning("Modified image path '%s' not found, face not loaded!", origImagePath.data());
              }
            } else {
              // OK an image path is not defined, so we just try and use the image - may be the path is in that
              if (!origImagePath.Exists()) {
                RavlWarning("Original Image Path '%s' not found, face not loaded!", origImagePath.data());
              } else {
                Insert(faceIt.Key(), faceIt.Data());
              }
            }
          } else {
            Insert(faceIt.Key(), faceIt.Data());
          }

        } // end face it

        m_imagePath += useImagePath;
      }
    }

    FaceInfoDbC::FaceInfoDbC(XMLIStreamC& xml)
    {
      StringC strType("");

      // Read tags until we get a FaceInfoDb
      while (xml && xml.ReadTag(strType) != XMLBeginTag && strType != "faceinfodb")
        ;
      if (!xml)
        return;

      // Read name attribute
      StringC root, name, imagePath;
      xml.GetAttrib("name", name);
      xml.GetAttrib("root", root);
      xml.GetAttrib("imagePath", imagePath);
      m_name = StringListC(name, ";");
      m_root = StringListC(root, ";");
      m_imagePath = StringListC(imagePath, ";");

      // Read features
      while (xml && xml.PeekTag(strType) != XMLEndTag) {
        if (strType == "faceinfo") {
          FaceInfoC info;
          xml >> info;

          // OK we want to sort out the image path
          FilenameC origImagePath = info.OrigImage();
          Insert(info.FaceId(), info);

        } else {
          // Skip this set of tags
          xml.ReadTag(strType);
          xml.ReadTag(strType);
        }
      }
      xml.ReadTag(strType); // the end enrol session tag
      return;
    }

    XMLOStreamC &FaceInfoDbC::SaveXML(XMLOStreamC &xml) const
    {

      // Set auto-indent mode
      xml.AutoIndent();

      // Start tag
      xml << XMLStartTag("faceinfodb");
      // Set name attribute
      StringC name;
      for (DLIterC<StringC> it(m_name); it; it++) {
        name += *it + ";";
      }
      StringC root;
      for (DLIterC<StringC> it(m_root); it; it++) {
        root += *it + ";";
      }
      StringC imagePath;
      for (DLIterC<StringC> it(m_imagePath); it; it++) {
        imagePath += *it + ";";
      }

      xml.SetAttrib("name", name);
      xml.SetAttrib("root", root);
      xml.SetAttrib("imagePath", imagePath);

      xml << XMLContent;

      for (HashIterC<StringC, FaceInfoC> it(*this); it; it++) {
        if (!it->IsValid()) {
          rWarning("Invalid face!");
          continue;
        }

        // If the image path is not empty we want to use
        // this where possible
        for (DLIterC<StringC> pathIt(m_imagePath); pathIt; pathIt++) {
          FilenameC imagePath = it.Data().OrigImage();
          StringC imageDir = imagePath.PathComponent();
          if (imageDir == *pathIt) {
            it.Data().OrigImage() = imagePath.NameComponent();
            break;
          }
        } // end pathIt

        xml << *it;
      }

      // End Tag
      xml << XMLEndTag;
      return xml;
    }

    FaceInfoDbC FaceInfoDbC::GenerateXml(const DirectoryC & ImageDirectory, const StringC & SplitChar, UIntT n)
    {

      FaceInfoDbC db;

      //: Get a master list of all files to process
      DListC<FilenameC> imageFiles;
      if (ImageDirectory.Exists()) {
        DListC<StringC> names;
        names += ImageDirectory.FiltList("*jpg");
        names += ImageDirectory.FiltList("*JPG");
        names += ImageDirectory.FiltList("*jpeg");
        names += ImageDirectory.FiltList("*JPEG");
        names += ImageDirectory.FiltList("*ppm");
        names += ImageDirectory.FiltList("*PPM");
        names += ImageDirectory.FiltList("*pgm");
        names += ImageDirectory.FiltList("*PGM");
        names += ImageDirectory.FiltList("*png");
        names += ImageDirectory.FiltList("*PNG");
        names += ImageDirectory.FiltList("*bmp");
        names += ImageDirectory.FiltList("*BMP");
        names += ImageDirectory.FiltList("*jpc");
        names += ImageDirectory.FiltList("*JPC");
        names += ImageDirectory.FiltList("*jp2");
        names += ImageDirectory.FiltList("*JP2");
        names += ImageDirectory.FiltList("*tif");
        names += ImageDirectory.FiltList("*tiff");
        names += ImageDirectory.FiltList("*TIF");
        names += ImageDirectory.FiltList("*TIFF");
        for (DLIterC<StringC> it(names); it; it++)
          imageFiles += (FilenameC) ImageDirectory + "/" + *it;
      }

      for (DLIterC<FilenameC> it(imageFiles); it; it++) {
        //: The most important thing is that we need to guess the
        //: subject id from the filename.  We do this by looking at the
        //: first n chars
        StringC subjectId;
        if (n == 0) {
          StringListC lst(it.Data().BaseNameComponent(), SplitChar);
          subjectId = lst.First();
        } else {
          subjectId = it.Data().BaseNameComponent();
          subjectId = subjectId.at(0, n);
        }
        StringC imageFile = *it;
        StringC faceId = it.Data().BaseNameComponent();
        ImagePointFeatureC rightEye(0, 1, "RightEyeCentre", 0, 0);
        ImagePointFeatureC leftEye(1, 1, "LeftEyeCentre", 0, 0);
        ImagePointFeatureSetC fset(true);
        fset.ImageFile() = imageFile;
        fset.AddFeature(rightEye);
        fset.AddFeature(leftEye);
        fset.Suspect(false);
        FaceInfoC info(subjectId, faceId, imageFile);
        info.FeatureSet() = fset;
        db.Insert(faceId, info);
      }
      return db;
    }

    FaceInfoDbC FaceInfoDbC::GenerateXml(const DListC<DirectoryC> & dirs, const StringC & SplitChar, UIntT n)
    {
      FaceInfoDbC db;
      for (DLIterC<DirectoryC> it(dirs); it; it++) {
        FaceInfoDbC d = FaceInfoDbC::GenerateXml(*it, SplitChar, n);
        db.AddFrom(d);
      }
      return db;
    }

    FaceInfoDbC FaceInfoDbC::GenerateXml(const DListC<StringC> & imageFiles, const StringC & name)
    {

      FaceInfoDbC db;

      for (DLIterC<StringC> it(imageFiles); it; it++) {
        FilenameC filename(*it);
        StringC faceId = filename.BaseNameComponent();
        ImagePointFeatureC rightEye(0, 1, "RightEyeCentre", 0, 0);
        ImagePointFeatureC leftEye(1, 1, "LeftEyeCentre", 0, 0);
        ImagePointFeatureSetC fset(true);
        fset.ImageFile() = *it;
        fset.AddFeature(rightEye);
        fset.AddFeature(leftEye);
        fset.Suspect(false);
        FaceInfoC info(name, faceId, *it);
        info.FeatureSet() = fset;
        db.Insert(faceId, info);
      }
      return db;
    }

    bool FaceInfoDbC::ExpandKnownDatabases2(const DListC<StringC> & databases)
    {

      for (DLIterC<StringC> it(databases); it; it++) {
        if (*it == "xm2vts") {
          StringC db("/vol/db/xm2vts/protocol/xm2vts.xml");
          *it = db;
        }
        if (*it == "banca_english_g1") {
          StringC db("/vol/db/banca/protocol/english/banca_g1.xml");
          *it = db;
        }
        if (*it == "banca_english_g2") {
          StringC db("/vol/db/banca/protocol/english/banca_g2.xml");
          *it = db;
        }
        if (*it == "banca_english") {
          StringC db("/vol/db/banca/protocol/english/banca.xml");
          *it = db;
        }
        if (*it == "banca_english_worldmodel") {
          StringC db("/vol/db/banca/protocol/english/worldmodel.xml");
          *it = db;
        }
        if (*it == "banca_french") {
          StringC db("/vol/db/banca/protocol/french/banca.xml");
          *it = db;
        }
        if (*it == "banca_french_g1") {
          StringC db("/vol/db/banca/protocol/french/banca_g1.xml");
          *it = db;
        }
        if (*it == "banca_french_g2") {
          StringC db("/vol/data/banca/protocol/french/banca_g2.xml");
          *it = db;
        }
        if (*it == "banca_french_worldmodel") {
          StringC db("/vol/db/banca/protocol/french/worldmodel.xml");
          *it = db;
        }
      }

#ifdef VISUAL_CPP
      for(DLIterC<StringC>it(databases);it;it++) {
        it.Data().gsub("/vol/db/", "h:/db/");
      }
#endif
      return true;
    }

    UIntT FaceInfoDbC::NoClients() const
    {
      RCHashC<StringC, bool> hsh;
      for (HashIterC<StringC, FaceInfoC> it(*this); it; it++) {
        if (!hsh.IsElm(it.Data().ActualId()))
          hsh.Insert(it.Data().ActualId(), true);
      }
      return hsh.Size();
    }

    DListC<StringC> FaceInfoDbC::Keys(bool markedUp) const
    {
      DListC<StringC> keys;
      for (HashIterC<StringC, FaceInfoC> it(*this); it; it++) {
        if (!markedUp)
          keys.InsLast(it.Key());
        else if (markedUp && it.Data().FeatureSet().IsValid())
          keys.InsLast(it.Key());
      }
      //: ok lets sort
      keys.MergeSort(compare_strings);
      return keys;
    }

    SArray1dC<StringC> FaceInfoDbC::AllFaces() const
    {
      SArray1dC<StringC> arr(Size());
      UIntT c = 0;
      for (HashIterC<StringC, FaceInfoC> it(*this); it; it++) {
        arr[c] = it.Key();
        c++;
      }
      return arr;
    }

    DListC<StringC> FaceInfoDbC::Clients() const
    {
      DListC<StringC> clients;
      RCHashC<StringC, bool> hsh;
      for (HashIterC<StringC, FaceInfoC> it(*this); it; it++) {
        if (!hsh.IsElm(it.Data().ActualId())) {
          hsh.Insert(it.Data().ActualId(), true);
          clients.InsLast(it.Data().ActualId());
        }
      }
      //: ok lets sort
      clients.MergeSort(compare_strings);
      return clients;
    }

    RCHashC<StringC, DListC<FaceInfoC> > FaceInfoDbC::Sort(bool markedUp) const
    {
      //: Get a hash of all face ids for each client
      RCHashC<StringC, DListC<FaceInfoC> > hsh;
      for (HashIterC<StringC, FaceInfoC> it(*this); it; it++) {
        StringC actualId = it.Data().ActualId();
        if (!markedUp) {
          if (!hsh.IsElm(actualId))
            hsh.Insert(actualId, DListC<FaceInfoC>());
          hsh[actualId].InsLast(it.Data());
        } else if (markedUp && it.Data().FeatureSet().IsValid()) {
          if (!hsh.IsElm(actualId))
            hsh.Insert(actualId, DListC<FaceInfoC>());
          hsh[actualId].InsLast(it.Data());
        }
      }
      //: sort all the seperate lists
      for (HashIterC<StringC, DListC<FaceInfoC> > it(hsh); it; it++)
        it.Data().MergeSort(compare_faceinfo);
      return hsh;
    }

    bool ExactMatch(const StringC & s1, const StringC & s2)
    {
      return s1 == s2;
    }

    bool Contains(const StringC & s1, const StringC & s2)
    {
      return s1.contains(s2);
    }

    FaceInfoDbC FaceInfoDbC::Search(const StringC & tag, const StringC & value, bool exactMatch) const
    {
      FaceInfoDbC result;

      bool (*cmpFunc)(const StringC & s1, const StringC &s2) = NULL;
      if (exactMatch) {
        cmpFunc = &ExactMatch;
      } else {
        cmpFunc = &Contains;
      }

      for (HashIterC<StringC, FaceInfoC> it(*this); it; it++) {
        if (tag == "glasses") {
          if (cmpFunc(it.Data().Glasses(), value)) {
            result.Insert(it.Key(), it.Data());
          }
        } else if (tag == "pose") {
          if (cmpFunc(it.Data().Pose(), value)) {
            result.Insert(it.Key(), it.Data());
          }
        } else if (tag == "spectrum") {
          if (cmpFunc(it.Data().Spectrum(), value)) {
            result.Insert(it.Key(), it.Data());
          }
        } else if (tag == "id") {
          if (cmpFunc(it.Data().ActualId(), value)) {
            result.Insert(it.Key(), it.Data());
          }
        } else if (tag == "faceId") {
          if (cmpFunc(it.Data().FaceId(), value)) {
            result.Insert(it.Key(), it.Data());
          }
        } else {
          std::cerr << "WARNING: Unknown tag, " << tag << std::endl;
        }
      }
      return result;
    }

    FaceInfoDbC FaceInfoDbC::Search(const StringC & tag, const DListC<StringC> & values) const
    {
      FaceInfoDbC result;
      for (HashIterC<StringC, FaceInfoC> it(*this); it; it++) {
        for (DLIterC<StringC> valIt(values); valIt; valIt++) {
          if (tag == "glasses") {
            if (it.Data().Glasses() == *valIt)
              result.Insert(it.Key(), it.Data());
          } else if (tag == "pose") {
            if (it.Data().Pose() == *valIt)
              result.Insert(it.Key(), it.Data());
          } else if (tag == "spectrum") {
            if (it.Data().Spectrum() == *valIt)
              result.Insert(it.Key(), it.Data());
          } else if (tag == "id") {
            if (it.Data().ActualId() == *valIt)
              result.Insert(it.Key(), it.Data());
          } else
            std::cerr << "WARNING: Unknown tag, " << tag << std::endl;
        }
      }
      return result;
    }

    bool FaceInfoDbC::ModifyImagePath(const StringC & from, const StringC & to)
    {
      for (HashIterC<StringC, FaceInfoC> it(*this); it; it++) {
        StringC mod = it.Data().OrigImage();
        mod.gsub(from, to);
        it.Data().OrigImage() = mod;
      }
      return true;

    }

    bool FaceInfoDbC::DeleteId(const StringC & id)
    {
      for (HashIterC<StringC, FaceInfoC> it(*this); it; it++) {
        if (it.Data().ActualId() == id) {
          it.Del();
        }
      }
      return true;
    }

    bool FaceInfoDbC::RenameId(const StringC & from, const StringC & to, bool actualIdOnly)
    {
      for (HashIterC<StringC, FaceInfoC> it(*this); it; it++) {
        if (it.Data().ActualId() == from) {

          if (!actualIdOnly) {
            FaceInfoC faceInfo = it.Data();
            faceInfo.ActualId() = to;
            faceInfo.FaceId().gsub(from, to);
            FilenameC fname(faceInfo.OrigImage());
            StringC nameComp = fname.NameComponent();
            nameComp.gsub(from, to);
            StringC imageName = fname.PathComponent() + "/" + nameComp;
            faceInfo.OrigImage() = imageName;
            fname.Rename(imageName);
            //: Delete it from hash table
            it.Del();
            Insert(faceInfo.FaceId(), faceInfo);
          } else {
            it.Data().ActualId() = to;
          }

        }
      }
      return true;
    }

    bool FaceInfoDbC::KeepId(const EnrolSessionC & enrolSession)
    {
      FaceInfoDbC db;
      for (HashIterC<StringC, EnrolC> it(enrolSession); it; it++) {
        for (DLIterC<StringC> fit(it.Data().FaceIds()); fit; fit++) {
          if (IsElm(*fit))
            db.Insert(*fit, Access(*fit));
        }
      }
      *this = db;
      return true;
    }

    FaceInfoDbC FaceInfoDbC::Ids(const EnrolSessionC & enrolSession) const
    {
      FaceInfoDbC db;
      for (HashIterC<StringC, EnrolC> it(enrolSession); it; it++) {
        for (DLIterC<StringC> fit(it.Data().FaceIds()); fit; fit++) {
          if (IsElm(*fit)) {
            FaceInfoC info;
            Lookup(*fit, info);
            db.Insert(*fit, info);
          }
        }
      }
      return db;
    }

    bool FaceInfoDbC::AddImagePath(const StringC & imagePath)
    {
      // Check path isn't already in list
      if (m_imagePath.Contains(imagePath)) {
        return true;
      }
      m_imagePath.InsLast(imagePath);
      return true;
    }

    XMLIStreamC &operator>>(XMLIStreamC &xml, FaceInfoDbC &data)
    {
      data = FaceInfoDbC(xml);
      return xml;
    }

    XMLOStreamC &operator<<(XMLOStreamC &xml, const FaceInfoDbC& data)
    {
      return data.SaveXML(xml);
    }

    void InitFaceInfoDbFormat()
    {
    }

    static TypeNameC typeNameFaceInfoDbC(typeid(FaceInfoDbC), "FaceN::FaceInfoDbC");

    static RavlN::XMLFactoryRegisterHandleC<FaceInfoDbC> g_registerFaceInfoDbC("FaceN::FaceInfoDbC");

    FileFormatXMLStreamC<FaceInfoDbC> FileFormatXMLStream_FaceInfoDbC;
  }
}
