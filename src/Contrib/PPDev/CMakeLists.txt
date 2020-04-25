
# Library: PPDev

# Requires: Linux
# Supports: linux

if((${CMAKE_SYSTEM_NAME} MATCHES "Linux32") OR FALSE)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")



set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_PPDev 1)

include_directories(../../Core/RavlCore/include)
include_directories(../../OS/RavlOS/include)




add_library(PPDev STATIC include/Ravl/OS/Parport.hh src/Parport.cc)
target_link_libraries(PPDev PUBLIC RavlCore RavlOS )


target_include_directories(
    PPDev PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )



install(TARGETS PPDev EXPORT PPDevConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT PPDevConfig DESTINATION lib/cmake/PPDev/Target)
install(TARGETS PPDev DESTINATION lib)

configure_package_config_file(cmake/PPDevConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/PPDevConfig.cmake
  INSTALL_DESTINATION lib/cmake/PPDev
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/PPDevConfig.cmake
        DESTINATION lib/cmake/PPDev )

install(DIRECTORY include DESTINATION .)








ENDIF()

ENDIF()
