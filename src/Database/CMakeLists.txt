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

IF(UNIX)

    OPTION(DATABASE_INCLUDE_Redis   "Database support Redis"    OFF)

    IF(DATABASE_INCLUDE_Redis)
        INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/hiredis)

        FILE(GLOB_RECURSE HIREDIS_SOURCES "RedisClient/hiredis/*.c")
        FILE(GLOB_RECURSE HIREDIS_HEADERS "RedisClient/hiredis/*.h")

        SET(REDIS_CLIENT_FILES
            RedisClient/RedisDB.cpp
            RedisClient/RedisDBStrings.cpp
            RedisClient/RedisDBStringBinary.cpp
            RedisClient/RedisDBStringNumber.cpp
            RedisClient/RedisDBSets.cpp
            RedisClient/RedisDBSortedSets.cpp
            RedisClient/RedisDBHashes.cpp)

        SOURCE_GROUP("Redis\\hiredis\\Include" FILES ${HIREDIS_HEADERS})
        SOURCE_GROUP("Redis\\hiredis\\Source" FILES ${HIREDIS_SOURCES})
        SOURCE_GROUP("Redis" FILES ${REDIS_CLIENT_FILES})

        SET(db_files ${db_files} ${HIREDIS_SOURCES} ${HIREDIS_HEADERS} ${REDIS_CLIENT_FILES})

    ENDIF(DATABASE_INCLUDE_Redis)
ENDIF(UNIX)

IF(DATABASE_INCLUDE_MySQL)
    SET(MYSQL_FILES
        SQL/MySQLConnection.cpp
        SQL/MySQLDriver.cpp
        SQL/MySQLField.cpp
        SQL/MySQLRow.cpp
        SQL/MySQLStoreResult.cpp
        SQL/SQLDriver.cpp
        )

    SOURCE_GROUP("MySQL" FILES ${MYSQL_FILES})

    SET(db_files ${db_files} ${MYSQL_FILES})
ENDIF(DATABASE_INCLUDE_MySQL)

IF(WIN32)
    ADD_LIBRARY(CM.Database STATIC ${db_files})
ELSE()
    ADD_LIBRARY(CM.Database SHARED ${db_files})
ENDIF()

IF(DATABASE_INCLUDE_MySQL)
    target_link_libraries(CM.Database mysqlclient)
ENDIF()
