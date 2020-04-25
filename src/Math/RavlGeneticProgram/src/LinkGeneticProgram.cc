// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlGeneticProgram
//! author=Charles Galambos
//! docentry=Ravl.API.Math.Genetic.Programming

namespace RavlN { namespace GeneticN {

  extern void LinkGPInstSendSignal();
  extern void LinkGPInstIf();
  extern void LinkGPInstRoutine();
  extern void LinkGPInstLoadConst();
  extern void LinkGPInstSubtract();
  extern void LinkGPInstAdd();
  extern void LinkGPInstAssign();
  extern void LinkGPInstPutArray();
  extern void LinkGPInstGetArray();
  extern void LinkGPInstIncrement();
  extern void LinkGPInstDecrement();
  extern void LinkGPInstLoop();
  extern void LinkGPInstMultiply();
  extern void LinkGPInstDivide();

  extern void LinkGPVariable();

  void LinkGeneticProgram() {

    LinkGPInstSendSignal();
    LinkGPInstIf();
    LinkGPInstRoutine();
    LinkGPInstLoadConst();
    LinkGPInstSubtract();
    LinkGPInstAdd();
    LinkGPInstAssign();
    LinkGPInstPutArray();
    LinkGPInstGetArray();
    LinkGPVariable();
    LinkGPInstIncrement();
    LinkGPInstDecrement();
    LinkGPInstLoop();
    LinkGPInstMultiply();
    LinkGPInstDivide();
  }

}}
