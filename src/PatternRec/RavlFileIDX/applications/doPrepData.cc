// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2013, React AI Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL) Version 2.1  See the lgpl.licence file
// for details or see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here

#include "Ravl/Option.hh"
#include "Ravl/DP/SequenceIO.hh"
#include "Ravl/DLIter.hh"
#include "Ravl/SArray1d.hh"
#include "Ravl/DP/SPort.hh"
#include "Ravl/SysLog.hh"
#include "Ravl/StringList.hh"
#include "Ravl/TVector.hh"
#include "Ravl/Collection.hh"
#include "Ravl/IO/FileUtilities.hh"

using RavlN::StringC;
using RavlN::FilenameC;
using RavlN::ByteT;
using RavlN::Int64T;

const unsigned g_allSamples = (unsigned) -1;

class BigDataInputFileC
{
public:
  Int64T m_samples;
  RavlN::DPISPortC<RavlN::TVectorC<float> > m_featIn;
  RavlN::DPISPortC<ByteT> m_labIn;
};

class BigDataFeatureIndexC
{
public:
  BigDataFeatureIndexC()
   : m_offset (0),
     m_fileNum(0)
  {}

  BigDataFeatureIndexC(Int64T offset,unsigned fileNum)
   : m_offset (offset),
     m_fileNum(fileNum)
  {}

  Int64T m_offset;
  unsigned m_fileNum;
};

// Write some output files.

class OutputFileSetC
 : public RavlN::RCBodyVC
{
public:
  OutputFileSetC(bool featureOnly)
   : m_featureOnly(featureOnly)
  {}

  OutputFileSetC(RavlN::FilenameC &outFilename,bool featureOnly)
   : m_featureOnly(featureOnly)
  {
    if(!Open(outFilename))
      throw RavlN::ExceptionOperationFailedC("Failed to open files.");
  }

  virtual bool Open(RavlN::FilenameC &outFilename)
  {
    StringC orignalBase;
    StringC oldSubType;

    if(!m_featureOnly) {
      RavlN::ION::FilenameComputeSubType(outFilename,
                                            "Feature",
                                            "idx",
                                            orignalBase,
                                            oldSubType,
                                            testFeatureFilename);
    } else {
      testFeatureFilename = outFilename;
    }
    if(!RavlN::OpenOSequence(testFeatOut,testFeatureFilename)) {
      RavlError("Failed to open feature file '%s' ",testFeatureFilename.c_str());
      return false;
    }


    if(!m_featureOnly) {
      RavlN::ION::FilenameComputeSubType(outFilename,
                                            "Label",
                                            "idx",
                                            orignalBase,
                                            oldSubType,
                                            testLabelFilename);

      if(!RavlN::OpenOSequence(testLabelOut,testLabelFilename)) {
        RavlError("Failed to open feature file '%s' ",testLabelFilename.c_str());
        return false;
      }
    }
    return true;
  }

  //! Write some data.
  virtual void Write(const RavlN::TVectorC<float> &vec,ByteT label)
  {
    testFeatOut.Put(vec);
    if(testLabelOut.IsValid())
      testLabelOut.Put(label);
  }

  // Handle to file set.
  typedef RavlN::SmartPtrC<OutputFileSetC> RefT;
protected:

  RavlN::FilenameC testFeatureFilename;
  RavlN::FilenameC testLabelFilename;

  RavlN::DPOSPortC<RavlN::TVectorC<float> > testFeatOut;
  RavlN::DPOSPortC<ByteT> testLabelOut;
  bool m_featureOnly;
};

// Write sorted set of files.

class OutputSortedFileSetC
 : public OutputFileSetC
{
public:
  OutputSortedFileSetC(bool featureOnly)
    : OutputFileSetC(featureOnly),
      m_fileSet(32)
  {}

  bool Open(RavlN::FilenameC &outFilename)
  {
    m_baseFilename = outFilename;
    return true;
  }

  void Write(const RavlN::TVectorC<float> &vec,ByteT label)
  {
    int index = label;
    // Make sure index exists.
    while(index >= m_fileSet.Size()) {
      m_fileSet.Append(OutputFileSetC::RefT());
    }
    OutputFileSetC::RefT &op = m_fileSet[index];
    if(!op.IsValid()) {
      op = new OutputFileSetC(m_featureOnly);
      RavlN::StringC ext = StringC(".") + m_baseFilename.Extension();
      RavlN::FilenameC labelFilename = m_baseFilename.before(ext,-1) + StringC(index) + ext;
      RavlDebug("Opening file '%s' ",labelFilename.c_str());
      if(!op->Open(labelFilename)) {
        RavlError("Failed to open %s ",labelFilename.c_str());
        throw RavlN::ExceptionOperationFailedC("Failed to open file.");
      }
    }
    op->Write(vec,label);
  }

protected:
  RavlN::FilenameC m_baseFilename;
  RavlN::CollectionC<OutputFileSetC::RefT> m_fileSet;
};


bool ExtractSamples(OutputFileSetC &outFiles,
                    unsigned samples,
                    RavlN::SArray1dC<BigDataInputFileC> &inFiles,
                    RavlN::CollectionC<BigDataFeatureIndexC> &sampleIndex,
                    bool featureOnly
                    )
{
  RavlN::TVectorC<float> featVec;
  ByteT label = 0;
  unsigned maxLabel = 0;

  for(unsigned i = 0;i < samples;i++) {
    if(sampleIndex.IsEmpty()) {
      if(samples != g_allSamples) {
        RavlError("Ran out of samples for test set.");
        return false;
      }
      return true;
    }
    BigDataFeatureIndexC samp = sampleIndex.Pick();
    if(!inFiles[samp.m_fileNum].m_featIn.GetAt(samp.m_offset,featVec)) {
      RavlError("Failed to get sample ");
      return false;
    }
    if(!featureOnly) {
      if(!inFiles[samp.m_fileNum].m_labIn.GetAt(samp.m_offset,label)) {
        RavlError("Failed to get sample ");
        return false;
      }
      if(label > maxLabel)
        maxLabel = label;
    }
    outFiles.Write(featVec,label);
  }
  RavlDebug("Done MaxLabel= %u   Feature size=%u ",maxLabel,featVec.Size().V());
  return true;
}


int main(int nargs,char **argv)
{
  RavlN::OptionC opts(nargs,argv);
  bool infoOnly = opts.Boolean("di",false,"Dump info only.");
  //bool shuffle = opts.Boolean("s",false,"Shuffle data.");
  unsigned testSamples = opts.Int("ts",0,"Number of items to take as test samples.");
  RavlN::StringListC inputFiles= opts.List("l","List of input files");
  bool sortTraining = opts.Boolean("st",false,"Sort training set.");
  bool featureOnly = opts.Boolean("fo",false,"Use features only, ignore labels.");
  FilenameC outputTestFileName = opts.String("ote","test.idx","Output file");
  FilenameC outputTrainFileName = opts.String("otr","train.idx","Output file");
  opts.Check();


  RavlN::SArray1dC<BigDataInputFileC> inFiles(inputFiles.Size());
  unsigned i = 0;

  StringC orignalBase;
  StringC oldSubType;

  RavlDebug("Opening source files.");
  Int64T sampleCount = 0;
  for(RavlN::DLIterC<StringC> it(inputFiles);it;it++,i++) {
    RavlN::FilenameC featureFilename;
    RavlN::FilenameC labelFilename;

    RavlN::ION::FilenameComputeSubType(*it,
                                        "Feature",
                                        "idx",
                                        orignalBase,
                                        oldSubType,
                                        featureFilename);

    RavlN::ION::FilenameComputeSubType(*it,
                                        "Label",
                                        "idx",
                                        orignalBase,
                                        oldSubType,
                                        labelFilename);

    if(featureOnly) {
      if(!RavlN::OpenISequence(inFiles[i].m_featIn,*it)) {
        RavlError("Failed to open feature file '%s' ",it->c_str());
      }
    } else {
      if(!RavlN::OpenISequence(inFiles[i].m_featIn,featureFilename)) {
        RavlError("Failed to open feature file '%s' ",featureFilename.c_str());
        return 1;
      }
    }
    Int64T samples = inFiles[i].m_featIn.Size64();
    if(!featureOnly) {
      if(!RavlN::OpenISequence(inFiles[i].m_labIn,labelFilename)) {
        RavlError("Failed to open label file '%s' ",labelFilename.c_str());
        return 1;
      }
      if(inFiles[i].m_labIn.Size64() != samples) {
        RavlError("Mismatch between number of features and labels in %s ",(*it).c_str());
        return 1;
      }
    }

    inFiles[i].m_samples = samples;
    sampleCount += inFiles[i].m_samples;
    RavlDebug("Samples %s in file %s ",RavlN::StringOf(samples).c_str(),(*it).c_str());
  }

  if(infoOnly)
    return 0;

  RavlDebug("Creating index.");
  RavlN::CollectionC<BigDataFeatureIndexC> sampleIndex(sampleCount);
  for(i = 0;i < inFiles.Size();i++) {
    for(Int64T offset = 0;offset < inFiles[i].m_samples;offset++)
      sampleIndex.Append(BigDataFeatureIndexC(offset, i));
  }

  if(testSamples > 0) {
    RavlDebug("Extracting test samples.");
    OutputFileSetC::RefT outputTestFiles = new OutputFileSetC(featureOnly);
    if(!outputTestFiles->Open(outputTestFileName)) {
      RavlError("Failed to open test output.");
      return false;
    }
    if(!ExtractSamples(*outputTestFiles,
                       testSamples,
                       inFiles,
                       sampleIndex,
                       featureOnly
                       ))
    {
      RavlError("Failed to extract test set.");
      return 1;
    }
  }

  RavlDebug("Extracting training samples.");

  OutputFileSetC::RefT outputFiles;

  if(!sortTraining)
    outputFiles = new OutputFileSetC(featureOnly);
  else {
    RavlDebug("Sorting output.");
    outputFiles = new OutputSortedFileSetC(featureOnly);
  }

  if(!outputFiles->Open(outputTrainFileName)) {
    RavlError("Failed to open train output.");
    return false;
  }

  if(!ExtractSamples(*outputFiles,
                     g_allSamples, // Everything else;
                     inFiles,
                     sampleIndex,
                     featureOnly
                     ))
  {
    RavlError("Failed to extract test set.");
    return 1;
  }

  RavlDebug("Done");
  return 0;
}
