
# Library: RavlLibMPEG2

# Requires: libmpeg2
# Supports: !VCPP

if((NOT ${CMAKE_CXX_COMPILER_ID} MATCHES "MSVC") OR FALSE)

if(libmpeg2_FOUND)

# MustLink: src/MustLinkLibMPEG2.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlLibMPEG2 1)

include_directories(../../Image/RavlImage/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../Core/RavlCore/include)
include_directories(../../OS/RavlOS/include)



if(DEFINED HAVE_RavlGUI)
include_directories(../../GUI/RavlGUI/include)
endif()
if(DEFINED HAVE_RavlDPDisplay)
include_directories(../../GUI/RavlDPDisplay/include)
endif()

add_library(RavlLibMPEG2_Body STATIC
 include/Ravl/Image/ImgIOMPEG2.hh include/Ravl/Image/LibMPEG2Format.hh include/Ravl/Image/MPEG2Demux.hh
 src/ImgIOMPEG2.cc src/LibMPEG2Format.cc src/MPEG2Demux.cc
)
target_link_libraries(RavlLibMPEG2_Body PUBLIC RavlImage RavlIO ${libmpeg2_LIBRARIES} RavlCore RavlOS )

add_library(RavlLibMPEG2 OBJECT src/MustLinkLibMPEG2.cc )
target_link_libraries(RavlLibMPEG2 PUBLIC RavlLibMPEG2_Body )


target_include_directories(
    RavlLibMPEG2 PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlLibMPEG2_BodyConfig DESTINATION lib/cmake/RavlLibMPEG2_Body/Target)
install(TARGETS RavlLibMPEG2_Body EXPORT RavlLibMPEG2_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlLibMPEG2_Body DESTINATION lib)

configure_package_config_file(cmake/RavlLibMPEG2_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlLibMPEG2_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlLibMPEG2_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlLibMPEG2_BodyConfig.cmake
        DESTINATION lib/cmake/RavlLibMPEG2_Body )


install(TARGETS RavlLibMPEG2 EXPORT RavlLibMPEG2Config
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlLibMPEG2Config DESTINATION lib/cmake/RavlLibMPEG2/Target)
install(TARGETS RavlLibMPEG2 DESTINATION lib)

configure_package_config_file(cmake/RavlLibMPEG2Config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlLibMPEG2Config.cmake
  INSTALL_DESTINATION lib/cmake/RavlLibMPEG2
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlLibMPEG2Config.cmake
        DESTINATION lib/cmake/RavlLibMPEG2 )

install(DIRECTORY include DESTINATION .)



# Examples
set(EXAMPLE_LIBS  RavlImage RavlIO ${libmpeg2_LIBRARIES} RavlCore RavlOS  )

add_executable(exMPEG2 EXCLUDE_FROM_ALL examples/exMPEG2.cc )
target_link_libraries(exMPEG2 RavlLibMPEG2 ${EXAMPLE_LIBS} )
install(TARGETS exMPEG2 DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)

add_executable(exMPEG2Seek EXCLUDE_FROM_ALL examples/exMPEG2Seek.cc )
target_link_libraries(exMPEG2Seek RavlLibMPEG2 ${EXAMPLE_LIBS} )
install(TARGETS exMPEG2Seek DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)





ENDIF()

ENDIF()
