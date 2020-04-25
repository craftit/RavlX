// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlDevAudio
//! file="Ravl/Contrib/Audio/DevAudio/RavlDevAudio.cc"

namespace RavlAudioN {
  extern void InitDevAudioFormat();
  
  void InitDevAudio() {
    InitDevAudioFormat();
  }
  
}
