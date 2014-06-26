IF(USE_ANGEL_SCRIPT_JIT)
	SET(ANGEL_SCRIPT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/AngelVM/jit_ver)
ELSE(USE_ANGEL_SCRIPT_JIT)
	SET(ANGEL_SCRIPT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/AngelVM/top_ver)
ENDIF(USE_ANGEL_SCRIPT_JIT)

INCLUDE_DIRECTORIES(${ANGEL_SCRIPT_PATH})
INCLUDE_DIRECTORIES(${ANGEL_SCRIPT_PATH}/include)

set(ANGELSCRIPT_SOURCE
		${ANGEL_SCRIPT_PATH}/source/as_atomic.cpp
		${ANGEL_SCRIPT_PATH}/source/as_builder.cpp
		${ANGEL_SCRIPT_PATH}/source/as_bytecode.cpp
		${ANGEL_SCRIPT_PATH}/source/as_callfunc.cpp
		${ANGEL_SCRIPT_PATH}/source/as_compiler.cpp
		${ANGEL_SCRIPT_PATH}/source/as_configgroup.cpp
		${ANGEL_SCRIPT_PATH}/source/as_context.cpp
		${ANGEL_SCRIPT_PATH}/source/as_datatype.cpp
		${ANGEL_SCRIPT_PATH}/source/as_gc.cpp
		${ANGEL_SCRIPT_PATH}/source/as_generic.cpp
		${ANGEL_SCRIPT_PATH}/source/as_globalproperty.cpp
		${ANGEL_SCRIPT_PATH}/source/as_memory.cpp
		${ANGEL_SCRIPT_PATH}/source/as_module.cpp
		${ANGEL_SCRIPT_PATH}/source/as_objecttype.cpp
		${ANGEL_SCRIPT_PATH}/source/as_outputbuffer.cpp
		${ANGEL_SCRIPT_PATH}/source/as_parser.cpp
		${ANGEL_SCRIPT_PATH}/source/as_restore.cpp
		${ANGEL_SCRIPT_PATH}/source/as_scriptcode.cpp
		${ANGEL_SCRIPT_PATH}/source/as_scriptengine.cpp
		${ANGEL_SCRIPT_PATH}/source/as_scriptfunction.cpp
		${ANGEL_SCRIPT_PATH}/source/as_scriptnode.cpp
		${ANGEL_SCRIPT_PATH}/source/as_scriptobject.cpp
		${ANGEL_SCRIPT_PATH}/source/as_string.cpp
		${ANGEL_SCRIPT_PATH}/source/as_string_util.cpp
		${ANGEL_SCRIPT_PATH}/source/as_thread.cpp
		${ANGEL_SCRIPT_PATH}/source/as_tokenizer.cpp
		${ANGEL_SCRIPT_PATH}/source/as_typeinfo.cpp
		${ANGEL_SCRIPT_PATH}/source/as_variablescope.cpp
)

SET(ANGEL_SCRIPT_ADDON_SOURCE	${ANGEL_SCRIPT_PATH}/add_on/scriptarray/scriptarray.cpp)

SET(ut_files
	AngelVM/AngelVM.cpp
	AngelVM/AngelMath.cpp
	AngelVM/AngelString.cpp
	AngelVM/AngelModule.cpp
	AngelVM/AngelContext.cpp
	AngelVM/AngelObject.cpp

# 	DevilVM/as_tokenizer.cpp
# 	DevilVM/DevilCommand.cpp
# 	DevilVM/DevilModule.cpp
# 	DevilVM/DevilContext.cpp
# 	DevilVM/DevilEnum.cpp
# 	DevilVM/DevilFunc.cpp
# 	DevilVM/DevilParse.cpp
# 	DevilVM/DevilVariable.cpp
# 	DevilVM/DevilVM.cpp
	FindPath/FindPath.cpp
	FindPath/FindPath_AStar.cpp
	FindPath/FindPath_Line.cpp
	Hash/adler32.cpp
	Hash/Hash.cpp
	PList/PList.cpp
	TaskManager/TaskManager.cpp
	Timer/Timer.cpp
	XML/XMLParse.cpp
)

SET(DB_FILES
	DB/CDBField.cpp
	DB/CDBTable.cpp
	DB/FieldType.cpp
	DB/TableDesc.cpp
	DB/FieldData.cpp
	DB/FieldStream.cpp
	DB/FieldSQL.cpp
	DB/FieldJson.cpp
	DB/TableDescXML.cpp
)

OPTION(USE_ANGEL_SCRIPT_JIT	"AngelScript use JIT" OFF)

IF(USE_ANGEL_SCRIPT_JIT)
	ADD_DEFINITIONS("-DUSE_ANGEL_SCRIPT_JIT")
	SET(ut_files ${ut_files} AngelVM/jit/as_jit.cpp)
ENDIF(USE_ANGEL_SCRIPT_JIT)

IF(UNIX)
	IF(USE_ANGEL_SCRIPT_JIT)
		SET(ut_files ${ut_files}	AngelVM/jit/virtual_asm_linux.cpp)
	ENDIF(USE_ANGEL_SCRIPT_JIT)
#
	IF(HGL_BITS EQUAL 64)
		SET(ut_files ${ut_files}	DevilVM/GccCallX64.cpp
									${ANGEL_SCRIPT_PATH}/source/as_callfunc_x64_gcc.cpp)

		IF(USE_ANGEL_SCRIPT_JIT)
			SET(ut_files ${ut_files}	AngelVM/jit/virtual_asm_x64.cpp)
		ENDIF(USE_ANGEL_SCRIPT_JIT)
	ELSE(HGL_BITS EQUAL 64)
		SET(ut_files ${ut_files}	${ANGEL_SCRIPT_PATH}/source/as_callfunc_x86.cpp)

		IF(USE_ANGEL_SCRIPT_JIT)
			SET(ut_files ${ut_files}	AngelVM/jit/virtual_asm_x86.cpp)
		ENDIF(USE_ANGEL_SCRIPT_JIT)
	ENDIF(HGL_BITS EQUAL 64)
ENDIF(UNIX)

IF(WIN32)
	IF(HGL_BITS EQUAL 64)
		SET(ut_files ${ut_files}	${ANGEL_SCRIPT_PATH}/source/as_callfunc_x64_msvc.cpp
									${ANGEL_SCRIPT_PATH}/source/as_callfunc_x64_msvc_asm.asm)
	ELSE(HGL_BITS EQUAL 64)
		SET(ut_files ${ut_files}	${ANGEL_SCRIPT_PATH}/source/as_callfunc_x86.cpp
									DevilVM/DevilCallConvX8632.cpp)
	ENDIF(HGL_BITS EQUAL 64)
ENDIF(WIN32)

#IF(WIN32 OR APPLE)
#	SET(ut_files ${ut_files}
#	Ctrl/CtrlPoint2D.cpp
#	Ctrl/CtrlRect.cpp
#	FlowTransition/FlowTransition.cpp
#	KeyMapping/KeyMapping.cpp
#	Movie/TheoraMovie.cpp
#	)
#ENDIF(WIN32 OR APPLE)

ADD_DEFINITIONS("-DAS_USE_NAMESPACE -DANGELSCRIPT_EXPORT")

ADD_LIBRARY(CM.UT SHARED ${ut_files} ${DB_FILES} ${ANGELSCRIPT_SOURCE} ${ANGEL_SCRIPT_ADDON_SOURCE})
