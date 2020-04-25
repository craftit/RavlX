
# Library: RavlRLog

# Requires: RLog
# Supports: 



if(librlog_FOUND)



set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlRLog 1)

include_directories(../../Core/RavlCore/include)
include_directories(../../OS/RavlOS/include)




add_library(RavlRLog STATIC include/Ravl/RLog.hh include/Ravl/StdioDateNode.hh src/RLog.cc src/StdioDateNode.cc)
target_link_libraries(RavlRLog PUBLIC RavlCore RavlOS ${librlog_LIBRARIES} )


target_include_directories(
    RavlRLog PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )



install(TARGETS RavlRLog EXPORT RavlRLogConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlRLogConfig DESTINATION lib/cmake/RavlRLog/Target)
install(TARGETS RavlRLog DESTINATION lib)

configure_package_config_file(cmake/RavlRLogConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlRLogConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlRLog
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlRLogConfig.cmake
        DESTINATION lib/cmake/RavlRLog )

install(DIRECTORY include DESTINATION .)



# Examples
set(EXAMPLE_LIBS  RavlCore RavlOS ${librlog_LIBRARIES}  )

add_executable(exRLog EXCLUDE_FROM_ALL examples/exRLog.cc )
target_link_libraries(exRLog RavlRLog ${EXAMPLE_LIBS} )
install(TARGETS exRLog DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)





ENDIF()


