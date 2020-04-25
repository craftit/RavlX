
# Library: RavlImgIOJasper

# Requires: LibJasper
# Supports: 



if(libjasper_FOUND)

# MustLink: src/JasperImgIO.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlImgIOJasper 1)

include_directories(../../Image/RavlImage/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../Core/RavlCore/include)



if(DEFINED HAVE_RavlDPDisplay)
include_directories(../../GUI/RavlDPDisplay/include)
endif()

add_library(RavlImgIOJasper_Body STATIC
 include/Ravl/Image/ImgIOJasper.hh include/Ravl/Image/JasperFormat.hh include/Ravl/Image/CompressedImageJ2k.hh
 src/ImgIOJasper.cc src/JasperFormat.cc src/CompressedImageJ2k.cc
)
target_link_libraries(RavlImgIOJasper_Body PUBLIC RavlImage ${libjasper_LIBRARIES} RavlIO RavlCore )

add_library(RavlImgIOJasper OBJECT src/JasperImgIO.cc )
target_link_libraries(RavlImgIOJasper PUBLIC RavlImgIOJasper_Body )


target_include_directories(
    RavlImgIOJasper PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlImgIOJasper_BodyConfig DESTINATION lib/cmake/RavlImgIOJasper_Body/Target)
install(TARGETS RavlImgIOJasper_Body EXPORT RavlImgIOJasper_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlImgIOJasper_Body DESTINATION lib)

configure_package_config_file(cmake/RavlImgIOJasper_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlImgIOJasper_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlImgIOJasper_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlImgIOJasper_BodyConfig.cmake
        DESTINATION lib/cmake/RavlImgIOJasper_Body )


install(TARGETS RavlImgIOJasper EXPORT RavlImgIOJasperConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlImgIOJasperConfig DESTINATION lib/cmake/RavlImgIOJasper/Target)
install(TARGETS RavlImgIOJasper DESTINATION lib)

configure_package_config_file(cmake/RavlImgIOJasperConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlImgIOJasperConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlImgIOJasper
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlImgIOJasperConfig.cmake
        DESTINATION lib/cmake/RavlImgIOJasper )

install(DIRECTORY include DESTINATION .)




# Tests
set(TEST_LIBS  RavlImage ${libjasper_LIBRARIES} RavlIO RavlCore  )

add_executable(testImgIOJasper EXCLUDE_FROM_ALL tests/testImgIOJasper.cc )
target_link_libraries(testImgIOJasper RavlImgIOJasper ${TEST_LIBS} )
add_test(testImgIOJasper testImgIOJasper)
install(TARGETS testImgIOJasper DESTINATION tests COMPONENT unittests EXCLUDE_FROM_ALL)




ENDIF()


