// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlGeneticOptimisation
//! author=Charles Galambos
//! docentry=Ravl.API.Math.Genetic.Optimisation

#include "Ravl/XMLFactoryRegister.hh"
#include "Ravl/Genetic/EvaluateFitness.hh"
#include "Ravl/VirtualConstructor.hh"

namespace RavlN {  namespace GeneticN {

  //! Constructor
  EvaluateFitnessC::EvaluateFitnessC()
  {}

  //! Factory constructor
  EvaluateFitnessC::EvaluateFitnessC(const XMLFactoryContextC &factory)
  {}

  //! Load form a binary stream
  EvaluateFitnessC::EvaluateFitnessC(BinIStreamC &strm)
  {
    ByteT version = 0;
    strm >> version;
    if(version != 1)
      throw RavlN::ExceptionUnexpectedVersionInStreamC("EvaluateFitnessC");
  }

  //! Load form a binary stream
  EvaluateFitnessC::EvaluateFitnessC(std::istream &strm)
  {
    RavlAssertMsg(0,"not implemented");
  }

  //! Is this fitness IO capable ?
  bool EvaluateFitnessC::CanSave() const
  { return false; }

  //! Save to binary stream
  bool EvaluateFitnessC::Save(BinOStreamC &strm) const
  {
    if(!RavlN::RCLayerBodyC::Save(strm))
      return false;
    ByteT version = 1;
    strm << version;
    return true;
  }

  //! Save to binary stream
  bool EvaluateFitnessC::Save(std::ostream &strm) const
  {
    RavlAssertMsg(0,"not implemented");
    return false;
  }

  //! Copy me.
  RCBodyVC &EvaluateFitnessC::Copy() const
  {
    RavlAssertMsg(0,"abstract class");
    throw RavlN::ExceptionOperationFailedC("abstract copy.");
  }

  //! Access type of object evaluated for fitness.
  const std::type_info &EvaluateFitnessC::ObjectType() const
  { return typeid(void); }

  //! Generate a new problem in the domain.
  bool EvaluateFitnessC::GenerateNewProblem() {
    return true;
  }

  //! Evaluate the fit
  bool EvaluateFitnessC::Evaluate(int generation,RCWrapAbstractC &obj,float &score)
  {
    RavlAssertMsg(0,"Abstract method called.");
    return false;
  }

  //: Called when owner handles drop to zero.
  void EvaluateFitnessC::ZeroOwners() {
    RCLayerBodyC::ZeroOwners();
  }

  //RAVL_INITVIRTUALCONSTRUCTOR_NAMED(EvaluateFitnessC,"RavlN::GeneticN::EvaluateFitnessC");

 }}
