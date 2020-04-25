// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2005, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlImageProc
//! file="Ravl/Image/Processing/Tools/ImagePointFeatureSet/ImagePointFeatureSet.cc"

#include "Ravl/Image/ImagePointFeatureSet.hh"
#include "Ravl/DLIter.hh"
#include "Ravl/HashIter.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/DP/FileFormatXMLStream.hh"

namespace RavlImageN {

  using namespace RavlN;
  
  ImagePointFeatureSetBodyC::ImagePointFeatureSetBodyC() 
    : suspect(false)
  {}
  
  ImagePointFeatureSetBodyC::ImagePointFeatureSetBodyC(XMLIStreamC& xml) {
    StringC strType("");

    // Read tags until we get a FeatureSet
    while (xml && xml.ReadTag(strType) != XMLBeginTag && strType != "FeatureSet");
    if (!xml) return;

    // Get Attributes
    xml.GetAttrib("image",m_strImageFile);
    xml.GetAttrib("database",m_strDatabase);
    IntT size;
    xml.GetAttrib("size",size);
    suspect = false;
    xml.GetAttrib("suspect",suspect);
    
    // Read features
    while (xml && xml.PeekTag(strType) != XMLEndTag) {
      //cerr << "PeekTag=" << strType << "\n";
      if (strType == "Feature") {
	ImagePointFeatureC feat;
	xml >> feat;
	AddFeature(feat);
      } else if (strType == "Line") {
	xml.ReadTag(strType);
	IntT from, to;
	xml.GetAttrib("from",from);
	xml.GetAttrib("to",to);
	AddLine(from,to);
	xml.SkipElement();
      } else if (strType == "Subset") {
	xml.ReadTag(strType);
	StringC name;
	IntT ssize;
	DListC<IntT> subset;

	xml.GetAttrib("name",name);
	xml.GetAttrib("size",ssize);
	for (int i=0; i<ssize; i++) {
	  IntT id;
	  xml >> id;
	  subset.InsLast(id);
	}
	AddSubset(name,subset);
	RCHashC<StringC,StringC> &subsetAttrib = SubsetAttrib(name);
	//cerr << "Got subset " << name << "\n";
	do {
	  StringC subType;
	  if(xml.ReadTag(subType) == XMLEndTag)
	    break;
	  if(subType == "Attrib") {
	    StringC name,value;
	    xml >> XMLAttribute("name",name) >> XMLAttribute("value",value);
	    //cerr << "Got Attrib " << name << " " << value << "\n";
	    subsetAttrib[name] = value;
	    xml.SkipElement();
	  } else
	    xml.SkipElement();
	} while(1);
      } else if(strType == "Attrib") {
	xml.ReadTag(strType);
	StringC name,value;
	xml.GetAttrib("name",name);
	xml.GetAttrib("value",value);
	m_attribs[name] = value;
	xml.SkipElement(); // Skip end tag.
      } else {
	// Skip this tag
	xml.ReadTag(strType);
      }
    }
    // Read end tag
    xml.ReadTag(strType);
    
    // Check size
    if (size != Size()) cerr << "WARNING: ImagePointFeatureSetBodyC(XMLIStreamC&) size mismatch" << endl;
    return;
  }
  
  void ImagePointFeatureSetBodyC::AddFeature(ImagePointFeatureC& feat) {
    m_hFeatures.Update(feat.ID(),feat);
    m_hFeaturesByStr.Update(feat.Description(),feat);
  }

  void ImagePointFeatureSetBodyC::AddLine(IntT from, IntT to) {
    Tuple2C<IntT, IntT> tuple(from,to);
    m_lLines.InsLast(tuple);
  }

  void ImagePointFeatureSetBodyC::AddSubset(const StringC& name, const DListC<IntT>& subset) {
    m_hSubsets.Update(name,subset);
  }

  //: Delete feature from set.
  
  bool ImagePointFeatureSetBodyC::DeleteFeature(IntT id) {
    ImagePointFeatureC feat;
    if(!m_hFeatures.Lookup(id,feat))
      return false; // Know nothing about this feature.
    m_hFeaturesByStr.Del(feat.Description());
    m_hFeatures.Del(id);
    // Delete feature from subsets.
    for(HashIterC<StringC, DListC<IntT> > sit(m_hSubsets);sit;sit++) {
      for(DLIterC<IntT> iit(sit.Data());iit;iit++) {
	if(*iit == id)
	  iit.Del();
      }
    }
    // Delete from lines
    for(DLIterC<Tuple2C<IntT, IntT> > lit(m_lLines);lit;lit++) {
      if(lit->Data1() == id || lit->Data2() == id)
	lit.Del();
    }
    return true;
  }
  
  //: Delete feature from set.
  
  bool ImagePointFeatureSetBodyC::DeleteSubset(const StringC &name) {
    return m_hSubsets.Del(name);
  }


  IntT ImagePointFeatureSetBodyC::Find(Point2dC position, RealT tolerance) const {
    HashIterC<IntT, ImagePointFeatureC> it(m_hFeatures);
    if(!it) return -1;
    RealT minDist = position.EuclidDistance(it->Location()) + 1;
    IntT id = -1;
    for (; it; it++) {
      RealT dist = position.EuclidDistance(it->Location());
      if (dist <= tolerance && dist < minDist) {
	minDist = dist;
	id = it.Key();
      }
    }
    return id;
  }

  bool ImagePointFeatureSetBodyC::Move(IntT id, Point2dC change) {
    ImagePointFeatureC* feat = m_hFeatures.Lookup(id);
    if (feat != NULL) {
      feat->Move(change);
      return true;
    }
    cerr << "ImagePointFeatureSetBodyC::Move(), Warning failed to move feature position for id " << id << "\n";
    return false;
  }

  bool ImagePointFeatureSetBodyC::Set(IntT id, Point2dC position) {
    ImagePointFeatureC* feat = m_hFeatures.Lookup(id);
    if (feat != NULL) {
      feat->Location() = position;
      ImagePointFeatureC* feat2 = m_hFeaturesByStr.Lookup(feat->Description());
      if (feat2 != NULL && feat2 != feat) {
        feat2->Location() = position;
      }
      return true;
    }
    cerr << "ImagePointFeatureSetBodyC::Set(), Warning failed to set feature position for id " << id << "\n";
    return false;
  }

  //: Get the position of a named feature.
  // An ExceptionOperationFailedC exception is thrown if feature is not in set.
  
  Point2dC ImagePointFeatureSetBodyC::Position(const StringC &name) const {
    const ImagePointFeatureC* feat = m_hFeaturesByStr.Lookup(name);
    if(feat == 0)
      throw ExceptionOperationFailedC("Unknown feature.");
    return feat->Location();
  }
  
  //: Get the position of a  feature by id
  // An ExceptionOperationFailedC exception is thrown if feature is not in set.
  
  Point2dC ImagePointFeatureSetBodyC::Position(UIntT id) const {
    const ImagePointFeatureC* feat = m_hFeatures.Lookup(id);
    if(feat == 0 || !feat->IsValid())
      throw ExceptionOperationFailedC("Unknown feature.");
    return feat->Location();    
  }

  bool ImagePointFeatureSetBodyC::Set(const StringC& desc, Point2dC position) {
    ImagePointFeatureC* feat = m_hFeaturesByStr.Lookup(desc);
    if (feat != NULL && feat->IsValid()) {
      feat->Location() = position;
      ImagePointFeatureC* feat2 = m_hFeatures.Lookup(feat->ID());
      if (feat2 != NULL && feat2->IsValid()) {
        feat2->Location() = position;
        return true;
      }
    }
    cerr << "ImagePointFeatureSetBodyC::Set(), Warning failed to set feature position for '" << desc << "'\n";
    return false;
  }

  bool ImagePointFeatureSetBodyC::Feature(IntT id, ImagePointFeatureC& feature) const {
    const ImagePointFeatureC* feat = m_hFeatures.Lookup(id);
    if (feat != NULL) {
      feature = *feat;
      return true;
    }
    return false;
  }

  bool ImagePointFeatureSetBodyC::Feature(const StringC& desc, ImagePointFeatureC& feature) const {
    const ImagePointFeatureC* feat = m_hFeaturesByStr.Lookup(desc);
    if (feat != NULL) {
      feature = *feat;
      return true;
    }
    return false;
  }

  bool ImagePointFeatureSetBodyC::Rename(const StringC& strold, const StringC& strnew) {
    const ImagePointFeatureC* pfeat = m_hFeaturesByStr.Lookup(strold);
    if (pfeat == NULL || pfeat->IsValid()) 
      return false;
    // Remove old feature
    ImagePointFeatureC feat = m_hFeaturesByStr.Get(strold);
    // Change description
    feat.Description() = strnew;
    // Update stored features
    m_hFeaturesByStr.Update(strnew,feat);
    m_hFeatures.Update(feat.ID(),feat);
    // Done
    return true;
  }

  bool ImagePointFeatureSetBodyC::Rename(IntT id, const StringC& strnew) {
    const ImagePointFeatureC* pfeat = m_hFeatures.Lookup(id);
    if (pfeat != NULL || pfeat->IsValid()) 
      return true;
    // Remove old feature
    ImagePointFeatureC feat = m_hFeaturesByStr.Get(pfeat->Description());
    // Change description
    feat.Description() = strnew;
    // Update stored features
    m_hFeaturesByStr.Update(strnew,feat);
    m_hFeatures.Update(id,feat);
    // Done
    return true;
  }

  bool ImagePointFeatureSetBodyC::Subset(StringC name, DListC<IntT>& subset) {
    DListC<IntT>* set = m_hSubsets.Lookup(name);
    if (set != NULL) {
      subset = *set;
      return true;
    }
    return false;
  }

  RCBodyC & ImagePointFeatureSetBodyC::Copy() const
  {
    ImagePointFeatureSetBodyC *feat = new ImagePointFeatureSetBodyC();
    
    for(HashIterC<IntT, ImagePointFeatureC> it(this->m_hFeatures);it;it++ ) {
      ImagePointFeatureC feature = it.Data().Copy();
      feat->m_hFeatures[it.Key()] = feature;
      feat->m_hFeaturesByStr[feature.Description()] = feature;
    }
    
    feat->m_hSubsets = this->m_hSubsets.Copy();
    feat->m_lLines = this->m_lLines.Copy();
    feat->m_strImageFile = this->m_strImageFile.Copy();
    feat->m_strDatabase = this->m_strDatabase.Copy();
    feat->suspect = this->suspect;
    
    return *feat;
  }



  XMLOStreamC &ImagePointFeatureSetBodyC::SaveXML(XMLOStreamC &xml) const {  

    // Set auto-indent mode
    xml.AutoIndent();

    // Start tag
    xml.StartTag("FeatureSet");
  
    // Write attributes
    xml.SetAttrib("size",Size());
    xml.SetAttrib("image",m_strImageFile);
    xml.SetAttrib("suspect",suspect);
    if(!m_strDatabase.IsEmpty())
      xml.SetAttrib("database",m_strDatabase);
    
    // Write child tags
    xml.StartContents();
    
    for (HashIterC<StringC,StringC> it(m_attribs); it; it++) {
      xml << XMLStartTag("Attrib");
      xml << XMLAttribute("name",it.Key()) << XMLAttribute("value",it.Data());
      xml << XMLEndTag;
    }
    for (HashIterC<IntT, ImagePointFeatureC> it(m_hFeatures); it; it++) {
      xml << it.Data();
    }
    for (DLIterC<Tuple2C<IntT,IntT> > it(m_lLines); it; it++) {
      xml.StartTag("Line");
      xml.SetAttrib("from",it->Data1());
      xml.SetAttrib("to",it->Data2());
      xml.EndTag();
    }
    for (HashIterC<StringC, DListC<IntT> > it(m_hSubsets); it; it++) {
      xml.StartTag("Subset");
      xml.SetAttrib("name",it.Key());
      xml.SetAttrib("size",it.Data().Size());
      xml.StartContents();
      for (DLIterC<IntT> sit(it.Data()); sit; sit++) {
	((ostream &)xml) << sit.Data() << ' ';
      }
      RCHashC<StringC,StringC> attrs;
      if(m_subsetAttrib.Lookup(it.Key(),attrs)) {
	for (HashIterC<StringC,StringC> it(attrs); it; it++) {
	  xml << XMLStartTag("Attrib");
	  xml << XMLAttribute("name",it.Key()) << XMLAttribute("value",it.Data());
	  xml << XMLEndTag;
	}
      }
      xml.EndTag();
    }

    // End Tag
    xml.EndTag();

    return xml;
  }

  XMLIStreamC &operator>>(XMLIStreamC &xml,ImagePointFeatureSetC &data) {
    data = ImagePointFeatureSetC(xml);
    return xml;
  }
  
  XMLOStreamC &operator<<(XMLOStreamC &xml, const ImagePointFeatureSetC& data) {
    return data.SaveXML(xml); 
  }

  void InitImagePointFeatureSetFormat() {}
  
  static TypeNameC typeNameImagePointFeatureSetC(typeid(ImagePointFeatureSetC),"RavlImage::ImagePointFeatureSetC");
  
  FileFormatXMLStreamC<ImagePointFeatureSetC> FileFormatXMLStream_ImagePointFeatureSetC;

}

