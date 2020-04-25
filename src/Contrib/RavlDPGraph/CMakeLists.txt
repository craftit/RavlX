
# Library: RavlDPGraph

# Requires: libGuppi libGTK2
# Supports: !VCPP

if((NOT ${CMAKE_CXX_COMPILER_ID} MATCHES "MSVC") OR FALSE)

if(libguppi_FOUND)
if(GTK2_FOUND)

# MustLink: src/RavlDPGraphWindow.cc


set(DEPENDS_ON RavlGuppi )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlDPGraph 1)

include_directories(../../Core/RavlIO/include)
include_directories(../../GUI/RavlGUI/include)
include_directories(../../Plot/RavlPlot/include)
include_directories(../../Core/RavlCore/include)




add_library(RavlDPGraph_Body STATIC
 include/Ravl/Plot/DPGraphWindow.hh include/Ravl/Plot/DPGraphWindowOPort.hh include/Ravl/Plot/DPGraphWindowFormat.hh
 src/DPGraphWindow.cc src/DPGraphWindowOPort.cc src/DPGraphWindowFormat.cc
)
target_link_libraries(RavlDPGraph_Body PUBLIC RavlIO RavlGUI RavlPlot RavlGuppi RavlCore )

add_library(RavlDPGraph OBJECT src/RavlDPGraphWindow.cc )
target_link_libraries(RavlDPGraph PUBLIC RavlDPGraph_Body )


target_include_directories(
    RavlDPGraph PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlDPGraph_BodyConfig DESTINATION lib/cmake/RavlDPGraph_Body/Target)
install(TARGETS RavlDPGraph_Body EXPORT RavlDPGraph_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlDPGraph_Body DESTINATION lib)

configure_package_config_file(cmake/RavlDPGraph_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlDPGraph_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlDPGraph_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlDPGraph_BodyConfig.cmake
        DESTINATION lib/cmake/RavlDPGraph_Body )


install(TARGETS RavlDPGraph EXPORT RavlDPGraphConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlDPGraphConfig DESTINATION lib/cmake/RavlDPGraph/Target)
install(TARGETS RavlDPGraph DESTINATION lib)

configure_package_config_file(cmake/RavlDPGraphConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlDPGraphConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlDPGraph
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlDPGraphConfig.cmake
        DESTINATION lib/cmake/RavlDPGraph )

install(DIRECTORY include DESTINATION .)



# Examples
set(EXAMPLE_LIBS  RavlIO RavlGUI RavlPlot RavlGuppi RavlCore  )

add_executable(exDPGraphWindow ${EXCLUDE_EXAMPLES} examples/exDPGraphWindow.cc )
target_link_libraries(exDPGraphWindow RavlDPGraph ${EXAMPLE_LIBS} )
install(TARGETS exDPGraphWindow DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})





ENDIF()
ENDIF()

ENDIF()
