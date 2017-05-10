include_directories(../PlaneGrid)
include_directories(../SpiralCube)

add_executable(Example.3D.SpiralCubeScene    MainUnit.cpp
                                ../PlaneGrid/PlaneGrid.cpp
                                ../SpiralCube/SpiralCube.cpp
                                SpiralCubeScene.cpp
                                ${HGL_GRAPHICS_MAIN_SOURCE})

target_link_libraries(Example.3D.SpiralCubeScene ${HGL_GRAPHICS_LIB})

SET_TARGET_PROPERTIES(Example.3D.SpiralCubeScene PROPERTIES WIN32_EXECUTABLE "true")
