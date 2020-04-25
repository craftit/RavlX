
# Library: RavlPatternRecIO

# Requires: 
# Supports: 




# MustLink: src/RavlPatternRecIO.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlPatternRecIO 1)

include_directories(../../Core/RavlCore/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../PatternRec/RavlPatternRec/include)
include_directories(../../PatternRec/RavlDataSet/include)
include_directories(../../Math/RavlMath/include)
include_directories(../../OS/RavlXMLFactory/include)
include_directories(../../OS/RavlThreads/include)




add_library(RavlPatternRecIO_Body STATIC
 
 src/SampleIO.cc src/DataSetIO.cc src/ClassifierIO.cc src/Classifier2IO.cc src/FunctionIO.cc src/GaussianMixtureIO.cc src/DistanceIO.cc
)
target_link_libraries(RavlPatternRecIO_Body PUBLIC RavlCore RavlIO RavlPatternRec RavlDataSet RavlMath RavlXMLFactory RavlThreads )

add_library(RavlPatternRecIO OBJECT src/RavlPatternRecIO.cc )
target_link_libraries(RavlPatternRecIO PUBLIC RavlPatternRecIO_Body )


target_include_directories(
    RavlPatternRecIO PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlPatternRecIO_BodyConfig DESTINATION lib/cmake/RavlPatternRecIO_Body/Target)
install(TARGETS RavlPatternRecIO_Body EXPORT RavlPatternRecIO_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlPatternRecIO_Body DESTINATION lib)

configure_package_config_file(cmake/RavlPatternRecIO_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlPatternRecIO_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlPatternRecIO_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlPatternRecIO_BodyConfig.cmake
        DESTINATION lib/cmake/RavlPatternRecIO_Body )


install(TARGETS RavlPatternRecIO EXPORT RavlPatternRecIOConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlPatternRecIOConfig DESTINATION lib/cmake/RavlPatternRecIO/Target)
install(TARGETS RavlPatternRecIO DESTINATION lib)

configure_package_config_file(cmake/RavlPatternRecIOConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlPatternRecIOConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlPatternRecIO
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlPatternRecIOConfig.cmake
        DESTINATION lib/cmake/RavlPatternRecIO )

install(DIRECTORY include DESTINATION .)










