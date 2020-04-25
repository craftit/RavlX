// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DPSUBSEQUENCESPEC_HEADER
#define RAVL_DPSUBSEQUENCESPEC_HEADER 1
/////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/OS/IO/SubSequenceSpec.hh"
//! lib=RavlOSIO
//! userlevel=Default
//! author="Charles Galambos"
//! date="13/12/2000"
//! docentry="Ravl.API.Core.Data Processing.Composite" 

#include "Ravl/RCHandleV.hh"
#include "Ravl/String.hh"
#include "Ravl/IndexRange1d.hh"
#include "Ravl/StdConst.hh"


namespace RavlN {
  class BinIStreamC;
  class BinOStreamC;
  class SubSequenceSpecC;
  
  //! userlevel=Develop
  //: Specification for sub sequence.
  
  class SubSequenceSpecBodyC 
    : public RCBodyVC
  {
  public:
    SubSequenceSpecBodyC();
    //: Default constructor.

    SubSequenceSpecBodyC(const StringC &nfn,
                         IndexC nfirstFrame = 0,
                         IndexC nlastFrame = RavlConstN::maxInt,
                         IntT nchannelId = 0,
                         const StringC &fmt = StringC(),
                         bool nsingle =false);
    //: Constructor.

    SubSequenceSpecBodyC(istream &is);
    //: Stream constructor.

    SubSequenceSpecBodyC(BinIStreamC &is);
    //: BinStream constructor.

    SubSequenceSpecBodyC(const SubSequenceSpecC &oth);
    //: Copy from another spec
    
    bool Save(ostream &os) const;
    //: Save to a stream.
    
    bool Save(BinOStreamC &os) const;
    //: Save to a BinStream.
    
    StringC &Filename()
    { return fn; }
    //: Access filename.
    // Name of file where sequence is stored.
    
    const StringC &Filename() const
    { return fn; }
    //: Access filename.
    // Name of file where sequence is stored.
    
    IndexC &FirstFrame()
    { return range.Min(); }
    //: Access index of first frame in sequence.

    const IndexC &FirstFrame() const
    { return range.Min(); }
    //: Access index of first frame in sequence.

    IndexC &LastFrame()
    { return range.Max(); }
    //: Access index of last frame in sequence.
    // If the last frame is ((IndexC) -1), then use
    // the rest of the sequence.
    
    const IndexC &LastFrame() const
    { return range.Max(); }
    //: Access index of last frame in sequence.
    // If the last frame is ((IndexC) -1), then use
    // the rest of the sequence.
    
    IndexRangeC &Range()
    { return range; }
    //: Access play range.

    const IndexRangeC &Range() const
    { return range; }
    //: Access play range.

    IntT &ChannelId()
    { return channelId; }
    //: Access channel id.

    IntT ChannelId() const
    { return channelId; }
    //: Access channel id.
    
    const StringC &FileFormat() const
    { return fileFormat; }
    //: Access format information.
    
    StringC &FileFormat()
    { return fileFormat; }
    //: Access format information.
    
    bool IsSingle() const
    { return single; }
    //: Is single frame.

    void SetSingle(bool nsing);
    //: Set to single frame.
  protected:
    StringC fn;      // Name of file.
    StringC fileFormat;    // Format information for file.
    IndexRangeC range; // Range in sequence.
    IntT channelId;    // Which channel in the source sequence, default 0.
    bool single;    // Treat as single frame ?
  };
  
  //! userlevel=Normal
  //: Specification for sub sequence.
  
  class SubSequenceSpecC 
    : public RCHandleC<SubSequenceSpecBodyC> 
  {
  public:
    SubSequenceSpecC()
    {}
    //: Default constructor.
    // creates an invalid handle.
    
    SubSequenceSpecC(const StringC &fn,IndexC firstFrame = 0,IndexC lastFrame = RavlConstN::maxInt,IntT nchannelId = 0, const StringC &fmt = StringC(),bool nsingle =false)
      : RCHandleC<SubSequenceSpecBodyC>(*new SubSequenceSpecBodyC(fn,firstFrame,lastFrame,nchannelId,fmt,nsingle))
    {}
    //: constructor.
    
    SubSequenceSpecC(istream &is)
      : RCHandleC<SubSequenceSpecBodyC>(*new SubSequenceSpecBodyC(is))
    {}
    //: Stream constructor.
    
    SubSequenceSpecC(BinIStreamC &is)
      : RCHandleC<SubSequenceSpecBodyC>(*new SubSequenceSpecBodyC(is))
    {}
    //: BinStream constructor.

  protected:
    SubSequenceSpecC(SubSequenceSpecBodyC &bod)
      : RCHandleC<SubSequenceSpecBodyC>(bod)
    {}
    //: Body constructor.
    
    SubSequenceSpecBodyC &Body()
    { return RCHandleC<SubSequenceSpecBodyC>::Body(); }
    //: Access body.
    
    const SubSequenceSpecBodyC &Body() const
    { return RCHandleC<SubSequenceSpecBodyC>::Body(); }
    //: Access body.
    
  public:
    bool Save(ostream &os) const
    { return Body().Save(os); }
    //: Save to a stream.
    
    bool Save(BinOStreamC &os) const
    { return Body().Save(os); }
    //: Save to a BinStream.
    
    StringC &Filename()
    { return Body().Filename(); }
    //: Access filename.
    // Name of file where sequence is stored.
    
    const StringC &Filename() const
    { return Body().Filename(); }
    //: Access filename.
    // Name of file where sequence is stored.
    
    IndexC &FirstFrame()
    { return Body().FirstFrame(); }
    //: Access index of first frame in sequence.

    const IndexC &FirstFrame() const
    { return Body().FirstFrame(); }
    //: Access index of first frame in sequence.
    
    IndexC &LastFrame()
    { return Body().LastFrame(); }
    //: Access index of last frame in sequence.
    // If the last frame is ((UIntT) -1), then use
    // the rest of the sequence.
    
    const IndexC &LastFrame() const
    { return Body().LastFrame(); }
    //: Access index of last frame in sequence.
    // If the last frame is ((UIntT) -1), then use
    // the rest of the sequence.

    IndexRangeC &Range()
    { return Body().Range(); }
    //: Access play range.
    
    const IndexRangeC &Range() const
    { return Body().Range(); }
    //: Access play range.

    IntT &ChannelId()
    { return Body().ChannelId(); }
    //: Access channel id.
    
    IntT ChannelId() const
    { return Body().ChannelId(); }
    //: Access channel id.
    
    const StringC &FileFormat() const
    { return Body().FileFormat(); }
    //: Access format information.
    
    StringC &FileFormat()
    { return Body().FileFormat(); }
    //: Access format information.

    bool IsSingle() const
    { return Body().IsSingle(); }
    //: Is single frame.
    
    void SetSingle(bool nsing)
    { Body().SetSingle(nsing); }
    //: Set to single frame.
    
    friend class SubSequenceSpecBodyC;
  };

  inline
  ostream &operator<<(ostream &strm,const SubSequenceSpecC &str) { 
    str.Save(strm);
    return strm;
  }
  //: Output to ostream.

  inline
  istream &operator>>(istream &strm,SubSequenceSpecC &str) { 
    str = SubSequenceSpecC(strm);
    return strm;
  }
  //: Input from istream.
  
  BinOStreamC &operator<<(BinOStreamC &strm,const SubSequenceSpecC &str); 
  //: Output to BinOStreamC.
  
  BinIStreamC &operator>>(BinIStreamC &strm,SubSequenceSpecC &str);
  //: Input from BinIStreamC.
  
  
}
#endif
