
# Library: RavlFileIDX

# Requires: 
# Supports: 




# MustLink: src/LinkRavlFileIDX.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlFileIDX 1)

include_directories(../../Core/RavlCore/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../OS/RavlOSIO/include)
include_directories(../../Math/RavlMath/include)
include_directories(../../OS/RavlOS/include)
include_directories(../../OS/RavlXMLFactory/include)
include_directories(../../OS/RavlThreads/include)
include_directories(../../PatternRec/RavlDataSet/include)
include_directories(../../PatternRec/RavlPatternRec/include)
include_directories(../../PatternRec/RavlPatternRecIO/include)




add_library(RavlFileIDX_Body STATIC
 include/Ravl/IO/DataStreamIDX.hh include/Ravl/IO/IPortIDX.hh include/Ravl/IO/OPortIDX.hh include/Ravl/IO/IPortIDXFormat.hh include/Ravl/IO/FileUtilities.hh
 src/DataStreamIDX.cc src/IPortIDX.cc src/OPortIDX.cc src/IPortIDXFormat.cc src/FileUtilities.cc
)
target_link_libraries(RavlFileIDX_Body PUBLIC RavlCore RavlIO RavlOSIO RavlMath RavlOS RavlXMLFactory RavlThreads RavlDataSet )

add_library(RavlFileIDX OBJECT src/LinkRavlFileIDX.cc )
target_link_libraries(RavlFileIDX PUBLIC RavlFileIDX_Body )


target_include_directories(
    RavlFileIDX PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlFileIDX_BodyConfig DESTINATION lib/cmake/RavlFileIDX_Body/Target)
install(TARGETS RavlFileIDX_Body EXPORT RavlFileIDX_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlFileIDX_Body DESTINATION lib)

configure_package_config_file(cmake/RavlFileIDX_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlFileIDX_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlFileIDX_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlFileIDX_BodyConfig.cmake
        DESTINATION lib/cmake/RavlFileIDX_Body )


install(TARGETS RavlFileIDX EXPORT RavlFileIDXConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlFileIDXConfig DESTINATION lib/cmake/RavlFileIDX/Target)
install(TARGETS RavlFileIDX DESTINATION lib)

configure_package_config_file(cmake/RavlFileIDXConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlFileIDXConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlFileIDX
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlFileIDXConfig.cmake
        DESTINATION lib/cmake/RavlFileIDX )

install(DIRECTORY include DESTINATION .)


# Mains
set(MAIN_LIBS  RavlCore RavlIO RavlOSIO RavlMath RavlOS RavlXMLFactory RavlThreads RavlDataSet RavlPatternRec RavlPatternRecIO  )

add_executable(doFileIDX ${EXCLUDE_APPLICATIONS} applications/doFileIDX.cc )
target_link_libraries(doFileIDX RavlFileIDX ${MAIN_LIBS} )
install(TARGETS doFileIDX DESTINATION bin ${EXCLUDE_APPLICATIONS})

add_executable(doPrepData ${EXCLUDE_APPLICATIONS} applications/doPrepData.cc )
target_link_libraries(doPrepData RavlFileIDX ${MAIN_LIBS} )
install(TARGETS doPrepData DESTINATION bin ${EXCLUDE_APPLICATIONS})








