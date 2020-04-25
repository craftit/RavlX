// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlGeneticProgram
//! author=Charles Galambos
//! docentry=Ravl.API.Math.Genetic.Programming

#include "Ravl/Genetic/GPInstruction.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/Genetic/GenomeClass.hh"
#include "Ravl/Genetic/GenomeMeta.hh"
#include "Ravl/Genetic/GeneTypeWeightedMeta.hh"
#include "Ravl/Genetic/GeneFactory.hh"
#include "Ravl/Genetic/GeneTypeProxy.hh"

namespace RavlN { namespace GeneticN {

  //! Constructor
  GPExecutionContextC::GPExecutionContextC()
   : m_depth(0),
     m_maxDepth(100),
     m_terminate(false)
  {}

  // ------------------------------------------

  //! Construct from gene factory
  GPInstructionC::GPInstructionC(const GeneFactoryC &factory)
  {

  }

  //! Factory constructor
  GPInstructionC::GPInstructionC(const XMLFactoryContextC &factory)
  {

  }

  //! Constructor
  GPInstructionC::GPInstructionC()
  {}

  //! Access gene type for instructions
  const GeneTypeC::RefT &GPInstructionC::GeneType() {
    static GeneTypeC::RefT x = new GeneTypeClassC(typeid(GPInstructionC::RefT));
    return x;
  }

  //! Execute instruction
  bool GPInstructionC::Execute(GPExecutionContextC &context)
  {

    return true;
  }

  GeneTypeMetaC::RefT &InstructionMetaType()
  {
    static GeneTypeMetaC::RefT x = new GeneTypeWeightedMetaC("GPInstructions",std::vector<GeneTypeC::ConstRefT>(),std::vector<float>());
    return x;
  }

  void AddToInstructionList(const GeneTypeC &instructionType) {
    InstructionMetaType()->AddType(instructionType);
  }

  //! Access list of available instructions
  GeneTypeC::RefT &InstructionGeneType()
  {
    static GeneTypeC::RefT x = new GeneTypeProxyC("GPInstruction");
    return x;
  }

  static RegisterGeneClassC<GPInstructionC> g_registerGPInstruction("RavlN::GeneticN::GPInstructionC");

}}
