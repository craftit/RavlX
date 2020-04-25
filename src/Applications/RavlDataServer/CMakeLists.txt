
# Library: RavlDataServer

# Requires: 
# Supports: 




# MustLink: src/MustLinkRavlDataServer.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlDataServer 1)

include_directories(../../OS/RavlNet/include)
include_directories(../../OS/RavlOSIO/include)
include_directories(../../Core/RavlCore/include)
include_directories(../../OS/RavlXMLFactory/include)
include_directories(../../OS/RavlThreads/include)
include_directories(../../OS/RavlOS/include)
include_directories(../../OS/RavlService/include)
include_directories(../../OS/RavlDPMT/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../Image/RavlImageIO/include)
include_directories(../../Image/RavlVideoIO/include)
include_directories(../../Image/RavlExtImgIO/include)
include_directories(../../Image/RavlImage/include)



if(DEFINED HAVE_RavlImgIOV4L2)
include_directories(../../Contrib/RavlImgIOV4L2/include)
endif()

add_library(RavlDataServer_Body STATIC
 include/Ravl/DataServer/DataServer.hh include/Ravl/DataServer/DataServerControlInterface.hh include/Ravl/DataServer/DataServerControlMessages.hh include/Ravl/DataServer/DataServerVFSNode.hh include/Ravl/DataServer/DataServerVFSRealFile.hh include/Ravl/DataServer/DataServerVFSRealDir.hh include/Ravl/DataServer/DataServerControlServer.hh include/Ravl/DataServer/DataServerControlConnection.hh include/Ravl/DataServer/DataServerControlClient.hh include/Ravl/DataServer/DataServerControlNetClient.hh
 src/DataServer.cc src/DataServerControlInterface.cc src/DataServerVFSNode.cc src/DataServerVFSRealFile.cc src/DataServerVFSRealDir.cc src/DataServerControlServer.cc src/DataServerControlConnection.cc src/DataServerControlClient.cc src/DataServerControlNetClient.cc
)
target_link_libraries(RavlDataServer_Body PUBLIC RavlNet RavlOSIO RavlCore RavlXMLFactory RavlThreads RavlOS RavlService RavlDPMT )

add_library(RavlDataServer OBJECT src/MustLinkRavlDataServer.cc )
target_link_libraries(RavlDataServer PUBLIC RavlDataServer_Body )


target_include_directories(
    RavlDataServer PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlDataServer_BodyConfig DESTINATION lib/cmake/RavlDataServer_Body/Target)
install(TARGETS RavlDataServer_Body EXPORT RavlDataServer_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlDataServer_Body DESTINATION lib)

configure_package_config_file(cmake/RavlDataServer_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlDataServer_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlDataServer_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlDataServer_BodyConfig.cmake
        DESTINATION lib/cmake/RavlDataServer_Body )


install(TARGETS RavlDataServer EXPORT RavlDataServerConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlDataServerConfig DESTINATION lib/cmake/RavlDataServer/Target)
install(TARGETS RavlDataServer DESTINATION lib)

configure_package_config_file(cmake/RavlDataServerConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlDataServerConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlDataServer
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlDataServerConfig.cmake
        DESTINATION lib/cmake/RavlDataServer )

install(DIRECTORY include DESTINATION .)


# Mains
set(MAIN_LIBS  RavlNet RavlOSIO RavlCore RavlXMLFactory RavlThreads RavlOS RavlService RavlDPMT RavlIO RavlImageIO RavlVideoIO RavlExtImgIO RavlImage  )

add_executable(dsd applications/dsd.cc )
target_link_libraries(dsd RavlDataServer ${MAIN_LIBS} )
install(TARGETS dsd DESTINATION bin)








