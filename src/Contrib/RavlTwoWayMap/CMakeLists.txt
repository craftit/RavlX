
# Library: RavlTwoWayMap

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

set(HAVE_RavlTwoWayMap 1)

include_directories(../../OS/RavlOS/include)
include_directories(../../Core/RavlCore/include)
include_directories(../../Core/RavlIO/include)




add_library(RavlTwoWayMap STATIC include/Ravl/TwoWayMap.hh src/TwoWayMap.cc)
target_link_libraries(RavlTwoWayMap PUBLIC RavlOS RavlCore RavlIO )


target_include_directories(
    RavlTwoWayMap PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )



install(TARGETS RavlTwoWayMap EXPORT RavlTwoWayMapConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlTwoWayMapConfig DESTINATION lib/cmake/RavlTwoWayMap/Target)
install(TARGETS RavlTwoWayMap DESTINATION lib)

configure_package_config_file(cmake/RavlTwoWayMapConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlTwoWayMapConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlTwoWayMap
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlTwoWayMapConfig.cmake
        DESTINATION lib/cmake/RavlTwoWayMap )

install(DIRECTORY include DESTINATION .)










