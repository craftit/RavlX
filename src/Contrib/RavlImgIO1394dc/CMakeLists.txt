
# Library: RavlImgIO1394dc

# Requires: libdc1394
# Supports: !VCPP

if((NOT ${CMAKE_CXX_COMPILER_ID} MATCHES "MSVC") OR FALSE)

if(libdc1394_FOUND)

# MustLink: src/InitLib1394dc.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlImgIO1394dc 1)

include_directories(../../Core/RavlCore/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../Image/RavlImage/include)
include_directories(../../OS/RavlOSIO/include)



if(DEFINED HAVE_RavlDPDisplay)
include_directories(../../GUI/RavlDPDisplay/include)
endif()

add_library(RavlImgIO1394dc_Body STATIC
 include/Ravl/Image/ImgIO1394dc.hh include/Ravl/Image/Lib1394dcFormat.hh
 src/ImgIO1394dc.cc src/Lib1394dcFormat.cc
)
target_link_libraries(RavlImgIO1394dc_Body PUBLIC RavlCore RavlIO RavlImage ${libdc1394_LIBRARIES} )

add_library(RavlImgIO1394dc OBJECT src/InitLib1394dc.cc )
target_link_libraries(RavlImgIO1394dc PUBLIC RavlImgIO1394dc_Body )


target_include_directories(
    RavlImgIO1394dc PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlImgIO1394dc_BodyConfig DESTINATION lib/cmake/RavlImgIO1394dc_Body/Target)
install(TARGETS RavlImgIO1394dc_Body EXPORT RavlImgIO1394dc_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlImgIO1394dc_Body DESTINATION lib)

configure_package_config_file(cmake/RavlImgIO1394dc_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlImgIO1394dc_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlImgIO1394dc_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlImgIO1394dc_BodyConfig.cmake
        DESTINATION lib/cmake/RavlImgIO1394dc_Body )


install(TARGETS RavlImgIO1394dc EXPORT RavlImgIO1394dcConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlImgIO1394dcConfig DESTINATION lib/cmake/RavlImgIO1394dc/Target)
install(TARGETS RavlImgIO1394dc DESTINATION lib)

configure_package_config_file(cmake/RavlImgIO1394dcConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlImgIO1394dcConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlImgIO1394dc
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlImgIO1394dcConfig.cmake
        DESTINATION lib/cmake/RavlImgIO1394dc )

install(DIRECTORY include DESTINATION .)


# Mains
set(MAIN_LIBS  RavlCore RavlIO RavlImage ${libdc1394_LIBRARIES} RavlOSIO  )

add_executable(twocam ${EXCLUDE_APPLICATIONS} applications/twocam.cc )
target_link_libraries(twocam RavlImgIO1394dc ${MAIN_LIBS} )
install(TARGETS twocam DESTINATION bin ${EXCLUDE_APPLICATIONS})

# Examples
set(EXAMPLE_LIBS  RavlCore RavlIO RavlImage ${libdc1394_LIBRARIES} RavlOSIO  )

add_executable(test1394dc ${EXCLUDE_EXAMPLES} examples/test1394dc.cc )
target_link_libraries(test1394dc RavlImgIO1394dc ${EXAMPLE_LIBS} )
install(TARGETS test1394dc DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})





ENDIF()

ENDIF()
