// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_AVFRAME_HEADER
#define RAVLIMAGE_AVFRAME_HEADER 1
//! rcsid="$Id$"
//! lib=RavlAV
//! author="Charles Galambos"
//! userlevel=Normal
//! docentry="Ravl.API.Images.Video"
//! file="Ravl/Image/AV/AVFrame.hh"

#include "Ravl/Image/Image.hh"
#include "Ravl/Audio/AudioFrame.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/Stream.hh"

namespace RavlImageN {
  using namespace RavlN ; 
  using namespace RavlAudioN;
  
  //! userlevel=Develop
  //: Audio-Video frame.
  class AVFrameBodyC : public RCBodyC 
  {
  public:
    AVFrameBodyC(){}
    //: Default constructor.
    
    AVFrameBodyC(const AudioFrameC &audioData,ImageC<ByteRGBValueC> &img);
    //: Constructor.

    AVFrameBodyC ( istream & s ) ; 
    //: Construcor from stream 

    AVFrameBodyC (BinIStreamC & s) ; 
    //: Constructor from binary stream 

    bool Save (ostream & s ) const ; 
    //: Save to stream 

    bool Save (BinOStreamC & s ) const ; 
    //: Save to binary stream 

    AudioFrameC &Audio()
    { return audio; }
    //: Access audio data.
    
    const AudioFrameC &Audio() const
    { return audio; }
    //: Access audio data.
    
    ImageC<ByteRGBValueC> &Image()
    { return image; }
    //: Image. 

    const ImageC<ByteRGBValueC> &Image() const
    { return image; }
    //: Image. 
    
  protected:
    AudioFrameC audio;
    ImageC<ByteRGBValueC> image;
  
  };
  
  //! userlevel=Normal
  //: Audio-Video frame.
  class AVFrameC : public RCHandleC<AVFrameBodyC> 
  {
  public : 

    inline AVFrameC(){}
    //: Default constructor - creates an invalid handle 

    inline AVFrameC(const AudioFrameC &audioData,ImageC<ByteRGBValueC> &img)
      : RCHandleC<AVFrameBodyC> ( * new AVFrameBodyC ( audioData, img ) ) {} 
    //: Constructor.

    inline AVFrameC ( istream & s )
      : RCHandleC<AVFrameBodyC>  ( *new AVFrameBodyC ( s ) ) {} 
    //: Construcor from stream 

    inline AVFrameC (BinIStreamC & s) 
      : RCHandleC<AVFrameBodyC> ( *new AVFrameBodyC (s) ) {} 
    //: Constructor from binary stream 

    inline bool Save (ostream & s ) const 
    { return Body().Save(s) ; }  
    //: Save to stream 
    
    inline bool Save (BinOStreamC & s ) const 
    { return Body().Save(s) ; } 
    //: Save to binary stream 
    
    inline AudioFrameC &Audio()
    { return Body().Audio() ;  }
    //: Access audio data.
    
    inline const AudioFrameC &Audio() const
    { return Body().Audio() ; }
    //: Access audio data.
    
    inline ImageC<ByteRGBValueC> &Image()
    { return Body().Image() ; }
    //: Image. 

    inline const ImageC<ByteRGBValueC> &Image() const
    { return Body().Image() ; }
    //: Image. 

  };
  
  inline BinIStreamC & operator >> ( BinIStreamC & s , AVFrameC & av ) 
  { av  = AVFrameC(s) ; return s ; } 
  // Load from binary stream 

  inline BinOStreamC & operator << ( BinOStreamC & s, const AVFrameC & av ) 
  { av.Save(s)  ; return s ; } 
  //: Save to binary stream 

  inline istream & operator >> ( istream & s , AVFrameC & av ) 
  { av = AVFrameC(s)  ; return s ; }
  //:  Load from stream 

  inline ostream & operator << ( ostream & s, const AVFrameC & av ) 
  {  av.Save(s)  ; return s ; } 
  //: Save to stream 
  
};

#endif
