
# Library: RavlImgIOuEye

# Requires: uEyeSDK
# Supports: linux linux64

if((${CMAKE_SYSTEM_NAME} MATCHES "Linux32") OR (${CMAKE_SYSTEM_NAME} MATCHES "Linux") OR FALSE)

if(uEyeSDK_FOUND)

# MustLink: src/linkRavlImgIOuEye.cc


set(DEPENDS_ON uEye )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlImgIOuEye 1)

include_directories(../../Image/RavlImageIO/include)
include_directories(../../OS/RavlThreads/include)
include_directories(../../OS/RavlOS/include)
include_directories(../../Core/RavlCore/include)




add_library(RavlImgIOuEye_Body STATIC
 include/Ravl/Image/ImgIOuEye.hh include/Ravl/Image/uEyeFileFormat.hh
 src/ImgIOuEye.cc src/uEyeFileFormat.cc
)
target_link_libraries(RavlImgIOuEye_Body PUBLIC RavlImageIO RavlThreads uEye RavlOS RavlCore )

add_library(RavlImgIOuEye OBJECT src/linkRavlImgIOuEye.cc )
target_link_libraries(RavlImgIOuEye PUBLIC RavlImgIOuEye_Body )


target_include_directories(
    RavlImgIOuEye PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlImgIOuEye_BodyConfig DESTINATION lib/cmake/RavlImgIOuEye_Body/Target)
install(TARGETS RavlImgIOuEye_Body EXPORT RavlImgIOuEye_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlImgIOuEye_Body DESTINATION lib)

configure_package_config_file(cmake/RavlImgIOuEye_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlImgIOuEye_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlImgIOuEye_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlImgIOuEye_BodyConfig.cmake
        DESTINATION lib/cmake/RavlImgIOuEye_Body )


install(TARGETS RavlImgIOuEye EXPORT RavlImgIOuEyeConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlImgIOuEyeConfig DESTINATION lib/cmake/RavlImgIOuEye/Target)
install(TARGETS RavlImgIOuEye DESTINATION lib)

configure_package_config_file(cmake/RavlImgIOuEyeConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlImgIOuEyeConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlImgIOuEye
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlImgIOuEyeConfig.cmake
        DESTINATION lib/cmake/RavlImgIOuEye )

install(DIRECTORY include DESTINATION .)








ENDIF()

ENDIF()
