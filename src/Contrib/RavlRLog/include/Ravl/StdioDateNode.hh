/*****************************************************************************
 * Author:   Alexey Kostin
 *
 *****************************************************************************
 * This code is based on StdioNodeC class from RLog library written by Valient Gough
 *
 * This library is free software; you can distribute it and/or modify it under
 * the terms of the GNU Lesser General Public License (LGPL), as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at your
 * option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the LGPL in the file COPYING for more
 * details.
 *
 */
//! lib=RavlRLog
// file-header-ends-here

                             
#ifndef _StdioDateNode_incl_
#define _StdioDateNode_incl_

#include <rlog/common.h>
#include <rlog/StdioNode.h>

namespace rlog
{
  // documentation in implementation file
  class RLOG_DECL StdioDateNode : public StdioNode
  {
  public:
    StdioDateNode( int fdOut = 2, int flags = (int)DefaultOutput );
	  // for backward compatibility
    StdioDateNode( int fdOut, bool colorize_if_tty );

    virtual ~StdioDateNode();

  protected:
    virtual void publish( const RLogData &data );

	  StdioDateNode( const StdioDateNode & );
	  StdioDateNode &operator = ( const StdioDateNode & );
  };

}

#endif
