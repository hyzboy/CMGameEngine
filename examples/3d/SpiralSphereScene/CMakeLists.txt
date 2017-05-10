include_directories(../PlaneGrid)

add_executable(Example.3D.SpiralSphereScene    MainUnit.cpp
                                    ../PlaneGrid/PlaneGrid.cpp
                                    SpiralSphere.cpp
                                    ${HGL_GRAPHICS_MAIN_SOURCE})

target_link_libraries(Example.3D.SpiralSphereScene ${HGL_GRAPHICS_LIB})

SET_TARGET_PROPERTIES(Example.3D.SpiralSphereScene PROPERTIES WIN32_EXECUTABLE "true")
