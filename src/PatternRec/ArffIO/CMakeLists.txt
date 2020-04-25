
# Library: ArffIO

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

set(HAVE_ArffIO 1)

include_directories(../../Core/RavlCore/include)
include_directories(../../PatternRec/RavlDataSet/include)
include_directories(../../Math/RavlMath/include)
include_directories(../../OS/RavlOS/include)




add_library(ArffIO STATIC include/Ravl/PatternRec/ArffIO.hh src/ArffIO.cc)
target_link_libraries(ArffIO PUBLIC RavlCore RavlDataSet RavlMath )


target_include_directories(
    ArffIO PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )



install(TARGETS ArffIO EXPORT ArffIOConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT ArffIOConfig DESTINATION lib/cmake/ArffIO/Target)
install(TARGETS ArffIO DESTINATION lib)

configure_package_config_file(cmake/ArffIOConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/ArffIOConfig.cmake
  INSTALL_DESTINATION lib/cmake/ArffIO
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/ArffIOConfig.cmake
        DESTINATION lib/cmake/ArffIO )

install(DIRECTORY include DESTINATION .)


# Mains
set(MAIN_LIBS  RavlCore RavlDataSet RavlMath RavlOS  )

add_executable(cutArff applications/cutArff.cc )
target_link_libraries(cutArff ArffIO ${MAIN_LIBS} )
install(TARGETS cutArff DESTINATION bin)

add_executable(glueArff applications/glueArff.cc )
target_link_libraries(glueArff ArffIO ${MAIN_LIBS} )
install(TARGETS glueArff DESTINATION bin)








