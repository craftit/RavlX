
# Library: RavlGnuPlot

# Requires: 
# Supports: 




# MustLink: src/LinkRavlGnuPlot.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlGnuPlot 1)

include_directories(../../Plot/RavlPlot/include)
include_directories(../../OS/RavlOS/include)
include_directories(../../PatternRec/RavlPatternRec/include)
include_directories(../../Core/RavlCore/include)
include_directories(../../Math/RavlMath/include)
include_directories(../../Image/RavlImage/include)
include_directories(../../PatternRec/RavlDataSet/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../OS/RavlXMLFactory/include)
include_directories(../../OS/RavlThreads/include)
include_directories(../../OS/RavlOSIO/include)
include_directories(../../PatternRec/Optimisation/include)
include_directories(../../Image/RavlExtImgIO/include)



if(DEFINED HAVE_RavlDPDisplay)
include_directories(../../GUI/RavlDPDisplay/include)
endif()

add_library(RavlGnuPlot_Body STATIC
 include/Ravl/Plot/GnuPlot2d.hh include/Ravl/Plot/GnuPlot.hh include/Ravl/Plot/GnuPlot3d.hh
 src/GnuPlot2d.cc src/GnuPlot.cc src/GnuPlot3d.cc
)
target_link_libraries(RavlGnuPlot_Body PUBLIC RavlPlot RavlOS RavlPatternRec RavlCore RavlMath RavlImage RavlDataSet RavlIO RavlXMLFactory RavlThreads RavlOSIO Optimisation )

add_library(RavlGnuPlot OBJECT src/LinkRavlGnuPlot.cc )
target_link_libraries(RavlGnuPlot PUBLIC RavlGnuPlot_Body )


target_include_directories(
    RavlGnuPlot PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlGnuPlot_BodyConfig DESTINATION lib/cmake/RavlGnuPlot_Body/Target)
install(TARGETS RavlGnuPlot_Body EXPORT RavlGnuPlot_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlGnuPlot_Body DESTINATION lib)

configure_package_config_file(cmake/RavlGnuPlot_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlGnuPlot_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlGnuPlot_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlGnuPlot_BodyConfig.cmake
        DESTINATION lib/cmake/RavlGnuPlot_Body )


install(TARGETS RavlGnuPlot EXPORT RavlGnuPlotConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlGnuPlotConfig DESTINATION lib/cmake/RavlGnuPlot/Target)
install(TARGETS RavlGnuPlot DESTINATION lib)

configure_package_config_file(cmake/RavlGnuPlotConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlGnuPlotConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlGnuPlot
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlGnuPlotConfig.cmake
        DESTINATION lib/cmake/RavlGnuPlot )

install(DIRECTORY include DESTINATION .)


# Mains
set(MAIN_LIBS  RavlPlot RavlOS RavlPatternRec RavlCore RavlMath RavlImage RavlDataSet RavlIO RavlXMLFactory RavlThreads RavlOSIO Optimisation RavlExtImgIO  )

add_executable(exGnuPlot ${EXCLUDE_APPLICATIONS} applications/exGnuPlot.cc )
target_link_libraries(exGnuPlot RavlGnuPlot ${MAIN_LIBS} )
install(TARGETS exGnuPlot DESTINATION bin ${EXCLUDE_APPLICATIONS})

add_executable(exGnuPlot2d ${EXCLUDE_APPLICATIONS} applications/exGnuPlot2d.cc )
target_link_libraries(exGnuPlot2d RavlGnuPlot ${MAIN_LIBS} )
install(TARGETS exGnuPlot2d DESTINATION bin ${EXCLUDE_APPLICATIONS})

add_executable(exGnuPlot3d ${EXCLUDE_APPLICATIONS} applications/exGnuPlot3d.cc )
target_link_libraries(exGnuPlot3d RavlGnuPlot ${MAIN_LIBS} )
install(TARGETS exGnuPlot3d DESTINATION bin ${EXCLUDE_APPLICATIONS})

add_executable(exGnuPlot2dClassifier ${EXCLUDE_APPLICATIONS} applications/exGnuPlot2dClassifier.cc )
target_link_libraries(exGnuPlot2dClassifier RavlGnuPlot ${MAIN_LIBS} )
install(TARGETS exGnuPlot2dClassifier DESTINATION bin ${EXCLUDE_APPLICATIONS})

add_executable(exGnuPlotTestFunctions ${EXCLUDE_APPLICATIONS} applications/exGnuPlotTestFunctions.cc )
target_link_libraries(exGnuPlotTestFunctions RavlGnuPlot ${MAIN_LIBS} )
install(TARGETS exGnuPlotTestFunctions DESTINATION bin ${EXCLUDE_APPLICATIONS})








