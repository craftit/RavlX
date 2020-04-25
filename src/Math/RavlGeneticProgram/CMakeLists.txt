
# Library: RavlGeneticProgram

# Requires: 
# Supports: 




# MustLink: src/LinkGeneticProgram.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlGeneticProgram 1)

include_directories(../../OS/RavlOS/include)
include_directories(../../Math/RavlGeneticOptimisation/include)
include_directories(../../Core/RavlCore/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../OS/RavlThreads/include)




add_library(RavlGeneticProgram_Body STATIC
 include/Ravl/Genetic/GPInstruction.hh include/Ravl/Genetic/GPVariable.hh include/Ravl/Genetic/GPInstSendSignal.hh include/Ravl/Genetic/GPInstIf.hh include/Ravl/Genetic/GPInstRoutine.hh include/Ravl/Genetic/GPInstLoadConst.hh include/Ravl/Genetic/GPInstAdd.hh include/Ravl/Genetic/GPInstSubtract.hh include/Ravl/Genetic/GPInstAssign.hh include/Ravl/Genetic/GPInstPutArray.hh include/Ravl/Genetic/GPInstGetArray.hh include/Ravl/Genetic/GPInstIncrement.hh include/Ravl/Genetic/GPInstDecrement.hh include/Ravl/Genetic/GPInstLoop.hh include/Ravl/Genetic/GPInstMultiply.hh include/Ravl/Genetic/GPInstDivide.hh
 src/GPInstruction.cc src/GPVariable.cc src/GPInstSendSignal.cc src/GPInstIf.cc src/GPInstRoutine.cc src/GPInstLoadConst.cc src/GPInstAdd.cc src/GPInstSubtract.cc src/GPInstAssign.cc src/GPInstPutArray.cc src/GPInstGetArray.cc src/GPInstIncrement.cc src/GPInstDecrement.cc src/GPInstLoop.cc src/GPInstMultiply.cc src/GPInstDivide.cc
)
target_link_libraries(RavlGeneticProgram_Body PUBLIC RavlOS RavlGeneticOptimisation RavlCore RavlIO RavlThreads )

add_library(RavlGeneticProgram OBJECT src/LinkGeneticProgram.cc )
target_link_libraries(RavlGeneticProgram PUBLIC RavlGeneticProgram_Body )


target_include_directories(
    RavlGeneticProgram PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlGeneticProgram_BodyConfig DESTINATION lib/cmake/RavlGeneticProgram_Body/Target)
install(TARGETS RavlGeneticProgram_Body EXPORT RavlGeneticProgram_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlGeneticProgram_Body DESTINATION lib)

configure_package_config_file(cmake/RavlGeneticProgram_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlGeneticProgram_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlGeneticProgram_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlGeneticProgram_BodyConfig.cmake
        DESTINATION lib/cmake/RavlGeneticProgram_Body )


install(TARGETS RavlGeneticProgram EXPORT RavlGeneticProgramConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlGeneticProgramConfig DESTINATION lib/cmake/RavlGeneticProgram/Target)
install(TARGETS RavlGeneticProgram DESTINATION lib)

configure_package_config_file(cmake/RavlGeneticProgramConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlGeneticProgramConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlGeneticProgram
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlGeneticProgramConfig.cmake
        DESTINATION lib/cmake/RavlGeneticProgram )

install(DIRECTORY include DESTINATION .)










