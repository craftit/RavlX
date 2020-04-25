// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlAV
//! file="Ravl/Image/AV/AVFrame.cc"

#include "Ravl/AVFrame.hh"
#include "Ravl/TypeName.hh"

namespace RavlImageN {

  //: Constructor
  //: -----------------------------------------------------------------------------------------------------------------------
  AVFrameBodyC::AVFrameBodyC(const AudioFrameC &audioData,ImageC<ByteRGBValueC> &img)
    : audio(audioData),
      image(img)
  {}

   //: Constructor from binary stream 
  //: -----------------------------------------------------------------------------------------------------------------------
  AVFrameBodyC::AVFrameBodyC(BinIStreamC & s) 
  {  s >> audio >> image ; }
  
  //: Constructor from stream 
  //: -----------------------------------------------------------------------------------------------------------------------
  AVFrameBodyC::AVFrameBodyC(istream & s) 
  { s >> audio >> image ; }


  //: Save to binary stream 
  //: -----------------------------------------------------------------------------------------------------------------------
  bool AVFrameBodyC::Save ( BinOStreamC  & s) const 
  { s << audio << image ; return true ; }

  //: Save to stream 
  //: -----------------------------------------------------------------------------------------------------------------------
  bool AVFrameBodyC::Save ( ostream & s ) const 
  { s << audio << image ; return true ; } 
  
  
  static TypeNameC type0(typeid(RavlImageN::AVFrameC),"RavlImageN::AVFrameC");

}
