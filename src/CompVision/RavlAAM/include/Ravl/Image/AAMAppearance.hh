// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2005, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_APPEARANCE_HEADER
#define RAVLIMAGE_APPEARANCE_HEADER 1
//! rcsid="$Id$"
//! lib=RavlAAM
//! file="Ravl/CompVision/ActiveAppearanceModels/AAMAppearance.hh"
//! docentry="Ravl.API.Images.AAM"
//! userlevel=Normal
//! author="Charles Galambos"
//! example = "aamBuildShapeModel.cc;aamBuildAppearanceModel.cc;aamBuildActiveAppearanceModel.cc"

#include "Ravl/SArray1d.hh"
#include "Ravl/Point2d.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/RCHash.hh"
#include "Ravl/PatternRec/Sample.hh"
#include "Ravl/Array2dIter2.hh"

namespace RavlImageN {

  using namespace RavlImageN;
  using namespace RavlN;

  //! userlevel=Normal
  //: Appearance.
  // An appearance consists of an image and a set of control points on the image.

  class AAMAppearanceC {
  public:
    AAMAppearanceC()
    {}
    //: Default constructor.

    AAMAppearanceC(const SArray1dC<Point2dC> &npoints,const ImageC<RealT> &img)
      : points(npoints)
    {
      image = ImageC<ByteT>(img.Frame());
      for(Array2dIter2C<ByteT,RealT> it(image,img);it;it++)
        it.Data1() = (ByteT) it.Data2();
    }
    //: Constructor.
    //  Creates an appearance made of image 'img' and control points 'npoints'.

    AAMAppearanceC(const SArray1dC<Point2dC> &npoints,const ImageC<ByteT> &img)
      : points(npoints),
        image(img)
    {}
    //: Constructor.
    //  Creates an appearance made of image 'img' and control points 'npoints'.

    AAMAppearanceC(const SArray1dC<Point2dC> &npoints)
      : points(npoints)
    {}
    //: Constructor.
    //  Creates an appearance with blank image and control points 'npoints'.

    SArray1dC<Point2dC> &Points()
    { return points; }
    //: Access control point list.

    const SArray1dC<Point2dC> &Points() const
    { return points; }
    //: Access control point list.

    ImageC<ByteT> &Image()
    { return image; }
    //: Access image.

    const ImageC<ByteT> &Image() const
    { return image; }
    //: Access image.

    StringC &SourceFile()
    { return sourceFile; }
    //: Source file.

    const StringC &SourceFile() const
    { return sourceFile; }
    //: Source file.

    bool IsValid() const
    { return points.Size() > 0; }
    //: Test that the appearance is valid, i.e. that it contains at least one control point.

    bool Draw(const StringC & filename,StringC fileformat = "",bool verbose = false) const;
    //: Draw appearance.
    //  Draws the image and the control points represented as white crosses.

  protected:
    StringC sourceFile;         // Path to the source file defining the appearance.
    SArray1dC<Point2dC> points; // Control points in image.
    ImageC<ByteT> image;        // Image.
  };

  inline
  BinOStreamC &operator<<(BinOStreamC &s,const AAMAppearanceC &ap) {
    s << ap.Points() << ap.Image();
    return s;
  }
  //: Write appearance to binary stream.

  inline
  BinIStreamC &operator>>(BinIStreamC &s,AAMAppearanceC &ap) {
    s >> ap.Points() >> ap.Image();
    return s;
  }
  //: Read appearance from binary stream.

  inline
  ostream &operator<<(ostream &s,const AAMAppearanceC &ap) {
    s << ap.Points() << ' ' << ap.Image();
    return s;
  }
  //: Write appearance to stream.

  inline
  istream &operator>>(istream &s,AAMAppearanceC &ap) {
    s >> ap.Points() >> ap.Image();
    return s;
  }
  //: Read appearance from stream.

  //:-

  //! userlevel=Normal
  //: Appearance mirror.
  //  Used to reflect an appearance about the vertical axis.

  class AAMAppearanceMirrorC
  {
  public:
    AAMAppearanceMirrorC()
    {}
    //: Default constructor.

    AAMAppearanceMirrorC(const StringC &filename)
    { ReadMirror(filename); }
    //: Constructor.
    //  Builds appearance mirror from file.

    bool ReadMirror(const StringC &filename);
    //: Read in a mirror mapping.

    bool Reflect(SampleC<AAMAppearanceC> &sample) const;
    //: Reflect a sample set and append it at the end of sample

    AAMAppearanceC Reflect(const AAMAppearanceC &appear) const;
    //: Return reflected appearance

    bool IsValid() const
    { return map.Size() > 0; }
    //: Is this a valid mirror ?

  protected:
    RCHashC<IntT,IntT> mirror; // Defines mapping of each point under mirror transformation.
    SArray1dC<UIntT> map;      // List of indices of the mirrored points.
  };

}



#endif
