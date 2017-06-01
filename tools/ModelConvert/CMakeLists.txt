add_executable(ModelConvert MainUnit.cpp AssimpLoader.cpp AssimpLoader.h ${HGL_CONSOLE_MAIN_SOURCE})

target_include_directories(ModelConvert PRIVATE ${CMGDK_PATH}/tools/ModelConvert/assimp/include)

IF(${CMGDK_BUILD_TYPE} STREQUAL Debug)
	SET(ZLIB_LIBFILE ${CMGDK_PATH}/src/PlugIn/Compress.zip/zlib/${CMGDK_BUILD_TYPE}/zlibstaticd.lib)
ELSE()
	SET(ZLIB_LIBFILE ${CMGDK_PATH}/src/PlugIn/Compress.zip/zlib/${CMGDK_BUILD_TYPE}/zlibstatic.lib)
ENDIF()

target_link_libraries(ModelConvert	${CMGDK_PATH}/tools/ModelConvert/assimp/lib/${CMGDK_BUILD_TYPE}/assimp-vc140-mt.lib
									${CMGDK_PATH}/tools/ModelConvert/assimp/lib/${CMGDK_BUILD_TYPE}/IrrXML.lib
									${ZLIB_LIBFILE}
									${HGL_CONSOLE_LIB})
