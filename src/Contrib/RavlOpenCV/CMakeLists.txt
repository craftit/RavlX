
# Library: RavlOpenCV

# Requires: OpenCV
# Supports: !VCPP

if((NOT ${CMAKE_CXX_COMPILER_ID} MATCHES "MSVC") OR FALSE)

if(opencv_FOUND)

# MustLink: src/LinkOpenCV.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlOpenCV 1)

include_directories(../../Image/RavlImage/include)
include_directories(../../Core/RavlCore/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../OS/RavlOS/include)
include_directories(../../Image/RavlImageIO/include)



if(DEFINED HAVE_RavlDPDisplay)
include_directories(../../GUI/RavlDPDisplay/include)
endif()

add_library(RavlOpenCV_Body STATIC
 include/Ravl/Image/OpenCVConvert.hh include/Ravl/Image/ImgIOOpenCV.hh include/Ravl/Image/ImgIOOpenCVFormat.hh
 src/OpenCVConvert.cc src/ImgIOOpenCV.cc src/ImgIOOpenCVFormat.cc
)
target_link_libraries(RavlOpenCV_Body PUBLIC RavlImage ${opencv_LIBRARIES} RavlCore RavlIO RavlOS )

add_library(RavlOpenCV OBJECT src/LinkOpenCV.cc )
target_link_libraries(RavlOpenCV PUBLIC RavlOpenCV_Body )


target_include_directories(
    RavlOpenCV PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlOpenCV_BodyConfig DESTINATION lib/cmake/RavlOpenCV_Body/Target)
install(TARGETS RavlOpenCV_Body EXPORT RavlOpenCV_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlOpenCV_Body DESTINATION lib)

configure_package_config_file(cmake/RavlOpenCV_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlOpenCV_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlOpenCV_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlOpenCV_BodyConfig.cmake
        DESTINATION lib/cmake/RavlOpenCV_Body )


install(TARGETS RavlOpenCV EXPORT RavlOpenCVConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlOpenCVConfig DESTINATION lib/cmake/RavlOpenCV/Target)
install(TARGETS RavlOpenCV DESTINATION lib)

configure_package_config_file(cmake/RavlOpenCVConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlOpenCVConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlOpenCV
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlOpenCVConfig.cmake
        DESTINATION lib/cmake/RavlOpenCV )

install(DIRECTORY include DESTINATION .)


# Mains
set(MAIN_LIBS  RavlImage ${opencv_LIBRARIES} RavlCore RavlIO RavlOS RavlImageIO  )

add_executable(exOpenCV applications/exOpenCV.cc )
target_link_libraries(exOpenCV RavlOpenCV ${MAIN_LIBS} )
install(TARGETS exOpenCV DESTINATION bin)


# Tests
set(TEST_LIBS  RavlImage ${opencv_LIBRARIES} RavlCore RavlIO RavlOS RavlImageIO  )

add_executable(testOpenCV EXCLUDE_FROM_ALL tests/testOpenCV.cc )
target_link_libraries(testOpenCV RavlOpenCV ${TEST_LIBS} )
add_test(testOpenCV testOpenCV)
install(TARGETS testOpenCV DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)




ENDIF()

ENDIF()
