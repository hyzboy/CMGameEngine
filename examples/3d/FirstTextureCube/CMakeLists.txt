add_executable(FirstTextureCube    MainUnit.cpp ${HGL_GRAPHICS_MAIN_SOURCE})

target_link_libraries(FirstTextureCube ${HGL_GRAPHICS_LIB})

SET_TARGET_PROPERTIES(FirstTextureCube PROPERTIES WIN32_EXECUTABLE "true")
