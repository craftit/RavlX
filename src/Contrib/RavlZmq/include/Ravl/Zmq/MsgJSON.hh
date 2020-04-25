// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_ZMQ_MSGJSON_HH_
#define RAVL_ZMQ_MSGJSON_HH_
//! lib=RavlZmq

#include "Ravl/SArray1d.hh"

namespace RavlN { namespace ZmqN {

  //! Message coded in JSON

  class MsgJSONC
    : public SArray1dC<char>
  {
  public:
    //! Create an empty buffer.
    MsgJSONC()
    {}

    //! Create a message buffer of a given size.
    MsgJSONC(size_t size);

    //! Constructor
    MsgJSONC(const SArray1dC<char> &array)
     : SArray1dC<char>(array)
    {}

  };

}}

#endif /* MSGJSON_HH_ */
