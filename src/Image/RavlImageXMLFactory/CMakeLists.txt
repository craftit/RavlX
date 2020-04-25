
# Library: RavlImageXMLFactory

# Requires: 
# Supports: 




# MustLink: src/linkRavlImageXMLFactory.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlImageXMLFactory 1)

include_directories(../../Core/RavlIO/include)
include_directories(../../OS/RavlXMLFactory/include)
include_directories(../../Image/RavlImage/include)
include_directories(../../OS/RavlOSIO/include)
include_directories(../../Image/RavlImageIO/include)
include_directories(../../Core/RavlCore/include)
include_directories(../../OS/RavlThreads/include)
include_directories(../../OS/RavlOS/include)
include_directories(../../Math/RavlMath/include)




add_library(RavlImageXMLFactory_Body STATIC
 include/Ravl/Image/XMLFactoryVideoIO.hh
 src/XMLFactoryVideoIO.cc
)
target_link_libraries(RavlImageXMLFactory_Body PUBLIC RavlIO RavlXMLFactory RavlImage RavlOSIO RavlImageIO RavlCore RavlThreads RavlOS RavlMath )

add_library(RavlImageXMLFactory OBJECT src/linkRavlImageXMLFactory.cc )
target_link_libraries(RavlImageXMLFactory PUBLIC RavlImageXMLFactory_Body )


target_include_directories(
    RavlImageXMLFactory PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlImageXMLFactory_BodyConfig DESTINATION lib/cmake/RavlImageXMLFactory_Body/Target)
install(TARGETS RavlImageXMLFactory_Body EXPORT RavlImageXMLFactory_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlImageXMLFactory_Body DESTINATION lib)

configure_package_config_file(cmake/RavlImageXMLFactory_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlImageXMLFactory_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlImageXMLFactory_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlImageXMLFactory_BodyConfig.cmake
        DESTINATION lib/cmake/RavlImageXMLFactory_Body )


install(TARGETS RavlImageXMLFactory EXPORT RavlImageXMLFactoryConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlImageXMLFactoryConfig DESTINATION lib/cmake/RavlImageXMLFactory/Target)
install(TARGETS RavlImageXMLFactory DESTINATION lib)

configure_package_config_file(cmake/RavlImageXMLFactoryConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlImageXMLFactoryConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlImageXMLFactory
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlImageXMLFactoryConfig.cmake
        DESTINATION lib/cmake/RavlImageXMLFactory )

install(DIRECTORY include DESTINATION .)










