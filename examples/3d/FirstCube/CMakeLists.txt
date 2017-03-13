add_executable(FirstCube    MainUnit.cpp ${HGL_GRAPHICS_MAIN_SOURCE})

target_link_libraries(FirstCube ${HGL_GRAPHICS_LIB})

SET_TARGET_PROPERTIES(FirstCube PROPERTIES WIN32_EXECUTABLE "true")