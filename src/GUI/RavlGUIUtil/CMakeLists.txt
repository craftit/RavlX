
# Library: RavlGUIUtil

# Requires: libGTK2
# Supports: 



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

set(HAVE_RavlGUIUtil 1)

include_directories(../../GUI/RavlGUI/include)
include_directories(../../Core/RavlCore/include)
include_directories(../../Core/RavlIO/include)
include_directories(../../GUI/RavlGUI2D/include)
include_directories(../../OS/RavlThreads/include)
include_directories(../../Image/RavlImage/include)
include_directories(../../Image/RavlImageProc/include)
include_directories(../../Math/RavlMath/include)
include_directories(../../OS/RavlOS/include)




add_library(RavlGUIUtil STATIC include/Ravl/GUI/AttributeEditor.hh include/Ravl/GUI/AttributeEditorWindow.hh include/Ravl/GUI/ThumbNailTimeLine.hh include/Ravl/GUI/EventTimeLine.hh src/AttributeEditor.cc src/AttributeEditorWindow.cc src/ThumbNailTimeLine.cc src/EventTimeLine.cc)
target_link_libraries(RavlGUIUtil PUBLIC RavlGUI RavlCore RavlIO RavlGUI2D ${GTK2_LIBRARIES} RavlThreads RavlImage RavlImageProc RavlMath RavlOS )


target_include_directories(
    RavlGUIUtil PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )



install(TARGETS RavlGUIUtil EXPORT RavlGUIUtilConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlGUIUtilConfig DESTINATION lib/cmake/RavlGUIUtil/Target)
install(TARGETS RavlGUIUtil DESTINATION lib)

configure_package_config_file(cmake/RavlGUIUtilConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlGUIUtilConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlGUIUtil
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlGUIUtilConfig.cmake
        DESTINATION lib/cmake/RavlGUIUtil )

install(DIRECTORY include DESTINATION .)



# Examples
set(EXAMPLE_LIBS  RavlGUI RavlCore RavlIO RavlGUI2D ${GTK2_LIBRARIES} RavlThreads RavlImage RavlImageProc RavlMath RavlOS  )

add_executable(exAttributeEditor ${EXCLUDE_EXAMPLES} examples/exAttributeEditor.cc )
target_link_libraries(exAttributeEditor RavlGUIUtil ${EXAMPLE_LIBS} )
install(TARGETS exAttributeEditor DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})





ENDIF()


