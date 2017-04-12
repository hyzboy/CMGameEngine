OPTION(ALGORITHM_INCLUDE_HASH   "Include Hash Library"  OFF)
OPTION(ALGORITHM_INCLUDE_CRYPT  "Include Crypt Library" OFF)

IF(ALGORITHM_INCLUDE_HASH)
    SET(HASH_SOURCE 
        Hash/adler32.cpp
        Hash/crc32.cpp
        Hash/md4.cpp
        Hash/md5.cpp
        Hash/sha1.cpp
        Hash/Hash.cpp)

    SOURCE_GROUP("Hash" FILES ${HASH_SOURCE})
    
    SET(algorithm_files ${algorithm_files} ${HASH_SOURCE})
ENDIF(ALGORITHM_INCLUDE_HASH)

IF(ALGORITHM_INCLUDE_CRYPT)
    SET(CRYPT_SOURCE_FILES  Crypt/aes.cpp
                            Crypt/Overflow.cpp
                            Crypt/rc4.cpp
                            Crypt/uECC.c)

    SOURCE_GROUP("Crypt" FILES ${CRYPT_SOURCE_FILES})
    
    SET(algorithm_files ${algorithm_files} ${CRYPT_SOURCE_FILES})
ENDIF(ALGORITHM_INCLUDE_CRYPT)
    
IF(WIN32)
    ADD_LIBRARY(CM.Algorithm STATIC ${algorithm_files})
ELSE()
    ADD_LIBRARY(CM.Algorithm SHARED ${algorithm_files})
ENDIF()
