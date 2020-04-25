
# Library: RavlExtImgIO

# Requires: 
# Supports: !cygwin !arm

if((NOT ${CMAKE_SYSTEM_NAME} STREQUAL "CYGWIN") OR (NOT ${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm") OR FALSE)


# MustLink: src/ExtImgIO.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlExtImgIO 1)

include_directories(../../Image/RavlImage/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../Image/RavlImageIO/include)
include_directories(../../Core/RavlCore/include)



if(DEFINED HAVE_RavlDPDisplay)
include_directories(../../GUI/RavlDPDisplay/include)
endif()

add_library(RavlExtImgIO_Body STATIC
 include/Ravl/Image/ExtImgIO.hh include/Ravl/Image/ImgIOJPeg.hh include/Ravl/Image/ImgIOJPegB.hh include/Ravl/Image/JPEGFormat.hh include/Ravl/Image/CompressedImageJPEG.hh include/Ravl/Image/ImgIOPNG.hh include/Ravl/Image/ImgIOPNGB.hh include/Ravl/Image/PNGFormat.hh include/Ravl/Image/ImgIOTiff.hh include/Ravl/Image/ImgIOTiffB.hh include/Ravl/Image/TiffFormat.hh
 src/Dummy.cc src/ImgIOJPeg.cc src/JPEGFormat.cc src/CompressedImageJPEG.cc src/ImgIOPNG.cc src/PNGFormat.cc src/ImgIOTiff.cc src/TiffFormat.cc
)
target_link_libraries(RavlExtImgIO_Body PUBLIC RavlImage RavlIO RavlImageIO ${PNG_LIBRARIES} ${JPEG_LIBRARIES} ${TIFF_LIBRARIES}  RavlCore )

add_library(RavlExtImgIO OBJECT src/ExtImgIO.cc )
target_link_libraries(RavlExtImgIO PUBLIC RavlExtImgIO_Body )


target_include_directories(
    RavlExtImgIO PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlExtImgIO_BodyConfig DESTINATION lib/cmake/RavlExtImgIO_Body/Target)
install(TARGETS RavlExtImgIO_Body EXPORT RavlExtImgIO_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlExtImgIO_Body DESTINATION lib)

configure_package_config_file(cmake/RavlExtImgIO_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlExtImgIO_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlExtImgIO_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlExtImgIO_BodyConfig.cmake
        DESTINATION lib/cmake/RavlExtImgIO_Body )


install(TARGETS RavlExtImgIO EXPORT RavlExtImgIOConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlExtImgIOConfig DESTINATION lib/cmake/RavlExtImgIO/Target)
install(TARGETS RavlExtImgIO DESTINATION lib)

configure_package_config_file(cmake/RavlExtImgIOConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlExtImgIOConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlExtImgIO
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlExtImgIOConfig.cmake
        DESTINATION lib/cmake/RavlExtImgIO )

install(DIRECTORY include DESTINATION .)



# Examples
set(EXAMPLE_LIBS  RavlImage RavlIO RavlImageIO ${PNG_LIBRARIES} ${JPEG_LIBRARIES} ${TIFF_LIBRARIES}  RavlCore  )

add_executable(exExtImgIO EXCLUDE_FROM_ALL examples/exExtImgIO.cc )
target_link_libraries(exExtImgIO RavlExtImgIO ${EXAMPLE_LIBS} )
install(TARGETS exExtImgIO DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)

add_executable(exImgMemIO EXCLUDE_FROM_ALL examples/exImgMemIO.cc )
target_link_libraries(exImgMemIO RavlExtImgIO ${EXAMPLE_LIBS} )
install(TARGETS exImgMemIO DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)

# Tests
set(TEST_LIBS  RavlImage RavlIO RavlImageIO ${PNG_LIBRARIES} ${JPEG_LIBRARIES} ${TIFF_LIBRARIES}  RavlCore  )

add_executable(testExtImgIO EXCLUDE_FROM_ALL tests/testExtImgIO.cc )
target_link_libraries(testExtImgIO RavlExtImgIO ${TEST_LIBS} )
add_test(testExtImgIO testExtImgIO)
install(TARGETS testExtImgIO DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)





ENDIF()
