
# Library: RavlVideo

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

set(HAVE_RavlVideo 1)

include_directories(../../Core/RavlCore/include)
include_directories(../../Image/RavlImage/include)




add_library(RavlVideo STATIC include/Ravl/Image/ShotBoundHistInt.hh src/ShotBoundHistInt.cc)
target_link_libraries(RavlVideo PUBLIC RavlCore RavlImage )


target_include_directories(
    RavlVideo PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )



install(TARGETS RavlVideo EXPORT RavlVideoConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlVideoConfig DESTINATION lib/cmake/RavlVideo/Target)
install(TARGETS RavlVideo DESTINATION lib)

configure_package_config_file(cmake/RavlVideoConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlVideoConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlVideo
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlVideoConfig.cmake
        DESTINATION lib/cmake/RavlVideo )

install(DIRECTORY include DESTINATION .)










