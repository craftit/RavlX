// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2005, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlAAM
//! file="Ravl/CompVision/ActiveAppearanceModels/AAMActiveAppearanceModel.cc"

#include "Ravl/Image/AAMActiveAppearanceModel.hh"
#include "Ravl/Image/AAMAppearanceUtil.hh"
#include "Ravl/Image/AAMSampleStream.hh"
#include "Ravl/Image/AAMSampleStreamFileList.hh"
#include "Ravl/PatternRec/DesignFuncLSQ.hh"
#include "Ravl/PatternRec/FuncLinear.hh"
#include "Ravl/PatternRec/FuncLinearCoeff.hh"
#include "Ravl/PatternRec/SampleVector.hh"
#include "Ravl/PatternRec/SampleIter.hh"
#include "Ravl/Affine2d.hh"
#include "Ravl/Image/WarpAffine.hh"
#include "Ravl/SArray1dIter2.hh"
#include "Ravl/SArray1dIter3.hh"
#include "Ravl/Array2dIter2.hh"
#include "Ravl/Angle.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/Random.hh"
#include "Ravl/DList.hh"
#include "Ravl/String.hh"
#include "Ravl/DLIter.hh"
#include "Ravl/Slice1d.hh"
#include "Ravl/Slice1dIter.hh"
#include "Ravl/MatrixRUT.hh"
#include "Ravl/Matrix.hh"
#include "Ravl/MatrixRS.hh"
#include "Ravl/SArray1d.hh"
#include "Ravl/DP/FileFormatIO.hh"
#include "Ravl/Image/GaussConvolve2d.hh"
#include "Ravl/OS/SysLog.hh"

#define DODEBUG 0
#if DODEBUG
#include "Ravl/OS/Date.hh"
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlImageN {

  using namespace RavlN;

  //: Constructor.
  AAMActiveAppearanceModelBodyC::AAMActiveAppearanceModelBodyC()
    : smooth(3)
  {}

  //: Load from bin stream.
  AAMActiveAppearanceModelBodyC::AAMActiveAppearanceModelBodyC(BinIStreamC &is)
    : RCBodyVC(is)
  {
    int version;
    is >> version;
    if(version != 1)
      throw ExceptionOutOfRangeC("AAMActiveAppearanceModelC:SAAMActiveAppearanceModelC(BinIStreamC &s), Bad version number in stream. ");

    is >> refHome >> appearanceModel >> refiner >> smooth;
  }

  //: Load from stream.
  AAMActiveAppearanceModelBodyC::AAMActiveAppearanceModelBodyC(istream &is)
    : RCBodyVC(is)
  {
    int version;
    is >> version;
    if(version != 1)
      throw ExceptionOutOfRangeC("AAMActiveAppearanceModelC:SAAMActiveAppearanceModelC(istream &s), Bad version number in stream. ");

    is >> refHome >> appearanceModel >> refiner >> smooth;
  }

  //: Save to binary stream 'out'.
  bool AAMActiveAppearanceModelBodyC::Save(BinOStreamC &out) const {
    if(!RCBodyVC::Save(out))
      return false;
    int version = 1;
    out << version << refHome << appearanceModel << refiner << smooth;
    return true;
  }

  //: Save to stream 'out'.
  bool AAMActiveAppearanceModelBodyC::Save(ostream &out) const {
    if(!RCBodyVC::Save(out))
      return false;
    int version = 1;
    out << ' ' << version << ' ' << ' ' << refHome << ' ' << appearanceModel << ' ' << refiner << ' ' << smooth;
    return true;
  }

  //: Returns the optimum appearance parameters for the image 'img' given an initial estimate 'paramEstimate'.
  //  The optimum parameters are the ones which minimise the residual error between model and image measured in normalised texture frame (i.e. shape free images).
  VectorC AAMActiveAppearanceModelBodyC::RefinePose(const ImageC<ByteT> &img, const VectorC &paramEstimate) const {
    RealT diff;
    return RefinePose(img,paramEstimate,diff);
  }

  //: Returns the optimum appearance parameters for the image 'img' given an initial estimate 'paramEstimate'.
  //  The optimum parameters are the ones which minimise the residual error between model and image measured in normalised texture frame (i.e. shape free images).
  // This function also returns the value of the corresponding residual error in 'diff'.
  VectorC AAMActiveAppearanceModelBodyC::RefinePose(const ImageC<ByteT> &img, const VectorC &paramEstimate, RealT &diff) const {
    ImageC<RealT> rimg(img.Frame());
    for(Array2dIter2C<RealT,ByteT> it(rimg,img);it;it++)
      it.Data1() = (RealT) it.Data2();

    return RefinePose(rimg,paramEstimate,diff);
  }

  //: Returns the optimum appearance parameters for the image 'img' given an initial estimate 'paramEstimate'.
  //  The optimum parameters are the ones which minimise the residual error between model and image measured in normalised texture frame (i.e. shape free images).
  // This function also returns the value of the corresponding residual error in 'diff'.
  VectorC AAMActiveAppearanceModelBodyC::RefinePose(const ImageC<RealT> &rimgi, const VectorC &paramEstimate, RealT &diff) const {

    ImageC<RealT> rimg(rimgi.Frame());
    rimg = smooth.Apply(rimgi); // Filter the image a little.

    VectorC lastParm = paramEstimate.Copy();
    VectorC errVec,lastErr;

    // Compute residual error.
    if(appearanceModel.ErrorVector(lastParm,rimg,errVec)==false) {
      cerr << "WARNING: Input shape exceeds image size.\n";
      // reduces the scale of the face so that it fits in the image
      UIntT max_iter = 0;
      do
      {
        lastParm[2] = 0.95*lastParm[2];
        max_iter++;
      } while(appearanceModel.ErrorVector(lastParm,rimg,errVec)==false && max_iter<20);
    }
    diff = errVec.SumOfSqr()/errVec.Size();
    RealT oldDiff;
    UIntT iters =0;
    do {
      oldDiff = diff;

      VectorC newDelta = refiner.Apply(errVec);
      VectorC newEst = lastParm - newDelta;
      RealT nErr;
      // Compute residual error.
      if (appearanceModel.ErrorVector(newEst,rimg,errVec)==false) {
        // parameter choice leads to shape exceeding image size -> maximum error
        nErr = RavlConstN::maxReal;
      } else {
        nErr = errVec.SumOfSqr()/errVec.Size();
      }
      iters++;
      if(nErr < diff) {
        lastParm = newEst;
        diff = nErr;
        continue;
      }
      RealT mul = 1.5;
      UIntT scanLimit = 6;
      // Go through successively smaller steps until we find one thats better.
      VectorC newParam = lastParm;
      for(UIntT i = 0;i < scanLimit;i++,mul /= 1.7) {
        VectorC newEst = lastParm - newDelta * mul;
        RealT nErr;
        // Compute residual error.
        if (appearanceModel.ErrorVector(newEst,rimg,errVec)==false) {
          // parameter choice leads to shape exceeding image size -> maximum error
          nErr = RavlConstN::maxReal;
          continue;
        } else {
          nErr = errVec.SumOfSqr()/errVec.Size();
        }
        if(nErr < diff) {
          newParam = newEst;
          diff = nErr;
          continue;
        }
      }
      lastParm = newParam;
#if 0
      // This code does some extra optimisation which avoids problems
      // with the standard AAM optimisation, it does take longer however
      // This should be made optional via a member in the class.
      if(diff >= oldDiff ) {
        for(int i = 0;i < lastParm.Size();i++) {
          RealT change = 0.0001;
          for(int j = 0;j < 2;j++) {
            VectorC testParam = newParam.Copy();
            if(j == 0)
              change = 0.000001;
            else
            change = -0.000001;
            do {
              testParam[i] += change ;

              // Compute residual error.
              if (appearanceModel.ErrorVector(testParam,rimg,errVec)==false) {
                // parameter choice leads to shape exceeding image size -> maximum error
                nErr = RavlConstN::maxReal;
                break;
              } else {
                nErr = errVec.SumOfSqr()/errVec.Size();
              }
              // Did we manage to do better?
              if(nErr < diff) {
                change *= 2;
              newParam = newEst;
              diff = nErr;
              continue;
            }
            } while(0);
          }
        }
        lastParm = newParam;
      }
#endif
    }
    while(diff < oldDiff);

    cerr << "Finished refinement. Raw Error=" << diff << "\t"  << iters << "\n";

    return lastParm;
  }

  //: Train the AAM given some data and an existing appearance model.
  //!param: apm        - input appearance model.
  //!param: fileList   - list of file names for training.
  //!param: dir        - name of directory containing training images.
  //!param: mirrorFile - name of mirror file to use for mirror appearances (if an empty string is provided, mirror images will not be considered).
  //!param: incrSize   - half number of displacements for each parameter when perturbing the model.
  // This functions considers each training appearance successively and perturbs the different parameters in order to analyse the effect of errors in the parameters on the residual error. The range of displacement is +/-0.5 standard deviation for each parameter. A value of n for 'incrSize' means that there will be 2n displacements uniformly spread on the interval [-0.5std,+0.5std] for each parameter.
  bool AAMActiveAppearanceModelBodyC::Design(const AAMAppearanceModelC &apm,
                                             const DListC<StringC> &fileList,
                                             const StringC &dir,
                                             const StringC &mirrorFile,
                                             UIntT incrSize,
                                             bool ignoreSuspect
                                             )
  {
    HashC<IntT,IntT> typeMap;
    HashC<StringC,IntT> namedTypeMap;
    bool useTypeId;
    AAMSampleStreamFileListC stream(fileList, dir, mirrorFile, typeMap, namedTypeMap, useTypeId, ignoreSuspect, true);
    return Design(apm, stream, incrSize);
  }

  //: Train the AAM given some data and an existing appearance model.
  //!param: apm        - input appearance model.
  //!param: sample     - list of appearance samples.
  //!param: dir        - name of directory containing training images.
  //!param: mirrorFile - name of mirror file to use for mirror appearances (if an empty string is provided, mirror images will not be considered).
  //!param: incrSize   - half number of displacements for each parameter when perturbing the model.
  // This functions considers each training appearance successively and perturbs the different parameters in order to analyse the effect of errors in the parameters on the residual error. The range of displacement is +/-0.5 standard deviation for each parameter. A value of n for 'incrSize' means that there will be 2n displacements uniformly spread on the interval [-0.5std,+0.5std] for each parameter.
  bool AAMActiveAppearanceModelBodyC::Design(const AAMAppearanceModelC &apm,
                                             SampleStreamC<AAMAppearanceC> &sample,
                                             UIntT incrSize
                                             )
  {
    SysLog(SYSLOG_DEBUG) << "AAMActiveAppearanceModelBodyC::Design(), Called.";
    appearanceModel = apm;
    AAMSampleStreamC ss(appearanceModel, smooth, sample, incrSize);
    DesignFuncLSQC design(1,false);
    refiner = design.Apply(ss);
    SysLog(SYSLOG_DEBUG) << "AAMActiveAppearanceModelBodyC::Design(), Done.";

    // Should check how well the function works ?
    return true;
  }

  //: Pre-process some data before final design of the AAM.
  //!param: apm        - input appearance model.
  //!param: fileList   - list of file names for training.
  //!param: dir        - name of directory containing training images.
  //!param: mirrorFile - name of mirror file to use for mirror appearances (if an empty string is provided, mirror images will not be considered).
  //!param: incrSize   - half number of displacements for each parameter when perturbing the model.
  //!param: op         - name of output file containing results of training for this list of file.
  // This functions considers each training appearance successively and perturbs the different parameters in order to analyse the effect of errors in the parameters on the residual error. The range of displacement is +/-0.5 standard deviation for each parameter. A value of n for 'incrSize' means that there will be 2n displacements uniformly spread on the interval [-0.5std,+0.5std] for each parameter.
  // Note: contrary to the Design method, PreDesign does not produce a complete appearance model. PreDesign needs to be followed by PostDesign in order to obtain the AAM. PreDesign allows to split the training of the AAM (which is usually computer intensive because of the number of files to process) into a large number of smaller jobs which can be run in parallel.
  bool AAMActiveAppearanceModelBodyC::PreDesign(const AAMAppearanceModelC &apm,
                                                const DListC<StringC> &fileList,
                                                const StringC &dir,
                                                const StringC &mirrorFile,
                                                UIntT incrSize,
                                                const StringC &op,
                                                bool ignoreSuspect)
  {
    HashC<IntT,IntT> typeMap;
    HashC<StringC,IntT> namedTypeMap;
    bool useTypeId;
    AAMSampleStreamFileListC stream(fileList, dir, mirrorFile, typeMap, namedTypeMap, useTypeId, ignoreSuspect, true);
    return PreDesign(apm, stream, incrSize, op);
  }

  //: Pre-process some data before final design of the AAM.
  //!param: apm        - input appearance model.
  //!param: sample     - list of sample appearances.
  //!param: incrSize   - half number of displacements for each parameter when perturbing the model.
  //!param: op         - name of output file containing results of training for this list of file.
  // This functions considers each training appearance successively and perturbs the different parameters in order to analyse the effect of errors in the parameters on the residual error. The range of displacement is +/-0.5 standard deviation for each parameter. A value of n for 'incrSize' means that there will be 2n displacements uniformly spread on the interval [-0.5std,+0.5std] for each parameter.
  // Note: contrary to the Design method, PreDesign does not produce a complete appearance model. PreDesign needs to be followed by PostDesign in order to obtain the AAM. PreDesign allows to split the training of the AAM (which is usually computer intensive because of the number of files to process) into a large number of smaller jobs which can be run in parallel.
  bool AAMActiveAppearanceModelBodyC::PreDesign(const AAMAppearanceModelC &apm,
                                                SampleStreamC<AAMAppearanceC> &sample,
                                                UIntT incrSize,
                                                const StringC &op)
  {
    SysLog(SYSLOG_DEBUG) << "AAMActiveAppearanceModelBodyC::PreDesign(), Called.";
    appearanceModel = apm;
    AAMSampleStreamC ss(appearanceModel, smooth, sample, incrSize);

    Tuple2C<VectorC,VectorC> tup;
    if(!ss.Get(tup)) {
      SysLog(SYSLOG_WARNING) << "AAMActiveAppearanceModelBodyC::PreDesign(), Dataset empty.";
      return false;
    }

    // initialise the matrices
    MatrixRUTC aaTu(tup.Data1().Size());
    aaTu.Fill(0);
    MatrixC aTb(tup.Data1().Size(),tup.Data2().Size());
    aTb.Fill(0);

     // Reset to the first element.
    ss.First();
    // Do some sums without duplicating data.
    while(ss.Get(tup)) {
      aaTu.AddOuterProduct(tup.Data1());
      aTb.AddOuterProduct(tup.Data1(),tup.Data2());
    }

    // to garantee correct conversion from MatrixRUTC to MatrixC
    aaTu.ZeroLowerLeft();

    SArray1dC<MatrixC> res(2);
    res[0] = aaTu;
    res[1] = aTb;

    if(!RavlN::Save(op,res)) {
      return false;
    }

    SysLog(SYSLOG_DEBUG) << "AAMActiveAppearanceModelBodyC::PreDesign(), Done.";

    return true;
  }

  //: Terminate the design of AAM by merging results of PreDesign.
  //!param: apm        - input appearance model.
  //!param: fileList   - list of names of files containing the results of the pre-design.
  //!param: dir        - name of directory containg results of the pre-design.
  bool AAMActiveAppearanceModelBodyC::PostDesign(const AAMAppearanceModelC &apm,
                                                 const DListC<StringC> &fileList,
                                                 const StringC &dir)
  {
    ONDEBUG(cerr << "AAMActiveAppearanceModelBodyC::PostDesign(), Called. \n");
    appearanceModel = apm;

    // initialise the matrices
    MatrixRUTC aaTu(appearanceModel.MaskArea());
    aaTu.Fill(0);
    MatrixC aTb(appearanceModel.MaskArea(),appearanceModel.Dimensions());
    aTb.Fill(0);

    SArray1dC<MatrixC> res;
    for(DLIterC<StringC> it(fileList);it;it++) {
      if(!RavlN::Load(dir + *it,res)) {
        return false;
      }
      aaTu += res[0];
      aTb += res[1];
    }

    aaTu.MakeSymmetric();
    MatrixRSC aaT = aaTu.Copy();

    ONDEBUG(cerr << "AAMActiveAppearanceModelBodyC::PostDesign(), Solving equations.. \n");

    if(!aaT.InverseIP()) {
      cerr << "AAMActiveAppearanceModelBodyC::PostDesign(), WARNING: Covariance of input has singular values. \n";
      aaT = aaTu.PseudoInverse();
    }

    MatrixC A =  (aaT * aTb).T();

    VectorC offset(appearanceModel.Dimensions());
    offset.Fill(0);
    refiner = FuncLinearC(A,offset);

    ONDEBUG(cerr << "\nAAMActiveAppearanceModelBodyC::PostDesign(), Done. \n");

    return true;
  }

  RAVL_INITVIRTUALCONSTRUCTOR_FULL(AAMActiveAppearanceModelBodyC,AAMActiveAppearanceModelC,RCHandleVC<AAMActiveAppearanceModelBodyC>);

}
