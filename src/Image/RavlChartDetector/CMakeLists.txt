
# Library: RavlChartDetector

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

set(HAVE_RavlChartDetector 1)

include_directories(../../Image/RavlImage/include)
include_directories(../../Image/RavlImageProc/include)
include_directories(../../Image/RavlImageIO/include)
include_directories(../../PatternRec/Optimisation/include)
include_directories(../../Core/RavlCore/include)
include_directories(../../Math/RavlMath/include)
include_directories(../../Math/RavlOptimise/include)
include_directories(../../PatternRec/RavlDataSet/include)
include_directories(../../OS/RavlXMLFactory/include)
include_directories(../../OS/RavlThreads/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../Image/RavlExtImgIO/include)
include_directories(../../OS/RavlOS/include)
include_directories(../../PatternRec/RavlPatternRec/include)



if(DEFINED HAVE_RavlDPDisplay)
include_directories(../../GUI/RavlDPDisplay/include)
endif()

add_library(RavlChartDetector STATIC include/Ravl/Image/ChartDetector.hh include/Ravl/Image/ChartDetectorRegion.hh include/Ravl/Image/ChartLocalise.hh include/Ravl/Image/CostAffineImageCorrelation.hh src/ChartDetector.cc src/ChartDetectorRegion.cc src/ChartLocalise.cc src/CostAffineImageCorrelation.cc)
target_link_libraries(RavlChartDetector PUBLIC RavlImage RavlImageProc RavlImageIO Optimisation RavlCore RavlMath RavlOptimise RavlDataSet RavlXMLFactory RavlThreads RavlIO )


target_include_directories(
    RavlChartDetector PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )



install(TARGETS RavlChartDetector EXPORT RavlChartDetectorConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlChartDetectorConfig DESTINATION lib/cmake/RavlChartDetector/Target)
install(TARGETS RavlChartDetector DESTINATION lib)

configure_package_config_file(cmake/RavlChartDetectorConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlChartDetectorConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlChartDetector
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlChartDetectorConfig.cmake
        DESTINATION lib/cmake/RavlChartDetector )

install(DIRECTORY include DESTINATION .)


# Mains
set(MAIN_LIBS  RavlImage RavlImageProc RavlImageIO Optimisation RavlCore RavlMath RavlOptimise RavlDataSet RavlXMLFactory RavlThreads RavlIO RavlExtImgIO RavlOS RavlPatternRec  )

add_executable(chartdet applications/chartdet.cc )
target_link_libraries(chartdet RavlChartDetector ${MAIN_LIBS} )
install(TARGETS chartdet DESTINATION bin)




# === Files for share/RAVL/pixmaps ===
set(AUXDIR share/RAVL/pixmaps)
set(AUXFILES 
  data/CalibrationChart.ps 
  data/CalibrationChart.png 
  data/CalibrationChart.coords )
file(COPY ${AUXFILES} DESTINATION ${BUILD_ROOT_AUXFILE}/${AUXDIR})
install(FILES ${AUXFILES} DESTINATION ${AUXDIR})





