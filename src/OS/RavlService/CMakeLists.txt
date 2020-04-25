
# Library: RavlService

# Requires: 
# Supports: 




# MustLink: src/LinkRavlService.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlService 1)

include_directories(../../OS/RavlXMLFactory/include)
include_directories(../../Core/RavlCore/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../OS/RavlThreads/include)
include_directories(../../OS/RavlOS/include)




add_library(RavlService_Body STATIC
 include/Ravl/Service.hh include/Ravl/ServiceList.hh include/Ravl/ServiceThread.hh
 src/Service.cc src/ServiceList.cc src/ServiceThread.cc
)
target_link_libraries(RavlService_Body PUBLIC RavlXMLFactory RavlCore RavlIO RavlThreads RavlOS )

add_library(RavlService OBJECT src/LinkRavlService.cc )
target_link_libraries(RavlService PUBLIC RavlService_Body )


target_include_directories(
    RavlService PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlService_BodyConfig DESTINATION lib/cmake/RavlService_Body/Target)
install(TARGETS RavlService_Body EXPORT RavlService_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlService_Body DESTINATION lib)

configure_package_config_file(cmake/RavlService_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlService_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlService_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlService_BodyConfig.cmake
        DESTINATION lib/cmake/RavlService_Body )


install(TARGETS RavlService EXPORT RavlServiceConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlServiceConfig DESTINATION lib/cmake/RavlService/Target)
install(TARGETS RavlService DESTINATION lib)

configure_package_config_file(cmake/RavlServiceConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlServiceConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlService
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlServiceConfig.cmake
        DESTINATION lib/cmake/RavlService )

install(DIRECTORY include DESTINATION .)










