include_directories(../PlaneGrid)

add_executable(SpiralCube    MainUnit.cpp
                            ../PlaneGrid/PlaneGrid.cpp
                            SpiralCube.cpp
                            ${HGL_GRAPHICS_MAIN_SOURCE})

target_link_libraries(SpiralCube ${HGL_GRAPHICS_LIB})

SET_TARGET_PROPERTIES(SpiralCube PROPERTIES WIN32_EXECUTABLE "true")
