IF(BUILD_NETWORK_SCTP)
    add_subdirectory(SCTPO2O)
    add_subdirectory(SCTPO2M)
ENDIF(BUILD_NETWORK_SCTP)

IF(UNIX)
add_subdirectory(AutoUpdate)
ENDIF(UNIX)

#add_subdirectory(TCPPBServer)

add_subdirectory(DownloadHTTPServer)
# add_subdirectory(FlashPolicyServer)

#add_subdirectory(WorkServer)

add_executable(Example.Network.DomainParse DomainParse.cpp ${HGL_CONSOLE_MAIN_SOURCE})
target_link_libraries(Example.Network.DomainParse ${HGL_CONSOLE_LIB})

add_executable(Example.Network.IPSupport IPSupport.cpp)
target_link_libraries(Example.Network.IPSupport ${HGL_CONSOLE_LIB})

add_executable(Example.Network.IP2Hostname IP2Hostname.cpp ${HGL_CONSOLE_MAIN_SOURCE})
target_link_libraries(Example.Network.IP2Hostname ${HGL_CONSOLE_LIB})

add_executable(Example.Network.MultiAcceptServer MultiAcceptServer.cpp ${HGL_CONSOLE_MAIN_SOURCE})
target_link_libraries(Example.Network.MultiAcceptServer ${HGL_CONSOLE_LIB})

add_executable(Example.Network.OnlyConnect OnlyConnectTest.cpp ${HGL_CONSOLE_MAIN_SOURCE})
target_link_libraries(Example.Network.OnlyConnect ${HGL_CONSOLE_LIB})

add_executable(Example.Network.MTEchoTCPServer MTEchoTCPServer.cpp ${HGL_CONSOLE_MAIN_SOURCE})
target_link_libraries(Example.Network.MTEchoTCPServer ${HGL_CONSOLE_LIB})

add_executable(Example.Network.TCPClientHello TCPClientHello.cpp ${HGL_CONSOLE_MAIN_SOURCE})
target_link_libraries(Example.Network.TCPClientHello ${HGL_CONSOLE_LIB})

add_executable(Example.Network.WebSocketHeaderParse WebSocketHeaderParse.cpp)
target_link_libraries(Example.Network.WebSocketHeaderParse ${HGL_CONSOLE_LIB})
