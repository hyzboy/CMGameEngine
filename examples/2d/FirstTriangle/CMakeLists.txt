add_executable(Example.2D.FirstTriangle    MainUnit.cpp ${HGL_GRAPHICS_MAIN_SOURCE})

target_link_libraries(Example.2D.FirstTriangle ${HGL_GRAPHICS_LIB})

SET_TARGET_PROPERTIES(Example.2D.FirstTriangle PROPERTIES WIN32_EXECUTABLE "true")
