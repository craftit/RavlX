// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU
// General Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/Contrib/DVDRead/exDVDRead.cc"
//! lib=RavlDVDRead
//! author = "Warren Moore"
//! docentry = "Ravl.API.Images.Video.Video IO.LibDVDRead"

#include "Ravl/Option.hh"
#include "Ravl/DVDRead.hh"
#include "Ravl/IO.hh"
#include "Ravl/OS/Date.hh"
#include "Ravl/DList.hh"

using namespace std;
using namespace RavlN;

int main(int nargs,char **argv)
{
  OptionC opts(nargs,argv);
  StringC device = opts.String("d", "/dev/dvd", "DVD device.");
  IntT title = opts.Int("t", 1, "DVD title.");
  StringC outfile = opts.String("o", "", "Output filename.");
  opts.Check();
  
  // Create the dvd 
  DVDReadC dvd(title, device);

  // Display the stream attributes
  DListC<StringC> attrs;
  if (dvd.GetAttrList(attrs))
  {
    DLIterC<StringC> it(attrs);
    while (it)
    {
      StringC value;
      if (dvd.GetAttr(*it, value))
        cerr << *it << " : " << value << endl;
      it++;
    }
  }
  
  // Create the output file
  if (outfile == "")
    outfile = StringC(title) + ".vob";
  OStreamC file(outfile);

  // Load the stream
  SArray1dC<ByteT> data(2048);
  while(!dvd.IsGetEOS())
  {
    UIntT len = dvd.GetArray(data);
    cerr << "Got " << len << endl;
    file.write(reinterpret_cast<const char*>(&(data[0])), len);
  }

  file.Close();
  
  return 0;
}
