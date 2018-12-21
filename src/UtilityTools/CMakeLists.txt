

SET(XML_PARSE XML/XMLParseClass.cpp)

SOURCE_GROUP("XML" FILES XML/XMLParseClass.cpp)

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

#IF(WIN32 OR APPLE)
#    SET(ut_files ${ut_files}
#    Ctrl/CtrlPoint2D.cpp
#    Ctrl/CtrlRect.cpp
#    FlowTransition/FlowTransition.cpp
#    KeyMapping/KeyMapping.cpp
#    Movie/TheoraMovie.cpp
#    )
#ENDIF(WIN32 OR APPLE)


SET(ut_files

    ${XML_PARSE}

    TaskManager/TaskManager.cpp
    Timer/Timer.cpp
    Parse/CmdParse.cpp
    Json/JsonTool.cpp
)

IF(WIN32)
    ADD_LIBRARY(CM.UT STATIC ${ut_files})
ELSE()
    ADD_LIBRARY(CM.UT SHARED ${ut_files})
ENDIF()

target_link_libraries(CM.UT PRIVATE jsoncpp)
