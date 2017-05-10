add_executable(Example.Audio.PlayerSound    MainUnit.cpp ${HGL_CONSOLE_MAIN_SOURCE})

target_link_libraries(Example.Audio.PlayerSound ${HGL_CONSOLE_LIB} CM.OpenALEE)
