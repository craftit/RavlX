// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DPEDITSPEC_HEADER
#define RAVL_DPEDITSPEC_HEADER 1
/////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/OS/IO/EditSpec.hh"
//! lib=RavlOSIO
//! userlevel=Default
//! author="Charles Galambos"
//! date="13/12/2000"
//! docentry="Ravl.API.Core.Data Processing.Composite" 

#include "Ravl/DP/SubSequenceSpec.hh"

namespace RavlN {
  
  class BinIStreamC;
  class BinOStreamC;
  
  //! userlevel=Develop
  //: Edit spec.
  // Includes where a SubSequence fits into a larger composite sequence.
  
  class EditSpecBodyC 
    : public SubSequenceSpecBodyC 
  {
  public:
    EditSpecBodyC()
    {}
    //: Default constructor.

    EditSpecBodyC(const SubSequenceSpecC &from,const IndexRangeC &ins);
    //: Constructor.

    EditSpecBodyC(const StringC &nfn,
		  IndexC nfirstFrame = 0,
		  IndexC nlastFrame = RavlConstN::maxInt,
		  IntT nchannelId = 0,
		  const StringC &fmt = StringC(),
		  const IndexRangeC &ins = IndexRangeC(0,RavlConstN::maxInt));
    //: Constructor.
    
    IndexRangeC &InsertRange()
    { return insertRange; }
    //: The location of the sequence in an output range.
    
    const IndexRangeC &InsertRange() const
    { return insertRange; }
    //: The location of the sequence in an output range.
    
  protected:
    IndexRangeC insertRange; // Range in final sequence.
  };
  
  //! userlevel=Normal
  //: Edit spec.
  // Includes where a SubSequence fits into a larger composite sequence.
  
  class EditSpecC
    : public SubSequenceSpecC
  {
  public:
    EditSpecC()
    {}
    //: Default constructor
    // creates an invalid handle.

    EditSpecC(const SubSequenceSpecC &from,const IndexRangeC &ins = IndexRangeC(0,0))
      : SubSequenceSpecC(*new EditSpecBodyC(from,ins))
    {}
    //: Constructor.
    
    EditSpecC(const StringC &nfn,
	      IndexC nfirstFrame = 0,
	      IndexC nlastFrame = RavlConstN::maxInt,
	      IntT nchannelId = 0, 
	      const StringC &fmt = StringC(),
	      const IndexRangeC &ins = IndexRangeC(0,RavlConstN::maxInt))
      : SubSequenceSpecC(*new EditSpecBodyC(nfn,nfirstFrame,nlastFrame,nchannelId,fmt,ins))
    {}
    //: Constructor from elements.
    
  protected:
    EditSpecC(EditSpecBodyC &bod)
      : SubSequenceSpecC(bod)
    {}
    //: Body constructor.
    
    EditSpecBodyC &Body()
    { return static_cast<EditSpecBodyC &>(SubSequenceSpecC::Body()); }
    //: Access body.
    
    const EditSpecBodyC &Body() const
    { return static_cast<const EditSpecBodyC &>(SubSequenceSpecC::Body()); }
    //: Access body.
    
  public:
    IndexRangeC &InsertRange()
    { return Body().InsertRange(); }
    //: The location of the sequence in an output range.
    
    const IndexRangeC &InsertRange() const
    { return Body().InsertRange(); }
    //: The location of the sequence in an output range.

    bool operator==(const EditSpecC &es) const
    { return &Body() == &es.Body(); }
    //: The same edit ?
    
    bool operator!=(const EditSpecC &es) const
    { return &Body() != &es.Body(); }
    //: The same edit ?

  };
}
  
#endif
