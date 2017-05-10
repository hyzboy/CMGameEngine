add_executable(Example.2D.GreenClamp    MainUnit.cpp ${HGL_GRAPHICS_MAIN_SOURCE})

target_link_libraries(Example.2D.GreenClamp ${HGL_GRAPHICS_LIB})

SET_TARGET_PROPERTIES(Example.2D.GreenClamp PROPERTIES WIN32_EXECUTABLE "true")
