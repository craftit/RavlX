
# Library: RavlGeneticOptimisation

# Requires: 
# Supports: 




# MustLink: src/LinkGeneticOpt.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlGeneticOptimisation 1)

include_directories(../../OS/RavlOS/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../OS/RavlXMLFactory/include)
include_directories(../../OS/RavlService/include)
include_directories(../../OS/RavlThreads/include)
include_directories(../../Core/RavlCore/include)
include_directories(../../Math/RavlMath/include)
include_directories(../../Math/RavlGeneticProgram/include)




add_library(RavlGeneticOptimisation_Body STATIC
 include/Ravl/Genetic/GeneType.hh include/Ravl/Genetic/Genome.hh include/Ravl/Genetic/GeneticOptimiser.hh include/Ravl/Genetic/GenomeConst.hh include/Ravl/Genetic/GenomeShare.hh include/Ravl/Genetic/GenomeList.hh include/Ravl/Genetic/GenomeClass.hh include/Ravl/Genetic/GenomeMeta.hh include/Ravl/Genetic/GeneTypeWeightedMeta.hh include/Ravl/Genetic/GeneTypeFloatGauss.hh include/Ravl/Genetic/EvaluateFitness.hh include/Ravl/Genetic/EvaluateFitnessFunc.hh include/Ravl/Genetic/GenePalette.hh include/Ravl/Genetic/GeneFactory.hh include/Ravl/Genetic/GeneTypeProxy.hh include/Ravl/Genetic/GeneticOptimiserCheckPoint.hh include/Ravl/Genetic/GeneTypeBool.hh include/Ravl/Genetic/GeneTypeFloatExp.hh include/Ravl/Genetic/GeneTypeClassDirect.hh include/Ravl/Genetic/GeneTypeClassDirectCall.hh include/Ravl/Genetic/GeneTypeArray.hh include/Ravl/Genetic/PopulationState.hh
 src/GeneType.cc src/Genome.cc src/GeneticOptimiser.cc src/GenomeConst.cc src/GenomeShare.cc src/GenomeList.cc src/GenomeClass.cc src/GenomeMeta.cc src/GeneTypeWeightedMeta.cc src/GeneTypeFloatGauss.cc src/Gene2ClassGenerator.cc src/EvaluateFitness.cc src/EvaluateFitnessFunc.cc src/GenePalette.cc src/GeneFactory.cc src/GeneTypeProxy.cc src/GeneticOptimiserCheckPoint.cc src/GeneTypeBool.cc src/GeneTypeFloatExp.cc src/GeneTypeClassDirect.cc src/GeneTypeClassDirectCall.cc src/GeneTypeArray.cc src/PopulationState.cc
)
target_link_libraries(RavlGeneticOptimisation_Body PUBLIC RavlOS RavlIO RavlXMLFactory RavlService RavlThreads RavlCore RavlMath )

add_library(RavlGeneticOptimisation OBJECT src/LinkGeneticOpt.cc )
target_link_libraries(RavlGeneticOptimisation PUBLIC RavlGeneticOptimisation_Body )


target_include_directories(
    RavlGeneticOptimisation PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlGeneticOptimisation_BodyConfig DESTINATION lib/cmake/RavlGeneticOptimisation_Body/Target)
install(TARGETS RavlGeneticOptimisation_Body EXPORT RavlGeneticOptimisation_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlGeneticOptimisation_Body DESTINATION lib)

configure_package_config_file(cmake/RavlGeneticOptimisation_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlGeneticOptimisation_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlGeneticOptimisation_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlGeneticOptimisation_BodyConfig.cmake
        DESTINATION lib/cmake/RavlGeneticOptimisation_Body )


install(TARGETS RavlGeneticOptimisation EXPORT RavlGeneticOptimisationConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlGeneticOptimisationConfig DESTINATION lib/cmake/RavlGeneticOptimisation/Target)
install(TARGETS RavlGeneticOptimisation DESTINATION lib)

configure_package_config_file(cmake/RavlGeneticOptimisationConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlGeneticOptimisationConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlGeneticOptimisation
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlGeneticOptimisationConfig.cmake
        DESTINATION lib/cmake/RavlGeneticOptimisation )

install(DIRECTORY include DESTINATION .)



# Examples
set(EXAMPLE_LIBS  RavlOS RavlIO RavlXMLFactory RavlService RavlThreads RavlCore RavlMath RavlGeneticProgram  )

add_executable(exGeneticOptimisation EXCLUDE_FROM_ALL examples/exGeneticOptimisation.cc )
target_link_libraries(exGeneticOptimisation RavlGeneticOptimisation ${EXAMPLE_LIBS} )
install(TARGETS exGeneticOptimisation DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)

# Tests
set(TEST_LIBS  RavlOS RavlIO RavlXMLFactory RavlService RavlThreads RavlCore RavlMath RavlGeneticProgram  )

add_executable(testGeneticOpt EXCLUDE_FROM_ALL tests/testGeneticOpt.cc )
target_link_libraries(testGeneticOpt RavlGeneticOptimisation ${TEST_LIBS} )
add_test(testGeneticOpt testGeneticOpt)
install(TARGETS testGeneticOpt DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)


# === Files for share/Ravl/Genetic ===
set(AUXDIR share/Ravl/Genetic)
set(AUXFILES 
  data/exGeneticOptimisation.xml )
file(COPY ${AUXFILES} DESTINATION ${BUILD_ROOT_AUXFILE}/${AUXDIR})
install(FILES ${AUXFILES} DESTINATION ${AUXDIR})





