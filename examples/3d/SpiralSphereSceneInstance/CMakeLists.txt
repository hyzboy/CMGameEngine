include_directories(../PlaneGrid)
include_directories(../SpiralSphereScene)

add_executable(Example.3D.SpiralSphereSceneInstance MainUnit.cpp
                                                    ../PlaneGrid/PlaneGrid.cpp
                                                    ../SpiralSphereScene/SpiralSphere.cpp
                                                    ${HGL_GRAPHICS_MAIN_SOURCE})

target_link_libraries(Example.3D.SpiralSphereSceneInstance ${HGL_GRAPHICS_LIB})

SET_TARGET_PROPERTIES(Example.3D.SpiralSphereSceneInstance PROPERTIES WIN32_EXECUTABLE "true")
