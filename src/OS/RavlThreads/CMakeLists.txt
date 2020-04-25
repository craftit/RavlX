
# Library: RavlThreads

# Requires: 
# Supports: 






set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlThreads 1)

include_directories(../../Core/RavlCore/include)
include_directories(../../OS/RavlOS/include)




add_library(RavlThreads STATIC include/Ravl/Threads/Thread.hh include/Ravl/Threads/Mutex.hh include/Ravl/Threads/ConditionalMutex.hh include/Ravl/Threads/Semaphore.hh include/Ravl/Threads/RWLock.hh include/Ravl/Threads/SemaphoreRC.hh include/Ravl/Threads/ThreadEvent.hh include/Ravl/Threads/RCRWLock.hh include/Ravl/Threads/ThreadSignal.hh include/Ravl/Threads/ConditionalVariable.hh include/Ravl/Threads/MessageQueue.hh include/Ravl/Threads/LaunchThread.hh include/Ravl/Threads/TickerTrigger.hh include/Ravl/Threads/Signal.hh include/Ravl/Threads/Signal1.hh include/Ravl/Threads/Signal2.hh include/Ravl/Threads/Signal3.hh include/Ravl/Threads/MTPriQueueL.hh include/Ravl/Threads/SignalConnectionSet.hh include/Ravl/Threads/TimedTriggerQueue.hh include/Ravl/Threads/ThreadState.hh include/Ravl/Threads/ThreadLimitCounter.hh src/Thread.cc src/Mutex.cc src/ConditionalMutex.cc src/Semaphore.cc src/RWLock.cc src/SemaphoreRC.cc src/MTLockImpl.cc src/ThreadEvent.cc src/AMutexImpl.cc src/RCRWLock.cc src/ThreadSignal.cc src/ConditionalVariable.cc src/MessageQueue.cc src/LaunchThread.cc src/TickerTrigger.cc src/Signal.cc src/SignalConnectionSet.cc src/TimedTriggerQueue.cc src/ThreadState.cc src/ThreadLimitCounter.cc)
target_link_libraries(RavlThreads PUBLIC RavlCore ${CMAKE_THREAD_LIBS_INIT} RavlOS )


target_include_directories(
    RavlThreads PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )



install(TARGETS RavlThreads EXPORT RavlThreadsConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlThreadsConfig DESTINATION lib/cmake/RavlThreads/Target)
install(TARGETS RavlThreads DESTINATION lib)

configure_package_config_file(cmake/RavlThreadsConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlThreadsConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlThreads
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlThreadsConfig.cmake
        DESTINATION lib/cmake/RavlThreads )

install(DIRECTORY include DESTINATION .)



# Examples
set(EXAMPLE_LIBS  RavlCore ${CMAKE_THREAD_LIBS_INIT} RavlOS  )

add_executable(exThread ${EXCLUDE_EXAMPLES} examples/exThread.cc )
target_link_libraries(exThread RavlThreads ${EXAMPLE_LIBS} )
install(TARGETS exThread DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exSignal ${EXCLUDE_EXAMPLES} examples/exSignal.cc )
target_link_libraries(exSignal RavlThreads ${EXAMPLE_LIBS} )
install(TARGETS exSignal DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exSignal1 ${EXCLUDE_EXAMPLES} examples/exSignal1.cc )
target_link_libraries(exSignal1 RavlThreads ${EXAMPLE_LIBS} )
install(TARGETS exSignal1 DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exSignal2 ${EXCLUDE_EXAMPLES} examples/exSignal2.cc )
target_link_libraries(exSignal2 RavlThreads ${EXAMPLE_LIBS} )
install(TARGETS exSignal2 DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exLaunchThread ${EXCLUDE_EXAMPLES} examples/exLaunchThread.cc )
target_link_libraries(exLaunchThread RavlThreads ${EXAMPLE_LIBS} )
install(TARGETS exLaunchThread DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(testAtomicSpeed ${EXCLUDE_EXAMPLES} examples/testAtomicSpeed.cc )
target_link_libraries(testAtomicSpeed RavlThreads ${EXAMPLE_LIBS} )
install(TARGETS testAtomicSpeed DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

# Tests
set(TEST_LIBS  RavlCore ${CMAKE_THREAD_LIBS_INIT} RavlOS  )

add_executable(testRWLock ${EXCLUDE_UNITTESTS} tests/testRWLock.cc )
target_link_libraries(testRWLock RavlThreads ${TEST_LIBS} )
add_test(testRWLock testRWLock)
install(TARGETS testRWLock DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})

add_executable(testLaunchThread ${EXCLUDE_UNITTESTS} tests/testLaunchThread.cc )
target_link_libraries(testLaunchThread RavlThreads ${TEST_LIBS} )
add_test(testLaunchThread testLaunchThread)
install(TARGETS testLaunchThread DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})

add_executable(testTickerTrigger ${EXCLUDE_UNITTESTS} tests/testTickerTrigger.cc )
target_link_libraries(testTickerTrigger RavlThreads ${TEST_LIBS} )
add_test(testTickerTrigger testTickerTrigger)
install(TARGETS testTickerTrigger DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})

add_executable(testTimedTriggerQueue ${EXCLUDE_UNITTESTS} tests/testTimedTriggerQueue.cc )
target_link_libraries(testTimedTriggerQueue RavlThreads ${TEST_LIBS} )
add_test(testTimedTriggerQueue testTimedTriggerQueue)
install(TARGETS testTimedTriggerQueue DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})

add_executable(testSignal ${EXCLUDE_UNITTESTS} tests/testSignal.cc )
target_link_libraries(testSignal RavlThreads ${TEST_LIBS} )
add_test(testSignal testSignal)
install(TARGETS testSignal DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})

add_executable(testAtomic ${EXCLUDE_UNITTESTS} tests/testAtomic.cc )
target_link_libraries(testAtomic RavlThreads ${TEST_LIBS} )
add_test(testAtomic testAtomic)
install(TARGETS testAtomic DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})

add_executable(testThreads ${EXCLUDE_UNITTESTS} tests/testThreads.cc )
target_link_libraries(testThreads RavlThreads ${TEST_LIBS} )
add_test(testThreads testThreads)
install(TARGETS testThreads DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})






