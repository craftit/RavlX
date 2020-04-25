// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlOSIO
//! file="Ravl/OS/IO/PlayList.cc"

#include "Ravl/DP/PlayList.hh"
#include "Ravl/StringList.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/TypeName.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {

  //: Default constructor.
  
  PlayListBodyC::PlayListBodyC()
   : maxIndexEdit(0),
     maxIndexOff(0)
  {}
  
  //: Constructor from a plain list of filenames.

  PlayListBodyC::PlayListBodyC(const DListC<StringC> &lst)
   : maxIndexEdit(0),
     maxIndexOff(0)
  {
    edits = SArray1dC<EditSpecC>(lst.Size());
    unsigned i = 0;
    for(DLIterC<StringC> it(lst);it;it++,i++)
      edits[i] = EditSpecC(*it);
  }

  //: binary stream constructor.
  
  PlayListBodyC::PlayListBodyC(BinIStreamC &is)
    : maxIndexEdit(0),
      maxIndexOff(0)
  {
    is >> edits;
  }
  //: Save to a ostream.
  
  bool PlayListBodyC::Save(BinOStreamC &os) const {
    os << edits;
    return os.Stream().good();
  }
  
  //: stream constructor.
  
  PlayListBodyC::PlayListBodyC(istream &is) {
    Load(is);
  }
  
  //: Save to a ostream.
  
  bool PlayListBodyC::Save(ostream &os) const {
    for(UIntT i = 0;i < edits.Size();i++) {
      os << edits[i] << "\n";
    }
    os << "#EditsEnd\n";
    return true;
  }

  //: Dump debug info about play list.
  
  void PlayListBodyC::Dump(std::ostream &os) {
    os << "PlayList dump. MaxIndexEdit:" << maxIndexEdit << " MaxIndexOff:" << maxIndexOff << " \n";;
    for(UIntT i = 0;i < edits.Size();i++) {
      os << edits[i] << " " << edits[i].InsertRange() << "\n";
    }
    os << "End of dump.\n";
    
  }
  
  //: Load from an ostream.
  
  bool PlayListBodyC::Load(std::istream &is) {
    cerr << "PlayListBodyC::Load(), Called \n";
    DListC<EditSpecC> spec;
    bool single = false;
    for(;is;) {
      StringC fileName = "";
      IntT sstart = 0;
      IntT send = RavlConstN::maxInt;
      IntT channel = 0;
      StringC fmt = "";
      
      StringC buff;
      int c = readline(is,buff);
      if(c == 0)
	continue;
      if(buff == "#EditsEnd") // End of file ?
	break;
      if(buff[0] == '!') { // Command ?
	StringC cmd = StringC(buff.after(0)).TopAndTail();
	ONDEBUG(cerr << "Found command : '" << cmd << "'\n");
	if(cmd == "single") {
	  single = true;
	}
	if(cmd == "sequence")
	  single = false;
	continue;
      }
      if(buff[0] == '#') // Skip comments.
	continue;
      StringListC sl(buff);
      DLIterC<StringC> it(sl);
      if(!it)
	continue; // Empty line.
      do {
	fileName = it.Data();
	it++; if(!it) break;
	sstart = it.Data().IntValue();
	it++; if(!it) break;
	send = it.Data().IntValue(); 
	it++; if(!it) break;
	channel = it.Data().IntValue(); 
	it++; if(!it) break;
	fmt = it.Data(); 
      } while(0);
      EditSpecC es(fileName,sstart,send,channel,fmt);
      // Change to single frame mode ?
      if(single)
	es.SetSingle(true);
      spec.InsLast(es);
    }
    Append(spec);
    //is.clear(ios::eofbit|is.rdstate()); // Clear end of file errors
    is.clear();
    cerr << "PlayListBodyC::Load(), Done. \n";
    return true;
  }
  

  //: Constructor from a plain list.
  
  PlayListBodyC::PlayListBodyC(const DListC<EditSpecC> &lst) {
    Append(lst);
  }
  
  //: Append a list of edits.
  
  bool PlayListBodyC::Append(const DListC<EditSpecC> &lst) {
    UIntT size = lst.Size();
    SArray1dC<EditSpecC> newEdits(size);
    SArray1dIterC<EditSpecC> ait(newEdits);
    DLIterC<EditSpecC> it(lst);
    for(;it;it++,ait++)
      *ait = EditSpecC(*it);
    edits.Append(newEdits); // FIXME :- A bit crap but it'll do for now.
    UpdateRangeIndex();
    return true;
  }

  //: Append a single edit.
  
  bool PlayListBodyC::Append(const EditSpecC &es) {
    SArray1dC<EditSpecC> newEdits(1);
    newEdits[0] = es;
    edits.Append(newEdits); // FIXME :- A bit crap but it'll do for now.
    UpdateRangeIndex();    
    return true;
  }
  
  //: Go through EditSpecs, and update the insert ranges.
  
  void PlayListBodyC::UpdateRangeIndex() {
    UIntT off = 0;
    SArray1dIterC<EditSpecC> it(edits);
    for(;it;it++) {
      if(it->Range().Max() == RavlConstN::maxInt)
	break;
      ONDEBUG(cerr << "PlayListBodyC::UpdateRangeIndex() Got range:" << it->Range() << "\n");
      it->InsertRange() = it->Range() + off;
      off += it->Range().Size();
    }
    if(it.IsElm()) {
      it->InsertRange().Min() = off;
      maxIndexEdit = it.Index().V();
    } else
      maxIndexEdit = edits.Size()-1;
    maxIndexOff = off;
    ONDEBUG(cerr << "PlayListBodyC::UpdateRangeIndex(), Results: \n");
    ONDEBUG(Dump(cerr));
  }
  
  //: Find edit containing the given offset.
  // An invalid EditSpec is returns if the
  // offset is not within the sequence. (or
  // if the search failed because of incomplete
  // edit information).
  
  EditSpecC PlayListBodyC::FindEdit(UIntT off) {
    // Do a binary search for the required edit.
    if(edits.Size() == 0)
      return EditSpecC(); // Empty play list.
    if(off == 0) { // Short cut for 0 offset.
     if(edits[0].InsertRange().Contains(0))
       return edits[0];
    }
    ONDEBUG(cerr << "PlayListBodyC::FindEdit() MaxIndexEdit:" << maxIndexEdit << " MaxIndexOff:" << maxIndexOff << " \n");
    IndexRangeC srng(0,maxIndexEdit);
    if(off > maxIndexOff) {
      if(maxIndexEdit < edits.Size())
	return edits[maxIndexEdit];
      return EditSpecC();
    }
    if(off > edits[srng.Max()].InsertRange().Max())
      return EditSpecC();
    for(;;) {
      RavlAssert(srng.Min() <= srng.Max());
      IndexC at = srng.Center();
      ONDEBUG(cerr << "PlayListBodyC::FindEdit() At:" << at << " " << srng << " for off " << off <<"\n");
      if(edits[at].InsertRange().Max() < off) {
	srng.Min() = at+1;
	continue;
      }
      if(edits[at].InsertRange().Min() > off) {
	srng.Max() = at-1;
	continue;
      }
      return edits[at];
    }
    //: Should never get this far.
    return EditSpecC();
  }

  //: get the total size of the play list.
  // i.e. the number of frames in the sequence.
  
  UIntT PlayListBodyC::TotalSize() const {
    if(edits.Size() == 0)
      return 0;
    return edits[edits.Size()-1].InsertRange().Max().V() + 1;
  }
  
}
