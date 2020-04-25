// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, James Smith
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////
//! lib=RavlURLIO
//! file="Ravl/Contrib/HTTP/URLStream.cc"

#include "Ravl/IO/URLStream.hh"
#include "Ravl/StreamType.hh"
#include "Ravl/Threads/LaunchThread.hh"
#include "Ravl/BufStream.hh"
#include "Ravl/HashIter.hh"

#include <curl/curl.h>
//#include <curl/types.h>
#include <curl/easy.h>

#include "Ravl/IO/Curl.hh"

#include <unistd.h>

// If set to 1 a file buffer will be used for download, this can
// cause problems with the current implementation as the URLStreamC
// destructor may be called before the stream is finished with.

#define URLISTREAM_USEFILEBUFFER 0


#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

#ifndef CURLOPT_WRITEDATA
#define CURLOPT_WRITEDATA CURLOPT_FILE
#endif

namespace RavlN {

  void InitURLStreamIO() {}

  static size_t dataWrite(void *ptr, size_t size, size_t nmemb, void *stream) {      
    static_cast<OStreamC*>(stream)->write(static_cast<char*>(ptr),size*nmemb);
    return size*nmemb;
  }

  static size_t dataRead(void *ptr, size_t size, size_t nmemb, void *stream) {
    static_cast<IStreamC*>(stream)->read(static_cast<char*>(ptr),size*nmemb);
    return static_cast<IStreamC*>(stream)->gcount();
  }


  //: Retrieve file into a byte array.
  
  IntT URLRetrieve(const StringC &url,SArray1dC<char> &buf) {
    static StringC user,passwd;
    return URLRetrieve(url,user,passwd,buf);
  }

  //: Retrieve file into a byte array.
  //!param:url - URL to retrieve
  //!param:user - Username
  //!param:passwd - Password
  //!param:buf - buffer to store data into.
  //!return: Error code, 0 if all's well.
  
  IntT URLRetrieve(const StringC &url,const StringC &user,const StringC &passwd,SArray1dC<char> &buf) {
    // Create temporary memory buffer
    BufOStreamC tmpstrm;
    // Fetch URL
    //CURL *curl = NULL;
    ONDEBUG(cerr << "Retrieving URL: " << url);
    
    // Initialise CURL
    CurlC curl;
    
    IntT errVal = 0;
    
    // Set options

    curl_easy_setopt(curl, CURLOPT_URL, url.chars());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dataWrite);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &tmpstrm);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 0);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1);
    
    // Setup password if provided.
    if(!user.IsEmpty() || !passwd.IsEmpty()) {
      curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
      StringC str = user + ':' + passwd;
      curl_easy_setopt(curl, CURLOPT_USERPWD,str.chars());
    }
    
    // Get the URL
    errVal = curl_easy_perform(curl);
    
    // Clean up
    ONDEBUG(cerr << "Building output stream. ");
    // Recreate IStream from the read pipe
    buf = tmpstrm.Data();
    
    return errVal;    
  }


  //: Retrieve file into a byte array.
  //!param:url - URL to retrieve
  //!param:reply - buffer to store data into.
  //!return: Error code, 0 if all's well.
  IntT URLPut(const StringC &url,const SArray1dC<char> &putData,SArray1dC<char> &reply)
  {
    // Create temporary memory buffer
    BufOStreamC tmpstrm;
    BufIStreamC tmpistrm(putData);
    // Fetch URL
    //CURL *curl = NULL;
    ONDEBUG(cerr << "Retrieving URL: " << url);

    // Initialise CURL
    CurlC curl;

    IntT errVal = 0;

    // Set options

    curl_easy_setopt(curl, CURLOPT_URL, url.chars());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dataWrite);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &tmpstrm);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, dataRead);
    curl_easy_setopt(curl, CURLOPT_READDATA, &tmpistrm);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 0);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1);

    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(curl, CURLOPT_PUT, 1L);
    curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE,(curl_off_t)putData.Size().V());

#if 0
    // Setup password if provided.
    if(!user.IsEmpty() || !passwd.IsEmpty()) {
      curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
      StringC str = user + ':' + passwd;
      curl_easy_setopt(curl, CURLOPT_USERPWD,str.chars());
    }
#endif

    // Get the URL
    errVal = curl_easy_perform(curl);

    // Clean up
    ONDEBUG(cerr << "Building output stream. ");
    // Recreate IStream from the read pipe
    reply = tmpstrm.Data();

    return errVal;
  }


  //:Form a url given the base url and key value pairs for the parameters.
  StringC FormURL(const StringC & baseURL, const RCHashC<StringC, StringC> & args)
  {
    StringC url = baseURL;
    bool firstArg = true;
    for (HashIterC<StringC, StringC> iter(args); iter.IsElm(); iter.Next()) {

      // prepend a &, ? for first argument.
      url += (firstArg ? "?" : "&");
      if (firstArg)
        firstArg = false;

      // append the argument.
      url += iter.Key() + "=" + iter.Data();
    }
    // remove whitespace from URL
    url.gsub(" ", "%20"); // subs white space.
    return url;
  }

  //: Get a string describing the error
  StringC URLErrorString(int errNum)
  {
    static const char* strerror[] = {
      "OK",                      /* 0 - Nothing wrong! */
      "UNSUPPORTED PROTOCOL",    /* 1 - */
      "FAILED INIT",             /* 2 - */
      "URL MALFORMAT",           /* 3 - */
      "URL MALFORMAT USER",      /* 4 - */
      "COULDNT RESOLVE PROXY",   /* 5 - */
      "COULDNT RESOLVE HOST",    /* 6 -  */
      "COULDNT CONNECT",
      "FTP WEIRD SERVER REPLY",
      "FTP ACCESS DENIED",
      "FTP USER PASSWORD INCORRECT",
      "FTP WEIRD PASS REPLY",
      "FTP WEIRD USER REPLY",
      "FTP WEIRD PASV REPLY",
      "FTP WEIRD 227 FORMAT",
      "FTP CANT GET HOST",
      "FTP CANT RECONNECT",
      "FTP COULDNT SET BINARY",
      "PARTIAL FILE",
      "FTP COULDNT RETR FILE",
      "FTP WRITE ERROR",
      "FTP QUOTE ERROR",
      "HTTP NOT FOUND",
      "WRITE ERROR",
      "MALFORMAT USER",          /* 24 - user name is illegally specified */
      "FTP COULDNT STOR FILE",   /* 25 - failed FTP upload */
      "READ ERROR",              /* 26 - could open/read from file */
      "OUT OF MEMORY",
      "OPERATION TIMEOUTED",     /* 28 - the timeout time was reached */
      "FTP COULDNT SET ASCII",   /* 29 - TYPE A failed */
      "FTP PORT FAILED",         /* 30 - FTP PORT operation failed */
      "FTP COULDNT USE REST",    /* 31 - the REST command failed */
      "FTP COULDNT GET SIZE",    /* 32 - the SIZE command failed */
      "HTTP RANGE ERROR",        /* 33 - RANGE "command" didn't work */
      "HTTP POST ERROR",
      "SSL CONNECT ERROR",       /* 35 - wrong when connecting with SSL */
      "FTP BAD DOWNLOAD RESUME", /* 36 - couldn't resume download */
      "FILE COULDNT READ FILE",
      "LDAP CANNOT BIND",
      "LDAP SEARCH FAILED",
      "LIBRARY NOT FOUND",
      "FUNCTION NOT FOUND",
      "ABORTED BY CALLBACK",
      "BAD FUNCTION ARGUMENT",
      "BAD CALLING ORDER",
      "HTTP PORT FAILED",        /* 45 - HTTP Interface operation failed */
      "BAD PASSWORD ENTERED",    /* 46 - my_getpass() returns fail */
      "TOO MANY REDIRECTS ",     /* 47 - catch endless re-direct loops */
      "UNKNOWN TELNET OPTION",   /* 48 - User specified an unknown option */
      "TELNET OPTION SYNTAX ",   /* 49 - Malformed telnet option */
      "OBSOLETE",                /* 50 - removed after 7.7.3 */
      "SSL PEER CERTIFICATE",    /* 51 - peer's certificate wasn't ok */
      "GOT NOTHING",             /* 52 - when this is a specific error */
      "SSL ENGINE NOTFOUND",
      "SSL ENGINE SETFAILED"
    };
    return StringC(strerror[errNum]);
  }


  URLIStreamC::URLIStreamC(const StringC& url,bool buffered)
    : m_strTemp("/tmp/ravldl")
  {
#if URLISTREAM_USEFILEBUFFER
    // Create temporary file
    m_strTemp.MkTemp();
    OStreamC tmpstrm(m_strTemp);
    // Fetch URL
    CURL *curl = NULL;
    // Initialise CURL
    curl = curl_easy_init();
    if(curl) {
      // Set options
      curl_easy_setopt(curl, CURLOPT_URL, url.chars());
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dataWrite);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &tmpstrm);
      curl_easy_setopt(curl, CURLOPT_VERBOSE, 0);
      curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1);
      // Get the URL
      m_iError = curl_easy_perform(curl);
      // Clean up
      curl_easy_cleanup(curl);
    }
    // Recreate IStream from the read pipe
    (*this).IStreamC::operator=(IStreamC(m_strTemp,true,buffered));
#else
    SArray1dC<char> buf;
    m_iError = URLRetrieve(url,buf);
    (*this).IStreamC::operator=(BufIStreamC(buf));
#endif
  }

  //: Open net connection for input.
  //: Uses Basic http authentication with the given username and password.

  URLIStreamC::URLIStreamC(const StringC &url,const StringC &user,const StringC &password)
    : m_strTemp("/tmp/ravldl")
  {
    SArray1dC<char> buf;
    m_iError = URLRetrieve(url,user,password,buf);
    (*this).IStreamC::operator=(BufIStreamC(buf));
  }

  URLIStreamC::~URLIStreamC() {
#if URLISTREAM_USEFILEBUFFER
    m_strTemp.Remove();
#endif
  }

  StringC URLIStreamC::ErrorString() const {
    return URLErrorString(m_iError);
  }

  StringC URLIStreamC::URLEncode(const StringC& string) {
    StringC encoded;
    for (unsigned int i=0; i<string.Size(); i++) {
      if (!isalpha(string[i]) && !isdigit(string[i])) {
	StringC str;
	str.form("%%%X",string[i]);
	encoded += str;
      }
      else {
	encoded += string[i];
      }
    }
    return encoded;
  }

  StringC URLIStreamC::AddUserPass(const StringC& url,const StringC& user,const StringC& pass) {
    // URL-encode the username
    StringC euser = URLIStreamC::URLEncode(user);
    // URL-encode the password
    StringC epass = URLIStreamC::URLEncode(pass);
    // Split URL
    StringC address = url;
    StringC protocol = address.before(":");
    address = address.after("//");
    // Insert username and password after URL
    return protocol + StringC("://") + euser + StringC(":") + epass + StringC("@") + address;
  }

   /////////////////// STREAM TYPES /////////////////////

   static class StreamType_HTTPIStreamC 
      : public StreamTypeC
   {
   public:
      StreamType_HTTPIStreamC()
      {}
      //: Default constructor.
    
      virtual const char *TypeName()
      { return "http"; }
      //: Get type of stream.
    
      virtual IStreamC OpenI(const StringC &url, bool binary = false,bool buffered = true) { 
	StringC rurl("http:" + url);
	return URLIStreamC(rurl,buffered); 
      }
      //: Open input stream.
    
   } Inst_StreamType_HTTPStream;

   static class StreamType_HTTPSIStreamC 
      : public StreamTypeC
   {
   public:
      StreamType_HTTPSIStreamC()
      {}
      //: Default constructor.
    
      virtual const char *TypeName()
      { return "https"; }
      //: Get type of stream.
    
      virtual IStreamC OpenI(const StringC &url, bool binary = false,bool buffered = true) { 
        StringC rurl("https:" + url);
        return URLIStreamC(rurl,buffered);
      }
      //: Open input stream.
    
   } Inst_StreamType_HTTPSStream;

   static class StreamType_FTPIStreamC 
      : public StreamTypeC
   {
   public:
      StreamType_FTPIStreamC()
      {}
      //: Default constructor.
    
      virtual const char *TypeName()
      { return "ftp"; }
      //: Get type of stream.
    
      virtual IStreamC OpenI(const StringC &url, bool binary = false,bool buffered = true) { 
        StringC rurl("ftp:" + url);
        return URLIStreamC(rurl,buffered);
      }
      //: Open input stream.
    
   } Inst_StreamType_FTPStream;

   static class StreamType_LDAPIStreamC 
      : public StreamTypeC
   {
   public:
      StreamType_LDAPIStreamC()
      {}
      //: Default constructor.
    
      virtual const char *TypeName()
      { return "ldap"; }
      //: Get type of stream.
    
      virtual IStreamC OpenI(const StringC &url, bool binary = false,bool buffered = true) { 
        StringC rurl("ldap:" + url);
        return URLIStreamC(rurl,buffered);
      }
      //: Open input stream.
    
   } Inst_StreamType_LDAPStream;

}
