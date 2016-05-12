add_executable(DownloadTaobaoData DownloadTaobaoData.cpp ${HGL_CONSOLE_MAIN_SOURCE})

target_link_libraries(DownloadTaobaoData ${HGL_CONSOLE_LIB} gumbo jsoncpp)
