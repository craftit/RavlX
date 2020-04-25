
# Library: RavlDVDRead

# Requires: dvdread libmpeg2
# Supports: !VCPP

if((NOT ${CMAKE_CXX_COMPILER_ID} MATCHES "MSVC") OR FALSE)

if(dvdread_FOUND)
if(libmpeg2_FOUND)

# MustLink: src/InitDVDFormat.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlDVDRead 1)

include_directories(../../Contrib/RavlLibMPEG2/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../Core/RavlCore/include)
include_directories(../../OS/RavlThreads/include)
include_directories(../../Image/RavlImage/include)



if(DEFINED HAVE_RavlDPDisplay)
include_directories(../../GUI/RavlDPDisplay/include)
endif()

add_library(RavlDVDRead_Body STATIC
 include/Ravl/DVDRead.hh include/Ravl/DVDFormat.hh include/Ravl/ImgIOMPEG2DVD.hh
 src/DVDRead.cc src/DVDFormat.cc src/ImgIOMPEG2DVD.cc
)
target_link_libraries(RavlDVDRead_Body PUBLIC RavlLibMPEG2 ${dvdread_LIBRARIES} RavlIO RavlCore RavlThreads )

add_library(RavlDVDRead OBJECT src/InitDVDFormat.cc )
target_link_libraries(RavlDVDRead PUBLIC RavlDVDRead_Body )


target_include_directories(
    RavlDVDRead PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlDVDRead_BodyConfig DESTINATION lib/cmake/RavlDVDRead_Body/Target)
install(TARGETS RavlDVDRead_Body EXPORT RavlDVDRead_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlDVDRead_Body DESTINATION lib)

configure_package_config_file(cmake/RavlDVDRead_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlDVDRead_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlDVDRead_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlDVDRead_BodyConfig.cmake
        DESTINATION lib/cmake/RavlDVDRead_Body )


install(TARGETS RavlDVDRead EXPORT RavlDVDReadConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlDVDReadConfig DESTINATION lib/cmake/RavlDVDRead/Target)
install(TARGETS RavlDVDRead DESTINATION lib)

configure_package_config_file(cmake/RavlDVDReadConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlDVDReadConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlDVDRead
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlDVDReadConfig.cmake
        DESTINATION lib/cmake/RavlDVDRead )

install(DIRECTORY include DESTINATION .)



# Examples
set(EXAMPLE_LIBS  RavlLibMPEG2 ${dvdread_LIBRARIES} RavlIO RavlCore RavlThreads RavlImage  )

add_executable(exDVDRead ${EXCLUDE_EXAMPLES} examples/exDVDRead.cc )
target_link_libraries(exDVDRead RavlDVDRead ${EXAMPLE_LIBS} )
install(TARGETS exDVDRead DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})





ENDIF()
ENDIF()

ENDIF()
