// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2005, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlAAM
//! file="Ravl/CompVision/ActiveAppearanceModels/AAMSampleStream.cc"

#include "Ravl/String.hh"
#include "Ravl/DList.hh"
#include "Ravl/DLIter.hh"
#include "Ravl/Image/AAMAppearanceUtil.hh"
#include "Ravl/Image/AAMSampleStream.hh"
#include "Ravl/Image/AAMSampleStreamFileList.hh"
#include "Ravl/OS/SysLog.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlImageN {

  //: Constructor.
  AAMSampleStreamBodyC::AAMSampleStreamBodyC(const AAMAppearanceModelC &nam,
                                             const GaussConvolve2dC<RealT> &psmooth,
                                             const DListC<StringC> &pfileList,
                                             const StringC &pdir,
                                             const StringC &pmirrorFile,
                                             const UIntT pincrSize,
                                             bool ignoreSuspect
                                             )
    : m_am(nam),
      m_smooth(psmooth),
      m_samplesPerFrame(0),
      m_sampleNo(0),
      m_incrSize(pincrSize),
      m_done(false),
      m_useTypeId(false)
  {
    m_samplesPerFrame = m_am.Dimensions() * 2*m_incrSize + 1;
    m_sample = AAMSampleStreamFileListC(pfileList, pdir, pmirrorFile, m_typeMap, m_namedTypeMap, m_useTypeId, ignoreSuspect, true);
  }

  AAMSampleStreamBodyC::AAMSampleStreamBodyC(const AAMAppearanceModelC &am,
                                             const GaussConvolve2dC<RealT> &smooth,
                                             SampleStreamC<AAMAppearanceC> &sample,
                                             const UIntT incrSize) :
     m_am(am),
     m_smooth(smooth),
     m_sample(sample),
     m_samplesPerFrame(0),
     m_sampleNo(0),
     m_incrSize(incrSize),
     m_done(false),
     m_useTypeId(false)

  {
    m_samplesPerFrame = m_am.Dimensions() * 2 * m_incrSize + 1;
  }

  //: Seek to location in stream.
  bool AAMSampleStreamBodyC::Seek(UIntT off) {
    SysLog(SYSLOG_DEBUG) << "AAMSampleStreamBodyC::Seek(), Offset=" << off;
    if(off == 0) {
      m_done = false;
      m_sample.Seek(0);
      m_sampleNo = 0;
      return true;
    }
    return false;
  }

  //: Find the total size of the stream. (assuming it starts from 0)
  UIntT AAMSampleStreamBodyC::Size() const
  { return m_sample.Size() * m_samplesPerFrame; }

  //: Get next piece of data.
  Tuple2C<VectorC,VectorC> AAMSampleStreamBodyC::Get() {
    Tuple2C<VectorC,VectorC> ret;
    if(!Get(ret))
      throw DataNotReadyC("Out of samples.");
    return ret;
  }

  //: Try and get next piece of data.
  bool AAMSampleStreamBodyC::Get(Tuple2C<VectorC,VectorC> &buff) {
    while(!m_done) {
      UIntT subNo = m_sampleNo % m_samplesPerFrame;
      if(subNo == 0) { // First from frame ?
        AAMAppearanceC appear;
        if (!m_sample.Get(appear)) {
          m_done = true;
          break;
        }

        SysLog(SYSLOG_DEBUG) << "AAMSampleStreamBodyC::Get(), Frame=" << (m_sampleNo / m_samplesPerFrame) << " " << appear.SourceFile();

        // Sort out a real image.
        if(m_image.Frame() != appear.Image().Frame())
          m_image = ImageC<RealT>(appear.Image().Frame());
        for(Array2dIter2C<RealT,ByteT> it(m_image,appear.Image());it;it++)
          it.Data1() = it.Data2();

        // smooth the image
        m_image = m_smooth.Apply(m_image);

        // Compute true parameters.

        m_trueVec = m_am.Parameters(appear);
        m_deltaVec = m_trueVec.Copy();

        // Generate entry with no errors.

        buff.Data2() = VectorC(m_trueVec.Size());
        buff.Data2().Fill(0);
        // Compute residual error
        if(m_am.ErrorVector(m_trueVec,m_image,buff.Data1())==false) {
          SysLog(SYSLOG_WARNING) << "Marked up points out of image range in file '" << appear.SourceFile();
          // Ignore it and continue.
          continue;
        }

        m_sampleNo++;
        return true;
      }
      subNo--;
      UIntT paramNo = subNo / (2*m_incrSize);
      UIntT incrNo = subNo % (2*m_incrSize);

      RealT trueVal = m_trueVec[paramNo];

      RealT maxVar;
      switch (paramNo)
      {
        case 0: // tx
          maxVar = 3*m_am.PixelSize()[0];
          break;
        case 1: // ty
          maxVar = 3*m_am.PixelSize()[1];
          break;
        case 2: // sx
        case 3: // sy
          maxVar = 0.3 * trueVal;
          break;
        case 4: // u1
        case 5: // u2
          if(!m_am.FixTextureMeanStdDev()) {
            maxVar = 0.3 * trueVal;
            break;
          }
          /* no break */
        default:
          maxVar = 3.0 * Sqrt(m_am.EigenValues()[paramNo]);
          break;
      }

      bool isInRange;
      // Change parameters.
      do {
        m_deltaVec[paramNo] = trueVal + (-1.0 + incrNo*2.0/(2*m_incrSize - 1)) * maxVar;
        // Compute residual error.
        isInRange = m_am.ErrorVector(m_deltaVec,m_image,buff.Data1());
        maxVar /= 2;
      } while(isInRange==false);

      buff.Data2() = m_deltaVec - m_trueVec;
      m_deltaVec[paramNo] = trueVal;
      m_sampleNo++;
      return true;
    }
    return false;
  }

  //: Has the End Of Stream been reached ?
  // true = yes.
  bool AAMSampleStreamBodyC::IsGetEOS() const {
    return true;
  }

}
