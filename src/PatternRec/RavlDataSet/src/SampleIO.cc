// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/DataSet/VectorLabelIO.cc"

#include "Ravl/PatternRec/SampleIO.hh"
#include "Ravl/IO.hh"
#include "Ravl/SysLog.hh"
#include "Ravl/OS/Filename.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {

  //! Load SampleVectorC from a file
  bool LoadSampleVector(const StringC & filename, SampleVectorC & sample) {

    FilenameC fname(filename);
    if (fname.HasExtension("csv")) {
      return LoadSampleVectorCSV(filename, sample);
    } else {
      return Load(filename, sample);
    }
    // should never get here
    return false;
  }

  //! Save SampleVectorC from a file
  bool SaveSampleVector(const StringC & filename, SampleVectorC & sample) {
    FilenameC fname(filename);
    if (fname.HasExtension("csv")) {
      return SaveSampleVectorCSV(filename, sample);
    } else {
      return Save(filename, sample);
    }
    // should never get here
    return false;
  }

  //! A function that loads a DataSetVectorLabel from a CSV file.
  bool LoadSampleVectorCSV(const StringC & filename, SampleVectorC & sample) {

    // Open output file and check all OK!
    FilenameC fname(filename);
    if (!fname.Exists()) {
      RavlError("Dataset file does not exist for loading '%s'", fname.data());
      return false;
    }

    TextFileC textFile(filename);
    // Now the first line might be the headings - we need to guess
    UIntT startLine = 1;
    StringListC headings(textFile[1].TopAndTail(), ",");
    for (DLIterC<StringC> it(headings); it; it++) {
      if (!it.Data().RealValue()) {
        startLine = 2;
        break;
      }
    }

    // Now lets put something into field information
    SArray1dC<FieldInfoC> fieldInfo(headings.Size());
    UIntT c = 0;
    for (DLIterC<StringC> it(headings); it; it++) {
      if (startLine == 2) {
        fieldInfo[c] = FieldInfoC(*it);
      } else {
        fieldInfo[c] = "Unknown_" + (StringC) c;
      }
      c++;
    }

    sample = SampleVectorC(textFile.NoLines() - startLine + 1, fieldInfo);
    for (UIntT i = startLine; i <= textFile.NoLines(); i++) {
      StringListC line(textFile[i], ",");
      VectorC vec(line.Size());
      UIntT c = 0;
      for (DLIterC<StringC> it(line); it; it++) {
        vec[c] = it.Data().RealValue();
        c++;
      }
      sample += vec;
    }
    return true;
  }

  //: Save sample to a CSV file

  bool SaveSampleVectorCSV(const StringC & filename, const SampleVectorC & sample) {

    OStreamC os(filename);
    if (!os.good())
      return false;

    // No point if empty sample size
    if (sample.Size() < 1) {
      RavlError("No samples to save to file '%s'", filename.data());
      return false;
    }

    // Put in some headings if required
    if (sample.FieldInfo().Size() > 0) {

      // check dimensions match
      if (sample.First().Size() != sample.FieldInfo().Size()) {
        RavlError("Samples dimension does not match number of headings");
        return false;
      }

      // save the headings
      for (SArray1dIterC<FieldInfoC> it(sample.FieldInfo()); it; it++) {
        os << *it;
        if (!it.IsLast()) {
          os << ",";
        }
      }
      // and a new line
      os << "\n";
    }

    // Save the sample stream
    for (SampleIterC<VectorC> it(sample); it; it++) {
      for (Array1dIterC<RealT> vecIt(*it); vecIt; vecIt++) {
        os << *vecIt;
        if (!vecIt.IsLast()) {
          os << ",";
        }
      }
      os << "\n";
    }

    // All OK
    return true;
  }

}
