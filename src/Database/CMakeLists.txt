OPTION(DATABASE_INCLUDE_CDB     "Database support CDB"      OFF)
OPTION(DATABASE_INCLUDE_MySQL   "Database support MySQL"    OFF)

IF(DATABASE_INCLUDE_CDB)
    SET(CDB_FILES
        CDB/CDBField.cpp
        CDB/CDBTable.cpp
        CDB/FieldType.cpp
        CDB/TableDesc.cpp
        CDB/FieldData.cpp
        CDB/FieldStream.cpp
        CDB/FieldSQL.cpp
        CDB/FieldJson.cpp
        CDB/TableDescXML.cpp
    )

    SOURCE_GROUP("CDB" FILES ${CDB_FILES})

    SET(db_files ${db_files} ${CDB_FILES})
ENDIF(DATABASE_INCLUDE_CDB)

IF(DATABASE_INCLUDE_MySQL)
    SET(MYSQL_FILES
        MySQL/MySQLField.cpp
		MySQL/MySQLStoreResult.cpp
		MySQL/SQLField.cpp
        MySQL/SQLFieldFromStream.cpp
        MySQL/SQLFieldToStream.cpp
        MySQL/SQLFieldData.cpp
        MySQL/SQLRecordRow.cpp
        MySQL/SQLTable.cpp
        MySQL/SQLTableFromMySQL.cpp
        MySQL/SQLTableFromStream.cpp
        MySQL/SQLTableToStream.cpp
        MySQL/SQLStoreResult.cpp
        MySQL/SQLStoreResultFromMySQL.cpp
        MySQL/SQLStoreResultFromStream.cpp
        MySQL/SQLStoreResultToStream.cpp
        )
        
    SOURCE_GROUP("MySQL" FILES ${MYSQL_FILES})

    SET(db_files ${db_files} ${MYSQL_FILES})
ENDIF(DATABASE_INCLUDE_MySQL)

IF(WIN32)
    ADD_LIBRARY(CM.Database STATIC ${db_files})
ELSE()
    ADD_LIBRARY(CM.Database SHARED ${db_files})
ENDIF()
