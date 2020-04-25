
# Library: RavlCore

# Requires: 
# Supports: linux64 linux

if((${CMAKE_SYSTEM_NAME} MATCHES "Linux") OR (${CMAKE_SYSTEM_NAME} MATCHES "Linux32") OR FALSE)


# MustLink: src/atexit.cc src/linkVectorUtilsI386SSE.cc


set(DEPENDS_ON )
foreach(D IN LISTS DEPENDS_ON)
  if(NOT ${D}_FOUND)
    find_package(${D} REQUIRED)
  endif()
endforeach()

include(CMakePackageConfigHelpers)

include_directories(include)
include_directories(src)

set(HAVE_RavlCore 1)

include_directories(../../Math/RavlMath/include)
include_directories(../../OS/RavlOS/include)



if(DEFINED HAVE_RavlI386SSE)
include_directories(../../Core/RavlI386SSE/include)
endif()

add_library(RavlCore_Body STATIC
 include/Ravl/Atomic.hh include/Ravl/Types.hh include/Ravl/config.h include/Ravl/Pair.hh include/Ravl/Resource.hh include/Ravl/Assert.hh include/Ravl/Exception.hh include/Ravl/EntryPnt.hh include/Ravl/Index.hh include/Ravl/String.hh include/Ravl/RefCounter.hh include/Ravl/HandleRefCounter.hh include/Ravl/Trigger.hh include/Ravl/Calls.hh include/Ravl/MTLocks.hh include/Ravl/StreamType.hh include/Ravl/Stream.hh include/Ravl/StrStream.hh include/Ravl/BinStream.hh include/Ravl/BitStream.hh include/Ravl/IndexRange1d.hh include/Ravl/Index2d.hh include/Ravl/IndexRange2d.hh include/Ravl/Index3d.hh include/Ravl/RCWrap.hh include/Ravl/Empty.hh include/Ravl/IntC.hh include/Ravl/TFVector.hh include/Ravl/FIndex.hh include/Ravl/StdConst.hh include/Ravl/RCAbstract.hh include/Ravl/Math.hh include/Ravl/CallMethods.hh include/Ravl/CallMethodRefs.hh include/Ravl/DeepCopy.hh include/Ravl/StdHash.hh include/Ravl/Const.hh include/Ravl/IndexRange3d.hh include/Ravl/Average.hh include/Ravl/RealC.hh include/Ravl/SmartPtr.hh include/Ravl/SubIndexRange2dIter.hh include/Ravl/SubIndexRange3dIter.hh include/Ravl/RealRange1d.hh include/Ravl/RealRange2d.hh include/Ravl/RealRange3d.hh include/Ravl/RCBodyV.hh include/Ravl/RCHandleV.hh include/Ravl/IndexRange2dIter.hh include/Ravl/IndexRange3dIter.hh include/Ravl/fdstreambuf.hh include/Ravl/fdstream.hh include/Ravl/GenBinStream.hh include/Ravl/Traits.hh include/Ravl/FPNumber.hh include/Ravl/QInt.hh include/Ravl/CompilerHints.hh include/Ravl/stdio_fdstream.hh include/Ravl/AMutex.hh include/Ravl/RCLayer.hh include/Ravl/SmartLayerPtr.hh include/Ravl/CallMethodPtrs.hh include/Ravl/CPUID.hh include/Ravl/FuncStream.hh include/Ravl/VectorUtils.hh include/Ravl/Float16.hh include/Ravl/RCHandle.hh include/Ravl/UnitTest.hh include/Ravl/Size.hh include/Ravl/QuickSort.hh include/Ravl/ExceptionTimeOut.hh include/Ravl/UUId.hh include/Ravl/SysLog.hh include/Ravl/Buffer.hh include/Ravl/CompositeBuffer.hh include/Ravl/RBfAcc.hh include/Ravl/SBfAcc.hh include/Ravl/BufferAccess.hh include/Ravl/BfAccIter.hh include/Ravl/BfAccIter2.hh include/Ravl/BfAccIter3.hh include/Ravl/BfAccIter4.hh include/Ravl/BfAccIter5.hh include/Ravl/BfAccIter6.hh include/Ravl/BfAccIter7.hh include/Ravl/BfAcc2Iter.hh include/Ravl/BfAcc2Iter2.hh include/Ravl/BfAcc2Iter3.hh include/Ravl/BfAcc2Iter4.hh include/Ravl/BfAcc2Iter5.hh include/Ravl/BfAcc2Iter6.hh include/Ravl/BfAcc2Iter7.hh include/Ravl/Buffer2d.hh include/Ravl/SBfAcc2d.hh include/Ravl/RBfAcc2d.hh include/Ravl/Slice1d.hh include/Ravl/Slice1dIter.hh include/Ravl/Slice1dIter2.hh include/Ravl/Slice1dIter3.hh include/Ravl/Buffer3d.hh include/Ravl/BfAcc3Iter.hh include/Ravl/BfAcc3Iter2.hh include/Ravl/BfAcc3Iter3.hh include/Ravl/SBfAcc3d.hh include/Ravl/RBfAcc3d.hh include/Ravl/BufferAccessIterR.hh include/Ravl/SingleBuffer.hh include/Ravl/SDArray1d.hh include/Ravl/SArray1d.hh include/Ravl/SArray1dIter.hh include/Ravl/SArray1dIter2.hh include/Ravl/SArray1dIter3.hh include/Ravl/SArray1dIter4.hh include/Ravl/SArray1dIter5.hh include/Ravl/SArray2d.hh include/Ravl/SArray2dIter.hh include/Ravl/SArray2dIter2.hh include/Ravl/SArray2dIter3.hh include/Ravl/SArray3d.hh include/Ravl/SArray3dIter.hh include/Ravl/SArray3dIter2.hh include/Ravl/SArray3dIter3.hh include/Ravl/SArray1dIterR.hh include/Ravl/Array1d.hh include/Ravl/Array1dIter.hh include/Ravl/Array1dIter2.hh include/Ravl/Array1dIter3.hh include/Ravl/Array1dIter4.hh include/Ravl/Array1dIter5.hh include/Ravl/Array1dIter6.hh include/Ravl/Array1dIter7.hh include/Ravl/Array2d.hh include/Ravl/Array2dIter.hh include/Ravl/Array2dIter2.hh include/Ravl/Array2dIter3.hh include/Ravl/Array2dIter4.hh include/Ravl/Array2dIter5.hh include/Ravl/Array2dIter6.hh include/Ravl/Array2dIter7.hh include/Ravl/Array3d.hh include/Ravl/Array3dIter.hh include/Ravl/Array3dIter2.hh include/Ravl/Array3dIter3.hh include/Ravl/Array2dSqr2Iter.hh include/Ravl/Array2dSqr2Iter2.hh include/Ravl/Array2dSqr3Iter.hh include/Ravl/Array2dSqr31Iter2.hh include/Ravl/Array2dSqr33Iter2.hh include/Ravl/Array2dSqr311Iter3.hh include/Ravl/Array2dSqr3111Iter4.hh include/Ravl/Array2dSqr3311Iter4.hh include/Ravl/Array1dIterR.hh include/Ravl/Array2dSqr51Iter2.hh include/Ravl/Array2dSqr71Iter2.hh include/Ravl/Stack.hh include/Ravl/StackIter.hh include/Ravl/Tuple2.hh include/Ravl/Tuple3.hh include/Ravl/Tuple4.hh include/Ravl/BlkStack.hh include/Ravl/BlkStackIter.hh include/Ravl/BinTable.hh include/Ravl/BinIter.hh include/Ravl/BinList.hh include/Ravl/Collection.hh include/Ravl/CollectionIter.hh include/Ravl/PriQueue.hh include/Ravl/PriQueueL.hh include/Ravl/HeapNode.hh include/Ravl/BlkQueue.hh include/Ravl/FixedQueue.hh include/Ravl/BinaryTree.hh include/Ravl/AVLTree.hh include/Ravl/BinaryTreeUpIter.hh include/Ravl/BinaryTreeDownIter.hh include/Ravl/HashTree.hh include/Ravl/DLink.hh include/Ravl/DList.hh include/Ravl/DLIter.hh include/Ravl/CDLIter.hh include/Ravl/IntrDList.hh include/Ravl/IntrDLIter.hh include/Ravl/Hash.hh include/Ravl/HashIter.hh include/Ravl/BiHash.hh include/Ravl/RCHash.hh include/Ravl/HSet.hh include/Ravl/HSetExtra.hh include/Ravl/GraphBase.hh include/Ravl/Graph.hh include/Ravl/GraphNode.hh include/Ravl/GraphEdge.hh include/Ravl/GraphAdj.hh include/Ravl/GraphBaseLinearIter.hh include/Ravl/GraphLinearIter.hh include/Ravl/GraphBestRoute.hh include/Ravl/GraphConnIt.hh include/Ravl/GraphBConIt.hh include/Ravl/HEMeshBaseVertex.hh include/Ravl/HEMeshBaseEdge.hh include/Ravl/HEMeshBaseFace.hh include/Ravl/HEMeshBase.hh include/Ravl/HEMeshBaseFaceIter.hh include/Ravl/HEMeshBaseVertexIter.hh include/Ravl/THEMeshVertex.hh include/Ravl/THEMeshEdge.hh include/Ravl/THEMeshFace.hh include/Ravl/THEMesh.hh include/Ravl/THEMeshFaceIter.hh include/Ravl/THEMeshVertexIter.hh include/Ravl/BLink.hh include/Ravl/BList.hh include/Ravl/BListIter.hh include/Ravl/BHash.hh include/Ravl/BHashIter.hh include/Ravl/BStack.hh include/Ravl/BDAGraph.hh include/Ravl/BGraphEdgeIter.hh include/Ravl/BGraphBase.hh include/Ravl/BGraph.hh include/Ravl/BGraphLinearIter.hh include/Ravl/BGraphBaseIter.hh include/Ravl/BGraphNodeIter.hh include/Ravl/BGraphEdge.hh include/Ravl/BGraphNode.hh include/Ravl/StdMath.hh include/Ravl/Random.hh include/Ravl/RandomGauss.hh include/Ravl/Angle.hh include/Ravl/ScalMath.hh include/Ravl/TFMatrix.hh include/Ravl/RandomMersenneTwister.hh include/Ravl/LogValue.hh include/Ravl/RankedScoreArray.hh include/Ravl/Complex.hh include/Ravl/StringList.hh include/Ravl/Option.hh include/Ravl/TypeName.hh include/Ravl/BufStream.hh include/Ravl/SubStringList.hh include/Ravl/XMLStream.hh include/Ravl/XMLTree.hh include/Ravl/IO.hh include/Ravl/BufferSTLVector.hh include/Ravl/DArray1d.hh include/Ravl/DArray1dIter.hh include/Ravl/DArray1dIter2.hh include/Ravl/DArray1dIter3.hh include/Ravl/DArray1dIter4.hh include/Ravl/DArray1dIter5.hh include/Ravl/VirtualConstructor.hh include/Ravl/FunctionRegister.hh include/Ravl/IndexNd.hh include/Ravl/Cache.hh include/Ravl/DListExtra.hh include/Ravl/VariableLengthCode.hh include/Ravl/IndexRange2dSet.hh include/Ravl/Base64.hh include/Ravl/PointerManager.hh include/Ravl/RealRange2dSet.hh include/Ravl/TimeCode.hh include/Ravl/Duration.hh include/Ravl/STL.hh include/Ravl/RCWrapIO.hh include/Ravl/MD5.hh include/Ravl/RealRange1dSet.hh include/Ravl/ParseCSV.hh include/Ravl/StringArray.hh include/Ravl/AString.hh include/Ravl/IndexRangeSet.hh
 src/Assert.cc src/Exception.cc src/EntryPnt.cc src/Index.cc src/String.cc src/RefCounter.cc src/HandleRefCounter.cc src/Trigger.cc src/Calls.cc src/MTLocks.cc src/StreamType.cc src/Stream.cc src/StrStream.cc src/BinStream.cc src/BitStream.cc src/IndexRange1d.cc src/IndexRange2d.cc src/Index2d.cc src/Index3d.cc src/IntC.cc src/StdConst.cc src/RCAbstract.cc src/BinString.cc src/RCWrap.cc src/IndexRange3d.cc src/Math.cc src/RealC.cc src/RealRange1d.cc src/RealRange2d.cc src/RealRange3d.cc src/RCBodyV.cc src/URLMapper.cc src/GenBinStream.cc src/Resource.cc src/AMutex.cc src/RCLayer.cc src/CPUID.cc src/FuncStream.cc src/VectorUtils.cc src/Float16.cc src/Size.cc src/ExceptionTimeOut.cc src/UUId.cc src/SysLog.cc src/VectorUtilsI386SSE.cc src/RBfAcc2d.cc src/SBfAcc2d.cc src/RBfAcc.cc src/SBfAcc.cc src/Stack.cc src/DLink.cc src/DList.cc src/Hash.cc src/GraphBase.cc src/Graph.cc src/GraphBaseLinearIter.cc src/GraphBConIt.cc src/HEMeshBaseVertex.cc src/HEMeshBaseEdge.cc src/HEMeshBaseFace.cc src/HEMeshBase.cc src/HEMeshBaseFaceIter.cc src/BGraphBase.cc src/BGraphBaseIter.cc src/BGraphLinearIter.cc src/StdMath.cc src/Random.cc src/RandomGauss.cc src/Angle.cc src/ScalMath.cc src/RandomMersenneTwister.cc src/Erf.cc src/LogValue.cc src/Complex.cc src/StringList.cc src/Option.cc src/TypeName.cc src/BufStream.cc src/SubStringList.cc src/XMLStream.cc src/XMLTree.cc src/StreamParse.cc src/VirtualConstructor.cc src/FunctionRegister.cc src/IndexNd.cc src/VariableLengthCode.cc src/IndexRange2dSet.cc src/Base64.cc src/PointerManager.cc src/RealRange2dSet.cc src/TimeCode.cc src/STL.cc src/MD5.cc src/RealRange1dSet.cc src/ParseCSV.cc src/StringArray.cc src/AString.cc src/IndexRangeSet.cc
)
target_link_libraries(RavlCore_Body PUBLIC )

add_library(RavlCore OBJECT src/atexit.cc src/linkVectorUtilsI386SSE.cc )
target_link_libraries(RavlCore PUBLIC RavlCore_Body )


target_include_directories(
    RavlCore PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

# MustLink fixes:
install(EXPORT RavlCore_BodyConfig DESTINATION lib/cmake/RavlCore_Body/Target)
install(TARGETS RavlCore_Body EXPORT RavlCore_BodyConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows
install(TARGETS RavlCore_Body DESTINATION lib)

configure_package_config_file(cmake/RavlCore_BodyConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlCore_BodyConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlCore_Body
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlCore_BodyConfig.cmake
        DESTINATION lib/cmake/RavlCore_Body )


install(TARGETS RavlCore EXPORT RavlCoreConfig
   ARCHIVE  DESTINATION lib
   LIBRARY  DESTINATION lib
   RUNTIME  DESTINATION bin)  # This is for Windows

install(EXPORT RavlCoreConfig DESTINATION lib/cmake/RavlCore/Target)
install(TARGETS RavlCore DESTINATION lib)

configure_package_config_file(cmake/RavlCoreConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RavlCoreConfig.cmake
  INSTALL_DESTINATION lib/cmake/RavlCore
  PATH_VARS)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RavlCoreConfig.cmake
        DESTINATION lib/cmake/RavlCore )

install(DIRECTORY include DESTINATION .)



# Examples
set(EXAMPLE_LIBS  RavlMath RavlOS  )

add_executable(exIndex ${EXCLUDE_EXAMPLES} examples/exIndex.cc )
target_link_libraries(exIndex RavlCore ${EXAMPLE_LIBS} )
install(TARGETS exIndex DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exRefCounter ${EXCLUDE_EXAMPLES} examples/exRefCounter.cc )
target_link_libraries(exRefCounter RavlCore ${EXAMPLE_LIBS} )
install(TARGETS exRefCounter DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exRefCounterInherit ${EXCLUDE_EXAMPLES} examples/exRefCounterInherit.cc )
target_link_libraries(exRefCounterInherit RavlCore ${EXAMPLE_LIBS} )
install(TARGETS exRefCounterInherit DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exRefCounterAbstractInherit ${EXCLUDE_EXAMPLES} examples/exRefCounterAbstractInherit.cc )
target_link_libraries(exRefCounterAbstractInherit RavlCore ${EXAMPLE_LIBS} )
install(TARGETS exRefCounterAbstractInherit DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exQInt ${EXCLUDE_EXAMPLES} examples/exQInt.cc )
target_link_libraries(exQInt RavlCore ${EXAMPLE_LIBS} )
install(TARGETS exQInt DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exTrigger ${EXCLUDE_EXAMPLES} examples/exTrigger.cc )
target_link_libraries(exTrigger RavlCore ${EXAMPLE_LIBS} )
install(TARGETS exTrigger DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exCPUID ${EXCLUDE_EXAMPLES} examples/exCPUID.cc )
target_link_libraries(exCPUID RavlCore ${EXAMPLE_LIBS} )
install(TARGETS exCPUID DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exSysLog ${EXCLUDE_EXAMPLES} examples/exSysLog.cc )
target_link_libraries(exSysLog RavlCore ${EXAMPLE_LIBS} )
install(TARGETS exSysLog DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exSBfAcc ${EXCLUDE_EXAMPLES} examples/exSBfAcc.cc )
target_link_libraries(exSBfAcc RavlCore ${EXAMPLE_LIBS} )
install(TARGETS exSBfAcc DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exRBfAcc ${EXCLUDE_EXAMPLES} examples/exRBfAcc.cc )
target_link_libraries(exRBfAcc RavlCore ${EXAMPLE_LIBS} )
install(TARGETS exRBfAcc DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exSArr1 ${EXCLUDE_EXAMPLES} examples/exSArr1.cc )
target_link_libraries(exSArr1 RavlCore ${EXAMPLE_LIBS} )
install(TARGETS exSArr1 DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exArray1d ${EXCLUDE_EXAMPLES} examples/exArray1d.cc )
target_link_libraries(exArray1d RavlCore ${EXAMPLE_LIBS} )
install(TARGETS exArray1d DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exArray2d ${EXCLUDE_EXAMPLES} examples/exArray2d.cc )
target_link_libraries(exArray2d RavlCore ${EXAMPLE_LIBS} )
install(TARGETS exArray2d DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exArray3d ${EXCLUDE_EXAMPLES} examples/exArray3d.cc )
target_link_libraries(exArray3d RavlCore ${EXAMPLE_LIBS} )
install(TARGETS exArray3d DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exStack ${EXCLUDE_EXAMPLES} examples/exStack.cc )
target_link_libraries(exStack RavlCore ${EXAMPLE_LIBS} )
install(TARGETS exStack DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exAVLTree ${EXCLUDE_EXAMPLES} examples/exAVLTree.cc )
target_link_libraries(exAVLTree RavlCore ${EXAMPLE_LIBS} )
install(TARGETS exAVLTree DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exBinaryTree ${EXCLUDE_EXAMPLES} examples/exBinaryTree.cc )
target_link_libraries(exBinaryTree RavlCore ${EXAMPLE_LIBS} )
install(TARGETS exBinaryTree DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exDList ${EXCLUDE_EXAMPLES} examples/exDList.cc )
target_link_libraries(exDList RavlCore ${EXAMPLE_LIBS} )
install(TARGETS exDList DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exHash ${EXCLUDE_EXAMPLES} examples/exHash.cc )
target_link_libraries(exHash RavlCore ${EXAMPLE_LIBS} )
install(TARGETS exHash DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(WordFreq ${EXCLUDE_EXAMPLES} examples/WordFreq.cc )
target_link_libraries(WordFreq RavlCore ${EXAMPLE_LIBS} )
install(TARGETS WordFreq DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exGraph ${EXCLUDE_EXAMPLES} examples/exGraph.cc )
target_link_libraries(exGraph RavlCore ${EXAMPLE_LIBS} )
install(TARGETS exGraph DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exComplx ${EXCLUDE_EXAMPLES} examples/exComplx.cc )
target_link_libraries(exComplx RavlCore ${EXAMPLE_LIBS} )
install(TARGETS exComplx DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exOption ${EXCLUDE_EXAMPLES} examples/exOption.cc )
target_link_libraries(exOption RavlCore ${EXAMPLE_LIBS} )
install(TARGETS exOption DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exXMLTree ${EXCLUDE_EXAMPLES} examples/exXMLTree.cc )
target_link_libraries(exXMLTree RavlCore ${EXAMPLE_LIBS} )
install(TARGETS exXMLTree DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exVariableLengthCode ${EXCLUDE_EXAMPLES} examples/exVariableLengthCode.cc )
target_link_libraries(exVariableLengthCode RavlCore ${EXAMPLE_LIBS} )
install(TARGETS exVariableLengthCode DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exXMLStream ${EXCLUDE_EXAMPLES} examples/exXMLStream.cc )
target_link_libraries(exXMLStream RavlCore ${EXAMPLE_LIBS} )
install(TARGETS exXMLStream DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

add_executable(exObjIO ${EXCLUDE_EXAMPLES} examples/exObjIO.cc )
target_link_libraries(exObjIO RavlCore ${EXAMPLE_LIBS} )
install(TARGETS exObjIO DESTINATION examples COMPONENT examples ${EXCLUDE_EXAMPLES})

# Tests
set(TEST_LIBS  RavlMath RavlOS  )

add_executable(testRavlBase ${EXCLUDE_UNITTESTS} tests/testRavlBase.cc )
target_link_libraries(testRavlBase RavlCore ${TEST_LIBS} )
add_test(testRavlBase testRavlBase)
install(TARGETS testRavlBase DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})

add_executable(testCalls ${EXCLUDE_UNITTESTS} tests/testCalls.cc )
target_link_libraries(testCalls RavlCore ${TEST_LIBS} )
add_test(testCalls testCalls)
install(TARGETS testCalls DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})

add_executable(testString ${EXCLUDE_UNITTESTS} tests/testString.cc )
target_link_libraries(testString RavlCore ${TEST_LIBS} )
add_test(testString testString)
install(TARGETS testString DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})

add_executable(testStream ${EXCLUDE_UNITTESTS} tests/testStream.cc )
target_link_libraries(testStream RavlCore ${TEST_LIBS} )
add_test(testStream testStream)
install(TARGETS testStream DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})

add_executable(testTFVector ${EXCLUDE_UNITTESTS} tests/testTFVector.cc )
target_link_libraries(testTFVector RavlCore ${TEST_LIBS} )
add_test(testTFVector testTFVector)
install(TARGETS testTFVector DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})

add_executable(testRCLayer ${EXCLUDE_UNITTESTS} tests/testRCLayer.cc )
target_link_libraries(testRCLayer RavlCore ${TEST_LIBS} )
add_test(testRCLayer testRCLayer)
install(TARGETS testRCLayer DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})

add_executable(testVectorUtils ${EXCLUDE_UNITTESTS} tests/testVectorUtils.cc )
target_link_libraries(testVectorUtils RavlCore ${TEST_LIBS} )
add_test(testVectorUtils testVectorUtils)
install(TARGETS testVectorUtils DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})

add_executable(testI386SSE ${EXCLUDE_UNITTESTS} tests/testI386SSE.cc )
target_link_libraries(testI386SSE RavlCore ${TEST_LIBS} )
add_test(testI386SSE testI386SSE)
install(TARGETS testI386SSE DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})

add_executable(testBuffer2d ${EXCLUDE_UNITTESTS} tests/testBuffer2d.cc )
target_link_libraries(testBuffer2d RavlCore ${TEST_LIBS} )
add_test(testBuffer2d testBuffer2d)
install(TARGETS testBuffer2d DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})

add_executable(testBuffer3d ${EXCLUDE_UNITTESTS} tests/testBuffer3d.cc )
target_link_libraries(testBuffer3d RavlCore ${TEST_LIBS} )
add_test(testBuffer3d testBuffer3d)
install(TARGETS testBuffer3d DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})

add_executable(testSlice ${EXCLUDE_UNITTESTS} tests/testSlice.cc )
target_link_libraries(testSlice RavlCore ${TEST_LIBS} )
add_test(testSlice testSlice)
install(TARGETS testSlice DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})

add_executable(testSArray1d ${EXCLUDE_UNITTESTS} tests/testSArray1d.cc )
target_link_libraries(testSArray1d RavlCore ${TEST_LIBS} )
add_test(testSArray1d testSArray1d)
install(TARGETS testSArray1d DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})

add_executable(testSArray2d ${EXCLUDE_UNITTESTS} tests/testSArray2d.cc )
target_link_libraries(testSArray2d RavlCore ${TEST_LIBS} )
add_test(testSArray2d testSArray2d)
install(TARGETS testSArray2d DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})

add_executable(testSArray3d ${EXCLUDE_UNITTESTS} tests/testSArray3d.cc )
target_link_libraries(testSArray3d RavlCore ${TEST_LIBS} )
add_test(testSArray3d testSArray3d)
install(TARGETS testSArray3d DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})

add_executable(testArray1d ${EXCLUDE_UNITTESTS} tests/testArray1d.cc )
target_link_libraries(testArray1d RavlCore ${TEST_LIBS} )
add_test(testArray1d testArray1d)
install(TARGETS testArray1d DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})

add_executable(testArray2d ${EXCLUDE_UNITTESTS} tests/testArray2d.cc )
target_link_libraries(testArray2d RavlCore ${TEST_LIBS} )
add_test(testArray2d testArray2d)
install(TARGETS testArray2d DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})

add_executable(testArray3d ${EXCLUDE_UNITTESTS} tests/testArray3d.cc )
target_link_libraries(testArray3d RavlCore ${TEST_LIBS} )
add_test(testArray3d testArray3d)
install(TARGETS testArray3d DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})

add_executable(testStack ${EXCLUDE_UNITTESTS} tests/testStack.cc )
target_link_libraries(testStack RavlCore ${TEST_LIBS} )
add_test(testStack testStack)
install(TARGETS testStack DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})

add_executable(testBlkStack ${EXCLUDE_UNITTESTS} tests/testBlkStack.cc )
target_link_libraries(testBlkStack RavlCore ${TEST_LIBS} )
add_test(testBlkStack testBlkStack)
install(TARGETS testBlkStack DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})

add_executable(testBinTable ${EXCLUDE_UNITTESTS} tests/testBinTable.cc )
target_link_libraries(testBinTable RavlCore ${TEST_LIBS} )
add_test(testBinTable testBinTable)
install(TARGETS testBinTable DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})

add_executable(testPriQH ${EXCLUDE_UNITTESTS} tests/testPriQH.cc )
target_link_libraries(testPriQH RavlCore ${TEST_LIBS} )
add_test(testPriQH testPriQH)
install(TARGETS testPriQH DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})

add_executable(testPriQ ${EXCLUDE_UNITTESTS} tests/testPriQ.cc )
target_link_libraries(testPriQ RavlCore ${TEST_LIBS} )
add_test(testPriQ testPriQ)
install(TARGETS testPriQ DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})

add_executable(testPriQueue ${EXCLUDE_UNITTESTS} tests/testPriQueue.cc )
target_link_libraries(testPriQueue RavlCore ${TEST_LIBS} )
add_test(testPriQueue testPriQueue)
install(TARGETS testPriQueue DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})

add_executable(testFixedQueue ${EXCLUDE_UNITTESTS} tests/testFixedQueue.cc )
target_link_libraries(testFixedQueue RavlCore ${TEST_LIBS} )
add_test(testFixedQueue testFixedQueue)
install(TARGETS testFixedQueue DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})

add_executable(testQueue ${EXCLUDE_UNITTESTS} tests/testQueue.cc )
target_link_libraries(testQueue RavlCore ${TEST_LIBS} )
add_test(testQueue testQueue)
install(TARGETS testQueue DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})

add_executable(testBinaryTree ${EXCLUDE_UNITTESTS} tests/testBinaryTree.cc )
target_link_libraries(testBinaryTree RavlCore ${TEST_LIBS} )
add_test(testBinaryTree testBinaryTree)
install(TARGETS testBinaryTree DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})

add_executable(testHashTree ${EXCLUDE_UNITTESTS} tests/testHashTree.cc )
target_link_libraries(testHashTree RavlCore ${TEST_LIBS} )
add_test(testHashTree testHashTree)
install(TARGETS testHashTree DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})

add_executable(testDList ${EXCLUDE_UNITTESTS} tests/testDList.cc )
target_link_libraries(testDList RavlCore ${TEST_LIBS} )
add_test(testDList testDList)
install(TARGETS testDList DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})

add_executable(testHash ${EXCLUDE_UNITTESTS} tests/testHash.cc )
target_link_libraries(testHash RavlCore ${TEST_LIBS} )
add_test(testHash testHash)
install(TARGETS testHash DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})

add_executable(testHSet ${EXCLUDE_UNITTESTS} tests/testHSet.cc )
target_link_libraries(testHSet RavlCore ${TEST_LIBS} )
add_test(testHSet testHSet)
install(TARGETS testHSet DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})

add_executable(testGraph ${EXCLUDE_UNITTESTS} tests/testGraph.cc )
target_link_libraries(testGraph RavlCore ${TEST_LIBS} )
add_test(testGraph testGraph)
install(TARGETS testGraph DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})

add_executable(testTHEMesh ${EXCLUDE_UNITTESTS} tests/testTHEMesh.cc )
target_link_libraries(testTHEMesh RavlCore ${TEST_LIBS} )
add_test(testTHEMesh testTHEMesh)
install(TARGETS testTHEMesh DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})

add_executable(testBGraph ${EXCLUDE_UNITTESTS} tests/testBGraph.cc )
target_link_libraries(testBGraph RavlCore ${TEST_LIBS} )
add_test(testBGraph testBGraph)
install(TARGETS testBGraph DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})

add_executable(testBranch ${EXCLUDE_UNITTESTS} tests/testBranch.cc )
target_link_libraries(testBranch RavlCore ${TEST_LIBS} )
add_test(testBranch testBranch)
install(TARGETS testBranch DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})

add_executable(testMath ${EXCLUDE_UNITTESTS} tests/testMath.cc )
target_link_libraries(testMath RavlCore ${TEST_LIBS} )
add_test(testMath testMath)
install(TARGETS testMath DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})

add_executable(testRavlCore ${EXCLUDE_UNITTESTS} tests/testRavlCore.cc )
target_link_libraries(testRavlCore RavlCore ${TEST_LIBS} )
add_test(testRavlCore testRavlCore)
install(TARGETS testRavlCore DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})

add_executable(testXMLStream ${EXCLUDE_UNITTESTS} tests/testXMLStream.cc )
target_link_libraries(testXMLStream RavlCore ${TEST_LIBS} )
add_test(testXMLStream testXMLStream)
install(TARGETS testXMLStream DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})

add_executable(testCollection ${EXCLUDE_UNITTESTS} tests/testCollection.cc )
target_link_libraries(testCollection RavlCore ${TEST_LIBS} )
add_test(testCollection testCollection)
install(TARGETS testCollection DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})

add_executable(testSTL ${EXCLUDE_UNITTESTS} tests/testSTL.cc )
target_link_libraries(testSTL RavlCore ${TEST_LIBS} )
add_test(testSTL testSTL)
install(TARGETS testSTL DESTINATION tests COMPONENT unittests ${EXCLUDE_UNITTESTS})


# === Files for share/RAVL/gdb ===
set(AUXDIR share/RAVL/gdb)
set(AUXFILES 
  data/ravlgdb.py )
file(COPY ${AUXFILES} DESTINATION ${BUILD_ROOT_AUXFILE}/${AUXDIR})
install(FILES ${AUXFILES} DESTINATION ${AUXDIR})




ENDIF()
