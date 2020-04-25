
# Library: RavlPythonSwig

# Requires: LibPython Swig SwigPython
# Supports: 



if(python_FOUND)
if(Swig_FOUND)
if(SwigPython_FOUND)



set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlPythonSwig 1)

include_directories(../../Math/RavlMath/include)
include_directories(../../CCMath/ccmath/include)
include_directories(../../Image/RavlImageIO/include)
include_directories(../../Image/RavlImage/include)
include_directories(../../OS/RavlNet/include)
include_directories(../../OS/RavlService/include)
include_directories(../../OS/RavlXMLFactory/include)
include_directories(../../OS/RavlThreads/include)
include_directories(../../OS/RavlOSIO/include)
include_directories(../../OS/RavlOS/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../Core/RavlCore/include)



if(DEFINED HAVE_RavlDPDisplay)
include_directories(../../GUI/RavlDPDisplay/include)
endif()
if(DEFINED HAVE_RavlGUI2D)
include_directories(../../GUI/RavlGUI2D/include)
endif()
if(DEFINED HAVE_RavlGUI)
include_directories(../../GUI/RavlGUI/include)
endif()

add_library(RavlPythonSwig STATIC  src/Ravl.i)
target_link_libraries(RavlPythonSwig PUBLIC RavlMath ccmath RavlImageIO RavlImage ${python_LIBRARIES} RavlNet RavlService RavlXMLFactory RavlThreads RavlOSIO RavlOS RavlIO RavlCore )


target_include_directories(
    RavlPythonSwig PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )



install(TARGETS RavlPythonSwig EXPORT RavlPythonSwigConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlPythonSwigConfig DESTINATION lib/cmake/RavlPythonSwig/Target)
install(TARGETS RavlPythonSwig DESTINATION lib)

configure_package_config_file(cmake/RavlPythonSwigConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlPythonSwigConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlPythonSwig
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlPythonSwigConfig.cmake
        DESTINATION lib/cmake/RavlPythonSwig )

install(DIRECTORY include DESTINATION .)






# === Files for /share/Python ===
set(AUXDIR /share/Python)
set(AUXFILES 
  data/Ravl.py )
file(COPY ${AUXFILES} DESTINATION ${BUILD_ROOT_AUXFILE}/${AUXDIR})
install(FILES ${AUXFILES} DESTINATION ${AUXDIR})



ENDIF()
ENDIF()
ENDIF()


