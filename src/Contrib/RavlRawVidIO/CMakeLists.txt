
# Library: RavlRawVidIO

# Requires: 
# Supports: !VCPP

if((NOT ${CMAKE_CXX_COMPILER_ID} MATCHES "MSVC") OR FALSE)


# MustLink: src/RavlRawVidIO.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlRawVidIO 1)

include_directories(../../Image/RavlImageIO/include)
include_directories(../../OS/RavlOSIO/include)
include_directories(../../Image/RavlImage/include)
include_directories(../../Image/RavlExtImgIO/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../Core/RavlCore/include)
include_directories(../../Audio/RavlAudioUtil/include)
include_directories(../../OS/RavlXMLFactory/include)
include_directories(../../OS/RavlThreads/include)
include_directories(../../Image/RavlImageProc/include)
include_directories(../../Math/RavlMath/include)
include_directories(../../OS/RavlOS/include)




add_library(RavlRawVidIO_Body STATIC
 include/Ravl/Image/ImgIOdvsypbpr.hh include/Ravl/Image/ImgIOdvsrgb.hh include/Ravl/Image/GrabfileCommon.hh include/Ravl/Image/GrabfileReader.hh include/Ravl/Image/GrabfileReaderV1.hh include/Ravl/Image/GrabfileWriter.hh include/Ravl/Image/GrabfileWriterV1.hh include/Ravl/Image/LegacyGrabfileReader.hh include/Ravl/Image/NewGrabfileReader.hh include/Ravl/Image/NewGrabfileWriter.hh include/Ravl/Image/dvsFormat.hh include/Ravl/Image/Utilities.hh
 src/ImgIOdvsypbpr.cc src/ImgIOdvsrgb.cc src/dvsFormat.cc src/GrabfileReader.cc src/GrabfileReaderV1.cc src/GrabfileWriterV1.cc src/LegacyGrabfileReader.cc src/NewGrabfileReader.cc src/NewGrabfileWriter.cc src/Utilities.cc
)
target_link_libraries(RavlRawVidIO_Body PUBLIC RavlImageIO RavlOSIO RavlImage RavlExtImgIO RavlIO RavlCore RavlAudioUtil RavlXMLFactory RavlThreads RavlImageProc RavlMath RavlOS )

add_library(RavlRawVidIO OBJECT src/RavlRawVidIO.cc )
target_link_libraries(RavlRawVidIO PUBLIC RavlRawVidIO_Body )


target_include_directories(
    RavlRawVidIO PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlRawVidIO_BodyConfig DESTINATION lib/cmake/RavlRawVidIO_Body/Target)
install(TARGETS RavlRawVidIO_Body EXPORT RavlRawVidIO_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlRawVidIO_Body DESTINATION lib)

configure_package_config_file(cmake/RavlRawVidIO_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlRawVidIO_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlRawVidIO_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlRawVidIO_BodyConfig.cmake
        DESTINATION lib/cmake/RavlRawVidIO_Body )


install(TARGETS RavlRawVidIO EXPORT RavlRawVidIOConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlRawVidIOConfig DESTINATION lib/cmake/RavlRawVidIO/Target)
install(TARGETS RavlRawVidIO DESTINATION lib)

configure_package_config_file(cmake/RavlRawVidIOConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlRawVidIOConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlRawVidIO
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlRawVidIOConfig.cmake
        DESTINATION lib/cmake/RavlRawVidIO )

install(DIRECTORY include DESTINATION .)









ENDIF()
