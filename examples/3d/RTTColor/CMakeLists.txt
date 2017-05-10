include_directories(../SpiralCube)
include_directories(../SpiralCubeScene)

add_executable(Example.3D.RTTColor    MainUnit.cpp
                                ../SpiralCube/SpiralCube.cpp
                                ../SpiralCubeScene/SpiralCubeScene.cpp
                                ${HGL_GRAPHICS_MAIN_SOURCE})

target_link_libraries(Example.3D.RTTColor ${HGL_GRAPHICS_LIB})

SET_TARGET_PROPERTIES(Example.3D.RTTColor PROPERTIES WIN32_EXECUTABLE "true")
