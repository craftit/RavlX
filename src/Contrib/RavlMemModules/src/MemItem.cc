#include "Ravl/Threads/MemItem.hh"

#include "Ravl/HashIter.hh"
#include "Ravl/XMLTree.hh"
#include "Ravl/CDLIter.hh"
#include <sstream>

using namespace RavlN;

//: loads item from XML stream.
void MemItemBodyC::Load(XMLIStreamC& Strm, const StringC& LoadPrefix)
{
  modulePath = LoadPrefix;
  bool timeTagRead = false;
  bool mainTagRead = false;
  XMLTreeC itemXML = XMLTreeC::ReadNode(Strm, "item");
  notFinalValues = itemXML.AttributeBool("NotFinalValues");

  for (ConstDLIterC<XMLTreeC> i(itemXML.Children()); i; ++i) {
    if (i->Name() == "time") {
      RealT frameRate = i->AttributeReal("framerate", 25.0);
      if(frameRate == 0)
        throw ExceptionInvalidStreamC("MemItemBodyC::Load:frameRate == 0");
      Start() = TimeCodeC(i->AttributeInt("beg"), frameRate);
      End()   = TimeCodeC(i->AttributeInt("end"), frameRate);
      timeTagRead = true;
    }
    else if (i->Name() == "image") {
      IndexRange2dC frame;
      const StringC& frameAttr = i->AttributeString("frame");
      StrIStreamC(frameAttr) >> frame;
      imageInfos.Insert(i->AttributeString("name"),
                        ImageInfoC(i->AttributeString("fileName"), frame));
    }
    else if(i->Name() == "data") {
      stringAttrs = i->Attributes();
      mainTagRead = true;
    }
    else {
      cerr << "MemItemBodyC::Load: Unknown tag:" << i->Name() << endl;
      throw ExceptionInvalidStreamC("MemItemBodyC::Load: Unknown tag: " + i->Name());
    }
  }
  if(!timeTagRead || !mainTagRead)
    throw ExceptionC("MemItemBodyC::Load:Memory item was not read properly."); 
}


//------------------------------------------------------------------------
//: saves item to XML stream.
void MemItemBodyC::Save(XMLOStreamC& Strm, const StringC& SavePrefix) const
{
  if (Eof()) return;
  if (notFinalValues) return;

  //save images to files
  modulePath = FilenameC(SavePrefix);
  DumpImages<ByteRGBValueC>(imageAttrs, "png");
  DumpImages<ByteT>(byteImageAttrs, "png");
  
  Strm.StartTag("item");
  if (notFinalValues)  Strm.SetAttrib("NotFinalValues", true);
  Strm.StartContents();

  //write timecode element
  Strm.StartTag("time");
  Strm.SetAttrib("framerate", Start().FrameRate());
  Strm.SetAttrib("beg", Start().getFrameCount());
  Strm.SetAttrib("end", End().getFrameCount());
  Strm.SetAttrib("start", Start().ToText());
  Strm.EndTag();

  //write main part: attributes
  Strm.StartTag("data", stringAttrs);
  Strm.EndTag();

  //save images info in XML (from imageInfos created in DumpImages)
  for(HashIterC<StringC, ImageInfoC> infoIter(imageInfos); infoIter; infoIter++)
  {
    Strm.StartTag("image");
    Strm.SetAttrib("name", infoIter.Key());
    Strm.SetAttrib("fileName", infoIter.Data().fileName);
    Strm.SetAttrib("frame", infoIter.Data().frame);
    Strm.EndTag();    
  }
  Strm.EndTag();//end of 'item' tag
}

//-----------------------------------------------------------------------------
//: Returns the image from item
const ImageC<ByteRGBValueC> MemItemBodyC::GetImage(const StringC& AttrName) const
{
  RavlAlwaysAssertMsg(imageInfos.IsElm(AttrName), 
    "MemItemBodyC::GetImage:no image attribute \"" + AttrName +
    "\" found for image at " + duration.Start().FrameCount()); //image possble to load
  const ImageInfoC& imgInfo = imageInfos[AttrName];
  RavlAlwaysAssertMsg(!imgInfo.fileName.IsEmpty(), "MemItemBodyC::GetImage: Empty file name for image " + AttrName);
  FilenameC imagePath = modulePath + "/" + imgInfo.fileName;
  //cout << "filename:" << imagePath << " frame:" << imgInfo.frame << endl;
  // load image to [0,0]
  ImageC<ByteRGBValueC> image;
  if(imgInfo.frame.Rows() != 0 && imgInfo.frame.Cols() != 0)
    {
      if(!RavlN::Load(imagePath, image))
        throw ExceptionInvalidStreamC("MemItemBodyC::GetImage: Can't read image from file: " + imagePath);
    }

  // shift image to original origin & check frame integrity
  image.ShiftArray(imgInfo.frame.TopLeft());
  StrOStreamC msg;
  msg << "MemItemBodyC::GetImage: Image frame from xml " << imgInfo.frame
      << " differs to this: " << image.Frame() << " from file " << imagePath;
    
  RavlAlwaysAssertMsg((image.Frame() == imgInfo.frame), msg.String());
  return image;
}

//-----------------------------------------------------------------------------
//: Returns Byte image from item
const ImageC<ByteT> MemItemBodyC::GetByteImage(const StringC& AttrName) const
{
  RavlAlwaysAssertMsg(imageInfos.IsElm(AttrName), 
                      "MemItemBodyC::GetByteImage: no image attribute \"" + AttrName + "\" found"); //image possble to load
  const ImageInfoC& imgInfo = imageInfos[AttrName];
  RavlAlwaysAssertMsg(!imgInfo.fileName.IsEmpty(), "MemItemBodyC::GetByteImage: Empty file name for image " + AttrName);
  FilenameC imagePath = modulePath + "/" + imgInfo.fileName;
  //cout << "filename:" << imagePath << " frame:" << imgInfo.frame << endl;
  // load image to [0,0]
  ImageC<ByteT> image;
  if(imgInfo.frame.Rows() != 0 && imgInfo.frame.Cols() != 0)
    {
      if(!RavlN::Load(imagePath, image))
        throw ExceptionInvalidStreamC("MemItemBodyC::GetByteImage: Can't read image from file" + imagePath);
    }

  // shift image to original origin & check frame integrity
  image.ShiftArray(imgInfo.frame.TopLeft());
  StrOStreamC msg;
  msg << "MemItemBodyC::GetByteImage: Image frame from xml " << imgInfo.frame
      << " differs to this: " << image.Frame() << " from file " << imgInfo.fileName;
    
  RavlAlwaysAssertMsg((image.Frame() == imgInfo.frame), msg.String());
  return image;
}


//-----------------------------------------------------------------------------
//: Dump contents of memory item
StringC MemItemBodyC::Dump(int MaxLen) const 
{ 
  ostringstream oss;
  oss.imbue(locale("C"));
  if(Eof()) 
  {
    oss << "---END ---";
  } 
  else 
  {
    oss << "Time frame: " << Duration().ToText() <<endl;
    bool empty(true);
    for(HashIterC<StringC, ImageC<ByteRGBValueC> > i(imageAttrs); i; ++i) 
    {
      oss << "img " << i.Key() << " = \"" << i.Data().Frame() << "\" ";
      empty = false;
    }
    for(HashIterC<StringC, ImageC<ByteT> > i(byteImageAttrs); i; ++i)
    {
      oss << "img " << i.Key() << " = \"" << i.Data().Frame() << "\" ";
      empty = false;
    }
    for (HashIterC<StringC, StringC> i(stringAttrs); i; ++i)
    {
      oss << i.Key() << " = \"" << i.Data() << "\" ";
      empty = false;
    }
    if(empty)
    {
      oss << "empty";
    }
  }
  oss << endl;
  if(MaxLen >= 0 && int(oss.str().size()) >= MaxLen) {
    string dump(oss.str());
    return StringC(dump.substr(0, MaxLen-1).c_str());
  } else {
    return StringC(oss.str().c_str());
  }
}


void MemItemBodyC::Details() const 
{
  cerr << "duration: " << duration.Start() << " - " << duration.End() << endl
       << "stringAttrs: ";
  for (HashIterC<StringC, StringC> i(stringAttrs); i; ++i) cerr << i.Key() << " " << i.Data() << endl;
  cerr << "imageAttrs: ";
  for (HashIterC<StringC, ImageC<ByteRGBValueC> > i(imageAttrs); i; ++i) cerr << i->Frame() << endl;
  cerr << "byteImageAttrs: ";
  for (HashIterC<StringC, ImageC<ByteT> > i(byteImageAttrs); i; ++i) cerr << i->Frame() << endl;
  cerr << "imageInfos: ";
  for (HashIterC<StringC, ImageInfoC> i(imageInfos); i; ++i) cerr << i->fileName << "; " << i->frame << endl;
}
