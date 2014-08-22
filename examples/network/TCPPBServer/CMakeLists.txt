add_executable(TCPPBServer main.cpp msg.pb.cc ${HGL_CONSOLE_MAIN_SOURCE})

target_link_libraries(TCPPBServer ${HGL_CONSOLE_LIB} protobuf)
