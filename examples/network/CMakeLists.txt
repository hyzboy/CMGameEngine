IF(BUILD_NETWORK_SCTP)
	add_subdirectory(SCTPO2O)
	add_subdirectory(SCTPO2M)
ENDIF(BUILD_NETWORK_SCTP)

add_subdirectory(TCPPBServer)

add_subdirectory(DownloadHTTPServer)
add_subdirectory(Unity3D843Server)
