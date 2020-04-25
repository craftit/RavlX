
# Library: RavlMathIO

# Requires: 
# Supports: 




# MustLink: src/linkMathIO.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlMathIO 1)

include_directories(../../Math/RavlMath/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../OS/RavlXMLFactory/include)
include_directories(../../Core/RavlCore/include)
include_directories(../../OS/RavlThreads/include)




add_library(RavlMathIO_Body STATIC
 include/Ravl/MathIO.hh
 src/FixedVectorIO.cc src/FixedPointIO.cc src/FixedMatrixIO.cc src/BoundryIO.cc src/MatrixIO.cc src/TMatrixIO.cc src/Geom2dIO.cc src/MathIO.cc
)
target_link_libraries(RavlMathIO_Body PUBLIC RavlMath RavlIO RavlXMLFactory RavlCore RavlThreads )

add_library(RavlMathIO OBJECT src/linkMathIO.cc )
target_link_libraries(RavlMathIO PUBLIC RavlMathIO_Body )


target_include_directories(
    RavlMathIO PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlMathIO_BodyConfig DESTINATION lib/cmake/RavlMathIO_Body/Target)
install(TARGETS RavlMathIO_Body EXPORT RavlMathIO_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlMathIO_Body DESTINATION lib)

configure_package_config_file(cmake/RavlMathIO_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlMathIO_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlMathIO_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlMathIO_BodyConfig.cmake
        DESTINATION lib/cmake/RavlMathIO_Body )


install(TARGETS RavlMathIO EXPORT RavlMathIOConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlMathIOConfig DESTINATION lib/cmake/RavlMathIO/Target)
install(TARGETS RavlMathIO DESTINATION lib)

configure_package_config_file(cmake/RavlMathIOConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlMathIOConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlMathIO
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlMathIOConfig.cmake
        DESTINATION lib/cmake/RavlMathIO )

install(DIRECTORY include DESTINATION .)










