IF(BUILD_NETWORK_WEBSOCKET)
    SET(ALGORITHM_INCLUDE_HASH    ON)
    SET(ALGORITHM_INCLUDE_Crypt   ON)
ELSE()
    OPTION(ALGORITHM_INCLUDE_HASH       "Include Hash Library"  OFF)
    OPTION(ALGORITHM_INCLUDE_Crypt      "Include Crypt Library" OFF)
ENDIF()

OPTION(ALGORITHM_INCLUDE_FindPath   "Include FindPath Library" OFF)
OPTION(ALGORITHM_INCLUDE_Affector   "Include Affector Library" OFF)

SET(algorithm_files
	Compress.cpp
	LSinCos.cpp
	LAtan.cpp
	ProbSelect.cpp
	PseudoRandom.cpp)

IF(ALGORITHM_INCLUDE_HASH)
    SET(HASH_SOURCE
        Hash/adler32.cpp
        Hash/crc32.cpp
        Hash/md4.cpp
        Hash/md5.cpp
        Hash/sha1.cpp
        Hash/sha1le.cpp
        Hash/sha256.cpp
        Hash/Hash.cpp)

    SOURCE_GROUP("Hash" FILES ${HASH_SOURCE})

    SET(algorithm_files ${algorithm_files} ${HASH_SOURCE})
ENDIF(ALGORITHM_INCLUDE_HASH)

IF(ALGORITHM_INCLUDE_Crypt)
    SET(CRYPT_SOURCE_FILES  Crypt/aes.cpp
                            Crypt/Overflow.cpp
                            Crypt/rc4.cpp
                            Crypt/uECC.c
							Crypt/base64.cpp)

    SOURCE_GROUP("Crypt" FILES ${CRYPT_SOURCE_FILES})

    SET(algorithm_files ${algorithm_files} ${CRYPT_SOURCE_FILES})
ENDIF(ALGORITHM_INCLUDE_Crypt)

IF(ALGORITHM_INCLUDE_FindPath)
    SET(FIND_PATH_SOURCE
        FindPath/FindPath.cpp
        FindPath/FindPath_AStar.cpp
        FindPath/FindPath_Line.cpp)

    SOURCE_GROUP("FindPath" FILES ${FIND_PATH_SOURCE})

    SET(algorithm_files ${algorithm_files} ${FIND_PATH_SOURCE})
ENDIF(ALGORITHM_INCLUDE_FindPath)

IF(ALGORITHM_INCLUDE_Affector)
    SET(AFFECTOR_SOURCE_FILES   Affector/Affector.cpp
                                Affector/AffectorTimer.cpp
                                Affector/AffectorTrackTimer.cpp)

    SOURCE_GROUP("Affector" FILES ${AFFECTOR_SOURCE_FILES})

    SET(algorithm_files ${algorithm_files} ${AFFECTOR_SOURCE_FILES})
ENDIF(ALGORITHM_INCLUDE_Affector)

IF(WIN32)
    ADD_LIBRARY(CM.Algorithm STATIC ${algorithm_files})
ELSE()
    ADD_LIBRARY(CM.Algorithm SHARED ${algorithm_files})
ENDIF()
