
#include "Ravl/Genetic/PopulationState.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/PointerManager.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/DP/FileFormatBinStream.hh"

namespace RavlN { namespace GeneticN {

  //! Default constructor
  PopulationStateC::PopulationStateC()
   : m_generation(0)
  {}

  //! Default constructor
  PopulationStateC::PopulationStateC(
      int gen,
      SArray1dC<RavlN::Tuple2C<float,GenomeC::RefT> > &seeds,
      SArray1dC<RavlN::Tuple2C<float,GenomeC::RefT> > &population
      )
    : m_generation(gen),
      m_seeds(seeds),
      m_population(population)
  {
  }

  //! Load form a binary stream
  PopulationStateC::PopulationStateC(BinIStreamC &strm)
   : RavlN::RCBodyVC(strm),
     m_generation(0)
  {
    ByteT version = 0;
    strm >> version;
    if(version != 1)
      throw RavlN::ExceptionUnexpectedVersionInStreamC("PopulationStateC");
    strm >> m_generation >> m_seeds >> m_population >> RavlN::ObjIO(m_fitnessFunction);
  }

  //! Load form a binary stream
  PopulationStateC::PopulationStateC(std::istream &strm)
   : RavlN::RCBodyVC(strm),
     m_generation(0)
  {
    RavlAssertMsg(0,"not implemented");
  }

  //! Save to binary stream
  bool PopulationStateC::Save(BinOStreamC &strm) const
  {
    if(!RavlN::RCBodyVC::Save(strm))
      return false;
    ByteT version = 1;
    strm << version;
    strm << m_generation << m_seeds << m_population << RavlN::ObjIO(m_fitnessFunction);
    return true;
  }

  //! Save to binary stream
  bool PopulationStateC::Save(std::ostream &strm) const
  {
    if(!RavlN::RCBodyVC::Save(strm))
      return false;
    RavlAssertMsg(0,"not implemented");

    return true;
  }

  RAVL_INITVIRTUALCONSTRUCTOR_NAMED(PopulationStateC,"RavlN::GeneticN::PopulationStateC");

  //XMLFactoryRegisterC<GenomeC> g_registerGenome("RavlN::GeneticN::GenomeC");
  static RavlN::TypeNameC g_typePtrGenome(typeid(PopulationStateC::RefT),"RavlN::SmartPtrC<RavlN::GeneticN::PopulationStateC>");
  static FileFormatBinStreamC<RavlN::SmartPtrC<RavlN::GeneticN::PopulationStateC> > g_registerPopulationState;

}}
