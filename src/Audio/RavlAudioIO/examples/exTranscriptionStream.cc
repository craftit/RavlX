// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlAudioIO
//! docentry="Ravl.API.Audio.Feature Extraction"
//! userlevel=Normal
//! file="Ravl/Audio/IO/exTranscriptionStream.cc"

#include "Ravl/Audio/TranscriptionStream.hh"
#include "Ravl/Option.hh"

using namespace RavlAudioN;

int main(int nargs,char **argv) {
  OptionC opts(nargs,argv);
  StringC dir = opts.String("d",".","Search directory. ");
  StringC transExt = opts.String("te","phn","Transcription file extension. ");
  StringC dataExt = opts.String("de","wav","Data file extension. ");
  StringC fileList = opts.String("l","","List of transcription files to process.");
  opts.Check();
  
  DPIPortC<TranscriptionC> ins;
  if(!fileList.IsEmpty())
    ins = OpenTranscriptionList(fileList,dataExt);
  else
    ins = OpenTranscriptionSearch(dir,transExt,dataExt);
  cout << "Data file - Number of entries.\n";
  while(1) {
    TranscriptionC transcription;
    if(!ins.Get(transcription))
      break;
    cerr << transcription.DataFile() << " - " << transcription.Size() << "\n";
  }
  return 0;
}
