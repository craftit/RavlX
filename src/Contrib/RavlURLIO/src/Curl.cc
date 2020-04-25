
#include "Ravl/IO/Curl.hh"
#include "Ravl/Exception.hh"

namespace RavlN {

  static int DoCurlInit() {
    curl_global_init(CURL_GLOBAL_ALL);
    return 1;
  }

  static int InitCurl() {
    static int val = DoCurlInit();
    return val;
  }

  //: Constructor.
  CurlC::CurlC()
   : m_curl(0)
  {
    InitCurl();
    m_curl = curl_easy_init();
    if(m_curl == 0)
      throw RavlN::ExceptionOperationFailedC("Failed to init curl ");

    // Things in RAVL should be thread safe by default.
    curl_easy_setopt(m_curl, CURLOPT_NOSIGNAL, 1);
    // This is needed for thread safety. See 'Multi-threading Issues' in http://curl.haxx.se/libcurl/c/libcurl-tutorial.html
    // There could be problems with DNS lookup timeouts though.
  }

  //: Constructor.

  CurlC::CurlC(const StringC &url)
   : m_curl(0)
  {
    m_curl = curl_easy_init();
    if(m_curl == 0)
      throw RavlN::ExceptionOperationFailedC("Failed to init curl ");
    // Things in RAVL should be thread safe by default.

    curl_easy_setopt(m_curl, CURLOPT_NOSIGNAL, 1);
    // This is needed for thread safety. See 'Multi-threading Issues' in http://curl.haxx.se/libcurl/c/libcurl-tutorial.html
    // There could be problems with DNS lookup timeouts though.

    curl_easy_setopt(m_curl, CURLOPT_URL, url.chars());

  }

  //: Destructor
  CurlC::~CurlC()
  {
    if(m_curl != 0) {
      curl_easy_cleanup(m_curl);
      m_curl = 0; // Help detect any nasty ptr problems
    }
  }

}
