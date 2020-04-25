// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_GENETIC_GENETYPEPROXY_HH
#define RAVL_GENETIC_GENETYPEPROXY_HH 1
//! lib=RavlGeneticOptimisation
//! author=Charles Galambos
//! docentry=Ravl.API.Math.Genetic.Optimisation

#include "Ravl/Genetic/GeneType.hh"

namespace RavlN { namespace GeneticN {

   //! Gene type which is refere's to another
   //! type via the palate

   class GeneTypeProxyC
    : public GeneTypeC
   {
   public:
     //! Factory constructor
     GeneTypeProxyC(const XMLFactoryContextC &factory);

     //! Constructor
     GeneTypeProxyC(const StringC &name);

     //! Load form a binary stream
     GeneTypeProxyC(BinIStreamC &strm);

     //! Load form a binary stream
     GeneTypeProxyC(std::istream &strm);

     //! Save to binary stream
     virtual bool Save(BinOStreamC &strm) const;

     //! Save to binary stream
     virtual bool Save(std::ostream &strm) const;

     //! Dump description in human readable form.
     virtual void Dump(std::ostream &strm,UIntT indent) const;

     //! Create randomise value
     virtual void Random(GenePaletteC &palette,GeneC::RefT &newValue) const;

     //! Mutate a gene
     virtual bool Mutate(GenePaletteC &palette,float fraction,bool mustChange,const GeneC &original,RavlN::SmartPtrC<GeneC> &newValue) const;

     // Reference to this gene.
     typedef RavlN::SmartPtrC<GeneTypeProxyC> RefT;

     // Const reference to this gene.
     typedef RavlN::SmartPtrC<const GeneTypeProxyC> ConstRefT;

   protected:
     StringC m_proxyName;
   };

}}

#endif
