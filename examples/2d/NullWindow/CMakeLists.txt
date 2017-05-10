add_executable(Example.2D.NullWindow    MainUnit.cpp ${HGL_GRAPHICS_MAIN_SOURCE})

target_link_libraries(Example.2D.NullWindow ${HGL_GRAPHICS_LIB})

SET_TARGET_PROPERTIES(Example.2D.NullWindow PROPERTIES WIN32_EXECUTABLE "true") 
