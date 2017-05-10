add_executable(Example.3D.PlaneSphere    MainUnit.cpp ${HGL_GRAPHICS_MAIN_SOURCE})

target_link_libraries(Example.3D.PlaneSphere ${HGL_GRAPHICS_LIB})

SET_TARGET_PROPERTIES(Example.3D.PlaneSphere PROPERTIES WIN32_EXECUTABLE "true")
