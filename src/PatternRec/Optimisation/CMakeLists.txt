
# Library: Optimisation

# Requires: 
# Supports: 






set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_Optimisation 1)

include_directories(../../Core/RavlCore/include)
include_directories(../../PatternRec/RavlDataSet/include)
include_directories(../../Plot/RavlPlot/include)
include_directories(../../Math/RavlMath/include)
include_directories(../../OS/RavlXMLFactory/include)
include_directories(../../OS/RavlThreads/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../Image/RavlImage/include)
include_directories(../../OS/RavlOS/include)
include_directories(../../PatternRec/RavlPatternRec/include)



if(DEFINED HAVE_RavlGnuPlot)
include_directories(../../Contrib/RavlGnuPlot/include)
endif()

add_library(Optimisation STATIC include/Ravl/PatternRec/Parameters.hh include/Ravl/PatternRec/Cost.hh include/Ravl/PatternRec/CostInvert.hh include/Ravl/PatternRec/CostFunction.hh include/Ravl/PatternRec/CostFunction1d.hh include/Ravl/PatternRec/BracketMinimum.hh include/Ravl/PatternRec/Optimise.hh include/Ravl/PatternRec/OptimiseDescent.hh include/Ravl/PatternRec/OptimiseConjugateGradient.hh include/Ravl/PatternRec/OptimiseRandomUniform.hh include/Ravl/PatternRec/OptimiseSobol.hh include/Ravl/PatternRec/OptimiseBrent.hh include/Ravl/PatternRec/OptimisePowell.hh include/Ravl/PatternRec/OptimiseGrid.hh include/Ravl/PatternRec/OptimiseLevenbergMarquardt.hh include/Ravl/PatternRec/OptimiseParticleSwarm.hh src/Parameters.cc src/Cost.cc src/CostInvert.cc src/CostFunction.cc src/CostFunction1d.cc src/BracketMinimum.cc src/Optimise.cc src/OptimiseDescent.cc src/OptimiseConjugateGradient.cc src/OptimiseRandomUniform.cc src/OptimiseSobol.cc src/OptimiseBrent.cc src/OptimisePowell.cc src/OptimiseGrid.cc src/OptimiseLevenbergMarquardt.cc src/OptimiseParticleSwarm.cc)
target_link_libraries(Optimisation PUBLIC RavlCore RavlDataSet RavlPlot RavlMath RavlXMLFactory RavlThreads RavlIO RavlImage RavlOS )


target_include_directories(
    Optimisation PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )



install(TARGETS Optimisation EXPORT OptimisationConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT OptimisationConfig DESTINATION lib/cmake/Optimisation/Target)
install(TARGETS Optimisation DESTINATION lib)

configure_package_config_file(cmake/OptimisationConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/OptimisationConfig.cmake
  INSTALL_DESTINATION lib/cmake/Optimisation
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/OptimisationConfig.cmake
        DESTINATION lib/cmake/Optimisation )

install(DIRECTORY include DESTINATION .)


# Mains
set(MAIN_LIBS  RavlCore RavlDataSet RavlPlot RavlMath RavlXMLFactory RavlThreads RavlIO RavlImage RavlOS RavlPatternRec  )

add_executable(exOptimise applications/exOptimise.cc )
target_link_libraries(exOptimise Optimisation ${MAIN_LIBS} )
install(TARGETS exOptimise DESTINATION bin)

add_executable(exOptimiseParticleSwarm applications/exOptimiseParticleSwarm.cc )
target_link_libraries(exOptimiseParticleSwarm Optimisation ${MAIN_LIBS} )
install(TARGETS exOptimiseParticleSwarm DESTINATION bin)


# Tests
set(TEST_LIBS  RavlCore RavlDataSet RavlPlot RavlMath RavlXMLFactory RavlThreads RavlIO RavlImage RavlOS RavlPatternRec  )

add_executable(testBrent EXCLUDE_FROM_ALL tests/testBrent.cc )
target_link_libraries(testBrent Optimisation ${TEST_LIBS} )
add_test(testBrent testBrent)
install(TARGETS testBrent DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)






