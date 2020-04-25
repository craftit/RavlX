
# Library: RavlEHS

# Requires: libehs
# Supports: !VCPP

if((NOT ${CMAKE_CXX_COMPILER_ID} MATCHES "MSVC") OR FALSE)

if(libehs_FOUND)



set(DEPENDS_ON EHS )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlEHS 1)

include_directories(../../OS/RavlThreads/include)
include_directories(../../OS/RavlOS/include)
include_directories(../../Core/RavlCore/include)




add_library(RavlEHS STATIC include/Ravl/HTTPRequest.hh include/Ravl/HTTPResponse.hh include/Ravl/HTTPServer.hh include/Ravl/EHS.hh src/HTTPRequest.cc src/HTTPResponse.cc src/HTTPServer.cc src/EHS.cc)
target_link_libraries(RavlEHS PUBLIC EHS RavlThreads RavlOS RavlCore )


target_include_directories(
    RavlEHS PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )



install(TARGETS RavlEHS EXPORT RavlEHSConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlEHSConfig DESTINATION lib/cmake/RavlEHS/Target)
install(TARGETS RavlEHS DESTINATION lib)

configure_package_config_file(cmake/RavlEHSConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlEHSConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlEHS
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlEHSConfig.cmake
        DESTINATION lib/cmake/RavlEHS )

install(DIRECTORY include DESTINATION .)








ENDIF()

ENDIF()
