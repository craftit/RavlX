
# Library: RavlIO

# Requires: 
# Supports: 




# MustLink: src/linkCoreIO.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlIO 1)

include_directories(../../Core/RavlCore/include)




add_library(RavlIO_Body STATIC
 include/Ravl/DP/Entity.hh include/Ravl/DP/Process.hh include/Ravl/DP/Port.hh include/Ravl/DP/SPort.hh include/Ravl/DP/SPortAttach.hh include/Ravl/DP/StreamOp.hh include/Ravl/DP/IOJoin.hh include/Ravl/DP/ProcOStream.hh include/Ravl/DP/ProcIStream.hh include/Ravl/DP/Func2Proc.hh include/Ravl/DP/FileIO.hh include/Ravl/DP/BinFileIO.hh include/Ravl/DP/FuncP2Proc.hh include/Ravl/DP/ProcCompose.hh include/Ravl/DP/ProcInfo.hh include/Ravl/DP/Converter.hh include/Ravl/DP/DataConv.hh include/Ravl/DP/TypeInfo.hh include/Ravl/DP/Pipes.hh include/Ravl/DP/FileFormat.hh include/Ravl/DP/FileFormatDesc.hh include/Ravl/DP/FileFormatStream.hh include/Ravl/DP/FileFormatBinStream.hh include/Ravl/DP/FileFormatGen.hh include/Ravl/DP/FileFormatIO.hh include/Ravl/DP/TypeConverter.hh include/Ravl/DP/FileFormatRegistry.hh include/Ravl/DP/CoreIO.hh include/Ravl/DP/ComposeSingle.hh include/Ravl/DP/Compose.hh include/Ravl/DP/Func2Stream.hh include/Ravl/DP/IOConnect.hh include/Ravl/DP/IFailOver.hh include/Ravl/DP/IOTap.hh include/Ravl/DP/Multiplex.hh include/Ravl/DP/SplitO.hh include/Ravl/DP/Event.hh include/Ravl/DP/EventSet.hh include/Ravl/DP/Method2Proc.hh include/Ravl/DP/MethodIO.hh include/Ravl/DP/ContainerIO.hh include/Ravl/DP/Plug.hh include/Ravl/DP/Tap.hh include/Ravl/DP/StreamProcess.hh include/Ravl/DP/Process21.hh include/Ravl/DP/Process31.hh include/Ravl/DP/Method2Proc21.hh include/Ravl/DP/Method2Proc31.hh include/Ravl/DP/StreamOp21.hh include/Ravl/DP/StreamOp31.hh include/Ravl/DP/Func2Proc21.hh include/Ravl/DP/Func2Proc31.hh include/Ravl/DP/OffsetScale.hh include/Ravl/DP/RunningAverage.hh include/Ravl/DP/IOPort.hh include/Ravl/DP/XMLFileIO.hh include/Ravl/DP/FileFormatXMLStream.hh include/Ravl/DP/ListIO.hh include/Ravl/DP/AttributeType.hh include/Ravl/DP/AttributeCtrl.hh include/Ravl/DP/AttributeValueTypes.hh include/Ravl/DP/AttributeSet.hh include/Ravl/DP/AttributeCtrlInternal.hh include/Ravl/DP/ByteFileIO.hh include/Ravl/DP/SampleStream.hh include/Ravl/DP/CacheIStream.hh include/Ravl/DP/ByteStream.hh include/Ravl/DP/SByteStream.hh include/Ravl/DP/MemIO.hh include/Ravl/DP/AttributeCtrlUpdateSignal.hh include/Ravl/DP/PrintIOInfo.hh include/Ravl/DP/MethodPtrIO.hh include/Ravl/DP/FileFormatByteFile.hh include/Ravl/DP/MethodPtr2Proc.hh include/Ravl/DP/FileFormatBinStreamMeta.hh include/Ravl/DP/FileFormatStreamMeta.hh
 src/Entity.cc src/Process.cc src/Port.cc src/SPort.cc src/SPortAttach.cc src/ProcInfo.cc src/Converter.cc src/TypeInfo.cc src/Pipes.cc src/FileFormat.cc src/FileFormatDesc.cc src/FileFormatStream.cc src/FileFormatBinStream.cc src/FileFormatGen.cc src/FileFormatIO.cc src/TypeConverter.cc src/FileFormatRegistry.cc src/IndexIO.cc src/Event.cc src/EventSet.cc src/Plug.cc src/StreamOp.cc src/StreamProcess.cc src/IOConnect.cc src/FileFormatXMLStream.cc src/DataConv.cc src/AttributeCtrl.cc src/AttributeType.cc src/AttributeValueTypes.cc src/AttributeCtrlXML.cc src/AttributeSet.cc src/ByteFileIO.cc src/ByteStream.cc src/SByteStream.cc src/AttributeCtrlInternal.cc src/AttributeCtrlUpdateSignal.cc src/PrintIOInfo.cc src/FileFormatByteFile.cc src/CoreIO.cc src/FileFormatBinStreamMeta.cc src/FileFormatStreamMeta.cc src/ProcIStream.cc src/ProcOStream.cc
)
target_link_libraries(RavlIO_Body PUBLIC RavlCore )

add_library(RavlIO OBJECT src/linkCoreIO.cc )
target_link_libraries(RavlIO PUBLIC RavlIO_Body )


target_include_directories(
    RavlIO PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlIO_BodyConfig DESTINATION lib/cmake/RavlIO_Body/Target)
install(TARGETS RavlIO_Body EXPORT RavlIO_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlIO_Body DESTINATION lib)

configure_package_config_file(cmake/RavlIO_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlIO_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlIO_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlIO_BodyConfig.cmake
        DESTINATION lib/cmake/RavlIO_Body )


install(TARGETS RavlIO EXPORT RavlIOConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlIOConfig DESTINATION lib/cmake/RavlIO/Target)
install(TARGETS RavlIO DESTINATION lib)

configure_package_config_file(cmake/RavlIOConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlIOConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlIO
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlIOConfig.cmake
        DESTINATION lib/cmake/RavlIO )

install(DIRECTORY include DESTINATION .)



# Examples
set(EXAMPLE_LIBS  RavlCore  )

add_executable(exComposeSingle ${EXCLUDE_EXAMPLES} examples/exComposeSingle.cc )
target_link_libraries(exComposeSingle RavlIO ${EXAMPLE_LIBS} )
install(TARGETS exComposeSingle DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exDataProc ${EXCLUDE_EXAMPLES} examples/exDataProc.cc )
target_link_libraries(exDataProc RavlIO ${EXAMPLE_LIBS} )
install(TARGETS exDataProc DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exFileFormat ${EXCLUDE_EXAMPLES} examples/exFileFormat.cc )
target_link_libraries(exFileFormat RavlIO ${EXAMPLE_LIBS} )
install(TARGETS exFileFormat DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

# Tests
set(TEST_LIBS  RavlCore  )

add_executable(testDataProc ${EXCLUDE_UNITTESTS} tests/testDataProc.cc )
target_link_libraries(testDataProc RavlIO ${TEST_LIBS} )
add_test(testDataProc testDataProc)
install(TARGETS testDataProc DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})






