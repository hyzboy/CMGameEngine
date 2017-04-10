SET(FIND_PATH_SOURCE 
    FindPath/FindPath.cpp
    FindPath/FindPath_AStar.cpp
    FindPath/FindPath_Line.cpp)

SOURCE_GROUP("FindPath" FILES ${FIND_PATH_SOURCE})

SET(HASH_SOURCE 
    Hash/adler32.cpp
    Hash/crc32.cpp
    Hash/md4.cpp
    Hash/md5.cpp
    Hash/sha1.cpp
    Hash/Hash.cpp)

SOURCE_GROUP("Hash" FILES ${HASH_SOURCE})

SET(XML_PARSE XML/XMLParse.cpp)

SOURCE_GROUP("XML" FILES XML/XMLParse.cpp)

IF(WIN32)
    INCLUDE_DIRECTORIES(XML/Expat)

    SET(EXPAT_SOURCE 
        XML/Expat/xmlparse.c
        XML/Expat/xmlrole.c
        XML/Expat/xmltok.c
        XML/Expat/xmltok_impl.c
        XML/Expat/xmltok_ns.c)

    SET(XML_PARSE ${XML_PARSE} ${EXPAT_SOURCE})        

    SOURCE_GROUP("XML\\Expat" FILES ${EXPAT_SOURCE})
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

SOURCE_GROUP("DB\\CDB" FILES ${CDB_FILES})

#IF(WIN32 OR APPLE)
#    SET(ut_files ${ut_files}
#    Ctrl/CtrlPoint2D.cpp
#    Ctrl/CtrlRect.cpp
#    FlowTransition/FlowTransition.cpp
#    KeyMapping/KeyMapping.cpp
#    Movie/TheoraMovie.cpp
#    )
#ENDIF(WIN32 OR APPLE)

SET(CRYPT_SOURCE_FILES  Crypt/aes.cpp
                        Crypt/Overflow.cpp
                        Crypt/rc4.cpp
                        Crypt/uECC.c)

SOURCE_GROUP("Crypt" FILES ${CRYPT_SOURCE_FILES})

SET(AFFECTOR_SOURCE_FILES   Affector/Affector.cpp
                            Affector/AffectorTimer.cpp
                            Affector/AffectorTrackTimer.cpp)

SOURCE_GROUP("Affector" FILES ${AFFECTOR_SOURCE_FILES})

SET(ut_files
    
    ${FIND_PATH_SOURCE}
    ${HASH_SOURCE}
    ${XML_PARSE}
    ${CDB_FILES}
    ${AFFECTOR_SOURCE_FILES}
    ${CRYPT_SOURCE_FILES}
    
    TaskManager/TaskManager.cpp
    Timer/Timer.cpp
    Parse/CmdParse.cpp
)

IF(WIN32)
    ADD_LIBRARY(CM.UT STATIC ${ut_files})
ELSE()
    ADD_LIBRARY(CM.UT SHARED ${ut_files})
ENDIF()
