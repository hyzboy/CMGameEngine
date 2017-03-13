add_executable(PlaneSphere    MainUnit.cpp ${HGL_GRAPHICS_MAIN_SOURCE})

target_link_libraries(PlaneSphere ${HGL_GRAPHICS_LIB})

SET_TARGET_PROPERTIES(PlaneSphere PROPERTIES WIN32_EXECUTABLE "true")
