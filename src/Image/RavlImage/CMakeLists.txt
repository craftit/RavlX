
# Library: RavlImage

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

set(HAVE_RavlImage 1)

include_directories(../../Core/RavlCore/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../Math/RavlMath/include)
include_directories(../../Image/RavlImageIO/include)
include_directories(../../OS/RavlOS/include)
include_directories(../../Image/RavlImageProc/include)



if(DEFINED HAVE_RavlDPDisplay)
include_directories(../../GUI/RavlDPDisplay/include)
endif()

add_library(RavlImage STATIC include/Ravl/Image/ImageRectangle.hh include/Ravl/Image/Image.hh include/Ravl/Image/RGBValue.hh include/Ravl/Image/ByteRGBValue.hh include/Ravl/Image/ByteRGBColour.hh include/Ravl/Image/UInt16RGBValue.hh include/Ravl/Image/RealRGBValue.hh include/Ravl/Image/RGBAValue.hh include/Ravl/Image/ByteRGBAValue.hh include/Ravl/Image/RealRGBAValue.hh include/Ravl/Image/BGRValue.hh include/Ravl/Image/ByteBGRValue.hh include/Ravl/Image/RealBGRValue.hh include/Ravl/Image/BGRAValue.hh include/Ravl/Image/ByteBGRAValue.hh include/Ravl/Image/YUVValue.hh include/Ravl/Image/ByteYUVValue.hh include/Ravl/Image/RealYUVValue.hh include/Ravl/Image/YUVAValue.hh include/Ravl/Image/ByteYUVAValue.hh include/Ravl/Image/YUV422Value.hh include/Ravl/Image/ByteYUV422Value.hh include/Ravl/Image/VYUValue.hh include/Ravl/Image/ByteVYUValue.hh include/Ravl/Image/RealHSVValue.hh include/Ravl/Image/FixedPointHSVValue.hh include/Ravl/Image/IAValue.hh include/Ravl/Image/ByteIAValue.hh include/Ravl/Image/ByteRGBMedian.hh include/Ravl/Image/RealRGBAverage.hh include/Ravl/Image/ScaleValues.hh include/Ravl/Image/Reflect.hh include/Ravl/Image/RGBcYUV.hh include/Ravl/Image/ImageConv.hh include/Ravl/Image/Font.hh include/Ravl/Image/PSFFont.h include/Ravl/Image/DrawSymbol.hh include/Ravl/Image/DrawFrame.hh include/Ravl/Image/DrawCross.hh include/Ravl/Image/DrawMask.hh include/Ravl/Image/DrawLine.hh include/Ravl/Image/DrawCircle.hh include/Ravl/Image/DrawPolygon.hh include/Ravl/Image/DrawEllipse.hh include/Ravl/Image/BilinearInterpolation.hh include/Ravl/Image/YCbCrBT601Value.hh include/Ravl/Image/ByteYCbCrBT601Value.hh include/Ravl/Image/UInt16YCbCrBT601Value.hh include/Ravl/Image/YCbCr422BT601Value.hh include/Ravl/Image/ByteYCbCr422BT601Value.hh include/Ravl/Image/UInt16YCbCr422BT601Value.hh include/Ravl/Image/YCbCrBT709Value.hh include/Ravl/Image/ByteYCbCrBT709Value.hh include/Ravl/Image/UInt16YCbCrBT709Value.hh include/Ravl/Image/YCbCr422BT709Value.hh include/Ravl/Image/ByteYCbCr422BT709Value.hh include/Ravl/Image/UInt16YCbCr422BT709Value.hh include/Ravl/Image/YPbPrBT601Value.hh include/Ravl/Image/FloatYPbPrBT601Value.hh include/Ravl/Image/YPbPrBT709Value.hh include/Ravl/Image/FloatYPbPrBT709Value.hh include/Ravl/Image/YPbPr422BT601Value.hh include/Ravl/Image/FloatYPbPr422BT601Value.hh include/Ravl/Image/YPbPr422BT709Value.hh include/Ravl/Image/FloatYPbPr422BT709Value.hh src/ImageRectangle.cc src/Image.cc src/ByteRGBValue.cc src/ByteYUVValue.cc src/RealRGBValue.cc src/RealYUVValue.cc src/RGBcYUV.cc src/Font.cc src/ImageConv.cc src/ImageConv2.cc src/ImageConv3.cc src/ImageConv4.cc src/ImageConv5.cc src/ByteRGBAValue.cc src/ByteVYUValue.cc src/ByteYUV422Value.cc src/ByteYUVAValue.cc src/ImgCnvRGB.cc src/YCbCrBT601Value.cc src/ByteYCbCrBT601Value.cc src/UInt16YCbCrBT601Value.cc src/ByteYCbCr422BT601Value.cc src/UInt16YCbCr422BT601Value.cc src/YCbCrBT709Value.cc src/ByteYCbCrBT709Value.cc src/UInt16YCbCrBT709Value.cc src/ByteYCbCr422BT709Value.cc src/UInt16YCbCr422BT709Value.cc src/YPbPrBT601Value.cc src/FloatYPbPrBT601Value.cc src/YPbPrBT709Value.cc src/FloatYPbPrBT709Value.cc src/FloatYPbPr422BT709Value.cc src/FloatYPbPr422BT601Value.cc)
target_link_libraries(RavlImage PUBLIC RavlCore RavlIO RavlMath )


target_include_directories(
    RavlImage PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )



install(TARGETS RavlImage EXPORT RavlImageConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlImageConfig DESTINATION lib/cmake/RavlImage/Target)
install(TARGETS RavlImage DESTINATION lib)

configure_package_config_file(cmake/RavlImageConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlImageConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlImage
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlImageConfig.cmake
        DESTINATION lib/cmake/RavlImage )

install(DIRECTORY include DESTINATION .)


# Mains
set(MAIN_LIBS  RavlCore RavlIO RavlMath RavlImageIO RavlOS RavlImageProc  )

add_executable(imgdiff ${EXCLUDE_APPLICATIONS} applications/imgdiff.cc )
target_link_libraries(imgdiff RavlImage ${MAIN_LIBS} )
install(TARGETS imgdiff DESTINATION bin ${EXCLUDE_APPLICATIONS})

# Examples
set(EXAMPLE_LIBS  RavlCore RavlIO RavlMath RavlImageIO RavlOS RavlImageProc  )

add_executable(exImage ${EXCLUDE_EXAMPLES} examples/exImage.cc )
target_link_libraries(exImage RavlImage ${EXAMPLE_LIBS} )
install(TARGETS exImage DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exDraw ${EXCLUDE_EXAMPLES} examples/exDraw.cc )
target_link_libraries(exDraw RavlImage ${EXAMPLE_LIBS} )
install(TARGETS exDraw DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exFont ${EXCLUDE_EXAMPLES} examples/exFont.cc )
target_link_libraries(exFont RavlImage ${EXAMPLE_LIBS} )
install(TARGETS exFont DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

# Tests
set(TEST_LIBS  RavlCore RavlIO RavlMath RavlImageIO RavlOS RavlImageProc  )

add_executable(testImage ${EXCLUDE_UNITTESTS} tests/testImage.cc )
target_link_libraries(testImage RavlImage ${TEST_LIBS} )
add_test(testImage testImage)
install(TARGETS testImage DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})

add_executable(testHSVValue ${EXCLUDE_UNITTESTS} tests/testHSVValue.cc )
target_link_libraries(testHSVValue RavlImage ${TEST_LIBS} )
add_test(testHSVValue testHSVValue)
install(TARGETS testHSVValue DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})

add_executable(test_imgdiff ${EXCLUDE_UNITTESTS} tests/test_imgdiff.cc )
target_link_libraries(test_imgdiff RavlImage ${TEST_LIBS} )
add_test(test_imgdiff test_imgdiff)
install(TARGETS test_imgdiff DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})


# === Files for share/RAVL/Fonts ===
set(AUXDIR share/RAVL/Fonts)
set(AUXFILES 
  data/default8x16.psf )
file(COPY ${AUXFILES} DESTINATION ${BUILD_ROOT_AUXFILE}/${AUXDIR})
install(FILES ${AUXFILES} DESTINATION ${AUXDIR})





