// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2009, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_AAMSAMPLESTREAMFILELIST_HEADER
#define RAVL_AAMSAMPLESTREAMFILELIST_HEADER 1
//! author="Robert Crida"
//! docentry="Ravl.API.Images.AAM"
//! rcsid="$Id: SampleStreamVector.hh 7324 2009-10-21 16:23:20Z craftit $"
//! lib=RavlAAM
//! file="Ravl/CompVision/ActiveAppearanceModels/AAMSampleStreamFileList.hh"

#include "Ravl/DP/SPort.hh"
#include "Ravl/DLIter.hh"
#include "Ravl/Image/AAMAppearance.hh"
#include "Ravl/PatternRec/SampleStream.hh"

namespace RavlImageN {

  //: Sample stream implementation that reads AAMAppearanceC objects from a
  // list of files, mirroring if required
  class AAMSampleStreamFileListBodyC
    : public DPISPortBodyC<AAMAppearanceC>
  {
  public:
    AAMSampleStreamFileListBodyC(const DListC<StringC> &fileList,
                                 const StringC &dir,
                                 const StringC &mirrorFile,
                                 HashC<IntT, IntT> &typeMap,
                                 HashC<StringC, IntT> &namedTypeMap,
                                 bool &useTypeId,
                                 bool ignoreSuspect,
                                 bool loadImages);
    //: Constructor

    virtual bool Seek(UIntT off);
    //: Seek to location in stream.
    // Returns false, if seek failed. (Maybe because its
    // not implemented.)
    // if an error occurered (Seek returned False) then stream
    // position will not be changed.

    virtual UIntT Tell() const;
    //: Find current location in stream.
    // Defined as the index of the next object to be written or read.
    // May return ((UIntT) (-1)) if not implemented.


    virtual UIntT Size() const;
    //: Find the total size of the stream. (assuming it starts from 0)
    // May return ((UIntT) (-1)) if not implemented.

    virtual AAMAppearanceC Get();
    //: Get next piece of data.

  private:
    StringC m_dir;
    bool m_ignoreSuspect;
    bool m_loadImages;
    DListC<StringC> m_fileList;
    DLIterC<StringC> m_fileListIter;
    AAMAppearanceMirrorC m_mirror;
    UIntT m_index;
    HashC<IntT,IntT> m_typeMap;
    HashC<StringC,IntT> m_namedTypeMap;
    bool m_useTypeId;
  };

  //! userlevel=Normal
  //: Stream of samples backed list of files and optional mirroring
  class AAMSampleStreamFileListC
    : public SampleStreamC<AAMAppearanceC>
  {
  public:
    AAMSampleStreamFileListC()
      : DPEntityC(true)
    {}
    //: Default constructor.
    // Creates an invalid handle

    AAMSampleStreamFileListC(const DListC<StringC> &fileList,
                             const StringC &dir,
                             const StringC &mirrorFile,
                             HashC<IntT, IntT> &typeMap,
                             HashC<StringC, IntT> &namedTypeMap,
                             bool &useTypeId,
                             bool ignoreSuspect,
                             bool loadImages) :
      DPEntityC(new AAMSampleStreamFileListBodyC(fileList, dir, mirrorFile, typeMap, namedTypeMap, useTypeId, ignoreSuspect, loadImages))
    {}
    //: Base constructor.

  protected:
    AAMSampleStreamFileListC(AAMSampleStreamFileListBodyC &bod)
      : DPEntityC(bod)
    {}
    //: Body constructor.

    inline AAMSampleStreamFileListBodyC &Body()
    { return dynamic_cast<AAMSampleStreamFileListBodyC&>(DPEntityC::Body()); }
    //: Access body class.

    inline const AAMSampleStreamFileListBodyC &Body() const
    { return dynamic_cast<const AAMSampleStreamFileListBodyC&>(DPEntityC::Body()); }
    //: Access body class.
  };
}


#endif
