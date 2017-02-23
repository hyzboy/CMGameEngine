include_directories(../PlaneGrid)

add_executable(SpiralSphereScene    MainUnit.cpp
                                    ../PlaneGrid/PlaneGrid.cpp
                                    SpiralSphere.cpp
                                    ${HGL_GRAPHICS_MAIN_SOURCE})

target_link_libraries(SpiralSphereScene ${HGL_GRAPHICS_LIB})

SET_TARGET_PROPERTIES(SpiralSphereScene PROPERTIES WIN32_EXECUTABLE "true")
