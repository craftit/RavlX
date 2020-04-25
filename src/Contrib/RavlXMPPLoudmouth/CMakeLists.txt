
# Library: RavlXMPPLoudmouth

# Requires: Loudmouth RLog gthread
# Supports: 



if(loudmouth_FOUND)
if(librlog_FOUND)
if(gthread_FOUND)

# MustLink: src/LinkRavlXMPP.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlXMPPLoudmouth 1)

include_directories(../../Contrib/RavlXMPP/include)
include_directories(../../Core/RavlCore/include)
include_directories(../../OS/RavlOS/include)
include_directories(../../OS/RavlThreads/include)
include_directories(../../OS/RavlXMLFactory/include)
include_directories(../../Core/RavlIO/include)




add_library(RavlXMPPLoudmouth_Body STATIC
 include/Ravl/XMPP/LMConnection.hh
 src/LMConnection.cc
)
target_link_libraries(RavlXMPPLoudmouth_Body PUBLIC RavlXMPP RavlCore ${loudmouth_LIBRARIES} RavlOS RavlThreads RavlXMLFactory ${gthread_LIBRARIES} RavlIO )

add_library(RavlXMPPLoudmouth OBJECT src/LinkRavlXMPP.cc )
target_link_libraries(RavlXMPPLoudmouth PUBLIC RavlXMPPLoudmouth_Body )


target_include_directories(
    RavlXMPPLoudmouth PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlXMPPLoudmouth_BodyConfig DESTINATION lib/cmake/RavlXMPPLoudmouth_Body/Target)
install(TARGETS RavlXMPPLoudmouth_Body EXPORT RavlXMPPLoudmouth_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlXMPPLoudmouth_Body DESTINATION lib)

configure_package_config_file(cmake/RavlXMPPLoudmouth_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlXMPPLoudmouth_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlXMPPLoudmouth_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlXMPPLoudmouth_BodyConfig.cmake
        DESTINATION lib/cmake/RavlXMPPLoudmouth_Body )


install(TARGETS RavlXMPPLoudmouth EXPORT RavlXMPPLoudmouthConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlXMPPLoudmouthConfig DESTINATION lib/cmake/RavlXMPPLoudmouth/Target)
install(TARGETS RavlXMPPLoudmouth DESTINATION lib)

configure_package_config_file(cmake/RavlXMPPLoudmouthConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlXMPPLoudmouthConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlXMPPLoudmouth
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlXMPPLoudmouthConfig.cmake
        DESTINATION lib/cmake/RavlXMPPLoudmouth )

install(DIRECTORY include DESTINATION .)


# Mains
set(MAIN_LIBS  RavlXMPP RavlCore ${loudmouth_LIBRARIES} RavlOS RavlThreads RavlXMLFactory ${gthread_LIBRARIES} RavlIO  )

add_executable(exLoudmouth applications/exLoudmouth.cc )
target_link_libraries(exLoudmouth RavlXMPPLoudmouth ${MAIN_LIBS} )
install(TARGETS exLoudmouth DESTINATION bin)






ENDIF()
ENDIF()
ENDIF()


