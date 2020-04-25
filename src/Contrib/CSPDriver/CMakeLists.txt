
# Library: CSPDriver

# Requires: libClipStationPro
# Supports: !VCPP

if((NOT ${CMAKE_CXX_COMPILER_ID} MATCHES "MSVC") OR FALSE)

if(DEFINED libClipStationPro_FOUND)

# MustLink: src/ClipStationPro.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_CSPDriver 1)

include_directories(../../Core/RavlCore/include)
include_directories(../../Image/RavlImage/include)
include_directories(../../Image/RavlImageIO/include)
include_directories(../../OS/RavlThreads/include)
include_directories(../../OS/RavlOSIO/include)
include_directories(../../Math/RavlMath/include)




add_library(CSPDriver_Body STATIC
 include/Ravl/Image/CSPControl.hh include/Ravl/Image/ImgIOCSP.hh include/Ravl/Image/CSPFormat.hh
 src/CSPControl.cc src/ImgIOCSP.cc src/CSPFormat.cc
)
target_link_libraries(CSPDriver_Body PUBLIC ${libClipStationPro_LIBRARIES} RavlCore RavlImage RavlImageIO RavlThreads )

add_library(CSPDriver OBJECT src/ClipStationPro.cc )
target_link_libraries(CSPDriver PUBLIC CSPDriver_Body )


target_include_directories(
    CSPDriver PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT CSPDriver_BodyConfig DESTINATION lib/cmake/CSPDriver_Body/Target)
install(TARGETS CSPDriver_Body EXPORT CSPDriver_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS CSPDriver_Body DESTINATION lib)

configure_package_config_file(cmake/CSPDriver_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/CSPDriver_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/CSPDriver_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/CSPDriver_BodyConfig.cmake
        DESTINATION lib/cmake/CSPDriver_Body )


install(TARGETS CSPDriver EXPORT CSPDriverConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT CSPDriverConfig DESTINATION lib/cmake/CSPDriver/Target)
install(TARGETS CSPDriver DESTINATION lib)

configure_package_config_file(cmake/CSPDriverConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/CSPDriverConfig.cmake
  INSTALL_DESTINATION lib/cmake/CSPDriver
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/CSPDriverConfig.cmake
        DESTINATION lib/cmake/CSPDriver )

install(DIRECTORY include DESTINATION .)



# Examples
set(EXAMPLE_LIBS  ${libClipStationPro_LIBRARIES} RavlCore RavlImage RavlImageIO RavlThreads RavlOSIO RavlMath  )

add_executable(exCSPControl EXCLUDE_FROM_ALL examples/exCSPControl.cc )
target_link_libraries(exCSPControl CSPDriver ${EXAMPLE_LIBS} )
install(TARGETS exCSPControl DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)

add_executable(exImgIOCSP EXCLUDE_FROM_ALL examples/exImgIOCSP.cc )
target_link_libraries(exImgIOCSP CSPDriver ${EXAMPLE_LIBS} )
install(TARGETS exImgIOCSP DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)





ENDIF()

ENDIF()
