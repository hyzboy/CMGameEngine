add_executable(Example.Network.SCTPO2OClient Client.cpp)
add_executable(Example.Network.SCTPO2OServer Server.cpp)

target_link_libraries(Example.Network.SCTPO2OClient ${HGL_CONSOLE_LIB})
target_link_libraries(Example.Network.SCTPO2OServer ${HGL_CONSOLE_LIB})
