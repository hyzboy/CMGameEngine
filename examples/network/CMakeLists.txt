IF(BUILD_NETWORK_SCTP)
    add_subdirectory(SCTPO2O)
    add_subdirectory(SCTPO2M)
ENDIF(BUILD_NETWORK_SCTP)

add_subdirectory(AutoUpdate)
add_subdirectory(TCPPBServer)

add_subdirectory(DownloadHTTPServer)
add_subdirectory(Web843Server)

#add_subdirectory(WorkServer)

add_executable(Example.Network.DomainParse DomainParse.cpp ${HGL_CONSOLE_MAIN_SOURCE})
target_link_libraries(Example.Network.DomainParse ${HGL_CONSOLE_LIB})

add_executable(Example.Network.IPSupport IPSupport.cpp)
target_link_libraries(Example.Network.IPSupport ${HGL_CONSOLE_LIB})

add_executable(Example.Network.IP2Hostname IP2Hostname.cpp ${HGL_CONSOLE_MAIN_SOURCE})
target_link_libraries(Example.Network.IP2Hostname ${HGL_CONSOLE_LIB})
