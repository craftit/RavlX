// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#include "Ravl/Genetic/Genome.hh"
#include "Ravl/DP/Converter.hh"
#include "Ravl/OS/SysLog.hh"
//! lib=RavlGeneticOptimisation
//! author=Charles Galambos
//! docentry=Ravl.API.Math.Genetic.Optimisation

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN { namespace GeneticN {


  static GeneC::RefT ConvertGenome2Gene(const GenomeC::RefT &genome)
  { return &genome->RootGene(); }

  DP_REGISTER_CONVERSION(ConvertGenome2Gene,1.0);

  void LinkGene2ClassGenerators()
  {}


}}
