add_executable(SCTPO2MClient Client.cpp)
add_executable(SCTPO2MServer Server.cpp)

target_link_libraries(SCTPO2MClient ${HGL_CONSOLE_LIB})
target_link_libraries(SCTPO2MServer ${HGL_CONSOLE_LIB})
