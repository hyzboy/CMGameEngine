if(win32)
    find_path(GUMBO_INCLUDE_DIR
                NAMES gumbo.h)

    find_path(GUMBO_LIB_DIR
                NAMES gumbo.lib)

    include_directories(${GUMBO_INCLUDE_DIR})
    link_directories(${GUMBO_LIB_DIR})
endif()

add_executable(ToutiaoSpider ${HGL_CONSOLE_MAIN_SOURCE} ToutiaoNews.cpp)
target_link_libraries(ToutiaoSpider ${HGL_CONSOLE_LIB} gumbo)

add_executable(SinaSpider ${HGL_CONSOLE_MAIN_SOURCE} SinaNews.cpp)
target_link_libraries(SinaSpider ${HGL_CONSOLE_LIB} gumbo)

add_executable(SinaAreaNews ${HGL_CONSOLE_MAIN_SOURCE} SinaAreaNews.cpp)
target_link_libraries(SinaAreaNews ${HGL_CONSOLE_LIB} gumbo)

add_executable(news_spider_heb  ${HGL_CONSOLE_MAIN_SOURCE}
                                NewsStorage.cpp
                                news_heb.cpp)

add_executable(news_spider_fz   ${HGL_CONSOLE_MAIN_SOURCE}
                                NewsStorage.cpp
                                news_fz.cpp)

target_link_libraries(news_spider_heb ${HGL_CONSOLE_LIB} gumbo jsoncpp)
target_link_libraries(news_spider_fz ${HGL_CONSOLE_LIB} gumbo jsoncpp)
