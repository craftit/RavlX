
# Library: RavlVideoIO

# Requires: 
# Supports: 




# MustLink: src/linkRavlVidIO.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlVideoIO 1)

include_directories(../../Image/RavlImageIO/include)
include_directories(../../OS/RavlOSIO/include)
include_directories(../../Image/RavlImage/include)
include_directories(../../Core/RavlCore/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../OS/RavlOS/include)



if(DEFINED HAVE_RavlDPDisplay)
include_directories(../../GUI/RavlDPDisplay/include)
endif()

add_library(RavlVideoIO_Body STATIC
 include/Ravl/Image/ImgIOCif.hh include/Ravl/Image/CifFormat.hh include/Ravl/Image/syuvFormat.hh include/Ravl/Image/ImgIOrgb.hh include/Ravl/Image/rgbFormat.hh include/Ravl/Image/rawFormat.hh include/Ravl/Image/ImgIOjs.hh include/Ravl/Image/jsFormat.hh include/Ravl/Image/VidIO.hh include/Ravl/Image/MultiVidIO.hh include/Ravl/Image/DVFrame.hh include/Ravl/Image/AviStream.hh include/Ravl/Image/BGRFrmIOAvi.hh include/Ravl/Image/DVFrmIOAvi.hh include/Ravl/Image/AviFormat.hh
 src/ImgIOCif.cc src/CifFormat.cc src/syuvFormat.cc src/ImgIOrgb.cc src/rgbFormat.cc src/rawFormat.cc src/ImgIOjs.cc src/jsFormat.cc src/MultiVidIO.cc src/DVFrame.cc src/AviIStream.cc src/AviOStream.cc src/BGRFrmIOAvi.cc src/DVFrmIOAvi.cc src/AviFormat.cc src/RavlVidIO.cc
)
target_link_libraries(RavlVideoIO_Body PUBLIC RavlImageIO RavlOSIO RavlImage RavlCore RavlIO RavlOS )

add_library(RavlVideoIO OBJECT src/linkRavlVidIO.cc )
target_link_libraries(RavlVideoIO PUBLIC RavlVideoIO_Body )


target_include_directories(
    RavlVideoIO PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlVideoIO_BodyConfig DESTINATION lib/cmake/RavlVideoIO_Body/Target)
install(TARGETS RavlVideoIO_Body EXPORT RavlVideoIO_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlVideoIO_Body DESTINATION lib)

configure_package_config_file(cmake/RavlVideoIO_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlVideoIO_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlVideoIO_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlVideoIO_BodyConfig.cmake
        DESTINATION lib/cmake/RavlVideoIO_Body )


install(TARGETS RavlVideoIO EXPORT RavlVideoIOConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlVideoIOConfig DESTINATION lib/cmake/RavlVideoIO/Target)
install(TARGETS RavlVideoIO DESTINATION lib)

configure_package_config_file(cmake/RavlVideoIOConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlVideoIOConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlVideoIO
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlVideoIOConfig.cmake
        DESTINATION lib/cmake/RavlVideoIO )

install(DIRECTORY include DESTINATION .)



# Examples
set(EXAMPLE_LIBS  RavlImageIO RavlOSIO RavlImage RavlCore RavlIO RavlOS  )

add_executable(exImgSeq EXCLUDE_FROM_ALL examples/exImgSeq.cc )
target_link_libraries(exImgSeq RavlVideoIO ${EXAMPLE_LIBS} )
install(TARGETS exImgSeq DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)







