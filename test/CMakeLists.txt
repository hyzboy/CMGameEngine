# add_executable(sse_test sse_test.cpp)
ADD_EXECUTABLE(endian endian.cpp)
ADD_EXECUTABLE(memblock memblock.cpp)
ADD_EXECUTABLE(strclass strclass.cpp)
ADD_EXECUTABLE(strconv strconv.cpp)
ADD_EXECUTABLE(teststr teststr.cpp)
ADD_EXECUTABLE(memleak memleak.cpp)

IF(BUILD_SCRIPT_ANGEL)
ADD_EXECUTABLE(as_test as_test.cpp ${HGL_CONSOLE_MAIN_SOURCE})
ENDIF()

ADD_EXECUTABLE(set_test set_test.cpp)
ADD_EXECUTABLE(save_convert save_convert.cpp)

# ADD_EXECUTABLE(es_test es_test.cpp ${HGL_CONSOLE_MAIN_SOURCE})

ADD_EXECUTABLE(randtest randtest.cpp)

ADD_EXECUTABLE(pt jsontype.cpp)

add_executable(MultiProc MultiProc.cpp)

# add_executable(dfsClientTest dfsClientTest.cpp)
# add_executable(dfsIDTest dfsIDTest.cpp)

add_executable(recvtest recvtest.cpp)

add_executable(RechargeTest RechargetTest.cpp)

add_executable(typesize size.cpp)

add_executable(check_error_char check_error_char.cpp)

ADD_EXECUTABLE(dt dt.cpp ${HGL_CONSOLE_MAIN_SOURCE})

add_executable(his_test his_test.cpp)

TARGET_LINK_LIBRARIES(dt ${HGL_CONSOLE_LIB})

TARGET_LINK_LIBRARIES(memblock ${HGL_CONSOLE_LIB})
TARGET_LINK_LIBRARIES(strclass ${HGL_CONSOLE_LIB})
TARGET_LINK_LIBRARIES(strconv ${HGL_CONSOLE_LIB})
TARGET_LINK_LIBRARIES(teststr ${HGL_CONSOLE_LIB})
# TARGET_LINK_LIBRARIES(as_test ${HGL_CONSOLE_LIB})
TARGET_LINK_LIBRARIES(set_test ${HGL_CONSOLE_LIB})
TARGET_LINK_LIBRARIES(save_convert ${HGL_CONSOLE_LIB})

TARGET_LINK_LIBRARIES(randtest ${HGL_CONSOLE_LIB})

# TARGET_LINK_LIBRARIES(es_test ${HGL_CONSOLE_LIB} embryo)

TARGET_LINK_LIBRARIES(pt ${HGL_CONSOLE_LIB})

TARGET_LINK_LIBRARIES(MultiProc ${HGL_CONSOLE_LIB})

# target_link_libraries(dfsClientTest ${HGL_CONSOLE_LIB})
# target_link_libraries(dfsIDTest ${HGL_CONSOLE_LIB})

target_link_libraries(recvtest ${HGL_CONSOLE_LIB})

target_link_libraries(RechargeTest ${HGL_CONSOLE_LIB} curl)

target_link_libraries(check_error_char ${HGL_CONSOLE_LIB})

target_link_libraries(his_test ${HGL_CONSOLE_LIB})

add_executable(trim_test trim_test.cpp)
