// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlOSIO
//! file="Ravl/OS/IO/CompositeStream.cc"

#include "Ravl/DP/CompositeStream.hh"
#include "Ravl/DP/FileFormatIO.hh"
#include "Ravl/OS/Filename.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x)
#else
#define ONDEBUG(x) x
#endif

namespace RavlN {
 

  //: Default constructor.
  
  CompositeStreamBaseBodyC::CompositeStreamBaseBodyC() 
    : pos(0),
      maxWriteSeq(100),
#if DODEBUG
      verbose(true),
#else
      verbose(false),
#endif
      gotEOS(false)
  {
  }
    
  //: Constructor.
  
  CompositeStreamBaseBodyC::CompositeStreamBaseBodyC(const PlayListC &nPlayList) 
    : pos(0),
      playList(nPlayList),
      maxWriteSeq(100),
#if DODEBUG
      verbose(true),
#else
      verbose(false),
#endif
      gotEOS(false)
  {
    if(playList.IsValid())
      playList.UpdateRangeIndex();
  }

  //: Constructor with name of play list.
  
  CompositeStreamBaseBodyC::CompositeStreamBaseBodyC(const StringC &plfn,bool forWrite)
    : pos(0),
      maxWriteSeq(100),
#if DODEBUG
      verbose(true),
#else
      verbose(false),
#endif
      gotEOS(false),
      playListFile(plfn)
  {
    if(!Load(playListFile,playList)) {
      cerr << "FileFormatCompositeBodyC::CreateInput(), WARNING: Failed to load playlist '" << plfn << "' \n";
      playList = PlayListC(true);
      StringC ext = FilenameC(playListFile).Extension();
      StringC rest = playListFile.before(-((int) ext.length()));      
      fileTempl = rest + "%d" + ext;
      playListFile = rest + ".play";
    } else {
      StringC baseFile;
      if(playList.IsEmpty())
	baseFile = plfn;
      else {
	EditSpecC ed = playList.FindEdit(0);
	if(ed.IsValid())
	  baseFile = ed.Filename();
	else
	  baseFile = plfn;
      }
      StringC ext = FilenameC(playListFile).Extension();
      StringC rest = playListFile.before(-((int) ext.length()));
      fileTempl = rest + "-seq-%d" + ext;
    }    
  }

  bool CompositeStreamBaseBodyC::DoSeek(UIntT off) {
    if(off == pos)
      return true;
    if(off > ((UIntT) RavlConstN::maxInt))
      return false;
    IndexC wasAt = pos;
    pos = off;
    if(!UpdateStream()) {
      pos = wasAt; // Restore old setting.
      UpdateStream();
      return false;
    }
    return true;
  }
  //: Seek to location in stream.
  
  bool CompositeStreamBaseBodyC::DoDSeek(IntT off) { 
    if(off == 0)
      return true;
    IndexC wasAt = pos;
    pos += off;
    if(pos < 0) {
      pos = wasAt;
      UpdateStream();
      return false;
    }
    if(!UpdateStream()) {
      pos = wasAt;
      UpdateStream();
      return false;
    }
    return true;
  }
  //: Delta Seek, goto location relative to the current one.

  
  UIntT CompositeStreamBaseBodyC::DoSize() const {
    UIntT ts = playList.TotalSize();
    if(((IntT) ts) == RavlConstN::maxInt)
      return ((UIntT) -1); // Don't know!.
    return ts; 
  }
  //: Work out size of sequence as best we can.
  
  bool CompositeStreamBaseBodyC::PostGet() {
    pos++;
    if(!curEdit.IsValid()) {
      //cerr << "PostGet(), Failed. \n";
      UpdateStream();
      return true; 
    }
      //cerr << "PostGet: " << pos << " "<<  curEdit.InsertRange().Max() << "\n";
    if(pos > curEdit.InsertRange().Max())
      UpdateStream();
    return true;
  }
  //: Post get .
  
  bool CompositeStreamBaseBodyC::PostPut() {
    pos++;
    if(!curEdit.IsValid()) {
      //cerr << "PostGet(), Failed. \n";
      UpdateStream();
      return true; 
    }
    if((UIntT) (pos - curEdit.InsertRange().Min()).V() > maxWriteSeq) {
      UpdateStream(); // Maximum length for a single sequence exceeded.
      return true;
    }
    //cerr << "PostGet: " << pos << " "<<  curEdit.InsertRange().Max() << "\n";
    if (pos > curEdit.InsertRange().Max())
      UpdateStream(); // Maximum length for a single sequence exceeded.
    return true;
  }
  //: Post put .

  
}

