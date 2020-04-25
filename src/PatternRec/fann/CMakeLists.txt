
# Library: fann

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

set(HAVE_fann 1)

include_directories(../../Core/RavlCore/include)




add_library(fann STATIC include/Ravl/fann/fann.h include/Ravl/fann/compat_time.h include/Ravl/fann/fann_activation.h include/Ravl/fann/fann_error.h include/Ravl/fann/fann_io.h include/Ravl/fann/floatfann.h include/Ravl/fann/fann_cascade.h include/Ravl/fann/fann_train.h include/Ravl/fann/doublefann.h include/Ravl/fann/fann_data.h include/Ravl/fann/fann_internal.h include/Ravl/fann/fixedfann.h include/Ravl/fann/fann_common.h src/floatfann.c)
target_link_libraries(fann PUBLIC RavlCore )


target_include_directories(
    fann PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )



install(TARGETS fann EXPORT fannConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT fannConfig DESTINATION lib/cmake/fann/Target)
install(TARGETS fann DESTINATION lib)

configure_package_config_file(cmake/fannConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/fannConfig.cmake
  INSTALL_DESTINATION lib/cmake/fann
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/fannConfig.cmake
        DESTINATION lib/cmake/fann )

install(DIRECTORY include DESTINATION .)










