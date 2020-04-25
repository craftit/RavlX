// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DPPLAYLIST_HEADER
#define RAVL_DPPLAYLIST_HEADER 1
/////////////////////////////////////////////////////////////
//! file="Ravl/OS/IO/PlayList.hh"
//! lib=RavlOSIO
//! userlevel=Normal
//! author="Charles Galambos"
//! date="13/12/2000"
//! docentry="Ravl.API.Core.Data Processing.Composite" 

#include "Ravl/DP/EditSpec.hh"
#include "Ravl/DLIter.hh"
#include "Ravl/SArray1dIter.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Play list body
  
  class PlayListBodyC
    : public RCBodyVC
  {
  public:
    PlayListBodyC();
    //: Default constructor.
    // creates an empty play list.

    PlayListBodyC(const DListC<EditSpecC> &lst);
    //: Constructor from a plain list.

    PlayListBodyC(const DListC<StringC> &lst);
    //: Constructor from a plain list of filenames.

    PlayListBodyC(std::istream &is);
    //: stream constructor.
    
    PlayListBodyC(BinIStreamC &is);
    //: binary stream constructor.
    
    bool Save(std::ostream &os) const;
    //: Save to a ostream.

    bool Load(std::istream &is);
    //: Load from an ostream.
    
    bool Save(BinOStreamC &os) const;
    //: Save to a ostream.
    
    bool Append(const DListC<EditSpecC> &es);
    //: Append a list of edits.

    bool Append(const EditSpecC &es);
    //: Append a single edit.
    
    void UpdateRangeIndex();
    //: Go through EditSpecs, and update the insert ranges.

    EditSpecC FindEdit(UIntT off);
    //: Find edit containing the given offset.
    // An invalid EditSpec is returns if the
    // offset is not within the sequence. (or
    // if the search failed because of incomplete
    // edit information).
    
    UIntT TotalSize() const;
    //: get the total size of the play list.
    // i.e. the number of frames in the sequence.

    bool IsLast(const EditSpecC &es) const {
      if(edits.Size() == 0)
	return true; // Nothing in list everything is last!
      return edits[edits.Size()-1] == es;
    }
    //: Last edit in play list ?
    
    bool IsEmpty() const
    { return edits.Size() == 0; }
    //: Is the edits list empty ?
    
    const SArray1dC<EditSpecC> &Edits() const
    { return edits; }
    //: Access array of edits.

    void Dump(std::ostream &out);
    //: Dump debug info about play list.
    
  protected:
    SArray1dC<EditSpecC> edits;
    UIntT maxIndexEdit;  // This is the number of edits that are indexed.
    UIntT maxIndexOff;   // Maximum indexed offset in the sequence.
  };

  //! userlevel=Normal
  //: Play list.
  
  class PlayListC
    : public RCHandleC<PlayListBodyC>
  {
  public:
    PlayListC()
    {}
    //: Default constructor.
    // creates an invalid handle.

    PlayListC(bool)
      : RCHandleC<PlayListBodyC>(*new PlayListBodyC())
    {}
    //: Constructor.
    // creates an empty play list.

    PlayListC(istream &is)
      : RCHandleC<PlayListBodyC>(*new PlayListBodyC(is))
    {}
    //: stream constructor.
    
    PlayListC(BinIStreamC &is)
      : RCHandleC<PlayListBodyC>(*new PlayListBodyC(is))
    {}
    //: Binary stream constructor.

    PlayListC(const DListC<EditSpecC> &lst)
      : RCHandleC<PlayListBodyC>(*new PlayListBodyC(lst))
    {}
    //: List constructor.

    PlayListC(const DListC<StringC> &lst)
      : RCHandleC<PlayListBodyC>(*new PlayListBodyC(lst))
    {}
    //: Constructor from a plain list of filenames

    void UpdateRangeIndex()
    { Body().UpdateRangeIndex(); }
    //: Go through EditSpecs, and update the insert ranges.
    
    EditSpecC FindEdit(UIntT off)
    { return Body().FindEdit(off); }
    //: Find edit containing the given offset.
    // An invalid EditSpec is returns if the
    // offset is not within the sequence. (or
    // if the search failed because of incomplete
    // edit information).
    
    UIntT TotalSize() const
    { return Body().TotalSize(); }
    //: get the total size of the play list.
    // i.e. the number of frames in the sequence.

    void Save(ostream &os) const
    { Body().Save(os); }
    //: Save to a ostream.

    void Load(istream &is)
    { Body().Load(is); }
    //: Load from an ostream.
    
    void Save(BinOStreamC &os) const
    { Body().Save(os); }
    //: Save to a ostream.
    
    bool Append(const DListC<EditSpecC> &es)
    { return Body().Append(es); }
    //: Append a list of edits.

    bool Append(const EditSpecC &es)
    { return Body().Append(es); }
    //: Append a single edit.
    
    bool IsLast(const EditSpecC &es) const 
    { return Body().IsLast(es); }
    //: Is this the last edit in the play list ?

    bool IsEmpty() const
    { return Body().IsEmpty(); }
    //: Is the edits list empty ?

    const SArray1dC<EditSpecC> &Edits() const
    { return Body().Edits(); }
    //: Access array of edits.

  };


  inline
  ostream &operator<<(ostream &strm,const PlayListC &str) { 
    str.Save(strm);
    return strm;
  }
  //: Output to ostream.
  
  inline
  istream &operator>>(istream &strm,PlayListC &str) { 
    str = PlayListC(strm);
    return strm;
  }
  //: Input from istream.
  
  inline
  BinOStreamC &operator<<(BinOStreamC &strm,const PlayListC &str) { 
    str.Save(strm);
    return strm;
  }
  
  //: Output to BinOStreamC.
  
  inline
  BinIStreamC &operator>>(BinIStreamC &strm,PlayListC &str) { 
    str = PlayListC(strm);
    return strm;
  }
  //: Input from BinIStreamC.

  
}  


#endif
