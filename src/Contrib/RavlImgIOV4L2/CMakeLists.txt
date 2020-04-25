
# Library: RavlImgIOV4L2

# Requires: devVideo4Linux2
# Supports: linux linux64

if((${CMAKE_SYSTEM_NAME} MATCHES "Linux32") OR (${CMAKE_SYSTEM_NAME} MATCHES "Linux") OR FALSE)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")

# MustLink: src/MustLinkV4L2.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlImgIOV4L2 1)

include_directories(../../Image/RavlImage/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../OS/RavlThreads/include)
include_directories(../../Core/RavlCore/include)
include_directories(../../OS/RavlOSIO/include)



if(DEFINED HAVE_RavlDPDisplay)
include_directories(../../GUI/RavlDPDisplay/include)
endif()

add_library(RavlImgIOV4L2_Body STATIC
 include/Ravl/Image/V4L2Format.hh include/Ravl/Image/ImgIOV4L2.hh include/Ravl/Image/V4L2Buffer.hh
 src/V4L2Format.cc src/ImgIOV4L2.cc
)
target_link_libraries(RavlImgIOV4L2_Body PUBLIC RavlImage RavlIO RavlThreads RavlCore )

add_library(RavlImgIOV4L2 OBJECT src/MustLinkV4L2.cc )
target_link_libraries(RavlImgIOV4L2 PUBLIC RavlImgIOV4L2_Body )


target_include_directories(
    RavlImgIOV4L2 PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlImgIOV4L2_BodyConfig DESTINATION lib/cmake/RavlImgIOV4L2_Body/Target)
install(TARGETS RavlImgIOV4L2_Body EXPORT RavlImgIOV4L2_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlImgIOV4L2_Body DESTINATION lib)

configure_package_config_file(cmake/RavlImgIOV4L2_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlImgIOV4L2_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlImgIOV4L2_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlImgIOV4L2_BodyConfig.cmake
        DESTINATION lib/cmake/RavlImgIOV4L2_Body )


install(TARGETS RavlImgIOV4L2 EXPORT RavlImgIOV4L2Config
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlImgIOV4L2Config DESTINATION lib/cmake/RavlImgIOV4L2/Target)
install(TARGETS RavlImgIOV4L2 DESTINATION lib)

configure_package_config_file(cmake/RavlImgIOV4L2Config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlImgIOV4L2Config.cmake
  INSTALL_DESTINATION lib/cmake/RavlImgIOV4L2
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlImgIOV4L2Config.cmake
        DESTINATION lib/cmake/RavlImgIOV4L2 )

install(DIRECTORY include DESTINATION .)








ENDIF()

ENDIF()
