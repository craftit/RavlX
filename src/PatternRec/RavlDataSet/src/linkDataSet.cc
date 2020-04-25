// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlPatternRec
namespace RavlN {

  extern void linkDataSetVectorLabel();
  extern void linkSampleVector();
  extern void LinkFunctionCascade();
  extern void LinkConcatenate();
  extern void LinkFunctionExpand();
  extern void LinkDPProcessFunction();

  void LinkDataSet() {
    linkDataSetVectorLabel();
    linkSampleVector();
    LinkFunctionCascade();
    LinkConcatenate();
    LinkFunctionExpand();
    LinkDPProcessFunction();
  }


}
