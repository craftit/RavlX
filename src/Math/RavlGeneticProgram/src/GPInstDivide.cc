// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlGeneticProgram
//! author=Charles Galambos
//! docentry=Ravl.API.Math.Genetic.Programming

#include "Ravl/Genetic/GPInstDivide.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/Genetic/GenomeClass.hh"
#include "Ravl/Genetic/GenomeList.hh"
#include "Ravl/Genetic/GenomeShare.hh"

namespace RavlN { namespace GeneticN {

  static RegisterGPInstructionC<GPInstDivideC<IntT> > g_regInst1("RavlN::GeneticN::GPInstDivideC<IntT>");
  static RegisterGPInstructionC<GPInstDivideC<float> > g_regInst2("RavlN::GeneticN::GPInstDivideC<float>");

  void LinkGPInstDivide()
  {}

}}
