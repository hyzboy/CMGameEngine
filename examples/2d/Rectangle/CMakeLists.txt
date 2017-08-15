add_executable(Example.2D.Rectangle    MainUnit.cpp ${HGL_GRAPHICS_MAIN_SOURCE})

target_link_libraries(Example.2D.Rectangle ${HGL_GRAPHICS_LIB})

SET_TARGET_PROPERTIES(Example.2D.Rectangle PROPERTIES WIN32_EXECUTABLE "true")
