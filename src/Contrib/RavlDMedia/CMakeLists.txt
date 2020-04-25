
# Library: RavlDMedia

# Requires: SGIVL
# Supports: sgi

if((${CMAKE_SYSTEM_NAME} MATCHES "SGI") OR FALSE)

if(SGI)

# MustLink: src/RavlInitDMediaIO.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlDMedia 1)

include_directories(../../Core/RavlCore/include)
include_directories(../../OS/RavlOSIO/include)
include_directories(../../OS/RavlOS/include)
include_directories(../../OS/RavlThreads/include)
include_directories(../../Image/RavlVideoIO/include)
include_directories(../../Image/RavlImageIO/include)



if(DEFINED HAVE_RavlDPDisplay)
include_directories(../../GUI/RavlDPDisplay/include)
endif()

add_library(RavlDMedia_Body STATIC
 include/Ravl/DMedia/Server.hh include/Ravl/DMedia/NodePath.hh include/Ravl/DMedia/RingMemory.hh include/Ravl/DMedia/Video.hh include/Ravl/DMedia/RingData.hh include/Ravl/DMedia/Memory.hh include/Ravl/DMedia/Buffer.hh include/Ravl/DMedia/DIVO.hh include/Ravl/DMedia/DIVOFormat.hh
 src/Server.cc src/NodePath.cc src/RingMemory.cc src/Video.cc src/RingData.cc src/Memory.cc src/DIVO.cc src/DIVOFormat.cc
)
target_link_libraries(RavlDMedia_Body PUBLIC RavlCore RavlOSIO RavlOS RavlThreads  )

add_library(RavlDMedia OBJECT src/RavlInitDMediaIO.cc )
target_link_libraries(RavlDMedia PUBLIC RavlDMedia_Body )


target_include_directories(
    RavlDMedia PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlDMedia_BodyConfig DESTINATION lib/cmake/RavlDMedia_Body/Target)
install(TARGETS RavlDMedia_Body EXPORT RavlDMedia_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlDMedia_Body DESTINATION lib)

configure_package_config_file(cmake/RavlDMedia_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlDMedia_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlDMedia_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlDMedia_BodyConfig.cmake
        DESTINATION lib/cmake/RavlDMedia_Body )


install(TARGETS RavlDMedia EXPORT RavlDMediaConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlDMediaConfig DESTINATION lib/cmake/RavlDMedia/Target)
install(TARGETS RavlDMedia DESTINATION lib)

configure_package_config_file(cmake/RavlDMediaConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlDMediaConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlDMedia
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlDMediaConfig.cmake
        DESTINATION lib/cmake/RavlDMedia )

install(DIRECTORY include DESTINATION .)


# Mains
set(MAIN_LIBS  RavlCore RavlOSIO RavlOS RavlThreads  RavlVideoIO RavlImageIO  )

add_executable(exDMediaIO ${EXCLUDE_APPLICATIONS} applications/exDMediaIO.cc )
target_link_libraries(exDMediaIO RavlDMedia ${MAIN_LIBS} )
install(TARGETS exDMediaIO DESTINATION bin ${EXCLUDE_APPLICATIONS})

# Examples
set(EXAMPLE_LIBS  RavlCore RavlOSIO RavlOS RavlThreads  RavlVideoIO RavlImageIO  )

add_executable(exDMedia ${EXCLUDE_EXAMPLES} examples/exDMedia.cc )
target_link_libraries(exDMedia RavlDMedia ${EXAMPLE_LIBS} )
install(TARGETS exDMedia DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})





ENDIF()

ENDIF()
