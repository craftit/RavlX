// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlAudioUtil
//! file="Ravl/Audio/Util/Transcription.cc"

#include "Ravl/Audio/Transcription.hh"
#include "Ravl/Stream.hh"
#include "Ravl/StringList.hh"

namespace RavlAudioN {
  
  //: Load from a file.
  
  TranscriptionC::TranscriptionC(const StringC &file,const StringC &nDataFile)
    : dataFile(nDataFile)
  { Load(file); }

  //: Load transcription from file.
  
  bool TranscriptionC::Load(const StringC &fileName) {
    IStreamC inf(fileName);
    if(!inf)
      return false;
    Empty();
    StringC tmp;
    while(inf) {
      readline(inf,tmp,'\n',true);
      StringListC fields(tmp);
      UIntT size = fields.Size();
      if(size == 0)
	continue; // Silently skip empty lines.
      if(size != 3) {
	cerr << "WARNING: Unexpected number of fields. " << fields.Size() << " in " << fileName << " \n";
	continue;
      }
      InsLast(SegmentC(fields.Nth(0).IntValue(),fields.Nth(1).IntValue(),fields.Nth(2)));
    }
    return true;
  }
  
  //: Save transcription to a file.
  bool TranscriptionC::Save(const StringC &fileName) {
    OStreamC outf(fileName);
    if(!outf)
      return false;
    for(DLIterC<SegmentC> it(*this);it;it++)
      outf << it->SampleRange().Min() << ' ' << it->SampleRange().Max() << ' ' << it->Label() << '\n';
    return true;
  }
 
}
