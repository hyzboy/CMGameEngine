SET(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE)

OPTION(LOG_INFO					"Output Log info"							FALSE	)

IF(LOG_INFO)
	MESSAGE("Output LogInfo")

    OPTION(LOG_INFO_THREAD			"Output Log info include ThreadPID"			TRUE	)
    OPTION(LOG_INFO_TIME			"Output Log info include time"				TRUE	)
    OPTION(LOG_INFO_SOURCE			"Output Log info include source and line"	OFF		)
    OPTION(LOG_FILE_ONLY_ERROR 		"Only log error to file"					TRUE	)
    OPTION(LOG_THREAD_MUTEX			"Log Thread Mutex"							TRUE	)

    IF(LOG_INFO_THREAD)
        add_definitions("-DLOG_INFO_THREAD")
    ENDIF(LOG_INFO_THREAD)

    IF(LOG_INFO_TIME)
        add_definitions("-DLOG_INFO_TIME")
    ENDIF(LOG_INFO_TIME)

    IF(LOG_INFO_SOURCE)
        add_definitions("-DLOG_INFO_SOURCE")
    ENDIF(LOG_INFO_SOURCE)

    IF(LOG_THREAD_MUTEX)
        add_definitions("-DLOGINFO_THREAD_MUTEX")
        MESSAGE("LogInfo use ThreadMutex")
    ELSE(LOG_THREAD_MUTEX)
        MESSAGE("LogInfo don't use ThreadMutex")
    ENDIF(LOG_THREAD_MUTEX)

    if(LOG_FILE_ONLY_ERROR)
        add_definitions("-DONLY_LOG_FILE_ERROR")
        MESSAGE("File Log only record ERROR")
    else(LOG_FILE_ONLY_ERROR)
        MESSAGE("File Log record all.")
    endif(LOG_FILE_ONLY_ERROR)

ELSE(LOG_INFO)
	add_definitions("-DNO_LOGINFO")
	MESSAGE("Don't output LogInfo")
ENDIF(LOG_INFO)
