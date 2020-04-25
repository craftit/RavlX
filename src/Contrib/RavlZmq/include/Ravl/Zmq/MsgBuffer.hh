// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlZmq

#ifndef RAVL_ZMQ_MSGBUFFER_HH_
#define RAVL_ZMQ_MSGBUFFER_HH_

#include "Ravl/SArray1d.hh"
#include <zmq.h>

namespace RavlN {
  namespace ZmqN {

    //! Body of message buffer.

    class MsgBufferBodyC
     : public RavlN::BufferBodyC<char>
    {
    public:
       //! Create a buffer of a given size
       MsgBufferBodyC(size_t size);

       //! Create buffer from a message
       // If moveMsg is true, ownership is copied to this class.
       // otherwise it is copied.
       MsgBufferBodyC(zmq_msg_t &msg,bool moveMsg = true);

       //! Create a buffer from some other data.
       MsgBufferBodyC(void *data,size_t size,zmq_free_fn *ffn,void *hint);

       //! Create an empty buffer.
       MsgBufferBodyC();

       //! Destructor
       virtual ~MsgBufferBodyC();

       //! Build access to buffer.
       //! Use after msg has been updated in any way
       void Build();

       //! Access message.
       zmq_msg_t *Msg()
       { return &m_msg; }

    protected:
       zmq_msg_t m_msg;
    };


    //! Handle to message buffer.

    class MsgBufferC
     : public RavlN::BufferC<char>
    {
    public:
       MsgBufferC()
       {}

       //! Create a buffer from some other data.
       MsgBufferC(void *data,size_t size,zmq_free_fn *ffn,void *hint)
        : RavlN::BufferC<char>(new MsgBufferBodyC(data,size,ffn,hint))
       {}

       //! Create a buffer with an empty message.
       MsgBufferC(size_t size)
         : RavlN::BufferC<char>(new MsgBufferBodyC(size))
       {}

       //! Build from a message.
       MsgBufferC(zmq_msg_t &msg)
        : RavlN::BufferC<char>(new MsgBufferBodyC(msg))
       {}

       //! Try and up cast a buffer.
       //! Will leave handle invalid if it fails.
       MsgBufferC(const RavlN::BufferC<char> &buf)
        : RavlN::BufferC<char>(dynamic_cast<const MsgBufferBodyC *>(BodyPtr(buf)))
       {}

    protected:
       MsgBufferBodyC &Body()
       { return static_cast<MsgBufferBodyC &>(RavlN::BufferC<char>::Body()); }

       const MsgBufferBodyC &Body() const
       { return static_cast<const MsgBufferBodyC &>(RavlN::BufferC<char>::Body()); }

    public:

       //! Access message.
       zmq_msg_t *Msg()
       { return Body().Msg(); }

       //! Build buffer
       void Build()
       { Body().Build(); }
    };


    //! Convert a byte array into a message.
    //! Note: This expects 'destMsg' to be uninitialised.
    void ArrayToMessage(zmq_msg_t &destMsg,const SArray1dC<char> &srcArr);
  }
}

#endif /* MSGBUFFER_HH_ */
