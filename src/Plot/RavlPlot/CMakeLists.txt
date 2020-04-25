
# Library: RavlPlot

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

set(HAVE_RavlPlot 1)

include_directories(../../Core/RavlCore/include)
include_directories(../../Math/RavlMath/include)
include_directories(../../OS/RavlOS/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../Image/RavlImage/include)
include_directories(../../PatternRec/RavlDataSet/include)
include_directories(../../OS/RavlXMLFactory/include)
include_directories(../../OS/RavlThreads/include)




add_library(RavlPlot STATIC include/Ravl/Plot/Plot2d.hh include/Ravl/Plot/GraphData2d.hh src/Plot2d.cc src/GraphData2d.cc)
target_link_libraries(RavlPlot PUBLIC RavlCore RavlMath RavlOS RavlIO RavlImage RavlDataSet RavlXMLFactory RavlThreads )


target_include_directories(
    RavlPlot PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )



install(TARGETS RavlPlot EXPORT RavlPlotConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlPlotConfig DESTINATION lib/cmake/RavlPlot/Target)
install(TARGETS RavlPlot DESTINATION lib)

configure_package_config_file(cmake/RavlPlotConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlPlotConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlPlot
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlPlotConfig.cmake
        DESTINATION lib/cmake/RavlPlot )

install(DIRECTORY include DESTINATION .)










