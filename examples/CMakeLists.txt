IF(BUILD_ALGORITHM)
    add_subdirectory(algorithm)
ENDIF()

IF(BUILD_OPENGL_LIB)
    add_subdirectory(2d)
    add_subdirectory(3d)
ENDIF()

IF(BUILD_OPENAL_LIB)
    add_subdirectory(audio)
ENDIF()

add_subdirectory(multithread)

IF(BUILD_NETWORK_LIB)
    add_subdirectory(network)
ENDIF()

add_subdirectory(datatype)
add_subdirectory(time)

IF(BUILD_DATABASE)
    add_subdirectory(database)

    IF(UNIX)
        IF(DATABASE_INCLUDE_Redis)
            add_subdirectory(db/redis)
        ENDIF()
    ENDIF()
ENDIF()
