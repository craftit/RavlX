// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2005, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_IMAGEPOINTFEATURESET_HEADER
#define RAVLIMAGE_IMAGEPOINTFEATURESET_HEADER 1
//! rcsid="$Id$"
//! lib=RavlImageProc
//! docentry="Ravl.API.Images.Feature Points"
//! userlevel="Develop"
//! file="Ravl/Image/Processing/Tools/ImagePointFeatureSet.hh"

#include "Ravl/RefCounter.hh"
#include "Ravl/XMLStream.hh"
#include "Ravl/DList.hh"
#include "Ravl/Hash.hh"
#include "Ravl/RCHash.hh"
#include "Ravl/Tuple2.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/Image/ImagePointFeature.hh"

namespace RavlImageN {

  using namespace RavlN;
  
  //! userlevel=Develop
  //: Feature set body.
  
  class ImagePointFeatureSetBodyC : public RCBodyC {

  public:

    ImagePointFeatureSetBodyC();
    //: Constructor

  protected: 
   
    ImagePointFeatureSetBodyC(XMLIStreamC& xml);
    //: Constructor from XML stream

    XMLOStreamC& SaveXML(XMLOStreamC &xml) const;
    //: Save to xml stream
  
  public:

    RCBodyC &Copy() const;
    //: Make copy of body.
    // This should be provided in derived classes.
    // this funtion will issue an assertion failure if called.

    IntT Size(void) const 
    { return m_hFeatures.Size(); }
    //: How many features?

    FilenameC ImageFile(void) const
    { return m_strImageFile; }
    //: The image filename

    FilenameC& ImageFile(void)
    { return m_strImageFile; }
    //: RW access to the image filename
    
    void AddFeature(ImagePointFeatureC& feat);
    //: Add a new feature to the set.

    void AddLine(IntT from, IntT to);
    //: Add a new line to the set
    
    void AddSubset(const StringC& name, const DListC<IntT>& subset);
    //: Adds an ordered feature subset
    // If a subset with the same name already exists, it will be overwritten
    
    int Find(Point2dC position, RealT tolerance) const;
    //: Find ID of closest feature within <code>tolerance</code> of <code>position</code>
    // Returns -1 if no features are in range

    bool Move(IntT id, Point2dC change);
    //: Move a particular feature by <code>change</code>
    // Note: The feature must already exist in the set.
    // Returns false if id could not be found

    bool Set(IntT id, Point2dC position);
    //: Set a particular feature to the passed position
    // Note: The feature must already exist in the set.
    // Returns false if id could not be found

    bool Set(const StringC& desc, Point2dC position);
    //: Set a particular feature to the passed position
    // Note: The feature must already exist in the set.
    // Returns false if id could not be found
    
    Point2dC Position(const StringC &name) const;
    //: Get the position of a named feature.
    // An ExceptionOperationFailedC exception is thrown if feature is not in set.
    
    Point2dC Position(UIntT id) const;
    //: Get the position of a  feature by id
    // An ExceptionOperationFailedC exception is thrown if feature is not in set.
    
    bool Feature(IntT id, ImagePointFeatureC& feature) const;
    //: Get a feature 
    // Feature is returned in <code>feature</code>.
    // Returns false if id could not be found.
    
    bool DeleteFeature(IntT id);
    //: Delete feature from set.
    
    bool DeleteSubset(const StringC &name);
    //: Delete feature from set.
    
    bool Feature(const StringC& desc, ImagePointFeatureC& feature) const;
    //: Get a feature by string
    // Feature is returned in <code>feature</code>.
    // Returns false if id could not be found.
 
    bool Rename(const StringC& oldName, const StringC& newName);
    //: Rename an individual feature
    // Returns false on failure

    bool Rename(IntT id, const StringC& newName);
    //: Rename an individual feature
    // Returns false on failure

    bool Subset(StringC name, DListC<IntT>& subset);
    //: Get an ordered subset
    // Subset is returned in <code>subset</code>.
    // Returns false if name could not be found.

    HashIterC<IntT, ImagePointFeatureC> FeatureIterator(void) const
    { return HashIterC<IntT, ImagePointFeatureC>(m_hFeatures); }
    //: Gets an iterator which will give access to every feature in the set
    
    DLIterC<Tuple2C<IntT, IntT> > LineIterator(void) const
    { return DLIterC<Tuple2C<IntT, IntT> >(m_lLines); }
    //: Gets an iterator which will give access to every line in the set

    HashIterC<StringC, DListC<IntT> > SubsetIterator(void) const
    { return HashIterC<StringC, DListC<IntT> >(m_hSubsets); }
    //: Gets an iterator for the subset list

    DLIterC<IntT> SingleSubsetIterator(StringC name) const
    { return DLIterC<IntT>(m_hSubsets[name]); }
    //: Gets an iterator for a single subset
    
    UIntT NoSubsets() const
    { return m_hSubsets.Size(); }
    //: Number of subsets.
    
    StringC &DatabaseID()
    { return m_strDatabase; }
    //: Database this annotation came from.
    
    bool IsSuspect() const
    { return suspect; }
    //: Is this markup suspect ?
    
    bool Suspect(bool sus)
    { return suspect = sus; }
    //: Set suspect flag.
    
    RCHashC<StringC,StringC> &Attributes()
    { return m_attribs; }
    //: Access feature set attributes.
    
    RCHashC<StringC,StringC> &SubsetAttrib(const StringC &name)
    { return m_subsetAttrib[name]; }
    //: Get attributes for a subset.
    
  protected:
    RCHashC<StringC,StringC> m_attribs;
    //: Attributes for whole markup.
    
    HashC<IntT, ImagePointFeatureC> m_hFeatures;
    //: Features
    // Keyed by ID number
    
    HashC<StringC, ImagePointFeatureC> m_hFeaturesByStr;
    //: Features
    // Keyed by description
    
    HashC<StringC, DListC<IntT> > m_hSubsets;
    //: Ordered feature subsets
    // Keyed by name
    
    HashC<StringC,RCHashC<StringC,StringC> > m_subsetAttrib;
    //: Attributes for subsets.
    
    DListC<Tuple2C<IntT, IntT> > m_lLines;
    //: Lines

    FilenameC m_strImageFile;
    //: The image file that this feature set refers to
    
    StringC m_strDatabase;
    //: Database this annotation came from.
    
    bool suspect;
    //: Is this markup suspect ?
    
    friend class ImagePointFeatureSetC;

  };

  //! userlevel="Basic"
  //: Feature set.
  
  class ImagePointFeatureSetC : public RCHandleC<ImagePointFeatureSetBodyC> {

  public:

    ImagePointFeatureSetC() 
    {}
    //: Default constructor
    // Creates invalid handle
    
    explicit ImagePointFeatureSetC(bool create) 
      : RCHandleC<ImagePointFeatureSetBodyC>(*new ImagePointFeatureSetBodyC)
    {}
    //: Constructor
    // Creates empty feature set
    
  protected:

    ImagePointFeatureSetC(XMLIStreamC& xml) 
      : RCHandleC<ImagePointFeatureSetBodyC>(*new ImagePointFeatureSetBodyC(xml))
    {}
    //: Constructor
    // Creates feature set from an XML stream

    XMLOStreamC& SaveXML(XMLOStreamC &xml) const 
    { return Body().SaveXML(xml); }
    //: Save to xml stream

  protected:
    
    ImagePointFeatureSetC(ImagePointFeatureSetBodyC &bod)
      : RCHandleC<ImagePointFeatureSetBodyC>(bod)
    {}
    //: Body constructor.

    ImagePointFeatureSetBodyC &Body()
    { return RCHandleC<ImagePointFeatureSetBodyC>::Body(); }
    //: Access body.
    
    const ImagePointFeatureSetBodyC &Body() const
    { return RCHandleC<ImagePointFeatureSetBodyC>::Body(); }
    //: Access body (const)

  public:
    
    ImagePointFeatureSetC Copy() const
    { return ImagePointFeatureSetC(static_cast<ImagePointFeatureSetBodyC &>(Body().Copy())); }
    //: Make a deep copy of this object.
    
    IntT Size(void) const 
    { return Body().Size(); }
    //: How many features?

    FilenameC ImageFile(void) const
    { return Body().ImageFile(); }
    //: The image filename

    FilenameC& ImageFile(void)
    { return Body().ImageFile(); }
    //: RW access to the image filename

    void AddFeature(ImagePointFeatureC feat) 
    { Body().AddFeature(feat); }
    //: Add a new feature to the set.
    
    void AddLine(IntT from, IntT to)
    { Body().AddLine(from,to); }
    //: Add a new line to the set

    void AddSubset(const StringC& name, const DListC<IntT>& subset)
    { Body().AddSubset(name,subset); }
    //: Adds an ordered feature subset
    // If a subset with the same name already exists, it will be overwritten

    int Find(Point2dC position, RealT tolerance=10.0) const
    { return Body().Find(position,tolerance); }
    //: Find ID of closest feature within <code>tolerance</code> of <code>position</code>
    // Returns -1 if no features are in range

    bool Move(IntT id, Point2dC change)
    { return Body().Move(id,change); }
    //: Move a particular feature by <code>change</code>
    // Returns false if feature could not be found

    bool Set(IntT id, Point2dC position)
    { return Body().Set(id,position); }
    //: Set a particular feature to the passed position
    // Returns false if id could not be found

    bool Set(const StringC & desc, Point2dC position)
    { return Body().Set(desc,position); }
    //: Set a particular feature to the passed position
    // Returns false if id could not be found
    
    Point2dC Position(const StringC &name) const
    { return Body().Position(name); }
    //: Get the position of a named feature.
    // An ExceptionOperationFailedC exception is thrown if feature is not in set.
    
    Point2dC Position(UIntT id) const
    { return Body().Position(id); }
    //: Get the position of a feature from an id
    // An ExceptionOperationFailedC exception is thrown if feature is not in set.
    
    bool Feature(IntT id, ImagePointFeatureC& feature) const
    { return Body().Feature(id,feature); }
    //: Get a feature 
    // Feature is returned in <code>feature</code>.
    // Returns false if id could not be found.

    bool Feature(const StringC& desc, ImagePointFeatureC& feature) const 
    { return Body().Feature(desc,feature); }
    //: Get a feature by string
    // Feature is returned in <code>feature</code>.
    // Returns false if id could not be found.

    bool Rename(const StringC& oldName, const StringC& newName)
    { return Body().Rename(oldName,newName); }
    //: Rename an individual feature
    // Returns false on failure

    bool Rename(IntT id, const StringC& newName)
    { return Body().Rename(id,newName); }
    //: Rename an individual feature
    // Returns false on failure

    bool Subset(StringC name, DListC<IntT>& subset) 
    { return Body().Subset(name,subset); }
    //: Get an ordered subset
    // Subset is returned in <code>subset</code>.
    // Returns false if name could not be found.

    HashIterC<IntT, ImagePointFeatureC> FeatureIterator(void) const
    { return Body().FeatureIterator(); }
    //: Gets an iterator which will give access to every feature in the set

    DLIterC<Tuple2C<IntT, IntT> > LineIterator(void) const
    { return Body().LineIterator(); }
    //: Gets an iterator which will give access to every line in the set

    HashIterC<StringC, DListC<IntT> > SubsetIterator(void) const
    { return Body().SubsetIterator(); }
    //: Gets an iterator for the subset list

    DLIterC<IntT> SingleSubsetIterator(StringC name) const
    { return Body().SingleSubsetIterator(name); }
    //: Gets an iterator for a single subset
    
    bool DeleteFeature(IntT id)
    { return Body().DeleteFeature(id); }
    //: Delete feature from set.
    
    bool DeleteSubset(const StringC &name)
    { return Body().DeleteSubset(name); }
    //: Delete feature from set.

    UIntT NoSubsets() const
    { return Body().NoSubsets(); }
    //: Number of subsets.
    
    StringC &DatabaseID()
    { return Body().DatabaseID(); }
    //: Database this annotation came from.
    
    bool IsSuspect() const
    { return Body().IsSuspect(); }
    //: Is this markup suspect ?
    
    bool Suspect(bool sus)
    { return Body().Suspect(sus); }
    //: Set suspect flag.
    
    RCHashC<StringC,StringC> &Attributes()
    { return Body().Attributes(); }
    //: Access feature set attributes.
    
    RCHashC<StringC,StringC> &SubsetAttrib(const StringC &name)
    { return Body().SubsetAttrib(name); }
    //: Get attributes for a subset.
    
    friend XMLIStreamC &operator>>(XMLIStreamC &xml, ImagePointFeatureSetC &data);
    friend XMLOStreamC &operator<<(XMLOStreamC &xml, const ImagePointFeatureSetC &data);
  };
  
  XMLIStreamC &operator>>(XMLIStreamC &xml, ImagePointFeatureSetC &data);
  //: XML Input
  // DO NOT USE DIRECTLY unless you're using this inside another XML loader and you know what you're doing.
  // Use Load() instead.

  XMLOStreamC &operator<<(XMLOStreamC &xml, const ImagePointFeatureSetC &data);
  //: XML Output
  // DO NOT USE DIRECTLY unless you're using this inside another XML output func and you know what you're doing.
  // Use Save() instead.

};


#endif
