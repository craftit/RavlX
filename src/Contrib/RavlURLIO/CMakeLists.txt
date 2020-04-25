
# Library: RavlURLIO

# Requires: libCurl
# Supports: 



if(CURL_FOUND)

# MustLink: src/URLIO.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlURLIO 1)

include_directories(../../Core/RavlCore/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../OS/RavlOS/include)
include_directories(../../OS/RavlThreads/include)




add_library(RavlURLIO_Body STATIC
 include/Ravl/IO/URLStream.hh include/Ravl/IO/Curl.hh
 src/URLStream.cc src/Curl.cc
)
target_link_libraries(RavlURLIO_Body PUBLIC RavlCore RavlIO RavlOS RavlThreads ${CURL_LIBRARIES} )

add_library(RavlURLIO OBJECT src/URLIO.cc )
target_link_libraries(RavlURLIO PUBLIC RavlURLIO_Body )


target_include_directories(
    RavlURLIO PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlURLIO_BodyConfig DESTINATION lib/cmake/RavlURLIO_Body/Target)
install(TARGETS RavlURLIO_Body EXPORT RavlURLIO_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlURLIO_Body DESTINATION lib)

configure_package_config_file(cmake/RavlURLIO_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlURLIO_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlURLIO_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlURLIO_BodyConfig.cmake
        DESTINATION lib/cmake/RavlURLIO_Body )


install(TARGETS RavlURLIO EXPORT RavlURLIOConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlURLIOConfig DESTINATION lib/cmake/RavlURLIO/Target)
install(TARGETS RavlURLIO DESTINATION lib)

configure_package_config_file(cmake/RavlURLIOConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlURLIOConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlURLIO
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlURLIOConfig.cmake
        DESTINATION lib/cmake/RavlURLIO )

install(DIRECTORY include DESTINATION .)



# Examples
set(EXAMPLE_LIBS  RavlCore RavlIO RavlOS RavlThreads ${CURL_LIBRARIES}  )

add_executable(exURL EXCLUDE_FROM_ALL examples/exURL.cc )
target_link_libraries(exURL RavlURLIO ${EXAMPLE_LIBS} )
install(TARGETS exURL DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)





ENDIF()


