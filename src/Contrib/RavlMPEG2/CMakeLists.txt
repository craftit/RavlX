
# Library: RavlMPEG2

# Requires: 
# Supports: 




# MustLink: src/MustLinkMPEG2.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlMPEG2 1)

include_directories(../../Image/RavlVideoIO/include)
include_directories(../../OS/RavlOS/include)
include_directories(../../Image/RavlImage/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../Core/RavlCore/include)
include_directories(../../Image/RavlImageIO/include)



if(DEFINED HAVE_RavlDPDisplay)
include_directories(../../GUI/RavlDPDisplay/include)
endif()

add_library(RavlMPEG2_Body STATIC
 include/Ravl/Image/ImgIOmpeg2dec.hh include/Ravl/Image/MPEG2Format.hh
 src/ImgIOmpeg2dec.cc src/MPEG2Format.cc
)
target_link_libraries(RavlMPEG2_Body PUBLIC RavlVideoIO RavlOS RavlImage RavlIO RavlCore RavlImageIO )

add_library(RavlMPEG2 OBJECT src/MustLinkMPEG2.cc )
target_link_libraries(RavlMPEG2 PUBLIC RavlMPEG2_Body )


target_include_directories(
    RavlMPEG2 PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlMPEG2_BodyConfig DESTINATION lib/cmake/RavlMPEG2_Body/Target)
install(TARGETS RavlMPEG2_Body EXPORT RavlMPEG2_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlMPEG2_Body DESTINATION lib)

configure_package_config_file(cmake/RavlMPEG2_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlMPEG2_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlMPEG2_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlMPEG2_BodyConfig.cmake
        DESTINATION lib/cmake/RavlMPEG2_Body )


install(TARGETS RavlMPEG2 EXPORT RavlMPEG2Config
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlMPEG2Config DESTINATION lib/cmake/RavlMPEG2/Target)
install(TARGETS RavlMPEG2 DESTINATION lib)

configure_package_config_file(cmake/RavlMPEG2Config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlMPEG2Config.cmake
  INSTALL_DESTINATION lib/cmake/RavlMPEG2
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlMPEG2Config.cmake
        DESTINATION lib/cmake/RavlMPEG2 )

install(DIRECTORY include DESTINATION .)










