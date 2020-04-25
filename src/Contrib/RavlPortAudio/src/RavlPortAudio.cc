// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2012, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlPortAudio
//! file="Ravl/Contrib/Audio/PortAudio/RavlPortAudio.cc"

namespace RavlAudioN {
  
  extern void InitPortAudioFormat();
  
  void InitPortAudio() {
    InitPortAudioFormat();
  }
  
}
