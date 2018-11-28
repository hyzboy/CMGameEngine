add_executable(NewsSpider ${HGL_CONSOLE_MAIN_SOURCE}
                            main.cpp
                            sina.cpp)

target_link_libraries(NewsSpider ${HGL_CONSOLE_LIB} gumbo jsoncpp)
