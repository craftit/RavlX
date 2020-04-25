
# Library: RavlSerialIO

# Requires: 
# Supports: 




# MustLink: src/linkSerialDirect.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlSerialIO 1)

include_directories(../../Core/RavlCore/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../OS/RavlOS/include)




add_library(RavlSerialIO_Body STATIC
 include/Ravl/OS/SerialAbstract.hh include/Ravl/OS/SerialDirect.hh include/Ravl/OS/SerialDFormat.hh include/Ravl/OS/SerialIO.hh include/Ravl/OS/SerialAbstractPort.hh
 src/SerialAbstract.cc src/SerialDirect.cc src/SerialDFormat.cc src/SerialIO.cc
)
target_link_libraries(RavlSerialIO_Body PUBLIC RavlCore RavlIO RavlOS )

add_library(RavlSerialIO OBJECT src/linkSerialDirect.cc )
target_link_libraries(RavlSerialIO PUBLIC RavlSerialIO_Body )


target_include_directories(
    RavlSerialIO PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlSerialIO_BodyConfig DESTINATION lib/cmake/RavlSerialIO_Body/Target)
install(TARGETS RavlSerialIO_Body EXPORT RavlSerialIO_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlSerialIO_Body DESTINATION lib)

configure_package_config_file(cmake/RavlSerialIO_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlSerialIO_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlSerialIO_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlSerialIO_BodyConfig.cmake
        DESTINATION lib/cmake/RavlSerialIO_Body )


install(TARGETS RavlSerialIO EXPORT RavlSerialIOConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlSerialIOConfig DESTINATION lib/cmake/RavlSerialIO/Target)
install(TARGETS RavlSerialIO DESTINATION lib)

configure_package_config_file(cmake/RavlSerialIOConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlSerialIOConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlSerialIO
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlSerialIOConfig.cmake
        DESTINATION lib/cmake/RavlSerialIO )

install(DIRECTORY include DESTINATION .)










