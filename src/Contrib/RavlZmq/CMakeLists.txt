
# Library: RavlZmq

# Requires: ZeroMQ
# Supports: !VCPP

if((NOT ${CMAKE_CXX_COMPILER_ID} MATCHES "MSVC") OR FALSE)

if(libzmq_FOUND)

# MustLink: src/LinkRavlZeroMQ.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlZmq 1)

include_directories(../../Core/RavlCore/include)
include_directories(../../OS/RavlXMLFactory/include)
include_directories(../../OS/RavlService/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../OS/RavlThreads/include)
include_directories(../../OS/RavlOS/include)




add_library(RavlZmq_Body STATIC
 include/Ravl/Zmq/Context.hh include/Ravl/Zmq/Message.hh include/Ravl/Zmq/Socket.hh include/Ravl/Zmq/MsgBuffer.hh include/Ravl/Zmq/SocketDispatcher.hh include/Ravl/Zmq/Reactor.hh include/Ravl/Zmq/SocketDispatchTrigger.hh include/Ravl/Zmq/MsgJSON.hh include/Ravl/Zmq/MsgSmartPtr.hh
 src/ZmqContext.cc src/ZmqMessage.cc src/ZmqSocket.cc src/ZmqMsgBuffer.cc src/ZmqSocketDispatcher.cc src/ZmqReactor.cc src/ZmqSocketDispatchTrigger.cc src/ZmqMsgJSON.cc src/ZmqMsgSmartPtr.cc
)
target_link_libraries(RavlZmq_Body PUBLIC RavlCore RavlXMLFactory ${libzmq_LIBRARIES} RavlService RavlIO RavlThreads RavlOS )

add_library(RavlZmq OBJECT src/LinkRavlZeroMQ.cc )
target_link_libraries(RavlZmq PUBLIC RavlZmq_Body )


target_include_directories(
    RavlZmq PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlZmq_BodyConfig DESTINATION lib/cmake/RavlZmq_Body/Target)
install(TARGETS RavlZmq_Body EXPORT RavlZmq_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlZmq_Body DESTINATION lib)

configure_package_config_file(cmake/RavlZmq_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlZmq_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlZmq_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlZmq_BodyConfig.cmake
        DESTINATION lib/cmake/RavlZmq_Body )


install(TARGETS RavlZmq EXPORT RavlZmqConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlZmqConfig DESTINATION lib/cmake/RavlZmq/Target)
install(TARGETS RavlZmq DESTINATION lib)

configure_package_config_file(cmake/RavlZmqConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlZmqConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlZmq
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlZmqConfig.cmake
        DESTINATION lib/cmake/RavlZmq )

install(DIRECTORY include DESTINATION .)


# Mains
set(MAIN_LIBS  RavlCore RavlXMLFactory ${libzmq_LIBRARIES} RavlService RavlIO RavlThreads RavlOS  )

add_executable(testRavlZeroMQ applications/testRavlZeroMQ.cc )
target_link_libraries(testRavlZeroMQ RavlZmq ${MAIN_LIBS} )
install(TARGETS testRavlZeroMQ DESTINATION bin)

add_executable(testRavlZeroMQXML applications/testRavlZeroMQXML.cc )
target_link_libraries(testRavlZeroMQXML RavlZmq ${MAIN_LIBS} )
install(TARGETS testRavlZeroMQXML DESTINATION bin)






ENDIF()

ENDIF()
