
# Library: RavlValidate

# Requires: 
# Supports: !VCPP

if((NOT ${CMAKE_CXX_COMPILER_ID} MATCHES "MSVC") OR FALSE)




set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlValidate 1)

include_directories(../../Core/RavlCore/include)
include_directories(../../OS/RavlOS/include)




add_library(RavlValidate STATIC  src/Extra.cc)
target_link_libraries(RavlValidate PUBLIC )


target_include_directories(
    RavlValidate PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )



install(TARGETS RavlValidate EXPORT RavlValidateConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlValidateConfig DESTINATION lib/cmake/RavlValidate/Target)
install(TARGETS RavlValidate DESTINATION lib)

configure_package_config_file(cmake/RavlValidateConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlValidateConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlValidate
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlValidateConfig.cmake
        DESTINATION lib/cmake/RavlValidate )

install(DIRECTORY include DESTINATION .)


# Mains
set(MAIN_LIBS  RavlCore RavlOS  )

add_executable(Validate applications/Validate.cc )
target_link_libraries(Validate RavlValidate ${MAIN_LIBS} )
install(TARGETS Validate DESTINATION bin)







ENDIF()
