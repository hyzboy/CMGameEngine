IF(USE_ANGEL_SCRIPT_JIT)
	SET(ANGEL_SCRIPT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/jit_ver)
ELSE(USE_ANGEL_SCRIPT_JIT)
	SET(ANGEL_SCRIPT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/top_ver)
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
SET(ANGEL_VM_SOURCE		AngelVM.cpp
						AngelMath.cpp
						AngelString.cpp
						AngelModule.cpp
						AngelContext.cpp
						AngelObject.cpp)

OPTION(USE_ANGEL_SCRIPT_JIT	"AngelScript use JIT" OFF)

IF(USE_ANGEL_SCRIPT_JIT)
	ADD_DEFINITIONS("-DUSE_ANGEL_SCRIPT_JIT")
	SET(ANGEL_JIT_SOURCE jit/as_jit.cpp)
ENDIF(USE_ANGEL_SCRIPT_JIT)

IF(UNIX)
	IF(USE_ANGEL_SCRIPT_JIT)
		SET(ANGEL_JIT_SOURCE ${ANGEL_JIT_SOURCE}	jit/virtual_asm_linux.cpp)
	ENDIF(USE_ANGEL_SCRIPT_JIT)
#
	IF(HGL_BITS EQUAL 64)
		SET(ANGEL_JIT_SOURCE ${ANGEL_JIT_SOURCE}	${ANGEL_SCRIPT_PATH}/source/as_callfunc_x64_gcc.cpp)

		IF(USE_ANGEL_SCRIPT_JIT)
			SET(ANGEL_JIT_SOURCE ${ANGEL_JIT_SOURCE}	jit/virtual_asm_x64.cpp)
		ENDIF(USE_ANGEL_SCRIPT_JIT)
	ELSE(HGL_BITS EQUAL 64)
		SET(ANGEL_JIT_SOURCE ${ANGEL_JIT_SOURCE}	${ANGEL_SCRIPT_PATH}/source/as_callfunc_x86.cpp)

		IF(USE_ANGEL_SCRIPT_JIT)
			SET(ANGEL_JIT_SOURCE ${ANGEL_JIT_SOURCE}	jit/virtual_asm_x86.cpp)
		ENDIF(USE_ANGEL_SCRIPT_JIT)
	ENDIF(HGL_BITS EQUAL 64)
ENDIF(UNIX)

IF(WIN32)
	IF(USE_ANGEL_SCRIPT_JIT)
		SET(ANGEL_JIT_SOURCE ${ANGEL_JIT_SOURCE}	jit/virtual_asm_windows.cpp)
	ENDIF(USE_ANGEL_SCRIPT_JIT)

	IF(HGL_BITS EQUAL 64)
		SET(ANGEL_CALL_SOURCE	${ANGEL_SCRIPT_PATH}/source/as_callfunc_x64_msvc.cpp
								${ANGEL_SCRIPT_PATH}/source/as_callfunc_x64_msvc_asm.asm)

		IF(USE_ANGEL_SCRIPT_JIT)
			SET(ANGEL_JIT_SOURCE ${ANGEL_JIT_SOURCE}	jit/virtual_asm_x64.cpp)
		ENDIF(USE_ANGEL_SCRIPT_JIT)
	ELSE(HGL_BITS EQUAL 64)
		SET(ANGEL_CALL_SOURCE	${ANGEL_SCRIPT_PATH}/source/as_callfunc_x86.cpp
								DevilVM/DevilCallConvX8632.cpp)

		IF(USE_ANGEL_SCRIPT_JIT)
			SET(ANGEL_JIT_SOURCE ${ANGEL_JIT_SOURCE}	jit/virtual_asm_x86.cpp)
		ENDIF(USE_ANGEL_SCRIPT_JIT)
	ENDIF(HGL_BITS EQUAL 64)
ENDIF(WIN32)

ADD_DEFINITIONS("-DAS_USE_NAMESPACE -DANGELSCRIPT_EXPORT")

SOURCE_GROUP("AngelVM" FILES ${ANGEL_VM_SOURCE})
SOURCE_GROUP("AngelVM\\AngelScript" FILES ${ANGELSCRIPT_SOURCE})
SOURCE_GROUP("AngelVM\\AngelScript\\Addon" FILES ${ANGEL_SCRIPT_ADDON_SOURCE})
SOURCE_GROUP("AngelVM\\AngelScript\\Call" FILES ${ANGEL_CALL_SOURCE})
SOURCE_GROUP("AngelVM\\JIT" FILES ${ANGEL_JIT_SOURCE})

IF(WIN32)
	ADD_LIBRARY(CM.Script.Angel STATIC ${ANGELSCRIPT_SOURCE} ${ANGEL_SCRIPT_ADDON_SOURCE} ${ANGEL_VM_SOURCE} ${ANGEL_CALL_SOURCE} ${ANGEL_JIT_SOURCE})
ELSE()
	ADD_LIBRARY(CM.Script.Angel SHARED ${ANGELSCRIPT_SOURCE} ${ANGEL_SCRIPT_ADDON_SOURCE} ${ANGEL_VM_SOURCE} ${ANGEL_CALL_SOURCE} ${ANGEL_JIT_SOURCE})
ENDIF()
