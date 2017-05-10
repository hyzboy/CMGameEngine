add_executable(Example.Network.TCPPBServer main.cpp msg.pb.cc ${HGL_CONSOLE_MAIN_SOURCE})

target_link_libraries(Example.Network.TCPPBServer ${HGL_CONSOLE_LIB} protobuf)
