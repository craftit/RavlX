
# Library: RavlNet

# Requires: Socket
# Supports: !cygwin

if((NOT ${CMAKE_SYSTEM_NAME} STREQUAL "CYGWIN") OR FALSE)

if(SOCKET_FOUND)

# MustLink: src/RavlSysNetworkML.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlNet 1)

include_directories(../../OS/RavlOS/include)
include_directories(../../Core/RavlCore/include)
include_directories(../../OS/RavlThreads/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../OS/RavlService/include)
include_directories(../../OS/RavlXMLFactory/include)




add_library(RavlNet_Body STATIC
 include/Ravl/OS/SktError.hh include/Ravl/OS/Socket.hh include/Ravl/OS/NetStream.hh include/Ravl/OS/Packet.hh include/Ravl/OS/NetMessage.hh include/Ravl/OS/NetEndPoint.hh include/Ravl/OS/NetMsgCall.hh include/Ravl/OS/NetIPort.hh include/Ravl/OS/NetIPortServer.hh include/Ravl/OS/NetPortManager.hh include/Ravl/OS/NetPortClient.hh include/Ravl/OS/NetPortFormat.hh include/Ravl/OS/NetOPort.hh include/Ravl/OS/NetOPortServer.hh include/Ravl/OS/NetPort.hh include/Ravl/OS/NetAttributeCtrl.hh include/Ravl/OS/NetRequestManager.hh include/Ravl/OS/NetAttributeCtrlServer.hh include/Ravl/OS/NetByteStream.hh include/Ravl/OS/NetMsgOPortData.hh include/Ravl/OS/WinSocket.hh include/Ravl/OS/SocketAddr.hh
 src/Socket.cc src/NetStream.cc src/SocketStrm.cc src/Packet.cc src/NetMessage.cc src/NetEndPoint.cc src/NetMsgCall.cc src/NetIPort.cc src/NetIPortServer.cc src/NetPortManager.cc src/NetPortClient.cc src/NetPortFormat.cc src/NetOPort.cc src/NetOPortServer.cc src/NetPort.cc src/NetAttributeCtrl.cc src/NetRequestManager.cc src/NetAttributeCtrlServer.cc src/NetByteStream.cc src/NetMsgOPortData.cc src/WinSocket.cc src/SocketAddr.cc
)
target_link_libraries(RavlNet_Body PUBLIC RavlOS RavlCore ${SOCKET_LIBRARIES} RavlThreads RavlIO RavlService RavlXMLFactory )

add_library(RavlNet OBJECT src/RavlSysNetworkML.cc )
target_link_libraries(RavlNet PUBLIC RavlNet_Body )


target_include_directories(
    RavlNet PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlNet_BodyConfig DESTINATION lib/cmake/RavlNet_Body/Target)
install(TARGETS RavlNet_Body EXPORT RavlNet_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlNet_Body DESTINATION lib)

configure_package_config_file(cmake/RavlNet_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlNet_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlNet_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlNet_BodyConfig.cmake
        DESTINATION lib/cmake/RavlNet_Body )


install(TARGETS RavlNet EXPORT RavlNetConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlNetConfig DESTINATION lib/cmake/RavlNet/Target)
install(TARGETS RavlNet DESTINATION lib)

configure_package_config_file(cmake/RavlNetConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlNetConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlNet
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlNetConfig.cmake
        DESTINATION lib/cmake/RavlNet )

install(DIRECTORY include DESTINATION .)



# Examples
set(EXAMPLE_LIBS  RavlOS RavlCore ${SOCKET_LIBRARIES} RavlThreads RavlIO RavlService RavlXMLFactory  )

add_executable(exSocket ${EXCLUDE_EXAMPLES} examples/exSocket.cc )
target_link_libraries(exSocket RavlNet ${EXAMPLE_LIBS} )
install(TARGETS exSocket DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exEndPoint ${EXCLUDE_EXAMPLES} examples/exEndPoint.cc )
target_link_libraries(exEndPoint RavlNet ${EXAMPLE_LIBS} )
install(TARGETS exEndPoint DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exNetPort ${EXCLUDE_EXAMPLES} examples/exNetPort.cc )
target_link_libraries(exNetPort RavlNet ${EXAMPLE_LIBS} )
install(TARGETS exNetPort DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

# Tests
set(TEST_LIBS  RavlOS RavlCore ${SOCKET_LIBRARIES} RavlThreads RavlIO RavlService RavlXMLFactory  )

add_executable(testNetPort ${EXCLUDE_UNITTESTS} tests/testNetPort.cc )
target_link_libraries(testNetPort RavlNet ${TEST_LIBS} )
add_test(testNetPort testNetPort)
install(TARGETS testNetPort DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})




ENDIF()

ENDIF()
