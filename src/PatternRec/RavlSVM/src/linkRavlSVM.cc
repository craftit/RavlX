// This file is used in conjunction with RAVL, Recognition And Vision Library
// Copyright (C) 2004, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU
// General Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
//! lib=RavlSVM

//! author="Alexey Kostin"

namespace RavlN {
  extern void InitRavlSVMIO2();
  extern void InitRavlSVMIO();
  extern void linkCommonKernels();
  extern void linkDesignSvm();
  extern void linkDesignOneClass();
  extern void linkDesignOneClassLarge();
  extern void linkDesignSvmSmo();
  extern void linkDesignSvmSmoLarge();
  
  void linkRavlSVM() {
    InitRavlSVMIO2();
    InitRavlSVMIO();
    linkCommonKernels();
    linkDesignSvm();
    linkDesignOneClass();
    linkDesignOneClassLarge();
    linkDesignSvmSmo();
    linkDesignSvmSmoLarge();
  }
}
