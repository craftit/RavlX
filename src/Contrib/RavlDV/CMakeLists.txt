
# Library: RavlDV

# Requires: libDV
# Supports: linux linux64

if((${CMAKE_SYSTEM_NAME} MATCHES "Linux32") OR (${CMAKE_SYSTEM_NAME} MATCHES "Linux") OR FALSE)

if(libdv_FOUND)

# MustLink: src/RAVLVidIDV.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlDV 1)

include_directories(../../Core/RavlCore/include)
include_directories(../../Image/RavlImage/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../OS/RavlOS/include)
include_directories(../../OS/RavlOSIO/include)
include_directories(../../Image/RavlVideoIO/include)
include_directories(../../Image/RavlAV/include)
include_directories(../../Audio/RavlAudioUtil/include)
include_directories(../../Image/RavlImageProc/include)
include_directories(../../Contrib/RavlDevAudio/include)



if(DEFINED HAVE_RavlDPDisplay)
include_directories(../../GUI/RavlDPDisplay/include)
endif()

add_library(RavlDV_Body STATIC
 include/Ravl/Image/ImgIODv.hh include/Ravl/Image/DvDecode.hh include/Ravl/Image/DvIFormat.hh
 src/ImgIODv.cc src/DvDecode.cc src/DvIFormat.cc src/DvFrameConvert.cc
)
target_link_libraries(RavlDV_Body PUBLIC RavlCore RavlImage RavlIO RavlOS RavlOSIO ${libdv_LIBRARIES} RavlVideoIO RavlAV RavlAudioUtil RavlImageProc )

add_library(RavlDV OBJECT src/RAVLVidIDV.cc )
target_link_libraries(RavlDV PUBLIC RavlDV_Body )


target_include_directories(
    RavlDV PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlDV_BodyConfig DESTINATION lib/cmake/RavlDV_Body/Target)
install(TARGETS RavlDV_Body EXPORT RavlDV_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlDV_Body DESTINATION lib)

configure_package_config_file(cmake/RavlDV_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlDV_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlDV_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlDV_BodyConfig.cmake
        DESTINATION lib/cmake/RavlDV_Body )


install(TARGETS RavlDV EXPORT RavlDVConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlDVConfig DESTINATION lib/cmake/RavlDV/Target)
install(TARGETS RavlDV DESTINATION lib)

configure_package_config_file(cmake/RavlDVConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlDVConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlDV
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlDVConfig.cmake
        DESTINATION lib/cmake/RavlDV )

install(DIRECTORY include DESTINATION .)


# Mains
set(MAIN_LIBS  RavlCore RavlImage RavlIO RavlOS RavlOSIO ${libdv_LIBRARIES} RavlVideoIO RavlAV RavlAudioUtil RavlImageProc RavlDevAudio  )

add_executable(doDvDisplay applications/doDvDisplay.cc )
target_link_libraries(doDvDisplay RavlDV ${MAIN_LIBS} )
install(TARGETS doDvDisplay DESTINATION bin)






ENDIF()

ENDIF()
