
# Library: RavlHtml

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

set(HAVE_RavlHtml 1)

include_directories(../../Core/RavlCore/include)
include_directories(../../OS/RavlOS/include)




add_library(RavlHtml STATIC include/Ravl/HtmlString.hh include/Ravl/HtmlTable.hh include/Ravl/HtmlPage.hh src/HtmlString.cc src/HtmlTable.cc src/HtmlPage.cc)
target_link_libraries(RavlHtml PUBLIC RavlCore RavlOS )


target_include_directories(
    RavlHtml PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )



install(TARGETS RavlHtml EXPORT RavlHtmlConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlHtmlConfig DESTINATION lib/cmake/RavlHtml/Target)
install(TARGETS RavlHtml DESTINATION lib)

configure_package_config_file(cmake/RavlHtmlConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlHtmlConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlHtml
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlHtmlConfig.cmake
        DESTINATION lib/cmake/RavlHtml )

install(DIRECTORY include DESTINATION .)










