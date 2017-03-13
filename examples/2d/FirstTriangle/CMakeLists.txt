add_executable(FirstTriangle    MainUnit.cpp ${HGL_GRAPHICS_MAIN_SOURCE})

target_link_libraries(FirstTriangle ${HGL_GRAPHICS_LIB})

SET_TARGET_PROPERTIES(FirstTriangle PROPERTIES WIN32_EXECUTABLE "true")
