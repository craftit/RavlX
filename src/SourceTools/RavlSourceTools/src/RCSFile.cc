// This file is part of CxxDoc, The RAVL C++ Documentation tool 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU General 
// Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
/////////////////////////////////////////////////////////////////
// RCSFile.cc
//! author="Charles Galambos"
//! rcsid="$Id$"
//! lib=RavlSourceTools
//! file="Ravl/SourceTools/CodeManager/RCSFile.cc"

#include "Ravl/SourceTools/RCSFile.hh"
#include "Ravl/OS/UserInfo.hh"
#include "Ravl/StringList.hh"
#include "Ravl/Hash.hh"

#include <stdlib.h>
#include <ctype.h>

#if RAVL_HAVE_ANSICPPHEADERS
#include <fstream>
#else
#include <fstream.h>
#endif

#include <sys/types.h>

#if RAVL_HAVE_UNISTD_H
#include <unistd.h>
#endif

#define DODEBUG 0

#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  
  //: Construct from a string.
  
  RCSVersionC::RCSVersionC(const StringC &strin) {
    StringC frm(strin);
    IntT at = 0,numstart = 0;
    UIntT n = 0;
    IntT len = frm.freq('.') + 1;
    ver = SArray1dC<UIntT>(len); // Allocate array.    
    for(at = 0;at < ((IntT) frm.length());at++) {
      if(isdigit(frm[at])) 
	continue;
      if(frm[at] == '.') {
	if(numstart == at)
	  ver[n] = 0; // Report an error here ??
	else
	  ver[n++] = atoi(StringC(frm.at(numstart,at-numstart)));
	numstart = at+1;
	continue;
      }
      std::cerr << "RCSVersionC(StringC), Unexpected character '" << frm[at] << "' in '" << frm <<"'\n";
      // FIXME :- throw an error ?
      break;
    }
    ver[n] = atoi(StringC(frm.at(numstart,at-numstart).chars())); // Get the last digit.
  }
  
  //: Convert to a string.
  
  RCSVersionC::operator StringC () const {
    StringC ret("",(ver.Size() * 4)+1);
    if(ver.Size() > 0) {
      UIntT i;
      for(i = 0;i < (ver.Size()-1);i++)
	ret += StringC(ver[i]) + '.';
      ret += StringC(ver[i]);
    } else
      ret = "0"; // No version.
    return ret;    
  }
  
  //: Is the same as another ?
  
  bool RCSVersionC::operator==(const RCSVersionC &oth) const {  
    UIntT i;
    if(ver.Size() != oth.ver.Size())
      return false;
    for(i = 0;i < ver.Size();i++)
      if(ver[i] != oth.ver[i])
	return false;
    return true;
  }
  

  //: Is the same as another ?
  
  bool RCSVersionC::operator>(const RCSVersionC &oth) const  {
    for(unsigned int i = 0;i < ver.Size() && i < oth.ver.Size();i++) {
      if(ver[i] != oth.ver[i])
	return (ver[i] > oth.ver[i]);
    }
    return ver.Size() > oth.ver.Size();
  }
  
  //: Is the same as another ?
  
  bool RCSVersionC::operator<(const RCSVersionC &oth) const  {
    for(unsigned int i = 0;i < ver.Size() && i < oth.ver.Size();i++) {
      if(ver[i] != oth.ver[i])
	return (ver[i] < oth.ver[i]);
    }
    return ver.Size() < oth.ver.Size();  
  }
  
  
  ///////////////////////////
  //: Is file checked out ?
  
  bool RCSFileBodyC::IsCheckedOut() { 
    if(!Exists())
      return false;
    if(!IsWritable())
      return false;
    return (LockedBy(HeadVersion()) == UserInfoC::WhoAmI());
  }
  
  ////////////////////////////////////
  //: Check out file.
  // Return true=Operation succeded.
  
  bool RCSFileBodyC::CheckOut()  {
    if(IsCheckedOut())
      return true;
    if(!UsesRCS())
      return true;
    int ret = system("co -q -l " + (*this));
    //cerr << "CheckOut:" << ret << "\n";
    if(ret != 0)
      return false;
    rcsParsed = false;
    access.Empty();
    locks.Empty();
    return IsCheckedOut();
  }
  
  /////////////////////////////////
  //: Checkout a version of this file to somewhere.
  
  StringC RCSFileBodyC::CheckOutUL(StringC version,FilenameC dest)  {
    StringC coname(dest + '/' + NameComponent());
    if(version == "") { // If no version specified.
      if(system("nouser_co -q " + DBFilename() + ' ' + coname) != 0)
	return StringC();
    } else {
      if(system("nouser_co -q -r" + version + ' ' + DBFilename() + ' ' + coname) != 0)
	return StringC();
    }
    return coname;
  }
  
  /////////////////////////////////
  //: Check in file.
  // Return true=Operation succeded.
  
  bool RCSFileBodyC::CheckIn(StringC cimsg) {
    int ret = system("ci -q -u -t\"-Auto checkin.\" -m'" + cimsg + "' " + (*this));
    //cerr << "CheckIn:" << ret << "\n";
    if(ret != 0)
      return false;
    rcsParsed = false;
    access.Empty();
    locks.Empty();
    return !IsCheckedOut();
  }
  

  //: Check the time stamps on the file to see if
  //: update is needed.
  
  bool RCSFileBodyC::NeedsUpdate()  {
    if(!Exists())
      return true;
    if(!UsesRCS())
      return false; // Can't be updated, no RCS file.
    return (dbFilename.LastModTime() > LastModTime());
  }

  /////////////////////////////////
  //: Make sure unlocked file is up to date.
  
  bool RCSFileBodyC::Update()  {
    if(IsCheckedOut())
      return true; // Is locked so must be uptodate.
    if(!NeedsUpdate())
      return true;
    if(!UsesRCS())
      return true; // Has to be uptodate.
    if(!PathComponent().IsWritable())
      return false;
    int ret = system("co -q " + (*this));
    //cerr << "CheckIn:" << ret << "\n";
    if(ret != 0)
      return false;
    return Exists();
  }
  
  /////////////////////////////////
  //: Add user to access list.
  
  bool RCSFileBodyC::AddUser(const StringC &login)  {
    if(!UserInfoC(login).Exists())
      return false;
    
    if(system("rcs -q -a" + login) != 0) 
      return false;
    access.InsLast(UserInfoC(login));
    return true;
  }
  
  
  /////////////////////////////////
  //: Remove user from access list.
  
  bool RCSFileBodyC::RemoveUser(const StringC &login)  {
    if(system("rcs -q -e" + login) != 0)
      return false;
    // FIXME :- A very lazy way of doing this.
    access.Empty();
    locks.Empty();
    rcsParsed = false;
    return true;
  }
  
  /////////////////////////////////
  // Get the RCS file name itself.
  
  FilenameC RCSFileBodyC::DBFilename() {
    FilenameC ret;
    FilenameC path = (*this).PathComponent();
    if(path.IsEmpty())
      path = ".";
    FilenameC fname = (*this).NameComponent();
    ret=  FilenameC(path + "/RCS/" + fname + ",v");
    if(ret.Exists())
      return ret;
    ret = FilenameC((*this) + ",v");
    if(ret.Exists())
      return ret;
    return StringC();  
  }
  
  /////////////////////////////////
  //: Test if file has an RCS database.
  
  bool RCSFileBodyC::UsesRCS() {
    if(!rcsParsed)
      ParseRCS();
    return usesRCS;
  }
  
  /////////////////////////////////
  //: Is this working file different from some other version of a particular file.
  
  bool RCSFileBodyC::IsDifferent(FilenameC other,StringC vers) {
    RCSFileC otherRCS(other);
    if(!otherRCS.UsesRCS())
      return true;
    FilenameC tmpFile("/tmp/" + other.NameComponent());
    tmpFile = tmpFile.MkTemp();
    StringC command = "co -q -kk -p";
    if(!vers.IsEmpty())
      command += vers;
    command += " " + other + " >" + tmpFile;
    if(system(command) != 0) {
      std::cerr << "RCSFileBodyC::IsDifferent(), Failed to check out version " << vers << " of " << other << std::endl;
      // FIXME :- Throw exception ?
      if(tmpFile.Exists())
	tmpFile.Remove(); // Tidy up.
      return true; // Assume different.
    }
    bool ret = VerDiff(tmpFile);
    ONDEBUG(std::cout << "RCSFileBodyC::IsDifferent() " << ret << " " << (*this) << " " << tmpFile << endl);
    tmpFile.Remove(); // Tidy up.
    return ret;
  }
  
  
  /////////////////////////////////
  //: Test if current version of file is different from head.
  
  bool RCSFileBodyC::IsDiffFromHead() {
    IntT ret = system("rcsdiff -q " + (*this) + " >/dev/null");
    //cerr << "RCSFileBodyC::IsDiffFromHead() Called on " << (*this) << " = " << ret << std::endl;
    if(ret == 2) {
      std::cerr << "RCSFileBodyC::IsDiffFromHead(), Failed. " << ((const char *)(*this)) << "\n";
      return true; // Don't know, assume the worst.
    }
    return (ret != 0);
  }
  
  
  /////////////////////////////////
  //: Extract info from RCS file.
  
  bool RCSFileBodyC::ParseRCS() {
    if(rcsParsed)
      return usesRCS;
    dbFilename = DBFilename();
    FilenameC rcsFile(dbFilename);
    usesRCS = false;
    rcsParsed = true;
    if(rcsFile.IsEmpty())
      return false; // Failed to find rcs file.
    ifstream inf(rcsFile.chars());
    if(!inf) {
      std::cerr << "RCSFileBodyC::ParseRCS(), Failed to open " << rcsFile << std::endl;
      return false;
    }
    usesRCS = true;
    int state = 0; // Start in admin.
    while(!inf.eof()) {
      StringC entry = GetEntry(inf);
      StringC id = entry.before(' ');
      if(id == "")
	id = entry;
      int nextoff = id.length();
      StringC info;
      if(nextoff < (int)entry.length())
	info = entry.after(nextoff);
      switch(state)
	{ 
	case 0: // Admin block.
	  if(isdigit(id.firstchar())) // Is it a delta.
	    state = 1; // Delta.
	  if(id == "desc") {
	    state = 2; // Desc.
	    break;
	  }
	  if(id == "head") {
	    //cerr << "Head:'" << info << "'\n";
	    head = RCSVersionC(info);
	    break;
	  }
	  if(id == "branch")
	    break;
	  if(id == "access") {
	    //cerr << "Access:'" << info << "'\n";
	    StringListC strlst(info," \n\t\0");
	    for(DLIterC<StringC> it(strlst);it.IsElm();it.Next()) {
	      //cerr << "Found user. " << it.Data() << "\n";
	      access.InsLast(UserInfoC(it.Data()));
	    }
	    break;
	  }
	  if(id == "symbols")
	    break;
	  if(id == "locks") {
	    //cerr << "Locks:'" << info << "'\n";
	    StringListC strlst(info);
	    for(DLIterC<StringC> it(strlst);it.IsElm();it.Next()) {
	      int col = it.Data().index(':');
	      if(col >= 0)
		locks.InsLast(Tuple2C<StringC,RCSVersionC>(it.Data().before(col),RCSVersionC(it.Data().after(col))));
	    }
	    break;
	  }
	  if(state == 0)
	    break; // Change of state ?
	  /* no break */
	case 1:
	  // Parse delta info...
	  // Don't need this info so just return.
	  return true;
	case 2:
	  return true;
	}
    }
    return true;
  }
  
  
  //: Who originaly checkin in the file and the date it happened.
  
  bool RCSFileBodyC::ExtractInital(StringC &author,StringC &date)  {
    FilenameC rcsFile;
    if(rcsParsed) {
      if(usesRCS)
	rcsFile = dbFilename;
    } else
      rcsFile = DBFilename();
    if(rcsFile.IsEmpty()) {
      std::cerr << "RCSFileBodyC::ExtractInital(), No RCS file for '" << ((const char *)(*this)) << "' found. \n";
      return false; // Failed to find rcs file.
    }
    ifstream inf(rcsFile.chars());
    if(!inf) {
      std::cerr << "RCSFileBodyC::ExtractInital(), Failed to open " << rcsFile << std::endl;
      return false;
    }
    int state = 0; // Start in admin.
    StringC cver; // Current version.
    while(!inf.eof()) {
      StringC entry = GetEntry(inf);
      StringC id = entry.before(' ');
      if(id == "")
	id = entry;
      int nextoff = id.length();
      StringC info;
      if(nextoff < (int)entry.length())
	info = entry.after(nextoff);
      switch(state)
	{ 
	case 0: // Admin block.
	  if(isdigit(id.firstchar())) // Is it a delta.
	    state = 1; // Delta.
	  if(id == "desc") {
	    state = 2;	
	    break;
	  }
	if(state == 0)
	  break; // Change of state ?
	/* no break */
	case 1:
	  // Parse delta info...
	  if(isdigit(id.firstchar())) {
	    cver = id;
	    //cerr << "version:'" << id << "'\n";
	    break;
	  }
	  if(id == "date") {
	    if(cver == "1.1") {
	      //cerr << "Raw date: '" << info << "'\n";
	      date = DateRCS2DMY(info);
	    }
	  //cerr << "date:'" << DateRCS2DMY(info.after(' ')) << "'\n";
	    break;
	  }
	  if(id == "author") {
	    if(cver == "1.1") {
	      UserInfoC ui(info);
	      if(ui.Exists())
		author = ui.FullName();
	      else author = info; // We can't do much else.
	    }
	    //cerr << "Author. Ver:" << cver << " '" << UserInfoC(info).FullName() << "'\n";
	    break;
	  }
	  if(id == "desc") {
	    state = 2;	
	    break;
	  }
	  if(state == 1)
	    break; // Change of state ?
	  /* no break */
	case 2:
	  // Parse delta info...
	  // Don't need this info so just return.
	  return true;
      }
    }
    return true;  
  }
  
  ////////////////////////////
  //: List all log entries after version afterVer.
  // Tuple4 Contains 
  //   1 - Date of mod
  //   2 - Author.
  //   3 - Version no.
  //   4 - Comment.
  
  DListC<Tuple4C<DateC,StringC,RCSVersionC,StringC> > RCSFileBodyC::ListLogEntries() {
    DListC<Tuple4C<DateC,StringC,RCSVersionC,StringC> > ret;
    HashC<StringC,Tuple4C<DateC,StringC,RCSVersionC,StringC> > verInfo;
    FilenameC rcsFile;
    if(rcsParsed) {
      if(usesRCS)
	rcsFile = dbFilename;
    } else
      rcsFile = DBFilename();
    if(rcsFile.IsEmpty()) {
      std::cerr << "RCSFileBodyC::ExtractInital(), No RCS file for '" << ((const char *) (*this)) << "' found. \n";
      return ret; // Failed to find rcs file.
    }
    ifstream inf(rcsFile.chars());
    if(!inf) {
      std::cerr << "RCSFileBodyC::ExtractInital(), Failed to open " << rcsFile << std::endl;
      return ret;
    }
    int state = 0; // Start in admin.
    StringC cver; // Current version.
    while(!inf.eof()) {
      StringC entry = GetEntry(inf);
      StringC id = entry.before(' ');
      if(id == "")
	id = entry;
      int nextoff = id.length();
      StringC info;
      if(nextoff < (int)entry.length())
	info = entry.after(nextoff);
      switch(state)
	{ 
	case 0: // Admin block.
	  if(isdigit(id.firstchar())) // Is it a delta.
	    state = 1; // Delta.
	  if(id == "desc")
	    state = 2;	
	  if(state == 0)
	    break; // Change of state ?
	  /* no break */
	case 1:
	  // Parse delta info...
	  if(isdigit(id.firstchar())) {
	    cver = id;
	    //cerr << "version:'" << id << "'\n";
	    break;
	  }
	  if(id == "author") {
	    verInfo[cver].Data2() = info;
	    break;
	  }
	  if(id == "date") {
	    verInfo[cver].Data1() = DateRCS2DateC(info);
	    break;
	  }
	  if(id == "desc") {
	    
	    state = 2;
	  }
	  if(state == 1)
	    break; // Change of state ?
	  /* no break */
	case 2:
	  if(isdigit(id.firstchar())) {
	    cver = id;
	    //cerr << "version:'" << id << "'\n";
	    break;
	  }
	  if(id == "log") {
	    Tuple4C<DateC,StringC,RCSVersionC,StringC> &it = verInfo[cver];
	    it.Data3() = RCSVersionC(cver);
	    it.Data4() = Unstuff(info);
	    ret.InsLast(it);
	    break;
	  }
	  break;
	}
    }
    return ret;
  }
  
  // Possess a file
  
  bool RCSFileBodyC::Possess(bool verbose) {
    UserInfoC cu = UserInfoC::WhoAmI();
    FilenameC fn = DBFilename();
    if(fn.IsEmpty()) {
      std::cerr << "WARNING: '" << chars() << "' has no RCS file. \n";
      return true;
    }
    UserInfoC orgowner = fn.OwnerInfo();
#if 0
    if(verbose)
      std::cerr << "Checking ownership of '" << fn << "'.  Target:'" << cu.Login() << "'  Cur:'" << orgowner.Login() << "'\n";
#endif
    if(cu == orgowner)
      return true; // Already right.
    if(verbose) 
      std::cerr << "Attempting to possess file '" << fn << "'\n";
    // First try an rename original file.
    FilenameC org(fn + ".orig");
    if(org.Exists()) // Make sure its unique.
      org = org.MkTemp();
    if(!fn.Rename(org)) {
      std::cerr << "ERROR: Failed to possess file '" << fn << "' rename failed. \n";
      return false;
    }
    if(!org.Exists()) { // Double check.
      std::cerr << "ERROR: Failed to possess file '" << fn << "' rename failed without error. \n";
      return false;
    }
    if(fn.Exists()) { // Triple check.
      std::cerr << "ERROR: Failed to possess file '" << fn << "' orginal file still exists. \n";
      return false;
    }
    // Copy file to newone with correct ownership.
    if(!org.CopyTo(fn)) {
      std::cerr << "ERROR: Posses failed. Can't copy. '" << org << "' to '" << fn << "'.  Tiding up. \n";
      // Tidy up.
      if(!org.Rename(fn)) 
	cerr << "ERROR: Renamed of '" << org << "' to '" << fn << "' failed. \n";
      return false;
    }
    // Double check all looks ok.
    if(org.FileSize() != fn.FileSize()) {
      std::cerr << "ERROR: Filesize mismatch.   Restoring orginal file '" << org << "' in directory. \n";
      if(!fn.Remove()) {
	cerr<< "ERROR: Failed to remove new file. '" << fn << "' \n";
	return false;
      }
      if(!org.Rename(fn))
	cerr<< "ERROR: Failed to restore original name to. '" << org << "' \n";
      return false;
    }
    if(!org.Remove())
      std::cerr << "WARNING: Failed to remove orginal file '" << org << "'.\n";
    else {
      if(verbose)
	cerr << "File '" << fn << "' successfully possessed. \n";
    }
    // Check RCS mode.
    if(system(StringC("rcs -q -L -a") + orgowner.Login() + "," + cu.Login() + " " + fn) != 0) {
      std::cerr << "WARNING: rcs command failed for file '" << fn << "'\n";
    }
    return true;
  }
  
  
  ////////////////////////////////
  //: Extract next entry from RCS file.
  // reads upto the next ';' character.
  
  StringC RCSFileBodyC::GetEntry(std::istream &in) {
    StringC ret;
    bool lastwasspace = true;
    UIntT maxStringLen = 32000;
    bool gotid = false;
    bool endwithstr = false;
    while(!in.eof()) {
      int letter = in.get();
      switch(letter)
	{
	case '\n': // Newline.
	case '\t': // Tab.
	case ' ':  // Space.
	case '\r': // Form feed.
	  if(lastwasspace)
	    break; // Skip repeated spaces.
	  lastwasspace = true;
	  if(ret.length() > 0) {
	    if(!gotid) {
	      StringC secttype = ret;
	      if(isdigit(secttype.firstchar()))
		return secttype;// ID Number.
	      if(secttype == "desc")
		endwithstr = true;
	      if(secttype == "log")
		endwithstr = true;
	      if(secttype == "text")
		endwithstr = true;
	      gotid = true;
	    }
	    ret += ' ';
	  }
	  break;
	case ';':
	  return ret;
	case '@': // found string.
	  // This doesn't unstuff '@'s
	  lastwasspace = false;
	  ret += '@';
	  while(!in.eof()) {
	    int alet = in.get();
	    if(alet == '@') {
	      ret += '@';
	      if(in.peek() != '@') 
		break;
	      ret += '@';
	      in.get(); // Skip next '@'.
	      continue;
	    }
	    ret += alet;
	    if(ret.length() > maxStringLen) {
	      std::cerr << "ERROR: RCS String too long in '" << chars() << "'\n"; 
	      std::cerr << ret;
	      return StringC();
	    }
	  }
	  if(endwithstr)
	    return ret;
	  break;
	default:
	  lastwasspace = false;
	  ret += (char) letter;
	  break;
	}
    }
    return ret;
  }
  
  // Unstuff first string after 'startat'
  
  StringC RCSFileBodyC::Unstuff(const StringC &str,int startat) {
    StringC ret;
    unsigned int at = (unsigned int) startat;
    for(;str[at] != '@';at++) {
      if(at >= str.length())
	return ret; // Empty.
    }
    at++;
    for(;at < str.length();at++) {
      if(str[at] == '@') {
	at++;
	if(at >= str.length())
	  return ret; // Finished.
	if(str[at] != '@')
	  return ret; // Finished.
      }
      ret += str[at];
    }
    // Finished incorrectly!
    std::cerr << "RCSFileBodyC::Unstuff(), WARNING: Incomplete string found.\n";
    return ret;
  }
  
  
  //: Test if a user has access.
  
  bool RCSFileBodyC::HasAccess(const UserInfoC &auser) {
    if(!UsesRCS()) { // No RCS file then depends on file permissions.
      //cerr << "RCSFileBodyC::HasAccess(), No RCS File found. \n";
      if(auser == (*this).OwnerInfo())
	return (*this).Permissions().IsOwnerWritable(); 
      return (*this).Permissions().IsWorldWritable();
    }
    // Must use RCS if we got here.
    if(auser == dbFilename.OwnerInfo())
      return true; // If we own the RCS file we have access.
    for(DLIterC<UserInfoC> it(access);it.IsElm();it.Next()) {
      //cerr << "Who:" << it.Data().Login() << std::endl;
      if(it.Data() == auser)
	return true;
    }
    //cerr << "RCSFileBodyC::HasAccess(), Not on access list. \n";
    return false; // Nope not on access list.
  }
  
  /////////////////////////////////////////
  //: Test if file is available to modify. 
  // User must have access to file and it can't be
  // locked by anyone else.
  
  bool RCSFileBodyC::IsAvailable() {
    UserInfoC uId = UserInfoC::WhoAmI();
    if(!HasAccess(uId))
      return false;
    UserInfoC lockBy = LockedBy(head);
    if(lockBy.IsValid())
      return (lockBy == uId);
    //  std::cerr << "File is available. \n";
    return true; // File not locked.
  }
  
  ////////////////////////////////////
  //: Get head version.
  
  RCSVersionC RCSFileBodyC::HeadVersion()  {
    if(!rcsParsed)
      ParseRCS();
    if(!usesRCS)
      return RCSVersionC("1.1");
    return head;
  }


  ////////////////////////////////////////
  //: Has file changed from version 'fromVer' ?
  
  bool RCSFileBodyC::HasChangedFrom(RCSVersionC fromVer)  {
    if(fromVer != HeadVersion()) {
      //cerr << "RCSFileBodyC::HasChangedFrom() Head Vers differ: " << fromVer << " " << HeadVersion() << std::endl;
      return true;
    }
#if 0
    if(!IsCheckedOut()) {
      //cerr << "RCSFileBodyC::HasChangedFrom(), '" << (*this) << "' : Not checked out. \n";
      return false;
    }
#endif
    return IsDifferent((*this),HeadVersion());
    // return IsDiffFromHead();
  }
  
  
  ////////////////////////////////////////////
  //: Has file changed from version 'fromVer' ?
  
  bool 
  RCSFileBodyC::HasChangedFrom(const RCSFileC &oth,RCSVersionC fromVer) 
  { return IsDifferent(oth.Filename(),fromVer); }
  
  
  
  //////////////////////////////////////
  //: Do a text file diff.
  // ignoring difference between RCS id info.
  
  bool 
  RCSFileBodyC::VerDiff(const FilenameC &othFn)  {
    const char *rcsInfoTxt[7] = {"$Id","$Revision","$Date","$RCSfile","$Author","$Header",0};
    //cerr << "VerDiff() " << (*this) << " " << othFn << std::endl;
    ifstream in1(this->chars());
    ifstream in2(othFn.chars());
    if(!in1) {
      std::cerr << "RCSFileBodyC::VerDiff(), Failed to open file :" << chars() << std::endl;
      return true;
    }
    if(!in2) {
      std::cerr << "RCSFileBodyC::VerDiff(), Failed to open file :" << othFn << std::endl;
      return true;
    }
    StringC buf1,buf2;  
    while(in1 && in2) {
      readline(in1,buf1);
      readline(in2,buf2);
      while(buf1 != buf2) {
	StringC matchTxt;
	int off,i;
	// FIXME :- Could be alot faster...
	int minoff = buf1.length() + 10;
	int mini = -1;
	if(buf1.index('$') < 0)
	  return true; // Can't be match, all RCS strings start with $
	for(i = 0;rcsInfoTxt[i] != 0;i++) {
	  off = buf1.index(rcsInfoTxt[i]); // Look for RCS id.
	  if(off < 0)
	    continue;
	  if(off < minoff) {
	    minoff = off;
	    mini = i;
	  }
	}
	if(mini < 0) {
	  ONDEBUG(std::cerr << "No subst :" << buf1 << endl);
	  return true; // Yep there different.
	}
	off = minoff;
	matchTxt = rcsInfoTxt[mini];
	if(!buf2.matches(matchTxt,off,false)) {
#if RCSDEBUG
	  std::cerr << "                Txt:" << matchTxt << std::endl;
	  std::cerr << "               buf1:" << buf1 << std::endl;
	  std::cerr << "No match in second :" << buf2 << std::endl;
#endif
	  return true; // No 'matchTxt' found in buf2.
	}
	//cerr << "F1: " << buf1 << std::endl;
	//cerr << "F2: " << buf2 << std::endl;
	// Work out where the text we want to ignore ends.
	int r1,r2,rest = off + matchTxt.Size();
	char tmp = buf1[rest];
	if(tmp == ':')
	  r1 = buf1.index('$',rest+1);
	else {
	  if(tmp != '$') {
	    std::cerr << "RCSFileBodyC::VerDiff(), Unexpected character in line :" << buf1 << std::endl;
	    return true;
	  }
	  r1 = rest;
	}
	tmp =buf2[rest];
	if(tmp == ':')
	  r2 = buf2.index('$',rest+1);
	else {
	  if(tmp != '$') {
	    std::cerr << "RCSFileBodyC::VerDiff(), Unexpected character in line :" << buf2 << std::endl;
	    return true;
	  }
	  r2 = rest;
	}
	// Remove RCS information and try again.
	buf1 = buf1.before(off) + buf1.after(r1);
	buf2 = buf2.before(off) + buf2.after(r2);
#if RCSDEBUG
	cerr << "Removed: " << matchTxt << std::endl;
	cerr << "   Buf1:" << buf1 << std::endl; 
	cerr << "   Buf2:" << buf2 << std::endl;
#endif
      }
      
      // All seems ok.
    }
    //cerr << "final:" << ((bool) in1) << " " << ((bool) in2) << std::endl;
    return in1 || in2; // Both streams should be finished
  }
  
  ///////////////////////////////////
  //: Find out who has a revision locked.
  
  UserInfoC RCSFileBodyC::LockedBy(const RCSVersionC &aver)  {
    if(!UsesRCS())
      return UserInfoC(); // We don't use rcs, so we can't lock.
    for(DLIterC<Tuple2C<StringC,RCSVersionC> > it(locks);it.IsElm();it.Next()) {
      if(it.Data().Data2() == aver)
	return it.Data().Data1();
    }
    return UserInfoC();
  }
  
  ////////////////////////////////////////
  //: Convert date from rcs form.
  
  DateC RCSFileBodyC::ReadDate(StringC str) {
    //StringC num = str.before('.');
    //int year = atoi(num);
    return DateC();
  }
  
  ////////////////////////////////////
  //: Convert a rcs date to day month year format.
  
  StringC RCSFileBodyC::DateRCS2DMY(StringC str) {
    //cerr << "Asked to convert date '" << str << "' \n";
    StringC year = str.before('.');
    StringC rest = str.after('.');  
    StringC month = rest.before('.');
    rest = rest.after('.');
    StringC day = rest.before('.');
    return day+'/'+month+'/'+year;
  }
  
  
  ////////////////////////////////////
  //: Convert a rcs date to DateC format.
  
  
  DateC RCSFileBodyC::DateRCS2DateC(StringC str) {
    // Cheat, well the formats are the same !
    RCSVersionC it(str);
    SArray1dC<UIntT> &arr = it.Array();
    if(arr.Size() != 6) {
      std::cerr << "RCSFileBodyC::DateRCS2DateC(), Illegal date. \n";
      return DateC();
    }
    return DateC(arr[0] + 1900,arr[1],arr[2],arr[3],arr[4],arr[5]);
  }
  
  ////////////////////////////////////////////////////////////
  
  RCSFileC::RCSFileC(const FilenameC &aname)
    : RCHandleC<RCSFileBodyC>(*new RCSFileBodyC(aname))
  {}
  
  RCSFileC::RCSFileC(const StringC &aname) 
    : RCHandleC<RCSFileBodyC>(*new RCSFileBodyC(aname))
  {}
}
