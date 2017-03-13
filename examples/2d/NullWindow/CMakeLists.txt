add_executable(NullWindow    MainUnit.cpp ${HGL_GRAPHICS_MAIN_SOURCE})

target_link_libraries(NullWindow ${HGL_GRAPHICS_LIB})

SET_TARGET_PROPERTIES(NullWindow PROPERTIES WIN32_EXECUTABLE "true") 