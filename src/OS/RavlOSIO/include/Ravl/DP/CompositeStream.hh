// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DPCOMPOSITESTREAM_HEADER
#define RAVL_DPCOMPOSITESTREAM_HEADER 1
/////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/OS/IO/CompositeStream.hh"
//! lib=RavlOSIO
//! userlevel=Normal
//! author="Charles Galambos"
//! date="13/12/2000"
//! docentry="Ravl.API.Core.Data Processing.Composite" 

#include "Ravl/DP/SPort.hh"
#include "Ravl/DP/PlayList.hh"
#include "Ravl/DList.hh"
#include "Ravl/DP/SequenceIO.hh"
#include "Ravl/IO.hh"

namespace RavlN {  

  //! userlevel=Develop
  //: Stream Composition base class.
  
  class CompositeStreamBaseBodyC {
  public:
    CompositeStreamBaseBodyC();
    //: Default constructor.
    
    CompositeStreamBaseBodyC(const PlayListC &playList);
    //: Constructor.

    CompositeStreamBaseBodyC(const StringC &plfn,bool forWrite = false);
    //: Constructor with filename of play list.
    
    virtual ~CompositeStreamBaseBodyC()
    {}
    //: Destructor.
    
    virtual bool UpdateStream() = 0;
    //: Update sub sequence handle..
    
    bool DoSeek(UIntT off);
    //: Seek to location in stream.
    
    bool DoDSeek(IntT off);
    //: Delta Seek, goto location relative to the current one.
    
    UIntT DoSize() const;
    //: Work out size of sequence as best we can.
    
    bool PostGet();
    //: Post get .
    
    bool PostPut();
    //: Post put .
    
    inline bool ChangeEdit() {
      if(!curEdit.IsValid())
	return true;
      return !curEdit.InsertRange().Contains(pos);
    }
    //: Need to change edit ?
    
  protected:
    IndexC pos;         // Index of next item to be written/read
    IndexC editNo;     // Edit number we're currently on.
    EditSpecC curEdit; // Edit we're currently playing.
    PlayListC playList;
    UIntT maxWriteSeq;  // Maximum length sequences that can be written. 
    bool verbose;

    // Stuff specifically for writing streams.
    bool gotEOS;
    StringC fileTempl;
    StringC playListFile;
  };
  
  //! userlevel=Develop
  //: Input Stream Composition.
  // This class takes a number of elements from other streams
  // and combines them into a single stream
  
  template<class DataT>
  class CompositeIStreamBodyC 
    : public DPISPortBodyC<DataT>,
      public CompositeStreamBaseBodyC
  {
  public:
    CompositeIStreamBodyC(const PlayListC &aplayList)
      : CompositeStreamBaseBodyC(aplayList)
      { UpdateStream(); }
    //: Construct from a play list.
    
    bool UpdateStream() {
      if(ChangeEdit()) {
	curEdit = playList.FindEdit(pos.V());
	if(!curEdit.IsValid()) {
	  if(verbose) 
	    cerr << "CompositeIStreamC: Failed to find EditSpec for offset :" << pos << "\n";
	  subSeq.Invalidate();
	  return false; // No edit for this offset.
	}
	if(verbose)
	  cerr << "CompositeIStreamC: Loading new sub sequence '" << curEdit.Filename() << "' in format '" << curEdit.FileFormat() << "' \n";
	subSeq.Invalidate();
	if(!curEdit.IsSingle()) {
	  // Open a sub sequence.
	  if(!OpenISequence(subSeq,curEdit.Filename(),curEdit.FileFormat(),verbose)) {
	    if(verbose) 
	      cerr << "CompositeIStreamC: Failed to open sub sequence '" << curEdit.Filename() << "' in format '" << curEdit.FileFormat() << "' \n";
	    return false; // Failed to open the new sequence.
	  }
	} else {
	  DPIPortC<DataT> ip(BaseLoad(curEdit.Filename(),curEdit.FileFormat(),typeid(DataT),verbose));
	  if(!ip.IsValid()) {
	    cerr << "CompositeIStreamC: Failed to open frame '" << curEdit.Filename() << "' in format '" << curEdit.FileFormat() << "' \n";
	    return false; // Failed to open the new frame sequence.
	  }
	  // Just load single frame.
	  subSeq = DPISPortAttachC<DataT>(ip,false); // Don't make is seekable.
	  RavlAssert(subSeq.IsValid());
	}
	// Need to retrieve more info about this sequence ?
	if(curEdit.Range().Max() == RavlConstN::maxInt) {
	  UIntT ssize = subSeq.Size();
	  if(ssize != ((UIntT) -1)) {
	    curEdit.Range().Max() = ssize;
	    playList.UpdateRangeIndex();	    
	  }
	}
	if(!subSeq.IsValid()) {
	  cerr << "CompositeIStreamC: Failed to open valid sub sequence '" << curEdit.Filename() << "' in format '" << curEdit.FileFormat() << "' \n";
	  return false;
	}
      }
      // Seek to the right place.
      if(!subSeq.IsValid()) {
	if(verbose)
	  cerr << "CompositeIStreamC: No sub sequence setup for '" << curEdit.Filename() << "' in format '" << curEdit.FileFormat() << "' \n";
	return false;
      }
      if(!curEdit.IsSingle()) { // Only seek if we can.
	UIntT openAt = subSeq.Tell();
	UIntT initalPos = (pos - curEdit.InsertRange().Min()).V();
	if(initalPos != openAt) {
	  if(!subSeq.Seek(initalPos)) {
	    UIntT np = subSeq.Tell();
	    if(np != ((UIntT) -1)) { // Got a valid position ?
	      cerr << "CompositeIStreamC: WARNING: Updating position in sub stream to " << (np + curEdit.InsertRange().Min()) << ", because seek to " << initalPos <<" failed. \n";
	      pos = np + curEdit.InsertRange().Min();
	    }
	  }
	}
      }
      return true;
    }
    //: Update the stream position.
    
    virtual bool Seek(UIntT off) {
      return DoSeek(off);
    }
    //: Seek to location in stream.
    
    virtual bool DSeek(IntT off) { 
      return DoDSeek(off);
    }
    //: Delta Seek, goto location relative to the current one.
    
    virtual UIntT Tell() const { 
      if(pos < 0)
	return 0; // This shouldn't happen but...
      return pos.V(); 
    }
    //: Find current location in stream.
    // Defined as the index of the next object to be written or read.
    
    virtual UIntT Size() const { 
      return DoSize();
    }
    //: Find the total size of the stream. (assuming it starts from 0)
        
    virtual bool Get(DataT &buff) {
      do {
	if(!subSeq.IsValid())
	  return false;
	if(subSeq.Get(buff)) {
	  PostGet();
	  break;
	}
	if(playList.IsLast(curEdit)) { // End of the sequence ?
	  subSeq.Invalidate();
	  return false;
	}
	if(verbose)
	  cerr << "CompositeIStreamBodyC::Get(), Failed to get item from subsequence, assuming end of stream at " << pos << " in '" << curEdit.Filename() << "'\n";
	curEdit.Range().Max() = ((pos - curEdit.InsertRange().Min()) + curEdit.Range().Min())-1;
	playList.UpdateRangeIndex();
	UpdateStream();
      } while(1);
      return true;
    }
    //: Get element from stream.
    
    virtual DataT Get() {
      DataT ret;
      if(!Get(ret))
	throw DataNotReadyC("CompositeIStreamBodyC::Get(), Data not ready. ");
      return ret;
    }
    //: Get an element from the stream.
    
    virtual bool IsGetReady() const {
      //cerr << "Checking IsGetReady(); " << subSeq.IsValid() << "\n";
      if(!subSeq.IsValid())
	return false;
      if(!curEdit.IsValid())
	return false;
      if(curEdit.InsertRange().Max() < pos)
	if(playList.IsLast(curEdit))
	  return false;
      return true;
    }
    //: Test is we're ready to get data.
    
    virtual bool IsGetEOS() const 
      { return !subSeq.IsValid(); }
    //: Has the End Of Stream been reached ?
    // true = yes.
    
  protected:
    DPISPortC<DataT> subSeq; // Handle to IO for current sub-sequence.
  };
  
  //! userlevel=Normal
  //: Input Stream Composition.
  // This class takes a number of elements from other streams
  // and generates a composite.
  
  template<class DataT>
  class CompositeIStreamC 
    : public DPISPortC<DataT>
  {
  public:
    CompositeIStreamC()
      {}
    //: Default constructor.

    CompositeIStreamC(const PlayListC &playList)
      : DPEntityC(*new CompositeIStreamBodyC<DataT>(playList))
      {}
    //: Play list constructor.
    
  protected:
  };
  
  ////// OUTPUT STREAMS ////////////////////////////////////////////////////////

  //! userlevel=Develop
  //: Output Stream Composition body
  // This class takes a number of elements from other streams
  // and generates a composite.
  
  template<class DataT>
  class CompositeOStreamBodyC 
    : public DPOSPortBodyC<DataT>,
      public CompositeStreamBaseBodyC
  {
  public:
    CompositeOStreamBodyC(const PlayListC &specs)
      :  CompositeStreamBaseBodyC(specs)
      {
	if(playList.IsEmpty()) {
	  StringC fn = fileTempl.Copy();
	  fn.gsub("%d",StringC(pos));
	  playList.Append(EditSpecC(fn,0,RavlConstN::maxInt));
	}
	UpdateStream();
      }
    //: Constructor.

    CompositeOStreamBodyC(const StringC &playListFn)
      :  CompositeStreamBaseBodyC(playListFn,true)
      {
	if(playList.IsEmpty()) {
	  StringC fn = fileTempl.Copy();
	  fn.gsub("%d",StringC(pos));
	  playList.Append(EditSpecC(fn,0,RavlConstN::maxInt));
	}
	UpdateStream();
      }
    //: Constructor.

    virtual bool Save(ostream &out) const 
    { return DPOSPortBodyC<DataT>::Save(out); }
    //: Save to ostream.
    
    bool UpdateStream() {
      if(gotEOS)
	return false;
      if(curEdit.InsertRange().Max() == RavlConstN::maxInt) {
	if((UIntT) (pos - curEdit.InsertRange().Min()).V() > maxWriteSeq) {	
	  curEdit.InsertRange().Max() = pos - (curEdit.InsertRange().Min() + 1); // Update old edit.
	  StringC fn = fileTempl.Copy();
	  fn.gsub("%d",StringC(pos));
	  playList.Append(EditSpecC(fn,0,RavlConstN::maxInt,curEdit.ChannelId(),curEdit.FileFormat()));
	  // Save updates to play list file.
	  if(playListFile != "") {
	    if(!RavlN::Save(playListFile,playList))
	      cerr << "CompositeOStreamBodyC::UpdateStream(), WARNING: Failed to save play list '" << playListFile << "' \n";
	  }
	}
      }
      if(ChangeEdit()) {
	curEdit = playList.FindEdit(pos.V());
	if(!curEdit.IsValid()) {
	  if(verbose) 
	    cerr << "CompositeOStreamC: Failed to find EditSpec for offset :" << pos << "\n";
	  subSeq.Invalidate();
	  return false; // No edit for this offset.
	}
	if(verbose)
	  cerr << "CompositeOStreamC: Loading new sub sequence '" << curEdit.Filename() << "' in format '" << curEdit.FileFormat() << "' \n";
	subSeq.Invalidate();
	if(!OpenOSequence(subSeq,curEdit.Filename(),curEdit.FileFormat(),verbose)) {
	  if(verbose) 
	    cerr << "CompositeOStreamC: Failed to open sub sequence '" << curEdit.Filename() << "' in format '" << curEdit.FileFormat() << "' \n";
	  return false; // Open the new sequence.
	}
      }
      // Seek to the right place.
      if(!subSeq.IsValid()) {
	if(verbose)
	  cerr << "CompositeOStreamC: No sub sequence setup for '" << curEdit.Filename() << "' in format '" << curEdit.FileFormat() << "' \n";
	return false;
      }
      if(!subSeq.Seek((pos - curEdit.InsertRange().Min()).V())) {
	UIntT np = subSeq.Tell();
	if(np != ((UIntT) -1)) { // Got a valid position ?
	  cerr << "CompositeOStreamC: WARNING: Updating position in sub stream, because seek failed. \n";
	  pos = np + curEdit.InsertRange().Min();
	}
      }
      return true;
    }
    
    virtual void PutEOS() { 
      if(!gotEOS) {
	if(playListFile != "") {
	  if(!RavlN::Save(playListFile,playList))
	    cerr << "CompositeOStreamBodyC::PutEOS(), WARNING: Failed to save play list '" << playListFile << "' \n";
	}
      }
      gotEOS = true;
    }
    //: Put End Of Stream marker.
    
    virtual bool IsPutReady() const {
      if(gotEOS)
	return false;
      return subSeq.IsValid();
    }
    //: Is port ready for data ?
    
    virtual bool Put(const DataT &dat) { 
      if(gotEOS)
	return false;
      if(!subSeq.Put(dat))
	return false;
      PostPut();
      return true;
    }
    //: Put data.

    virtual bool Seek(UIntT off) {
      if(gotEOS)
	return false;
      return DoSeek(off);
    }
    //: Seek to location in stream.
    
    virtual bool DSeek(IntT off) { 
      if(gotEOS)
	return false;
      return DoDSeek(off);
    }
    //: Delta Seek, goto location relative to the current one.
    
    virtual UIntT Tell() const
      { return pos.V(); }
    //: Find current location in stream.
    // Defined as the index of the next object to be written or read.
    
    virtual UIntT Size() const 
      { return DoSize(); }
    //: Find the total size of the stream. (assuming it starts from 0)
    
  protected:
    DPOSPortC<DataT> subSeq; // Handle to IO for current sub-sequence.
  };
  
  //! userlevel=Normal
  //: Output Stream Composition.
  // This class takes a number of elements from other streams
  // and generates a composite.
  
  template<class DataT>
  class CompositeOStreamC 
    : public DPOSPortC<DataT>
  {
  public:
    CompositeOStreamC(const PlayListC &specs)
      : DPEntityC(*new CompositeOStreamBodyC<DataT>(specs))
      {}
    //: Constructor.

    CompositeOStreamC(const StringC &fn)
      : DPEntityC(*new CompositeOStreamBodyC<DataT>(fn))
      {}
    //: Constructor.
    
  protected:
  };

}
#endif
