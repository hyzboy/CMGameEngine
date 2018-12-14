add_executable(ToutiaoSpider ${HGL_CONSOLE_MAIN_SOURCE} ToutiaoNews.cpp)
target_link_libraries(ToutiaoSpider ${HGL_CONSOLE_LIB} gumbo)

add_executable(SinaSpider ${HGL_CONSOLE_MAIN_SOURCE} SinaNews.cpp)
target_link_libraries(SinaSpider ${HGL_CONSOLE_LIB} gumbo)
