// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlGeneticOptimisation
//! author=Charles Galambos
//! docentry=Ravl.API.Math.Genetic.Optimisation

namespace RavlN { namespace GeneticN {
  extern void LinkGene2ClassGenerators();
  extern void LinkGenomeMeta();
  extern void LinkGenomeList();
  extern void LinkGenomeShare();
  extern void LinkGenomeWeightedMeta();
  extern void LinkGeneticOptimiser();
  extern void LinkGeneTypeProxy();
  extern void LinkGeneticOptimiserCheckPoint();
  extern void LinkGeneTypeBool();
  extern void LinkGenomeArray();

  void LinkGeneticOpt() {
    LinkGene2ClassGenerators();
    LinkGenomeMeta();
    LinkGenomeList();
    LinkGenomeShare();
    LinkGenomeWeightedMeta();
    LinkGeneticOptimiser();
    LinkGeneticOptimiserCheckPoint();
    LinkGeneTypeProxy();
    LinkGeneTypeBool();
    LinkGenomeArray();
  }

}}
