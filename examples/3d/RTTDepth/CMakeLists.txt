include_directories(../SpiralCube)
include_directories(../SpiralCubeScene)

add_executable(RTTDepth    MainUnit.cpp
                                ../SpiralCube/SpiralCube.cpp
                                ../SpiralCubeScene/SpiralCubeScene.cpp
                                ${HGL_GRAPHICS_MAIN_SOURCE})

target_link_libraries(RTTDepth ${HGL_GRAPHICS_LIB})

SET_TARGET_PROPERTIES(RTTDepth PROPERTIES WIN32_EXECUTABLE "true")
