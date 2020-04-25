// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2005, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlAAM
//! file="Ravl/CompVision/ActiveAppearanceModels/AAMAppearance.cc"

#include "Ravl/Image/AAMAppearance.hh"
#include "Ravl/PatternRec/SampleIter.hh"
#include "Ravl/SArray1dIter2.hh"
#include "Ravl/Image/Reflect.hh"
#include "Ravl/StringList.hh"
#include "Ravl/IO.hh"
#include "Ravl/DP/FileFormatIO.hh"
#include "Ravl/Image/DrawCross.hh"

#define DODEBUG 0
#if DODEBUG
#include "Ravl/IO.hh"
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlImageN {

  //: Read in a mirror mapping.
  bool AAMAppearanceMirrorC::ReadMirror(const StringC &filename) {
    IStreamC is(filename);
    if(!is) {
      cerr << "Failed to open mirror file '" << filename << "' \n";
      return false;
    }
    while(is) {
      StringC line;
      readline(is,line);
      SubStringC ss = line.TopAndTail();
      if(ss.Size() <= 0)
        continue; // Skip empty lines.
      if(ss.firstchar() == '#')
        continue; // Skip comments.
      IntT id1,id2;
      //cerr << "Got line:" << line << "\n";
      int sep = ss.index(' ');
      SubStringC s1 = ss.before(sep);
      SubStringC s2 = ss.after(sep);
      //cerr << "Got line:-" << s1 << "-" << s2  << "-\n";
      id1 = s1.IntValue();
      id2 = s2.IntValue();

      if(mirror.IsElm(id1))
        cerr << "WARNING: Duplicate entry found for id1 " << id1 << " reading mirror file '" << filename << "'\n";
      mirror[id1] = id2;
      if(id1 != id2) {
        if(mirror.IsElm(id2))
          cerr << "WARNING: Duplicate entry found for id2 " << id2 << " reading mirror file '" << filename << "'\n";
        mirror[id2] = id1;
      }
    }

    UIntT size = mirror.Size();
    map = SArray1dC<UIntT>(size);
    for(UIntT i = 0;i < size;i++)
      map[i] = mirror[i];

    return true;
  }

  //: Reflect a sample set and append it at the end of sample
  bool AAMAppearanceMirrorC::Reflect(SampleC<AAMAppearanceC> &sample) const {
    SampleC<AAMAppearanceC> reflection(sample.Size());
    for(SampleIterC<AAMAppearanceC> it(sample);it;it++) {
      AAMAppearanceC appear = Reflect(*it);
      reflection.Append(appear);
    }

    sample.Append(reflection);
    return true;
  }

  //: Return reflected appearance
  AAMAppearanceC AAMAppearanceMirrorC::Reflect(const AAMAppearanceC &appear) const {
      UIntT size = map.Size();
      RavlAssert(appear.Points().Size() == size);
      SArray1dC<Point2dC> newun(appear.Points().Size());
      const SArray1dC<Point2dC> &old = appear.Points();

      RealT offset = 0;
      ImageC<ByteT> nimg;
      if(appear.Image().IsValid()) {
        ReflectVertical(appear.Image(),nimg);
        offset = (RealT) (nimg.Range2().Max() + nimg.Range2().Min());
      }
      else
      {
        Point2dC mid(0,0);
        for(SArray1dIterC<Point2dC> ait(old);ait;ait++)
          mid += *ait;
        mid /= size;
        offset = mid[1] * 2;
      }

      for(SArray1dIter2C<Point2dC,UIntT> pit(newun,map);pit;pit++) {
        const Point2dC &oth = old[pit.Data2()];
        pit.Data1() = Point2dC(oth[0],offset - oth[1]);
      }
      AAMAppearanceC reflection(newun,nimg);
      StringC source_file = appear.SourceFile();
      reflection.SourceFile() =  StringListC(source_file, ".").First()+"_reflect.xml";

      return reflection;
  }


  //: Draw appearance.
  //  Draws the image and the control points represented as white crosses.
  bool AAMAppearanceC::Draw(const StringC &filename,StringC fileformat,bool verbose) const {
    ImageC<ByteT> img = image.Copy();
    ByteT byte(255);
    for(SArray1dIterC<Point2dC> it(points);it;it++) {
      Index2dC ind(it.Data().X(), it.Data().Y());
      DrawCross(img,byte, ind, 2);
    }
    if(!RavlN::Save(filename,img,fileformat,verbose))
    {
      cerr << "Warning: unable to save appearance image to " << filename << endl;
      return false;
    }

    return true;
  }

}

