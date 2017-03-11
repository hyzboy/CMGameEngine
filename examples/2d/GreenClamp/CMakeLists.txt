add_executable(GreenClamp    MainUnit.cpp ${HGL_GRAPHICS_MAIN_SOURCE})

target_link_libraries(GreenClamp ${HGL_GRAPHICS_LIB})

SET_TARGET_PROPERTIES(GreenClamp PROPERTIES WIN32_EXECUTABLE "true")
