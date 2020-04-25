
# Library: RavlXMPP

# Requires: RLog
# Supports: 



if(librlog_FOUND)



set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlXMPP 1)

include_directories(../../OS/RavlOS/include)
include_directories(../../OS/RavlThreads/include)
include_directories(../../Contrib/RavlRLog/include)
include_directories(../../Core/RavlCore/include)
include_directories(../../OS/RavlXMLFactory/include)
include_directories(../../Core/RavlIO/include)




add_library(RavlXMPP STATIC include/Ravl/XMPP/XMPPConnection.hh src/XMPPConnection.cc)
target_link_libraries(RavlXMPP PUBLIC RavlOS RavlThreads RavlRLog ${librlog_LIBRARIES} RavlCore RavlXMLFactory RavlIO )


target_include_directories(
    RavlXMPP PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )



install(TARGETS RavlXMPP EXPORT RavlXMPPConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlXMPPConfig DESTINATION lib/cmake/RavlXMPP/Target)
install(TARGETS RavlXMPP DESTINATION lib)

configure_package_config_file(cmake/RavlXMPPConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlXMPPConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlXMPP
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlXMPPConfig.cmake
        DESTINATION lib/cmake/RavlXMPP )

install(DIRECTORY include DESTINATION .)








ENDIF()


