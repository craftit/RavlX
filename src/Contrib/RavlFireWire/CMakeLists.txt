
# Library: RavlFireWire

# Requires: libDV libavc1394
# Supports: linux linux64

if((${CMAKE_SYSTEM_NAME} MATCHES "Linux32") OR (${CMAKE_SYSTEM_NAME} MATCHES "Linux") OR FALSE)

if(libdv_FOUND)
if(libavc1394_FOUND)



set(DEPENDS_ON LibFireWire )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlFireWire 1)

include_directories(../../Contrib/RavlDV/include)
include_directories(../../Core/RavlCore/include)



if(DEFINED HAVE_RavlDPDisplay)
include_directories(../../GUI/RavlDPDisplay/include)
endif()

add_library(RavlFireWire STATIC include/Ravl/Image/DvDevice.hh include/Ravl/Image/WavFile.hh include/Ravl/Image/PalFrame.hh src/DvDevice.cc src/WavFile.cc src/PalFrame.cc)
target_link_libraries(RavlFireWire PUBLIC RavlDV LibFireWire RavlCore )


target_include_directories(
    RavlFireWire PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )



install(TARGETS RavlFireWire EXPORT RavlFireWireConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlFireWireConfig DESTINATION lib/cmake/RavlFireWire/Target)
install(TARGETS RavlFireWire DESTINATION lib)

configure_package_config_file(cmake/RavlFireWireConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlFireWireConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlFireWire
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlFireWireConfig.cmake
        DESTINATION lib/cmake/RavlFireWire )

install(DIRECTORY include DESTINATION .)


# Mains
set(MAIN_LIBS  RavlDV LibFireWire RavlCore  )

add_executable(doDvGrab ${EXCLUDE_APPLICATIONS} applications/doDvGrab.cc )
target_link_libraries(doDvGrab RavlFireWire ${MAIN_LIBS} )
install(TARGETS doDvGrab DESTINATION bin ${EXCLUDE_APPLICATIONS})






ENDIF()
ENDIF()

ENDIF()
