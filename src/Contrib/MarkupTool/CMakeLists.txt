
# Library: MarkupTool

# Requires: RLog libGTK2
# Supports: 



if(librlog_FOUND)
if(GTK2_FOUND)



set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_MarkupTool 1)

include_directories(../../GUI/RavlGUI/include)
include_directories(../../Contrib/RavlFace/include)
include_directories(../../GUI/RavlGUI2D/include)
include_directories(../../Contrib/RavlRLog/include)
include_directories(../../Core/RavlCore/include)
include_directories(../../OS/RavlThreads/include)
include_directories(../../Image/RavlImage/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../Image/RavlImageProc/include)
include_directories(../../OS/RavlOS/include)
include_directories(../../Math/RavlMath/include)
include_directories(../../OS/RavlXMLFactory/include)
include_directories(../../Image/RavlExtImgIO/include)



if(DEFINED HAVE_RavlImgIOJasper)
include_directories(../../Contrib/RavlImgIOJasper/include)
endif()

add_library(MarkupTool STATIC  src/ViewPage.cc src/ControlWin.cc)
target_link_libraries(MarkupTool PUBLIC RavlGUI RavlFace RavlGUI2D RavlRLog ${librlog_LIBRARIES} ${GTK2_LIBRARIES} RavlCore RavlThreads RavlImage RavlIO RavlImageProc RavlOS RavlMath RavlXMLFactory )


target_include_directories(
    MarkupTool PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )



install(TARGETS MarkupTool EXPORT MarkupToolConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT MarkupToolConfig DESTINATION lib/cmake/MarkupTool/Target)
install(TARGETS MarkupTool DESTINATION lib)

configure_package_config_file(cmake/MarkupToolConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/MarkupToolConfig.cmake
  INSTALL_DESTINATION lib/cmake/MarkupTool
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/MarkupToolConfig.cmake
        DESTINATION lib/cmake/MarkupTool )

install(DIRECTORY include DESTINATION .)


# Mains
set(MAIN_LIBS  RavlGUI RavlFace RavlGUI2D RavlRLog ${librlog_LIBRARIES} ${GTK2_LIBRARIES} RavlCore RavlThreads RavlImage RavlIO RavlImageProc RavlOS RavlMath RavlXMLFactory RavlExtImgIO  )

add_executable(markupTool ${EXCLUDE_APPLICATIONS} applications/markupTool.cc )
target_link_libraries(markupTool MarkupTool ${MAIN_LIBS} )
install(TARGETS markupTool DESTINATION bin ${EXCLUDE_APPLICATIONS})






ENDIF()
ENDIF()


