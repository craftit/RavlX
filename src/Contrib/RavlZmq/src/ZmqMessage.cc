// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlZmq

#include "Ravl/Zmq/Message.hh"
#include "Ravl/Zmq/MsgBuffer.hh"
#include "Ravl/OS/SysLog.hh"
#include <zmq.h>
#include <string.h>

namespace RavlN {
  namespace ZmqN {

    //! Construct an empty message.
    MessageC::MessageC()
    {

    }

    //! Destructor.
    MessageC::~MessageC()
    {

    }

    //! Construct a message from some data
    MessageC::MessageC(const SArray1dC<char> &data)
    {
      m_parts.push_back(data);
    }

    MessageC::MessageC(const char *data)
    {
      Push(data);
    }

    MessageC::MessageC(const StringC &data)
    {
      Push(data);
    }


    //! Push a string.
    void MessageC::Push(const char *msg) {
      size_t len = strlen(msg);
      SArray1dC<char> arr(MsgBufferC(len),len);
      if(len > 0)
        memcpy(arr.ReferenceElm(),msg,len);
      Push(arr);
    }

    //! Push a string.
    void MessageC::Push(const std::string &msg) {
      SArray1dC<char> arr(MsgBufferC(msg.size()),msg.size());
      if(arr.Size() > 0)
        memcpy(arr.ReferenceElm(),&msg[0],msg.size());
      Push(arr);
    }

    //! Push a string.
    void MessageC::Push(const RavlN::StringC &msg) {
      SArray1dC<char> arr(MsgBufferC((size_t) msg.Size()),msg.Size());
      if(arr.Size() > 0)
        memcpy(arr.ReferenceElm(),&msg[0],msg.size());
      Push(arr);
    }

    //! Push contents of another message onto the end of this one.
    void MessageC::Push(const MessageC &msg) {
      //m_parts.reserve(msg.)
      for(size_t i = 0;i < msg.Parts().size();i++)
        Push(msg.Parts()[i]);
    }

    //! Push a buffer onto the message stack
    void MessageC::Push(const SArray1dC<char> &buff)
    {
      m_parts.push_back(buff);
    }

    //! Pop a buffer from the message stack
    void MessageC::Pop(SArray1dC<char> &buff)
    {
      buff = m_parts.back();
      m_parts.pop_back();
    }

    //! Pop a message
    void MessageC::Pop(std::string &str) {
      SArray1dC<char> &part = m_parts.back();
      //RavlDebug("Got part:%zu ",part.Size().V());
      if(part.Size() == 0) {
        str = std::string();
      } else {
        str = std::string(&part[0],part.Size().V());
      }
      m_parts.pop_back();
    }

    //! Pop a message
    void MessageC::Pop(RavlN::StringC &str) {
      SArray1dC<char> &part = m_parts.back();
      //RavlDebug("Got part:%zu ",part.Size().V());
      if(part.Size() == 0) {
        str = RavlN::StringC();
      } else {
        str = RavlN::StringC(&part[0],part.Size().V());
      }
      m_parts.pop_back();
    }

    //! Write out message in human readable form.
    void MessageC::Dump(std::ostream &strm) const
    {
      strm << "Got " << Parts().size() << " part message:\n";
      for(unsigned i = 0;i < Parts().size();i++) {
        StringC src;
        unsigned size = Parts()[i].Size();
        if(size > 0) {
          if(Parts()[i][0] != 0) {
            src = StringC(Parts()[i].Size(),&Parts()[i][0]);
          } else {
            src += "[";
            const RavlN::SArray1dC<char> &p = Parts()[i];
            for(unsigned j = 0;j < p.Size();j++)
              src += StringC((unsigned) p[j]) + StringC(' ');
            src += "]";
          }
        }
        strm << " Part:" << i << " '" << src.c_str() << "' (" << (unsigned) size << ")";
      }

    }

    //! Dump part of message as text
    StringC MessageC::DumpPart(int part,int maxLen) const
    {
      RavlAssert(part >= 0);
      if(Parts().size() <=(unsigned) part) {
        return "*no part*";
      }
      const SArray1dC<char> &arr = Parts()[part];
      if(arr.Size() == 0)
        return "";
      if(arr.Size() < maxLen) {
        StringC ret(arr.Size(),&(arr[0]));
        ret += (char) 0;
        return ret;
      }
      StringC ret(maxLen,&arr[0]);
      ret += (char) 0;
      return ret;
    }

    //! Dump human readable version of message
    std::ostream &operator<<(std::ostream &strm,const MessageC &msg)
    {
      msg.Dump(strm);
      return strm;
    }


}}
