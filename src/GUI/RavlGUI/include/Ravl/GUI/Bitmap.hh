// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUIBITMAP_HEADER
#define RAVLGUIBITMAP_HEADER 1
////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/GUI/GTK/Bitmap.hh"
//! lib=RavlGUI
//! docentry="Ravl.API.Graphics.GTK.Layout"
//! author="Charles Galambos"
//! date="17/08/2000"

#include "Ravl/GUI/Widget.hh"

namespace RavlGUIN {
  //! userlevel=Develop
  //: Bitmap body.
  
  class BitmapBodyC 
    : public RCBodyC
  {
  public:
    BitmapBodyC(int width,int height);
    //: Create a bit map of the given size.
    
    ~BitmapBodyC();
    //: Destructor.
  
    GdkBitmap &Bitmap()
      { return *bmp; }
    //: access bitmap.
    
    void Clear();
    //: Clear the bitmap.
    
  protected:
    int width,height;
    GdkBitmap *bmp;
  };
  
  //! userlevel=Normal
  //: Bitmap.
  // Hold a binary image. often used as a mask.
  
  class BitmapC 
    : public RCHandleC<BitmapBodyC>
  {
  public:
    BitmapC()
      {}
    //: Default constructor.
    // creates an invalid handle.
    
    BitmapC(int width,int height) 
      : RCHandleC<BitmapBodyC>(*new BitmapBodyC(width,height))
      {}
    //: Create a bit map of the given size.
    
    GdkBitmap &Bitmap()
      { return Body().Bitmap(); }
    //: access bitmap.
    
    void Clear()
      { Body().Clear(); }
    //: Clear the bitmap.
    
  };
}

#endif
