#ifndef RAVLIMAGE_MULTI_VID_IO_HH_
#define RAVLIMAGE_MULTI_VID_IO_HH_
// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlVideoIO
//! author="Joel Mitchelson"
//! docentry="Ravl.API.Images.Video.Video IO"
//! file="Ravl/Image/VideoIO/MultiVidIO.hh"

#include "Ravl/DP/FileFormat.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/SArray1d.hh"
#include "Ravl/SDArray1d.hh"
#include "Ravl/StrStream.hh"
#include "Ravl/DP/SPort.hh"
#include "Ravl/DP/SequenceIO.hh"
#include "Ravl/CDLIter.hh"
#include "Ravl/Stream.hh"
#include "Ravl/OS/Filename.hh"

#if RAVL_HAVE_ANSICPPHEADERS
#include <iomanip>
#else
#include <iomanip.h>
#endif

namespace RavlImageN
{
  using namespace RavlN;
  using namespace RavlImageN;

  //! userlevel=Develop
  //: Multi-view video sequence input body class

  template<class PixelC> class DPIMultiVidBodyC 
    : public DPISPortBodyC< SArray1dC< ImageC< PixelC > > >
  {
  public:
    DPIMultiVidBodyC(const DListC<StringC>& view_list)
    {
      LoadFileList(view_list);
    }
    //: construct from list of file names

    DPIMultiVidBodyC(const StringC &fnBase, const StringC& fnFormat) 
    {
      // std::cerr << "DPIMultiVidBodyC::DPIMultiVidBodyC(...)" << std::endl;

      // list of names of sequences for each view
      DListC<StringC> view_list;

      // look for sequence files
      for (UIntT iview = 0; true; iview++) 
      {
	// filename of first frame for this view
	FilenameC fnFirstFile;
	fnFirstFile.form("%s.%d.00000.%s", 
			 (const char*)fnBase,
			 iview,
			 (const char*)fnFormat);

	// check it exists
	if (!fnFirstFile.IsReadable())
	  break;
        
	// filename of sequence
	FilenameC fnSequence;
	fnSequence.form("%s.%d.%s.%s", 
			(const char*)fnBase,
			iview,
			"%05d",
			(const char*)fnFormat);
        
	// add to list
	view_list += fnSequence;
      }
      
      // load files
      if (!LoadFileList(view_list))
	return;

      // numframes should be set by now, but it seems
      // port.Size() doesn't always work
      if (numframes == 0)
      {
	while (1)
	{
	  FilenameC str_frame; 
	  str_frame.form("%s.0.%05d.%s",
			 (const char*)fnBase,
			 numframes,
			 (const char*)fnFormat);
	  // std::cerr << "Looking for: " << str_frame.String() << std::endl;
	  if (!str_frame.IsReadable())
	    break;
	  ++numframes;
	}
      }
    }
    //: Construct from base filename and format

  protected:

    bool LoadFileList(const DListC<StringC>& view_list) 
    {
      std::cerr << "DPIMultiVidBodyC::LoadFileList(...)" << std::endl;
      
      // empty stream
      frame = 0;
      numviews = 0;
      numframes = 0;
      
      // look for sequence files
      for (ConstDLIterC<StringC> ifile(view_list); ifile; ifile++)
      {
	// create new port for sequence
	DPISPortC< ImageC<PixelC> > port;

	// open sequence
	// if not ready, can't use this sequence
	if (!OpenISequence(port, *ifile, "", false) || 
	    !port.IsGetReady())
	{
	  array_port.Empty();
	  numviews = 0;
	  return false;
	}
		
	// take min frame count as number of frames
	if (numframes == 0)
	  numframes = port.Size();
	else if (port.Size() < numframes)
	  numframes = port.Size();

	// add to array
	array_port.Add(port);

	// continue until we have found all the views there are
	++numviews;
      }

      return true;
    }
    //: Initialise port from list of filenames

  public:    
    virtual bool GetAttr(const StringC &attrName,StringC &attrValue)
    {
      if (downcase(attrName) == "numviews")
      {
	attrValue = StringC(numviews);
	return true;
      }
      else
      {
	return DPISPortBodyC< SArray1dC< ImageC< PixelC > > >::GetAttr(attrName, attrValue);
      }
    }
    //: Port attributes.  The <i>numviews</i> attribute is implemented in this class.

    virtual bool Seek(UIntT off)
    {
      if (off >= numframes)
	return false;
      frame = off;
      SyncViews();
      return true;
    }
    //: Seek to location in stream.
    // Returns FALSE, if seek failed.
    // if an error occurered (Seek returned False) then stream
    // position will not be changed.
    
    virtual bool DSeek(IntT off) {
      // std::cerr << "DSeek: " << frame << " + " << off << std::endl;
      if (off < 0) {
	UIntT neg_off = (UIntT)(-off);
	if (neg_off > frame)
	  return false;
	frame -= neg_off;
      } else {
	if (off + frame >= numframes)
	  return false;
	frame += off;
      }
      SyncViews();
      return true;
    }
    //: Delta Seek, goto location relative to the current one.
    
    virtual UIntT Tell() const
    { return frame; } 
    //: Find current location in stream.
    
    virtual UIntT Size() const
    { return numframes; }
    //: Find the total size of the stream.
    
    virtual SArray1dC< ImageC<PixelC> > Get() {
      SArray1dC< ImageC<PixelC> > view(numviews);
      Get(view);
      return view;
    }
    //: Get next image.
    
    virtual bool Get(SArray1dC< ImageC<PixelC> > &buff) {
      // std::cerr << "Get()" << std::endl;
      
      if (!IsGetReady())
	return false;

      // std::cerr << "getting all views" << std::endl;

      RavlAssert(buff.Size() == 0 || buff.Size() == numviews); // only know how to deal with these
      
      if (buff.Size() == 0) {
	// std::cerr << "resizing array" << std::endl;
	buff = SArray1dC< ImageC<PixelC> > (numviews);
      }
      
      for (UIntT iview = 0; iview < numviews; iview++)
	array_port[iview].Get(buff[iview]);
      
      ++frame;
      SyncViews();
      return true;
    }
    //: Get next image.
    
    virtual bool IsGetReady() const 
    { return (frame < numframes); }
    //: Is some data ready ?
    // TRUE = yes.
    // Defaults to !IsGetEOS().
    
    virtual bool IsGetEOS() const
    { return (frame >= numframes); }
    //: Has the End Of Stream been reached ?
    // TRUE = yes.

  protected:
    void SyncViews() {
      for (UIntT iview = 0; iview < numviews; iview++)
	array_port[iview].Seek(frame);
    }

  protected:
    UIntT numviews;
    UIntT frame;
    UIntT numframes;
    SDArray1dC< DPISPortC< ImageC<PixelC> > > array_port;
  };

  //! userlevel=Develop
  //:Multi-view video sequence input port
  //
  // Used for reading syncronised multi-view video sequences
  // from disk or from frame grabbers. <p>
  // 
  // Can open an arbitary list of video files/sequences, 
  // or sequences of the form basename.C.FFFFF.type <br>
  // Where:<br>
  //   basename is a text id for the file<br>
  //   C        is a 1 digit view number<br>
  //   FFFFF    is a 5 digit frame number<br>
  //   type     is the file type, such as "tif", "ppm", etc<br>
  // <br>
  // To open a file sequence named in this way use: <br>
  // <br>
  //   DPISPortC< SArray1dC< ImageC<DataT> > > port;<br>
  //   OpenISequence(port, "@multiview:basename:type");<br>
  // <br>
  // Sequences are expected to start from frame 0, view 0
  // If no type is specified, tif is assumed
  // <br>
  // To open a predefined list, use:<br>
  // <br>
  //   DPISPortC< SArray1dC< ImageC<DataT> > > port;<br>
  //   OpenISequence(port, "@viewlist:seq1;seq2;...;seqN");<br>
  //

  template<class PixelC> class DPIMultiVidC 
    : public DPISPortC< SArray1dC< ImageC< PixelC > > >
  {
  public:
    DPIMultiVidC(const DListC<StringC>& view_list)
      : DPEntityC(*new DPIMultiVidBodyC<PixelC>(view_list))
    { }
    //: construct from list of sequences/files, one per view

    DPIMultiVidC(const StringC &fnBase, const StringC& fnFormat)
      : DPEntityC(*new DPIMultiVidBodyC<PixelC>(fnBase, fnFormat))
    { }
    //: construct from base filename and format
  };

  //! userlevel=Develop
  //: Multi-view video sequence output body class

  template<class PixelC> class DPOMultiVidBodyC 
    : public DPOSPortBodyC< SArray1dC< ImageC<PixelC> > >
  {
  public:
    DPOMultiVidBodyC(const StringC &nfnBase, const StringC& nfnFormat) :
      fnBase(nfnBase),
      fnFormat(nfnFormat),
      done_init(false),
      frame(0)
    {}
    //: Construct from base filename and format.
    //  Actual opening of streams is delayed until the first Put(...) operation.

    DPOMultiVidBodyC(const DListC<StringC>& view_list) :
      done_init(false),
      frame(0)
    {
      // allocate array of ports, one for each view
      array_port = SArray1dC< DPOSPortC< ImageC<PixelC> > > (view_list.Size());

      // open the ports
      UIntT iview = 0;
      for (ConstDLIterC<StringC> ifile(view_list); ifile; ifile++)
	OpenOSequence(array_port[iview++], *ifile, "", false);

      // initialisation complete
      done_init = true;
    }
    //: Construct from list of sequence names

    virtual bool Seek(UIntT off)
    { return false; }
    //: Seek to location in stream.
    // Returns FALSE, if seek failed. (Maybe because its
    // not implemented.)
    // if an error occurered (Seek returned False) then stream
    // position will not be changed.
    
    virtual bool DSeek(IntT off)
    { return false; }
    //: Delta Seek, goto location relative to the current one.
    
    virtual UIntT Tell() const
    { return frame; }
    //: Find current location in stream.
    
    virtual UIntT Size() const
    { return frame; }
    //: Find the total size of the stream.
    
    bool Put(const SArray1dC< ImageC<PixelC> > &buff)
    {
      if (array_port.Size() == 0)
      {
	RavlAssert(buff.Size() > 0);
	array_port = SArray1dC< DPOSPortC< ImageC<PixelC> > > (buff.Size());
      }

      if (!done_init) {
	for (UIntT iview = 0; iview < array_port.Size(); iview++) {
	  StrOStreamC fnSequence;
	  fnSequence << fnBase << "." << setw(1) << iview << ".%05d." << fnFormat;
	  OpenOSequence(array_port[iview], fnSequence.String(), "", false);
	}
	done_init = true;
      }

      RavlAssert(array_port.Size() == buff.Size()); 
      // must used fixed number of views per frame

      bool putok = true;
      for (UIntT iview = 0; iview < array_port.Size(); iview++)
      {
	if (!array_port[iview].Put(buff[iview]))
	  putok = false;
      }

      ++frame;
      return putok;
    }
    //: Put image to a stream.
    
    virtual bool IsPutReady() const {
      if (!done_init)
	return true;

      for (UIntT iview = 0; iview < array_port.Size(); iview++) {
	if (!array_port[iview].IsPutReady())
	  return false;
      }
      
      return true;
    }
    //: Read to write some data ?
    // TRUE = yes.
    
    virtual void PutEOS() {}
    //: Put End Of Stream marker.
    
   protected:
    StringC fnBase;
    StringC fnFormat;
    bool done_init;
    SArray1dC< DPOSPortC< ImageC<PixelC> > > array_port;
    UIntT frame;
   };
  
  //! userlevel=Normal
  //:Multi-view video sequence output port
  //
  // Used for saving multi-view video sequences to disk<p>
  //
  // Saves sequences to a specified list of files, 
  // or files with names of the form basename.C.FFFFF.type <br>
  // Where:<br>
  //   basename is a text id for the file<br>
  //   C        is a 1 digit view number<br>
  //   FFFFF    is a 5 digit frame number<br>
  //   type     is the file type, such as "tif", "ppm", etc<p>
  //
  // To save a sequence to disk using this convention: <br>
  // <br>
  //   DPOSPortC< SArray1dC< ImageC<DataT> > > port;<br>
  //   OpenOSequence(port, "@multiview:basename:type");<p>
  // <br>
  // Sequences start at frame 0, view 0<br>
  // If no type is specified, tif is assumed<br>
  // <br>
  // Otherwise, use:<br>
  // <br>
  //   DPOSPortC< SArray1dC< ImageC<DataT> > > port;<br>
  //   OpenOSequence(port, "@viewlist:seq1;seq2;...;seqN");<p>
  // <br>

  template<class PixelC> class DPOMultiVidC 
    : public DPOSPortC< SArray1dC< ImageC< PixelC > > >
  {
  public:

    DPOMultiVidC(const DListC<StringC>& view_list)
      : DPEntityC(*new DPOMultiVidBodyC<PixelC>(view_list))
    { }
    //: Construct from base filename and format.
    //  Actual opening of streams is delayed until the first Put(...) operation.

    DPOMultiVidC(const StringC &fnBase, const StringC& fnFormat)
      : DPEntityC(*new DPOMultiVidBodyC<PixelC>(fnBase, fnFormat))
    { }
    //: Construct from base filename and format.
    //  Actual opening of streams is delayed until the first Put(...) operation.  
  };
  
  //! docentry="Ravl.API.Images.Video.Video IO"
  //! userlevel=Develop
  //:Multi-view video sequence format body

  template<class PixelC>  class FileFormatMultiVidBodyC : public FileFormatBodyC
  {
  public:
    FileFormatMultiVidBodyC() :
      FileFormatBodyC("multiview-video-sequence")
    {}
    
    virtual const std::type_info &ProbeLoad(IStreamC &in,const std::type_info &obj_type) const
    { return typeid(void); }
    //: always return 0 - cannot determine type from stream
    
    virtual const std::type_info &ProbeLoad(const StringC &filename,IStreamC &in,const std::type_info &obj_type) const {
      if ((filename.contains("@multiview:") || 
	   filename.contains("@viewlist")) &&
	  obj_type == typeid(SArray1dC< ImageC< PixelC > >))
      { return obj_type; }
      return typeid(void);
    }
    //: Probe for load.  
    
    virtual const std::type_info &ProbeSave(const StringC &filename,const std::type_info &obj_type,bool forceFormat) const {
      if ((filename.contains("@multiview:") ||
	   filename.contains("@viewlist:")) &&
	  obj_type == typeid(SArray1dC< ImageC< PixelC > >))
	return obj_type; 
      return typeid(void);    
    }
    //: Probe for Save.
    
    virtual DPIPortBaseC CreateInput(IStreamC &in,const std::type_info &obj_type) const {
      DPIPortC< ImageC<PixelC> > null_port;
      return null_port;
    }
    //: Create a input port for loading.
    // Will create an Invalid port if not supported.
    
    virtual DPOPortBaseC CreateOutput(OStreamC &out,const std::type_info &obj_type) const {
      DPOPortC< ImageC<PixelC> > null_port;
      return null_port;
    }
    //: Create a output port for saving.
    // Will create an Invalid port if not supported.
    
    virtual DPIPortBaseC CreateInput(const StringC &filename,const std::type_info &obj_type) const 
    {
      if (filename.contains("@viewlist:"))
      {
	DListC<StringC> view_list;
	ParsePathName(filename, view_list);
	return DPIMultiVidC<PixelC>(view_list);
      }
      else
      {
	StringC fnBase;
	StringC fnFormat;
	ParsePathName(filename, fnBase, fnFormat);
	return DPIMultiVidC<PixelC>(fnBase, fnFormat);
      }
    }
    //: Create a input port for loading from file 'filename'.
    // Will create an Invalid port if not supported. <p>
    
    virtual DPOPortBaseC CreateOutput(const StringC &filename,const std::type_info &obj_type) const 
    {
      if (filename.contains("@viewlist:"))
      {
	DListC<StringC> view_list;
	ParsePathName(filename, view_list);
	return DPOMultiVidC<PixelC>(view_list);
      }
      else
      {
	StringC fnBase;
	StringC fnFormat;
	ParsePathName(filename, fnBase, fnFormat);
	return DPOMultiVidC<PixelC>(fnBase, fnFormat);      
      }
    }
    //: Create a output port for saving to file 'filename'..
    // Will create an Invalid port if not supported. <p>
    
    virtual const std::type_info &DefaultType() const
    { return typeid( SArray1dC< ImageC< PixelC > > ); }
    //: Get prefered IO type.
    
    virtual IntT Priority() const { return 0; }
    //: Find the priority of the format. the higher the better.
    // Default is zero, use this if nothing better.
    
    virtual bool IsStream() const { return true; }
    //: Test if format is a fully streamable.
    // i.e. check if you can read/write more than object object.

  protected:
    static bool ParsePathName(const StringC& filename, StringC& fnBase, StringC& fnFormat) {
      StringC fnBaseLong = StringC(filename).after(':');

      // base is first iterm after @multiview
      if (fnBaseLong.contains(':')) {
	fnBase = fnBaseLong.before(':');
	fnFormat = fnBaseLong.after(':');
      } else {
	fnBase = fnBaseLong;
	fnFormat = "tif"; // default format
      }
      return true;
    }
    //: work out video file base name and file format from specified path

    static bool ParsePathName(const StringC& filename, DListC<StringC>& view_list)
    {
      StrIStreamC stream_list(StringC(filename).after(':'));
      for (int i = 0; i < 10; i++)
      {
	StringC str_view = stream_list.ClipTo(';');
	if (str_view.IsEmpty())
	  break;
	view_list += str_view;
      }
      return true;
    }
    //: work out list of files from specified path
  };

  //! docentry="Ravl.API.Images.Video.Video IO"
  //! userlevel=Advanced
  //:Multi-view video sequence format definition class

  template<class PixelC> class FileFormatMultiVidC 
    : public FileFormatC< SArray1dC< ImageC<PixelC> > >
  {
  public:
    FileFormatMultiVidC()
      : FileFormatC< SArray1dC< ImageC<PixelC> > >(*new FileFormatMultiVidBodyC<PixelC>())
    {}
  };
  
  //! userlevel=Develop
  //:Initialise multi-view video sequence IO 
  // See DPIMultiVidC and DPOMultiVidC for details
  void InitMultiVidIO();
};

#endif
