// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLAUDIO_AUDIOFRAME_HEADER
#define RAVLAUDIO_AUDIOFRAME_HEADER 1
//! lib=RavlAudioUtil
//! author="Charles Galambos"
//! docentry="Ravl.API.Audio.IO"
//! userlevel=Normal
//! date="13/07/2003"
//! file="Ravl/Audio/Util/AudioFrame.hh"

#include "Ravl/Audio/Types.hh"
#include "Ravl/SArray1d.hh"

namespace RavlAudioN {
  
  //! userlevel=Develop
  //: Frame of audio data.
  
  class AudioFrameBodyC : public RCBodyC {
  
  public:
    AudioFrameBodyC()
      : channels(0),freq(0) {}
    //: Default constructor.
        
    AudioFrameBodyC(const SArray1dC<SampleElemC<2,Int16T> > &data,RealT nfreq);
    //: Construct from components.

    AudioFrameBodyC( BinIStreamC & stream ) ; 
    //: Constructor from binary stream 

    AudioFrameBodyC( istream & stream ) ; 
    //: constructor from stream 

    bool Save ( BinOStreamC & stream ) const  ; 
    //: Save to binary stream 

    bool Save ( ostream & stream ) const ; 
    //: Save to stream 

    
    RealT AudioFrequency() const
    { return freq; }
    //: Access frequency.

    
    const SArray1dC<SampleElemC<2,Int16T> > &Stereo() const
    { return stereoData; }
    //: Get as stereo data. 

    
  protected:
    //SArray1dC<ByteT> audio; // Raw audio data.
    SArray1dC<SampleElemC<2,Int16T> > stereoData; // Data in 16-bit stereo.
    IntT channels;
    RealT freq;
  };




  //! userlevel=Normal
  //: Frame of audio data.
  class AudioFrameC : public RCHandleC<AudioFrameBodyC> 
{

 public: 

  inline AudioFrameC()
    : RCHandleC<AudioFrameBodyC> ( * new AudioFrameBodyC ) {} 
    //: Default constructor. - Creates an empty frame
    
  
  inline AudioFrameC(const SArray1dC<SampleElemC<2,Int16T> > &data,RealT nfreq)
    : RCHandleC<AudioFrameBodyC> ( * new AudioFrameBodyC ( data, nfreq ) ) {} 
    //: Construct from components.

  inline AudioFrameC( BinIStreamC & stream )  
    : RCHandleC<AudioFrameBodyC>  ( *new AudioFrameBodyC ( stream ) ) {} 
  //: Constructor from binary stream 

  inline AudioFrameC( istream & stream ) 
    : RCHandleC<AudioFrameBodyC> ( *new AudioFrameBodyC ( stream ) ) {} 
    //: constructor from stream 

  inline bool Save ( BinOStreamC & stream ) const  
    { return Body().Save(stream) ; }  
    //: Save to binary stream 

  inline bool Save ( ostream & stream ) const 
    { return Body().Save(stream) ; } 
    //: Save to stream 

  inline RealT AudioFrequency() const
    { return Body().AudioFrequency() ; }
    //: Access frequency.

    
    inline const SArray1dC<SampleElemC<2,Int16T> > &Stereo() const
    { return Body().Stereo() ; }
    //: Get as stereo data


}; 



inline BinIStreamC & operator >> ( BinIStreamC & s , AudioFrameC & av ) 
{ av = AudioFrameC(s) ; return s ; } 
//: input from binary stream 

inline BinOStreamC & operator << ( BinOStreamC & s, const AudioFrameC & av ) 
{ av.Save(s) ; return s ; } 
  //: output to binary stream 
  
inline istream & operator >> ( istream & s , AudioFrameC & av ) 
{ av = AudioFrameC(s) ; return s ; }  
  //: input from stream   
  
inline ostream & operator << ( ostream & s, const AudioFrameC & av ) 
{ av.Save(s) ; return s ; } 
  //: output to stream 














};
#endif
