add_executable(PlayerSound    MainUnit.cpp ${HGL_CONSOLE_MAIN_SOURCE})

target_link_libraries(PlayerSound ${HGL_CONSOLE_LIB} CM.OpenALEE)
