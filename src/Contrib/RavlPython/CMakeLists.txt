
# Library: RavlPython

# Requires: LibPython
# Supports: 



if(python_FOUND)



set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlPython 1)

include_directories(../../OS/RavlOS/include)
include_directories(../../Core/RavlCore/include)
include_directories(../../OS/RavlThreads/include)
include_directories(../../GUI/RavlGUI/include)




add_library(RavlPython STATIC include/Ravl/Python.hh include/Ravl/PythonObject.hh include/Ravl/PythonException.hh include/Ravl/PythonLock.hh include/Ravl/PythonMainState.hh src/Python.cc src/PythonObject.cc src/PythonException.cc src/PythonLock.cc src/PythonMainState.cc)
target_link_libraries(RavlPython PUBLIC RavlOS RavlCore RavlThreads ${python_LIBRARIES} )


target_include_directories(
    RavlPython PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )



install(TARGETS RavlPython EXPORT RavlPythonConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlPythonConfig DESTINATION lib/cmake/RavlPython/Target)
install(TARGETS RavlPython DESTINATION lib)

configure_package_config_file(cmake/RavlPythonConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlPythonConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlPython
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlPythonConfig.cmake
        DESTINATION lib/cmake/RavlPython )

install(DIRECTORY include DESTINATION .)



# Examples
set(EXAMPLE_LIBS  RavlOS RavlCore RavlThreads ${python_LIBRARIES} RavlGUI  )

add_executable(exPython EXCLUDE_FROM_ALL examples/exPython.cc )
target_link_libraries(exPython RavlPython ${EXAMPLE_LIBS} )
install(TARGETS exPython DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)

add_executable(exPythonThreaded EXCLUDE_FROM_ALL examples/exPythonThreaded.cc )
target_link_libraries(exPythonThreaded RavlPython ${EXAMPLE_LIBS} )
install(TARGETS exPythonThreaded DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)

add_executable(exPythonMultipleInterpreters EXCLUDE_FROM_ALL examples/exPythonMultipleInterpreters.cc )
target_link_libraries(exPythonMultipleInterpreters RavlPython ${EXAMPLE_LIBS} )
install(TARGETS exPythonMultipleInterpreters DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)





ENDIF()


