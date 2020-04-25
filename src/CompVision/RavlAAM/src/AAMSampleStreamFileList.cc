// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2009, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id: AAMSampleStreamFileList.cc 7351 2009-11-03 12:44:53Z omn-crida $"
//! lib=RavlAAM
//! file="Ravl/CompVision/ActiveAppearanceModels/AAMSampleStreamFileList.cc"

#include "Ravl/Image/AAMSampleStreamFileList.hh"
#include "Ravl/Image/AAMAppearanceUtil.hh"

namespace RavlImageN {

  AAMSampleStreamFileListBodyC::AAMSampleStreamFileListBodyC(const DListC<StringC> &fileList,
                                                             const StringC &dir,
                                                             const StringC &mirrorFile,
                                                             HashC<IntT, IntT> &typeMap,
                                                             HashC<StringC, IntT> &namedTypeMap,
                                                             bool &useTypeId,
                                                             bool ignoreSuspect,
                                                             bool loadImages) :
    m_dir(dir),
    m_ignoreSuspect(ignoreSuspect),
    m_loadImages(loadImages),
    m_fileList(fileList),
    m_fileListIter(fileList),
    m_index(0),
    m_typeMap(typeMap),
    m_namedTypeMap(namedTypeMap),
    m_useTypeId(useTypeId)
  {
    if (!mirrorFile.IsEmpty()) {
      m_mirror = AAMAppearanceMirrorC(mirrorFile);
    }
  }

  bool AAMSampleStreamFileListBodyC::Seek(UIntT off) {
    m_fileListIter.Nth(off / (m_mirror.IsValid() ? 2 : 1));
    m_index = off;
    return off < Size();
  }

  UIntT AAMSampleStreamFileListBodyC::Tell() const  {
    return m_index;
  }

  UIntT AAMSampleStreamFileListBodyC::Size() const {
    return m_fileList.Size() * (m_mirror.IsValid() ? 2 : 1);
  }

  AAMAppearanceC AAMSampleStreamFileListBodyC::Get() {
    if (!m_fileListIter.IsElm()) {
      throw DataNotReadyC();
    }
    AAMAppearanceC data = LoadFeatureFile(*m_fileListIter, m_dir, m_typeMap, m_namedTypeMap, m_useTypeId, m_ignoreSuspect, m_loadImages);
    if (m_mirror.IsValid()) {
      if (m_index % 2) {
        data = m_mirror.Reflect(data);
        m_fileListIter.Next();
      }
    } else {
      m_fileListIter.Next();
    }
    m_index++;
    return data;
  }

}
