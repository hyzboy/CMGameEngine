add_executable(Example.Network.SCTPO2MClient Client.cpp)
add_executable(Example.Network.SCTPO2MServer Server.cpp)

target_link_libraries(Example.Network.SCTPO2MClient ${HGL_CONSOLE_LIB})
target_link_libraries(Example.Network.SCTPO2MServer ${HGL_CONSOLE_LIB})
