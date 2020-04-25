#ifndef MEM_ITEM_HH
#define MEM_ITEM_HH

#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/String.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/StrStream.hh"
#include "Ravl/Hash.hh"
#include "Ravl/TimeCode.hh"
#include "Ravl/Duration.hh"
#include "Ravl/XMLStream.hh"
#include "Ravl/IO.hh"

//! docentry="Ravl.API.OS.Multithreading Framework"
//! author="Bill Christmas"
//! lib=RavlMemModules
//! example=testMemModules.cc

//! userlevel=Normal

namespace RavlN {
  using namespace RavlImageN;

  
  //! userlevel=Develop
  //: Body for MemItemC
  class MemItemBodyC
    : public RCBodyC {

    DurationC duration;      //duration of memory item
    bool notFinalValues;  // indicates not the final item for that time code
    RCHashC<StringC, StringC>    stringAttrs; //data attributes as strings
    RCHashC<StringC, ImageC<ByteRGBValueC> > imageAttrs;  // temporary RGB image attributes, only used when SetImage() - table items should be deleted after use
    RCHashC<StringC, ImageC<ByteT> > byteImageAttrs;  // temporary byte image attributes - table items should be deleted after use

    struct ImageInfoC 
    {
      StringC fileName;  //  the image file name + number of dir it is in
      IndexRange2dC frame;
      ImageInfoC() {} 
      ImageInfoC(const StringC& FN, const IndexRange2dC& F) : fileName(FN), frame(F) {}
    };

    // The following 2 members are mutable because Save() func *has* to be
    // const, & that's the only point at which the image path can be
    // determined & put into an imageInfo.  (I tried everyting to avoid it)
    mutable FilenameC modulePath; //directory for saving and loading images
    mutable RCHashC<StringC, ImageInfoC> imageInfos; // Image attributes (file path, image frame) that are kept in memory.

    template <class PixelT>
    void DumpImages(const RCHashC<StringC, ImageC<PixelT> >& ImageAttrs,
                    const StringC& Format) const;
    //: Dump item's images to file

  public:
    // ------------  Constructors ----------------------------------------------
    MemItemBodyC()
      : duration(-1), notFinalValues(false)
    {}
    //: Default constructor with negative time (means invalid memory item)
    // Shouldn't ever be needed

    MemItemBodyC(const TimeCodeC& Time, bool NotFinalValues)
      : duration(Time), notFinalValues(NotFinalValues)
    {}
    //: Constructor on time instant.

    MemItemBodyC(const DurationC& Duration, bool NotFinalValues)
      : duration(Duration), notFinalValues(NotFinalValues)
    {}
    //: Constructor on time interval.

    MemItemBodyC(const TimeCodeC & Start, const TimeCodeC & End, bool NotFinalValues) 
      : duration(Start, End), notFinalValues(NotFinalValues)
    {}
    //: Constructor on start and end time instants. 
      
      
    //---------------- IO functions ------------------------------------------

    void Save(XMLOStreamC& Strm, const StringC& SavePrefix) const;
    //: saves item to XML stream.

    void Load(XMLIStreamC& Strm, const StringC& LoadPrefix);
    //: loads item from XML stream.


    // -------------  data access functions  -----------------------------------
    template <class typeT> typeT GetAttr(const StringC& AttrName) const;
    //: Gets an attribute from a memory item as an object

    template <class typeT> void SetAttr(const StringC& AttrName, const typeT& Obj);
    //: Sets an attribute to a memory item as an object

    const ImageC<ByteRGBValueC> GetImage(const StringC& AttrName) const;
    //: Returns the image from item

    void SetImage(const StringC& AttrName, const ImageC<ByteRGBValueC>& Image)
    { imageAttrs.Insert(AttrName, Image); }
    //: Set the image 

    const ImageC<ByteT> GetByteImage(const StringC& AttrName) const;
    //: Returns byte image from item

    void SetByteImage(const StringC& AttrName, const ImageC<ByteT>& Image)
    { byteImageAttrs.Insert(AttrName, Image); }
    //: Set byte image

    StringC Dump(int MaxLen = -1) const;
    //: Dump contents of memory item

    bool IsFinalValue() const
    { return !notFinalValues; }
    //: Indicates whether or not this is the final item for this time code when reading from XML stream

    void Details() const;

    // --------------------   parameters access  ------------------------------
    bool Eof() const
    { return (duration.Start().FrameCount() == TimeCodeC::Max().FrameCount()); }
    //: True then this is the last item in the memory. It is a null item

    bool IsValid() const
    { return duration.Start().IsValid(); }
    //: Check if current elem has been properly constructed (not by default constructor)

    bool IsAttr(const StringC& AttrName) const
    { return stringAttrs.IsElm(AttrName); }
    //: True if attribute is present in item

    TimeCodeC& Start()
    { return duration.Start(); }
    //: Get start time from memory item

    const TimeCodeC& Start() const
    { return duration.Start(); }
    //: Get start time from memory item

    TimeCodeC& End()
    { return duration.End(); }
    //: Get end time from memory item

    const TimeCodeC& End() const
    { return duration.End(); }
    //: Get end time from memory item

    DurationC& Duration()
    { return duration; }
    //: Get time bounds from memory item

    const DurationC& Duration() const
    { return duration; }
    //: Get time bounds from memory item

  };


  //! userlevel=Normal
  //: One item of a memory module storage unit

  // Storage for data of one category at one time slot, including:<ul>
  //<li>  XML text string </li>
  //<li>  optional image(s)</li>
  //<li>  time stamp</li></ul>

  // <p>A list of these forms the <a href="MemUnitC.html">memory</a> for a <a href="MemModuleC.html">MemModuleC</a> memory module, 
  // which provides input data for other modules.</p>
  // <p>Memory items are constructed on the time instant or time interval for the item;
  // use SetAttr and SetImage functions to set data.</p>

  class MemItemC
    : public RCHandleC<MemItemBodyC> 
  {
  public:
    //:------------ 
    //: Constructors

    MemItemC() 
      : RCHandleC<MemItemBodyC>(*new MemItemBodyC())
    {}
    //: Default constructor with negative time (means invalid memory item) 
    // Shouldn't ever be needed
    
    MemItemC(const TimeCodeC & Time, bool NotFinalValues=false) 
      : RCHandleC<MemItemBodyC>(*new MemItemBodyC(Time, NotFinalValues))
    {}
    //: Constructor on time instant. 
      
    MemItemC(const DurationC & Duration, bool NotFinalValues=false) 
      : RCHandleC<MemItemBodyC>(*new MemItemBodyC(Duration, NotFinalValues))
    {}
    //: Constructor on time interval. 
  
    
    MemItemC(const TimeCodeC & Start, const TimeCodeC & End, bool NotFinalValues=false) 
      : RCHandleC<MemItemBodyC>(*new MemItemBodyC(Start, End, NotFinalValues))
    {}
    //: Constructor on start and end time instants. 
      
    static MemItemC EofItem()
    { return MemItemC(TimeCodeC::Max()); }
    //: Returns item for end of memory unit

  
    //---------------- IO functions ------------------------------------------
    void Save(XMLOStreamC& Strm, const StringC& SavePrefix) const
    { Body().Save(Strm, SavePrefix); }
    //: saves item to XML stream.

    void Load(XMLIStreamC& Strm, const StringC& LoadPrefix)
    { Body().Load(Strm, LoadPrefix); }
    //: loads item from XML stream.


    //:-------------
    //: Data access functions

    template <class TypeT> TypeT GetAttr(const StringC& AttrName) const
    { return Body().GetAttr<TypeT>(AttrName); }
    //: Gets an attribute from a memory item as an object

    template <class typeT> void SetAttr(const StringC& AttrName, const typeT& Obj)
    { Body().SetAttr(AttrName, Obj); }
    //: Sets an attribute to a memory item as an object

    ImageC<ByteRGBValueC> GetImage(const StringC& AttrName) const
    { return Body().GetImage(AttrName); }
    //: Returns the image from item

    void SetImage(const StringC& AttrName, const ImageC<ByteRGBValueC>& Image)
    { Body().SetImage(AttrName, Image); }
    //: Set the image 

    ImageC<ByteT> GetByteImage(const StringC& AttrName) const
    { return Body().GetByteImage(AttrName); }
    //: Returns byte image from item

    void SetByteImage(const StringC& AttrName, const ImageC<ByteT>& Image)
    { Body().SetByteImage(AttrName, Image); }
    //: Set byte image

    StringC Dump(int MaxLen = -1) const
    { return Body().Dump(MaxLen); }
    //: Dump contents of memory item

    // --------------------   parameters access  ------------------------------
    bool Eof() const
    { return Body().Eof(); }
    //: True then this is the last item in the memory. It is a null item

    bool IsValid() const
    { return Body().IsValid(); }
    //: Check if current elem has been properly constructed (not by default constructor)

    bool IsAttr(const StringC& AttrName) const
    { return Body().IsAttr(AttrName); }
    //: True if attribute is present in item

    TimeCodeC& Start()
    { return Body().Start(); }
    //: Get start time from memory item

    const TimeCodeC& Start() const
    { return Body().Start(); }
    //: Get start time from memory item

    TimeCodeC& End()
    { return Body().End(); }
    //: Get end time from memory item

    const TimeCodeC& End() const
    { return Body().End(); }
    //: Get end time from memory item

    DurationC& Duration()
    { return Body().Duration(); }
    //: Get time bounds from memory item

    const DurationC& Duration() const
    { return Body().Duration(); }
    //: Get time bounds from memory item

    bool IsFinalValue() const
    { return Body().IsFinalValue(); }
    //: Indicates whether or not this is the final item for this time code

    void Details() const
    { return Body().Details(); }


  protected:

    MemItemC(MemItemBodyC &bod)
      : RCHandleC<MemItemBodyC>(bod)
    {}
    //: Body constructor. 
    
    MemItemBodyC& Body()
    { return static_cast<MemItemBodyC &>(RCHandleC<MemItemBodyC>::Body()); }
    //: Body Access. 
    
    const MemItemBodyC& Body() const
    { return static_cast<const MemItemBodyC &>(RCHandleC<MemItemBodyC>::Body()); }
    //: Body Access. 
  };

}  

using namespace RavlN;

//-------------------- MemItemBodyC implementation ------------------------
template <class TypeT> 
void MemItemBodyC::SetAttr(const StringC& AttrName, const TypeT& Obj)
{ 
  StrOStreamC str;
  str << Obj;
  stringAttrs.Insert(AttrName, str.String());
}
//: Sets an attribute to a memory item as an object

template <class TypeT> 
TypeT MemItemBodyC::GetAttr(const StringC& AttrName) const
{
  if(!stringAttrs.IsElm(AttrName)) 
    {
      cerr << "No attribute '" << AttrName << "' found.  Item contents:\n" << Dump() << endl;
      throw ExceptionC("MemItemBodyC::GetAttr: Cannot find attribute: " + AttrName);
    }
  StrIStreamC str(stringAttrs[AttrName]);
  if(str.String().IsEmpty()) 
    {
      cerr << "Could not get memory item attribute:" << AttrName << endl;
      throw ExceptionC("MemItemBodyC::GetAttr:Empty attribute");
    }
  TypeT attr;
  str >> attr;
  return attr;
}


//: dump all images of type PixelT into files
template <class PixelT>
void MemItemBodyC::DumpImages(const RCHashC<StringC, ImageC<PixelT> >& imageAttrs,
                                 const StringC& Format) const
{
  if (imageAttrs.IsEmpty())
    return;
  if (!modulePath.Exists())
    RavlAlwaysAssertMsg(modulePath.MakeDir(),
                        "MemItemBodyC::DumpImages: Could not make directory " + modulePath);
  for(HashIterC<StringC, ImageC<PixelT> > imageIter(imageAttrs); imageIter; imageIter.Del())  {
    // Note image deleted from imageAttrs as last "for" arg
    int dirNum = int(Start().getFrameCount() / 10000); // create subdir name
    FilenameC dir = modulePath + "/" + StringC(dirNum); // subdir full path 
    dir.MakeDir();
    StrOStreamC buf;
    buf.form("%06i", int(Start().getFrameCount()));  // create file no.
    StringC imageName = imageIter.Key() + buf.String() + "." + Format;
    StringC imageString = StringC(dirNum) + "/" + imageName;
    StringC imagePath = dir + "/" + imageName;
    RavlAlwaysAssertMsg(imageIter->IsValid(),
                        "MemItemBodyC::DumpImages: Invalid image: " + imageName);
    if (imageIter->Size() <= 0) 
      RavlN::Save(imagePath, *imageIter, "ppm", false);
    else
      RavlAlwaysAssertMsg(RavlN::Save(imagePath, *imageIter, Format, false),
                          "MemItemBodyC::DumpImages: Cannot save image: " + imagePath);
    imageInfos.Insert(imageIter.Key(), ImageInfoC(imageString, imageIter->Frame())); 
  }

}


//! userlevel=Normal

inline ostream & operator<<(ostream &Stream, const MemItemC& M)
{ return Stream << M.Dump() << endl; }
//: Stream operator for MemItemC uses Dump() format



#endif

