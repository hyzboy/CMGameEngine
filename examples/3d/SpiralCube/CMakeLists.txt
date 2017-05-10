include_directories(../PlaneGrid)

add_executable(Example.3D.SpiralCube    MainUnit.cpp
                            ../PlaneGrid/PlaneGrid.cpp
                            SpiralCube.cpp
                            ${HGL_GRAPHICS_MAIN_SOURCE})

target_link_libraries(Example.3D.SpiralCube ${HGL_GRAPHICS_LIB})

SET_TARGET_PROPERTIES(Example.3D.SpiralCube PROPERTIES WIN32_EXECUTABLE "true")
