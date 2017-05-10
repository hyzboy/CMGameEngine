add_executable(Example.3D.FirstTextureCube    MainUnit.cpp ${HGL_GRAPHICS_MAIN_SOURCE})

target_link_libraries(Example.3D.FirstTextureCube ${HGL_GRAPHICS_LIB})

SET_TARGET_PROPERTIES(Example.3D.FirstTextureCube PROPERTIES WIN32_EXECUTABLE "true")
