
# Library: RavlImgIOV4L

# Requires: devVideo4Linux
# Supports: linux linux64

if((${CMAKE_SYSTEM_NAME} MATCHES "Linux32") OR (${CMAKE_SYSTEM_NAME} MATCHES "Linux") OR FALSE)

if(FALSE)

# MustLink: src/InitV4L.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlImgIOV4L 1)

include_directories(../../Image/RavlImageIO/include)
include_directories(../../Image/RavlVideoIO/include)
include_directories(../../Core/RavlCore/include)
include_directories(../../Image/RavlImage/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../OS/RavlOSIO/include)



if(DEFINED HAVE_RavlDPDisplay)
include_directories(../../GUI/RavlDPDisplay/include)
endif()
if(DEFINED HAVE_RavlExtImgIO)
include_directories(../../Image/RavlExtImgIO/include)
endif()

add_library(RavlImgIOV4L_Body STATIC
 include/Ravl/Image/ImgIOV4L.hh include/Ravl/Image/V4LFormat.hh
 src/ImgIOV4L.cc src/ImgIOV4L1.cc src/V4LFormat.cc src/ImgIOV4LAux.cc
)
target_link_libraries(RavlImgIOV4L_Body PUBLIC RavlImageIO RavlVideoIO RavlCore RavlImage RavlIO )

add_library(RavlImgIOV4L OBJECT src/InitV4L.cc )
target_link_libraries(RavlImgIOV4L PUBLIC RavlImgIOV4L_Body )


target_include_directories(
    RavlImgIOV4L PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlImgIOV4L_BodyConfig DESTINATION lib/cmake/RavlImgIOV4L_Body/Target)
install(TARGETS RavlImgIOV4L_Body EXPORT RavlImgIOV4L_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlImgIOV4L_Body DESTINATION lib)

configure_package_config_file(cmake/RavlImgIOV4L_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlImgIOV4L_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlImgIOV4L_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlImgIOV4L_BodyConfig.cmake
        DESTINATION lib/cmake/RavlImgIOV4L_Body )


install(TARGETS RavlImgIOV4L EXPORT RavlImgIOV4LConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlImgIOV4LConfig DESTINATION lib/cmake/RavlImgIOV4L/Target)
install(TARGETS RavlImgIOV4L DESTINATION lib)

configure_package_config_file(cmake/RavlImgIOV4LConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlImgIOV4LConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlImgIOV4L
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlImgIOV4LConfig.cmake
        DESTINATION lib/cmake/RavlImgIOV4L )

install(DIRECTORY include DESTINATION .)


# Mains
set(MAIN_LIBS  RavlImageIO RavlVideoIO RavlCore RavlImage RavlIO RavlOSIO  )

add_executable(testV4LIO ${EXCLUDE_APPLICATIONS} applications/testV4LIO.cc )
target_link_libraries(testV4LIO RavlImgIOV4L ${MAIN_LIBS} )
install(TARGETS testV4LIO DESTINATION bin ${EXCLUDE_APPLICATIONS})






ENDIF()

ENDIF()
