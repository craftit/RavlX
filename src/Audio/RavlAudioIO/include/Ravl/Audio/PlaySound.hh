// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLAUDIO_PLAYSOUND_HH
#define	RAVLAUDIO_PLAYSOUND_HH
//! lib=RavlAudioIO

#include "Ravl/String.hh"

namespace RavlAudioN {
  using namespace RavlN;
  
  //! Play a sound to the default audio device.
  bool PlaySound(const StringC &filename);

  //! Change default sound device.
  bool SetDefaultSoundDevice(const StringC &deviceName);
}

#endif

