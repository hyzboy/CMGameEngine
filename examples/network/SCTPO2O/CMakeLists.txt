add_executable(SCTPO2OClient Client.cpp)
add_executable(SCTPO2OServer Server.cpp)

target_link_libraries(SCTPO2OClient ${HGL_CONSOLE_LIB})
target_link_libraries(SCTPO2OServer ${HGL_CONSOLE_LIB})
