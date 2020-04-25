
# Library: RavlImgIOMeteor1

# Requires: libGTK2 Meteor1
# Supports: linux

if((${CMAKE_SYSTEM_NAME} MATCHES "Linux32") OR FALSE)

if(GTK2_FOUND)
if(Meteor1_FOUND)

# MustLink: src/InitMeteor1.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlImgIOMeteor1 1)

include_directories(../../Image/RavlImageIO/include)
include_directories(../../Core/RavlCore/include)
include_directories(../../Image/RavlImage/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../GUI/RavlGUI/include)
include_directories(../../GUI/RavlGUI2D/include)




add_library(RavlImgIOMeteor1_Body STATIC
 include/Ravl/Image/ImgIOMeteor1.hh include/Ravl/Image/Meteor1Format.hh
 src/ImgIOMeteor1.cc src/Meteor1Format.cc
)
target_link_libraries(RavlImgIOMeteor1_Body PUBLIC RavlImageIO RavlCore RavlImage RavlIO )

add_library(RavlImgIOMeteor1 OBJECT src/InitMeteor1.cc )
target_link_libraries(RavlImgIOMeteor1 PUBLIC RavlImgIOMeteor1_Body )


target_include_directories(
    RavlImgIOMeteor1 PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlImgIOMeteor1_BodyConfig DESTINATION lib/cmake/RavlImgIOMeteor1_Body/Target)
install(TARGETS RavlImgIOMeteor1_Body EXPORT RavlImgIOMeteor1_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlImgIOMeteor1_Body DESTINATION lib)

configure_package_config_file(cmake/RavlImgIOMeteor1_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlImgIOMeteor1_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlImgIOMeteor1_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlImgIOMeteor1_BodyConfig.cmake
        DESTINATION lib/cmake/RavlImgIOMeteor1_Body )


install(TARGETS RavlImgIOMeteor1 EXPORT RavlImgIOMeteor1Config
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlImgIOMeteor1Config DESTINATION lib/cmake/RavlImgIOMeteor1/Target)
install(TARGETS RavlImgIOMeteor1 DESTINATION lib)

configure_package_config_file(cmake/RavlImgIOMeteor1Config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlImgIOMeteor1Config.cmake
  INSTALL_DESTINATION lib/cmake/RavlImgIOMeteor1
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlImgIOMeteor1Config.cmake
        DESTINATION lib/cmake/RavlImgIOMeteor1 )

install(DIRECTORY include DESTINATION .)


# Mains
set(MAIN_LIBS  RavlImageIO RavlCore RavlImage RavlIO RavlGUI RavlGUI2D  )

add_executable(tuneMeteor ${EXCLUDE_APPLICATIONS} applications/tuneMeteor.cc )
target_link_libraries(tuneMeteor RavlImgIOMeteor1 ${MAIN_LIBS} )
install(TARGETS tuneMeteor DESTINATION bin ${EXCLUDE_APPLICATIONS})

# Examples
set(EXAMPLE_LIBS  RavlImageIO RavlCore RavlImage RavlIO RavlGUI RavlGUI2D  )

add_executable(exMeteor ${EXCLUDE_EXAMPLES} examples/exMeteor.cc )
target_link_libraries(exMeteor RavlImgIOMeteor1 ${EXAMPLE_LIBS} )
install(TARGETS exMeteor DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})





ENDIF()
ENDIF()

ENDIF()
