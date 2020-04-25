
# Library: RavlImageIO

# Requires: 
# Supports: 




# MustLink: src/RavlImgIO.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlImageIO 1)

include_directories(../../Core/RavlCore/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../Image/RavlImage/include)
include_directories(../../OS/RavlOS/include)




add_library(RavlImageIO_Body STATIC
 include/Ravl/Image/ImgIOPNMB.hh include/Ravl/Image/ImgIOPNM.hh include/Ravl/Image/ImgIOYUV.hh include/Ravl/Image/PNMFormat.hh include/Ravl/Image/YUVFormat.hh include/Ravl/Image/ImgIO.hh
 src/ImgIOPNM.cc src/ImgIOYUV.cc src/PNMFormat.cc src/YUVFormat.cc src/ImgIOInt.cc src/ImgIOByte.cc src/ImgIOUInt16.cc src/ImgIOByteRGB.cc src/ImgIOByteYUV.cc src/ImgIOReal.cc src/ImgIORealRGB.cc src/ImgIOByteRGBA.cc src/ImgIORealRGBA.cc src/ImgIORealYUV.cc src/ImgIOUInt16RGB.cc src/ImgIOByteYUVA.cc src/ImgIOFloat.cc src/ImgIOByteYCbCrBT601Value.cc src/ImgIOUInt16YCbCrBT601Value.cc src/ImgIOYCbCrBT601ValueFloat.cc src/ImgIOByteYCbCr422BT601Value.cc src/ImgIOUInt16YCbCr422BT601Value.cc src/ImgTypeCnv.cc src/ImgTypeCnv2.cc src/ImgTypeCnv3.cc src/ImgTypeCnv4.cc src/ImgIOByteYUV422.cc src/ImgIOByteVYU.cc src/ImgTypeCnv5.cc src/ImgTypeCnv6.cc src/ImgIOByteYCbCrBT709Value.cc src/ImgIOUInt16YCbCrBT709Value.cc src/ImgIOYCbCrBT709ValueFloat.cc src/ImgIOByteYCbCr422BT709Value.cc src/ImgIOUInt16YCbCr422BT709Value.cc src/ImgIOYPbPrBT709ValueFloat.cc src/ImgIOFloatYPbPr422BT709Value.cc src/ImgIOInit.cc src/ImgTypeCnv7.cc
)
target_link_libraries(RavlImageIO_Body PUBLIC RavlCore RavlIO RavlImage )

add_library(RavlImageIO OBJECT src/RavlImgIO.cc )
target_link_libraries(RavlImageIO PUBLIC RavlImageIO_Body )


target_include_directories(
    RavlImageIO PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlImageIO_BodyConfig DESTINATION lib/cmake/RavlImageIO_Body/Target)
install(TARGETS RavlImageIO_Body EXPORT RavlImageIO_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlImageIO_Body DESTINATION lib)

configure_package_config_file(cmake/RavlImageIO_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlImageIO_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlImageIO_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlImageIO_BodyConfig.cmake
        DESTINATION lib/cmake/RavlImageIO_Body )


install(TARGETS RavlImageIO EXPORT RavlImageIOConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlImageIOConfig DESTINATION lib/cmake/RavlImageIO/Target)
install(TARGETS RavlImageIO DESTINATION lib)

configure_package_config_file(cmake/RavlImageIOConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlImageIOConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlImageIO
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlImageIOConfig.cmake
        DESTINATION lib/cmake/RavlImageIO )

install(DIRECTORY include DESTINATION .)



# Examples
set(EXAMPLE_LIBS  RavlCore RavlIO RavlImage RavlOS  )

add_executable(exImgIO ${EXCLUDE_EXAMPLES} examples/exImgIO.cc )
target_link_libraries(exImgIO RavlImageIO ${EXAMPLE_LIBS} )
install(TARGETS exImgIO DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

# Tests
set(TEST_LIBS  RavlCore RavlIO RavlImage RavlOS  )

add_executable(testImgIO ${EXCLUDE_UNITTESTS} tests/testImgIO.cc )
target_link_libraries(testImgIO RavlImageIO ${TEST_LIBS} )
add_test(testImgIO testImgIO)
install(TARGETS testImgIO DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})


# === Files for share/RAVL/pixmaps ===
set(AUXDIR share/RAVL/pixmaps)
set(AUXFILES 
  data/monkey.ppm )
file(COPY ${AUXFILES} DESTINATION ${BUILD_ROOT_AUXFILE}/${AUXDIR})
install(FILES ${AUXFILES} DESTINATION ${AUXDIR})





