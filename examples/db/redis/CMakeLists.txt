add_executable(Examples.DB.Redis.SADD SADD.cpp)

target_link_libraries(Examples.DB.Redis.SADD ${HGL_CONSOLE_LIB} RedisClient)
