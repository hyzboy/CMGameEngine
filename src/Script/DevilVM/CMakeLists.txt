SET(DEVILSCRIPT_SOURCE
	as_tokenizer.cpp
	DevilCommand.cpp
	DevilModule.cpp
	DevilContext.cpp
	DevilEnum.cpp
	DevilFunc.cpp
	DevilParse.cpp
	DevilVariable.cpp
	DevilVM.cpp
)

IF(WIN32)
	ADD_LIBRARY(CM.Script.Devil STATIC ${DEVILSCRIPT_SOURCE})
ELSE()
	ADD_LIBRARY(CM.Script.Devil SHARED ${DEVILSCRIPT_SOURCE})
ENDIF()
