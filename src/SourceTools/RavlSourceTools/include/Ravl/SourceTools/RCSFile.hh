// This file is part of CxxDoc, The RAVL C++ Documentation tool 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU General 
// Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
#ifndef RAVLRCSFILE_HEADER
#define RAVLRCSFILE_HEADER 1
////////////////////////////////////////////////////////
//! file="Ravl/SourceTools/CodeManager/RCSFile.hh"
//! lib=RavlSourceTools
//! userlevel=Normal
//! author="Charles Galambos"
//! date="6/4/1998"
//! docentry="Ravl.API.Source Tools.Code Manager"
//! rcsid="$Id$"

#include "Ravl/SArray1d.hh"
#include "Ravl/Tuple2.hh"
#include "Ravl/Tuple4.hh"
#include "Ravl/RefCounter.hh"
#include "Ravl/DList.hh"
#include "Ravl/OS/Date.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/OS/UserInfo.hh"

namespace RavlN {
  class DateC;
  class RCSFileC;
  
  ///////////////////////////////
  //: Class representing an RCS version number.
  
  class RCSVersionC {
  public:
    RCSVersionC() {}
    //: Default constructor.
    
    RCSVersionC(const RCSVersionC &oth)
      : ver(oth.ver)
    {}
    //: Copy constructor.
    
    RCSVersionC(const StringC &frm);
    //: Construct from a string.
    
    operator StringC () const;
    //: Convert to a string.
    
    bool operator==(const RCSVersionC &oth) const;
    //: Is the same as another ?
    
    bool operator!=(const RCSVersionC &oth) const
    { return !operator==(oth); }
    //: Is the same as another ?
    
    bool operator>(const RCSVersionC &oth) const;
    //: Is the same as another ?
    
    bool operator<(const RCSVersionC &oth) const;
    //: Is the same as another ?
    
    bool IsValid() { return !(((ver.Size() == 1) && (ver[0] == 0)) || (ver.Size() == 0)) ; }
    //: Is version valid ?
    
    unsigned int Hash() const { 
      unsigned int no = ver.Size();
      for(unsigned int i = 0;i < ((unsigned int)ver.Size());i++)
	no = no + (ver[i] << (i+1));
      return no;
    }
    //: Hash version no.
    
    SArray1dC<UIntT> &Array() { return ver; }
    //: Access array.
  protected:
    SArray1dC<UIntT> ver; // Version.
  };
  
  
  /////////////////////////////
  //! userlevel=Develop
  //: Class for dealing with RCS files.
  
  class RCSFileBodyC 
    : public RCBodyC, 
      public FilenameC 
  {
  public:
    RCSFileBodyC(const FilenameC &aname) 
      : FilenameC(aname),
      rcsParsed(false)
      {}
    //: Construct from a filename.
    
    RCSFileBodyC(const StringC &aname) 
      : FilenameC(aname),
      rcsParsed(false)
      {}
    //: Construct from a string.
    
    bool IsCheckedOut();
    //: Is file checked out ?
    
    bool CheckOut(); 
    //: Check out file.
    // Return true=Operation succeded.
    
    bool NeedsUpdate();
    //: Check the time stamps on the file to see if
    //: update is needed.
    
    bool CheckIn(StringC cimsg = "RCSFileC Checkin"); 
    //: Check in file.
    // Return true=Operation succeded.
    
    StringC CheckOutUL(StringC version = "",FilenameC dest = ".");
    //: Checkout a unlocked version of this file to somewhere.
    // dest is a directory in which to put the file. <p>
    // Returns name of checkedout file, or empty string if failed.
    
    bool Update();
    //: Make sure unlocked file is up to date.
    
    bool AddUser(const StringC &login);
    //: Add user to access list.
    
    bool RemoveUser(const StringC &login);
    //: Remove user from access list.
    
    bool UsesRCS();
    //: Test if file has an RCS database.
    
    bool IsDifferent(FilenameC other,StringC vers = "");
    //: Is this file different from some other version of a particular file.
    
    bool IsDiffFromHead();
    //: Test if current version of file is different from head.
    
    bool ParseRCS();
    //: Extract info from RCS file.
    
    bool ExtractInital(StringC &author,StringC &date);
    //: Who originaly checkin in the file and the date it happened.
    
    bool HasAccess(const UserInfoC &auser);
    //: Test if a user has access.

    bool IsAvailable();
    //: Test if file is available to modify. 
    // User must have access to file and it can't be
    // locked by anyone else.
    
    RCSVersionC HeadVersion();
    //: Get head version.
    
    bool HasChangedFrom(RCSVersionC fromVer);
    //: Has file changed from version 'fromVer' ?
    
    bool HasChangedFrom(const RCSFileC &oth,RCSVersionC fromVer);
    //: Has file changed from version 'fromVer' of oth ?
    // Similar to IsDifferent()
    
    bool VerDiff(const FilenameC &othFn);
    //: Do a text file diff.
    // ignoring difference between RCS id info.
    // this only ignores content of xxx in lines containing $id:xxx$
    // (with a capital I in id. )
    
    UserInfoC LockedBy(const RCSVersionC &aver) ;
    //: Which has a partiuclar revision locked.
    // Returns an invalid UserInfoC if none.
    
    DListC<Tuple4C<DateC,StringC,RCSVersionC,StringC> > ListLogEntries();
    //: List all log entries after version afterVer.
    // Tuple4 Contains 
    //   1 - Date of mod
    //   2 - Author.
    //   3 - Version no.
    //   4 - Comment.
    
    bool Possess(bool verbose = false);
    //: Possess file.
    // Change owner ship of RCS db file to that of the
    // the current user.  (If possible).
    // NB. This user must own the RCS directory!
    
  protected:  
    FilenameC DBFilename();
    //: Get the RCS file name itself.
    
    StringC GetEntry(std::istream &in);
    //: Extract next entry from RCS file.
    // reads upto the next ';' character.
    // the ';' is discarded.
    
    StringC Unstuff(const StringC &str,int startat = 0);
    // Unstuff first string after 'startat'
    
    DateC ReadDate(StringC str);
    //: Convert date from rcs form.
    
    StringC DateRCS2DMY(StringC str);
    //: Convert a rcs date to day month year format.
    
    DateC DateRCS2DateC(StringC str);
    //: Convert a rcs date to DateC format.
    
    bool rcsParsed; // Set to true after RCS file is parsed.
    bool usesRCS;   // Does actual use RCS, only valid if rcsParsed is true. 
    FilenameC dbFilename;
    DListC<UserInfoC> access; // Access list.
    DListC<Tuple2C<StringC,RCSVersionC> > locks;
    RCSVersionC head;    // Head version.
  };
  
  
  /////////////////////////////
  //! userlevel=Normal
  //: Body class for dealing with RCS files.
  
  class RCSFileC : public RCHandleC<RCSFileBodyC> {
  public:
    RCSFileC() {}
    //: Default constructor.
    
    RCSFileC(const FilenameC &aname);
    //: Construct from a filename.
    
    RCSFileC(const StringC &aname);
    //: Construct from a string.
    
    RCSFileC(const RCSFileC &oth)
      : RCHandleC<RCSFileBodyC>(oth)
      {}
    //: Copy Constructor.
    
    inline bool Exists() const 
      { return Body().Exists(); }
    //: Does file exists ?
    
    inline bool IsCheckedOut()
      { return Body().IsCheckedOut() ; }
    //: Is file checked out ?
    
    inline bool CheckOut()
      { return Body().CheckOut() ; }
    //: Check out file.
    // Return true=Operation succeded.
    
    inline bool NeedsUpdate()
      { return Body().NeedsUpdate() ; }
    //: Check the time stamps on the file to see if
    //: update is needed.
    
    inline bool CheckIn(StringC cimsg = "RCSFileC Checkin")
      { return Body().CheckIn(cimsg) ; }
    //: Check in file.
    // Return true=Operation succeded.
    
    inline StringC CheckOutUL(StringC version = "",FilenameC dest = ".")
      { return Body().CheckOutUL(version,dest) ; }
    //: Checkout a unlocked version of this file to somewhere.
    // dest is a directory in which to put the file. <p>
    // Returns name of checkedout file, or empty string if failed.
  
    inline bool Update()
      { return Body().Update() ; }
    //: Make sure unlocked file is up to date.
  
    inline bool AddUser(const StringC &login)
      { return Body().AddUser(login) ; }
    //: Add user to access list.
    
    inline bool RemoveUser(const StringC &login)
      { return Body().RemoveUser(login) ; }
    //: Remove user from access list.
    
    inline bool UsesRCS()
      { return Body().UsesRCS(); }
    //: Test if file has an RCS database.
    
    inline bool IsDifferent(FilenameC other,StringC vers = "")
      { return Body().IsDifferent(other,vers) ; }
    //: Is this working file different from some other version of a particular file.
    
    inline bool IsDiffFromHead()
      { return Body().IsDiffFromHead() ; }
    //: Test if current version of file is different from head.
    
    inline bool ParseRCS()
      { return Body().ParseRCS() ; }
    //: Extract info from RCS file.
    
    inline bool ExtractInital(StringC &author,StringC &date)
      { return Body().ExtractInital(author,date); }
    //: Who originaly checkin in the file and the date it happened.
    
    DListC<Tuple4C<DateC,StringC,RCSVersionC,StringC> > ListLogEntries()
      { return Body().ListLogEntries(); }
    //: List all log entries after version 'afterVer'.
    
    bool Possess(bool verbose = false)
      { return Body().Possess(verbose); }
    //: Possess file.
    // Change owner ship of RCS db file to that of the
    // the current user.  (If possible).
    // NB. This user must own the RCS directory!
    
    inline bool HasAccess(const UserInfoC &auser)
      { return Body().HasAccess(auser); }
    //: Test if a user has access.
    
    inline bool IsAvailable()
      { return Body().IsAvailable() ; }
    //: Test if file is available to modify. 
    // User must have access to file and it can't be
    // locked by anyone else.
    
    inline RCSVersionC HeadVersion()
      { return Body().HeadVersion() ; }
    //: Get head version.
    
    inline bool HasChangedFrom(RCSVersionC fromVer)
      { return Body().HasChangedFrom(fromVer); }
    //: Has file changed from version 'fromVer' ?
    
    inline bool HasChangedFrom(const RCSFileC &oth,RCSVersionC fromVer)
      { return Body().HasChangedFrom(oth,fromVer); }
    //: Has file changed from version 'fromVer' of oth ?
    // Similar to IsDifferent()
    
    inline FilenameC &Filename() 
      { return Body(); }
    //: Access filename.
    
    inline const FilenameC &Filename() const 
      { return Body(); }
    //: Access filename.
    
    inline bool BinCompare(const FilenameC &fn) const
      { return Body().BinCompare(fn); }
    //: Binary compair working files.
    
    inline bool CopyTo(const FilenameC &fn) const 
      { return Body().CopyTo(fn); }
    //: Copy this working file somewhere else.
    
    operator StringC () const { return Body(); }
    //: Access as a string.
    
    operator FilenameC () const { return Body(); }
    //: Access as a Filename.
    
  };
  
  inline std::ostream &operator <<(std::ostream &out,const RCSFileC &file) { 
    out << file.Filename();
    return out;
  }
  
}
#endif
