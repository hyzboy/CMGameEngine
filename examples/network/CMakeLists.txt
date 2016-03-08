IF(BUILD_NETWORK_SCTP)
	add_subdirectory(SCTPO2O)
	add_subdirectory(SCTPO2M)
ENDIF(BUILD_NETWORK_SCTP)

add_subdirectory(TCPPBServer)

add_subdirectory(DownloadHTTPServer)
add_subdirectory(Unity3D843Server)

#add_subdirectory(WorkServer)

add_executable(DomainParse DomainParse.cpp ${HGL_CONSOLE_MAIN_SOURCE})
target_link_libraries(DomainParse ${HGL_CONSOLE_LIB})
