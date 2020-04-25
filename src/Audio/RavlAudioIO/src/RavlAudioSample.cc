// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlAudioIO
//! file="Ravl/Audio/IO/RavlAudioSample.cc"


namespace RavlAudioN {
  extern void InitAudioSample();
  extern void InitSphereFormat();
  
  void RavlAudioSample() {
    InitAudioSample();
    InitSphereFormat();
  }
}
