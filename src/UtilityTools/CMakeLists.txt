SET(ut_files
	FindPath/FindPath.cpp
	FindPath/FindPath_AStar.cpp
	FindPath/FindPath_Line.cpp
	Hash/adler32.cpp
	Hash/crc32.cpp
	Hash/md4.cpp
	Hash/md5.cpp
	Hash/sha1.cpp
	Hash/Hash.cpp
	PList/PList.cpp
	TaskManager/TaskManager.cpp
	Timer/Timer.cpp
	Parse/CmdParse.cpp
)

SET(XML_PARSE XML/XMLParse.cpp)

IF(WIN32)
    INCLUDE_DIRECTORIES(XML/Expat)

    SET(XML_PARSE ${XML_PARSE}
        XML/Expat/xmlparse.c
        XML/Expat/xmlrole.c
        XML/Expat/xmltok.c
        XML/Expat/xmltok_impl.c
        XML/Expat/xmltok_ns.c)
ENDIF(WIN32)

SET(CDB_FILES
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

SOURCE_GROUP("DB\\CDB" CDB_FILES)


#IF(WIN32 OR APPLE)
#	SET(ut_files ${ut_files}
#	Ctrl/CtrlPoint2D.cpp
#	Ctrl/CtrlRect.cpp
#	FlowTransition/FlowTransition.cpp
#	KeyMapping/KeyMapping.cpp
#	Movie/TheoraMovie.cpp
#	)
#ENDIF(WIN32 OR APPLE)

SET(CRYPT_SOURCE_FILES	Crypt/aes.cpp
						Crypt/Overflow.cpp
						Crypt/rc4.cpp
						Crypt/uECC.c)

SOURCE_GROUP("Crypt" FILES ${CRYPT_SOURCE_FILES})

SET(AFFECTOR_SOURCE_FILES	Affector/Affector.cpp
							Affector/AffectorTimer.cpp
							Affector/AffectorTrackTimer.cpp)

SOURCE_GROUP("Affector" FILES ${AFFECTOR_SOURCE_FILES})

IF(WIN32)
	ADD_LIBRARY(CM.UT STATIC ${ut_files} ${XML_PARSE} ${CDB_FILES} ${AFFECTOR_SOURCE_FILES} ${CRYPT_SOURCE_FILES})
ELSE()
	ADD_LIBRARY(CM.UT SHARED ${ut_files} ${XML_PARSE} ${CDB_FILES} ${AFFECTOR_SOURCE_FILES} ${CRYPT_SOURCE_FILES})
ENDIF()
