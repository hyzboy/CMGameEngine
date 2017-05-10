include_directories(../SpiralCube)
include_directories(../SpiralCubeScene)

add_executable(Example.3D.RTTColorDepth    MainUnit.cpp
                                ../SpiralCube/SpiralCube.cpp
                                ../SpiralCubeScene/SpiralCubeScene.cpp
                                ${HGL_GRAPHICS_MAIN_SOURCE})

target_link_libraries(Example.3D.RTTColorDepth ${HGL_GRAPHICS_LIB})

SET_TARGET_PROPERTIES(Example.3D.RTTColorDepth PROPERTIES WIN32_EXECUTABLE "true")
