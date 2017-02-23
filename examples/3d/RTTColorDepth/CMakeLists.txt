include_directories(../SpiralCube)
include_directories(../SpiralCubeScene)

add_executable(RTTColorDepth    MainUnit.cpp
                                ../SpiralCube/SpiralCube.cpp
                                ../SpiralCubeScene/SpiralCubeScene.cpp
                                ${HGL_GRAPHICS_MAIN_SOURCE})

target_link_libraries(RTTColorDepth ${HGL_GRAPHICS_LIB})

SET_TARGET_PROPERTIES(RTTColorDepth PROPERTIES WIN32_EXECUTABLE "true")
