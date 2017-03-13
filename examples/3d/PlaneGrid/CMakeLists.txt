add_executable(PlaneGrid    MainUnit.cpp PlaneGrid.cpp ${HGL_GRAPHICS_MAIN_SOURCE})

target_link_libraries(PlaneGrid ${HGL_GRAPHICS_LIB})

SET_TARGET_PROPERTIES(PlaneGrid PROPERTIES WIN32_EXECUTABLE "true")