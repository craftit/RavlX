
# Library: RavlAV

# Requires: 
# Supports: 






set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlAV 1)

include_directories(../../Audio/RavlAudioUtil/include)
include_directories(../../Image/RavlImageIO/include)
include_directories(../../Core/RavlCore/include)
include_directories(../../Image/RavlImage/include)
include_directories(../../Core/RavlIO/include)




add_library(RavlAV STATIC include/Ravl/AVFrame.hh include/Ravl/AVMixer.hh src/AVFrame.cc src/AVMixer.cc)
target_link_libraries(RavlAV PUBLIC RavlAudioUtil RavlImageIO RavlCore RavlImage RavlIO )


target_include_directories(
    RavlAV PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )



install(TARGETS RavlAV EXPORT RavlAVConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlAVConfig DESTINATION lib/cmake/RavlAV/Target)
install(TARGETS RavlAV DESTINATION lib)

configure_package_config_file(cmake/RavlAVConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlAVConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlAV
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlAVConfig.cmake
        DESTINATION lib/cmake/RavlAV )

install(DIRECTORY include DESTINATION .)










