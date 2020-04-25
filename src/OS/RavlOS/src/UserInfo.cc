// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlOS
//! file="Ravl/OS/FileSystem/UserInfo.cc"

#include "Ravl/config.h"

#if RAVL_OS_SOLARIS
#define __EXTENSIONS__ 1
#include <pwd.h>
#endif

#include "Ravl/OS/UserInfo.hh"
#include "Ravl/Exception.hh"

#if RAVL_HAVE_PWD_H
#include <pwd.h>
#endif
#if RAVL_HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <errno.h>
#include <stdio.h>

#define PWBUFFLEN 2048

namespace RavlN {

  // FIXME :- This code is not always re-entrant.
  
  ///////////////////////
  // Default constructor.
  
  UserInfoC::UserInfoC()
    : exists(false),
      uid(0),
      login("")
  {}
  
  ///////////////////////
  // From login.
  
  UserInfoC::UserInfoC(StringC nlogin) 
    : exists(false),
      uid(0),
      login(nlogin.Copy())
  {
    Init(nlogin);
  }
  
  ///////////////////////
  // Constructor.

  UserInfoC::UserInfoC(UIntT uid) 
    : exists(false),
      uid(0),
      login("unknown")
  {
    Init(uid);
  }
  
  ///////////////////////////
  // Make this entry invalid.
  
  void UserInfoC::NullUser() {
    exists = false;
    uid = 0;
    gid = 0;
    login = StringC("");
    fullname = StringC("");
    homedir = StringC("");
    shell = StringC("");
  }
  
  ////////////////////////
  // Initalise from userid.
  
  bool UserInfoC::Init(UIntT aUserid) {
#if RAVL_OS_UNIX
    struct passwd *pwentry;
#if RAVL_HAVE_GETPWUID_R
    struct passwd pwbuff;
    char buff[PWBUFFLEN];
#if RAVL_HAVE_GETPW_WITH_RESULT
    if(getpwuid_r(aUserid,&pwbuff,buff,PWBUFFLEN,&pwentry) == 0)
#elif RAVL_HAVE_GETPW_RET_PW
    if((pwentry = getpwuid_r(aUserid,&pwbuff,buff,PWBUFFLEN)) == 0) 
#else
    pwentry = &pwbuff;
    if(getpwuid_r(aUserid,&pwbuff,buff,PWBUFFLEN) == 0)
#endif
#else
    // FIXME :- Do some locking here for multhreaded applications.
    if((pwentry = getpwuid(aUserid)) == 0) 
#endif
    {
      NullUser();
#if RAVL_ERRNO_IS_FUNC
      if(errno() == ERANGE)
#else
      if(errno == ERANGE)
#endif
	perror("UserInfoC::Init(), ERROR:Buffer to small ");
      return false;
    }
    Init(pwentry);
    return true;
#else
    throw ExceptionC("UserInfoC::Init(), Not implemented. ");
#endif
  }

////////////////////////
// Initalise from login.
  
  bool UserInfoC::Init(StringC aLogin)  {
#if RAVL_OS_UNIX
    struct passwd *pwentry;
#if RAVL_HAVE_GETPWNAM_R
    struct passwd pwbuff;
    char buff[PWBUFFLEN];
#if RAVL_HAVE_GETPW_WITH_RESULT
    if(getpwnam_r(aLogin,&pwbuff,buff,PWBUFFLEN,&pwentry) == 0)
#elif RAVL_HAVE_GETPW_RET_PW
    if((pwentry = getpwnam_r(aLogin,&pwbuff,buff,PWBUFFLEN)) == 0)
#else
    pwentry = &pwbuff;
    if(getpwnam_r(aLogin,&pwbuff,buff,PWBUFFLEN) == 0)
#endif
#else
    // FIXME :- Do some locking here for multhreaded applications.
    if((pwentry = getpwnam(aLogin)) == 0) 
#endif
      {
	NullUser();
#if RAVL_ERRNO_IS_FUNC
	if(errno() == ERANGE)
#else
	if(errno == ERANGE)
#endif
	  perror("UserInfoC::Init(), ERROR:Buffer to small ");
	return false;
      }
    Init(pwentry);
    return true;
#else
    throw ExceptionC("UserInfoC::Init(), Not implemented. ");
#endif
  }

  ////////////////////////
  // Initalise from a password structure.
  
  void UserInfoC::Init(struct passwd *pw)  {
#if RAVL_OS_UNIX
    exists = true;
    uid = pw->pw_uid;
    gid = pw->pw_gid;
    login = pw->pw_name;
    fullname = pw->pw_gecos;
    homedir = pw->pw_dir;
    shell = pw->pw_shell;
#else
    throw ExceptionC("UserInfoC::Init(), Not implemented. ");
#endif
  }

  //////////////////////////////
  //: Get current user.
  
  UserInfoC UserInfoC::WhoAmI()  { 
#if RAVL_OS_UNIX
    static UserInfoC itsme(getuid()); 
    return itsme;
#else
    throw ExceptionC("UserInfoC::WhoAmI(), Not implemented. ");
#endif
  }
}


