
# Library: RavlUUId

# Requires: UUIdTheo
# Supports: 



if(uuid_FOUND)



set(DEPENDS_ON uuid )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlUUId 1)

include_directories(../../OS/RavlThreads/include)
include_directories(../../Core/RavlCore/include)




add_library(RavlUUId STATIC include/Ravl/GenerateUUId.hh src/GenerateUUId.cc)
target_link_libraries(RavlUUId PUBLIC uuid RavlThreads RavlCore )


target_include_directories(
    RavlUUId PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )



install(TARGETS RavlUUId EXPORT RavlUUIdConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlUUIdConfig DESTINATION lib/cmake/RavlUUId/Target)
install(TARGETS RavlUUId DESTINATION lib)

configure_package_config_file(cmake/RavlUUIdConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlUUIdConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlUUId
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlUUIdConfig.cmake
        DESTINATION lib/cmake/RavlUUId )

install(DIRECTORY include DESTINATION .)








ENDIF()


