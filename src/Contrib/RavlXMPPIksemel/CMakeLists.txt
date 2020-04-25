
# Library: RavlXMPPIksemel

# Requires: iksemel RLog
# Supports: 



if(iksemel_FOUND)
if(librlog_FOUND)

# MustLink: src/LinkRavlXMPPIksemel.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlXMPPIksemel 1)

include_directories(../../OS/RavlOS/include)
include_directories(../../Contrib/RavlXMPP/include)
include_directories(../../Contrib/RavlRLog/include)
include_directories(../../OS/RavlXMLFactory/include)
include_directories(../../Core/RavlCore/include)
include_directories(../../OS/RavlThreads/include)
include_directories(../../Core/RavlIO/include)




add_library(RavlXMPPIksemel_Body STATIC
 include/Ravl/XMPP/IksemelConnection.hh
 src/IksemelConnection.cc
)
target_link_libraries(RavlXMPPIksemel_Body PUBLIC RavlOS ${iksemel_LIBRARIES} RavlXMPP RavlRLog ${librlog_LIBRARIES} RavlXMLFactory RavlCore RavlThreads )

add_library(RavlXMPPIksemel OBJECT src/LinkRavlXMPPIksemel.cc )
target_link_libraries(RavlXMPPIksemel PUBLIC RavlXMPPIksemel_Body )


target_include_directories(
    RavlXMPPIksemel PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlXMPPIksemel_BodyConfig DESTINATION lib/cmake/RavlXMPPIksemel_Body/Target)
install(TARGETS RavlXMPPIksemel_Body EXPORT RavlXMPPIksemel_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlXMPPIksemel_Body DESTINATION lib)

configure_package_config_file(cmake/RavlXMPPIksemel_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlXMPPIksemel_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlXMPPIksemel_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlXMPPIksemel_BodyConfig.cmake
        DESTINATION lib/cmake/RavlXMPPIksemel_Body )


install(TARGETS RavlXMPPIksemel EXPORT RavlXMPPIksemelConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlXMPPIksemelConfig DESTINATION lib/cmake/RavlXMPPIksemel/Target)
install(TARGETS RavlXMPPIksemel DESTINATION lib)

configure_package_config_file(cmake/RavlXMPPIksemelConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlXMPPIksemelConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlXMPPIksemel
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlXMPPIksemelConfig.cmake
        DESTINATION lib/cmake/RavlXMPPIksemel )

install(DIRECTORY include DESTINATION .)


# Mains
set(MAIN_LIBS  RavlOS ${iksemel_LIBRARIES} RavlXMPP RavlRLog ${librlog_LIBRARIES} RavlXMLFactory RavlCore RavlThreads RavlIO  )

add_executable(testIksemel applications/testIksemel.cc )
target_link_libraries(testIksemel RavlXMPPIksemel ${MAIN_LIBS} )
install(TARGETS testIksemel DESTINATION bin)






ENDIF()
ENDIF()


