add_executable(Example.3D.PlaneGrid    MainUnit.cpp PlaneGrid.cpp ${HGL_GRAPHICS_MAIN_SOURCE})

target_link_libraries(Example.3D.PlaneGrid ${HGL_GRAPHICS_LIB})

SET_TARGET_PROPERTIES(Example.3D.PlaneGrid PROPERTIES WIN32_EXECUTABLE "true")
