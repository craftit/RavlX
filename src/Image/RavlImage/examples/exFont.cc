// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlImage

#include "Ravl/Image/Font.hh"
#include "Ravl/Option.hh"
#include "Ravl/IO.hh"


using namespace RavlImageN;

int main(int nargs,char **argv) {
  OptionC opt(nargs,argv); 
  StringC fontName = opt.String("f","","Font to load ");
  IntT maxWidth = opt.Int("w",600,"Maximum width of the image ");
  opt.Check();
  
  FontC font;
  if(fontName.IsEmpty())
    font = DefaultFont();
  else {
    if(!(font = LoadPSF1(fontName)).IsValid())
      font = LoadPSF2(fontName);
  }
  
  if(!font.IsValid()) {
    std::cerr << "No font loaded. \n";
    return 1;
  }
  
  IndexRange2dC fsize(1,1);
  Index2dC at(1,1);
  SizeT vsize = 1;
  SizeT hsize = 1;
  for(UIntT i = 0;i < font.Count();i++) {
    vsize = Max(vsize,font[i].Rows());
    hsize += font[i].Cols() + 1; 
  }
  vsize++;
  IntT crows = (hsize / maxWidth) + 1;
  IndexRange2dC imgSize(crows * (vsize+1) + 1,maxWidth);
  
  ImageC<ByteT> img(imgSize);
  img.Fill(0);
  ByteT val = 255;
  hsize = 1;
  IntT hpos = 1;
  for(UIntT i = 0;i < font.Count();i++) {
    IntT row = (hsize / maxWidth);
    if((hpos + (IntT)(font[i].Cols() + 1))  > maxWidth)
      hpos = 1;
    Index2dC at(row * (vsize+1),hpos);
    StringC let(" ");
    let[0] = i;
    DrawText(font,val,at,let,img);
    hpos += font[i].Cols() + 1;
    hsize += font[i].Cols() + 1;
  }  
  
  RavlN::Save("@X",img);
  
  return 0;
}
