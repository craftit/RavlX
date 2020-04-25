
# Library: RavlLibGlade

# Requires: libglade2 libGTK2
# Supports: 



if(libglade_FOUND)
if(GTK2_FOUND)

# MustLink: src/linkRavlLibGlade.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlLibGlade 1)

include_directories(../../GUI/RavlGUI/include)
include_directories(../../OS/RavlXMLFactory/include)
include_directories(../../Core/RavlCore/include)
include_directories(../../OS/RavlThreads/include)
include_directories(../../Image/RavlImage/include)
include_directories(../../Math/RavlMath/include)
include_directories(../../OS/RavlOS/include)
include_directories(../../Core/RavlIO/include)




add_library(RavlLibGlade_Body STATIC
 include/Ravl/GUI/GladeWidget.hh include/Ravl/GUI/GladeXML.hh include/Ravl/GUI/GladeWindow.hh
 src/GladeWidget.cc src/GladeXML.cc src/GladeWindow.cc
)
target_link_libraries(RavlLibGlade_Body PUBLIC RavlGUI RavlXMLFactory ${libglade_LIBRARIES} RavlCore RavlThreads RavlImage RavlMath RavlOS RavlIO )

add_library(RavlLibGlade OBJECT src/linkRavlLibGlade.cc )
target_link_libraries(RavlLibGlade PUBLIC RavlLibGlade_Body )


target_include_directories(
    RavlLibGlade PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlLibGlade_BodyConfig DESTINATION lib/cmake/RavlLibGlade_Body/Target)
install(TARGETS RavlLibGlade_Body EXPORT RavlLibGlade_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlLibGlade_Body DESTINATION lib)

configure_package_config_file(cmake/RavlLibGlade_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlLibGlade_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlLibGlade_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlLibGlade_BodyConfig.cmake
        DESTINATION lib/cmake/RavlLibGlade_Body )


install(TARGETS RavlLibGlade EXPORT RavlLibGladeConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlLibGladeConfig DESTINATION lib/cmake/RavlLibGlade/Target)
install(TARGETS RavlLibGlade DESTINATION lib)

configure_package_config_file(cmake/RavlLibGladeConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlLibGladeConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlLibGlade
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlLibGladeConfig.cmake
        DESTINATION lib/cmake/RavlLibGlade )

install(DIRECTORY include DESTINATION .)



# Examples
set(EXAMPLE_LIBS  RavlGUI RavlXMLFactory ${libglade_LIBRARIES} RavlCore RavlThreads RavlImage RavlMath RavlOS RavlIO  )

add_executable(exLibGlade EXCLUDE_FROM_ALL examples/exLibGlade.cc )
target_link_libraries(exLibGlade RavlLibGlade ${EXAMPLE_LIBS} )
install(TARGETS exLibGlade DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)

add_executable(exLibGladeCustom EXCLUDE_FROM_ALL examples/exLibGladeCustom.cc )
target_link_libraries(exLibGladeCustom RavlLibGlade ${EXAMPLE_LIBS} )
install(TARGETS exLibGladeCustom DESTINATION examples COMPONENT examples EXCLUDE_FROM_ALL)



# === Files for share/RAVL/Glade ===
set(AUXDIR share/RAVL/Glade)
set(AUXFILES 
  data/exlibgladecustommain.glade 
  data/exlibgladecustomwidget.glade 
  data/exlibglade.glade )
file(COPY ${AUXFILES} DESTINATION ${BUILD_ROOT_AUXFILE}/${AUXDIR})
install(FILES ${AUXFILES} DESTINATION ${AUXDIR})



ENDIF()
ENDIF()


