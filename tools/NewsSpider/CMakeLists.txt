add_executable(SinaSpider ${HGL_CONSOLE_MAIN_SOURCE}
                            SinaNews.cpp)

target_link_libraries(SinaSpider ${HGL_CONSOLE_LIB} gumbo)
