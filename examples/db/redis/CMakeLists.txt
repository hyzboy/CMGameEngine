add_executable(SADD SADD.cpp)

target_link_libraries(SADD ${HGL_CONSOLE_LIB} RedisClient)
