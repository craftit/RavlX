// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, James Smith
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlURLIO
//! file="Ravl/Contrib/HTTP/exURL.cc"
//! author="James Smith"
//! docentry="Ravl.API.Core.IO.URL Handling"
//! userlevel=Normal

#include "Ravl/IO/URLStream.hh"
#include "Ravl/OS/Date.hh"
#include "Ravl/DP/FileFormatIO.hh"
#include "Ravl/Option.hh"
#include "Ravl/EntryPnt.hh"

using namespace RavlN;

int exURL(int nargs,char *args[]) {
   
   // Get command-line options
   OptionC opt(nargs,args);
   StringC url = opt.String("i","http://ravl.sourceforge.net","URL to load");
   StringC out = opt.String("o","-","Where to write data. ");
   StringC username = opt.String("u","","The username to use for the connection");
   StringC password = opt.String("p","","The password to use for the connection");
   opt.Check();

   // Construct URL with user/pass if required
   if (!username.IsEmpty() && !password.IsEmpty()) {
     url = URLIStreamC::AddUserPass(url,username,password);
   }
   
   OStreamC os(out);
   URLIStreamC strm(url);
   
   if (strm.Error()) cerr << strm.ErrorString() << endl;

   strm.CopyTo(os);
   
   return 0;

}

RAVL_ENTRY_POINT(exURL);
