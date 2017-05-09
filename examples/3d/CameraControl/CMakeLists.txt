include_directories(../PlaneGrid)
include_directories(../SpiralSphereScene)

add_executable(CameraControl    MainUnit.cpp
                                CameraControl.cpp
                                ../PlaneGrid/PlaneGrid.cpp
                                ../SpiralSphereScene/SpiralSphere.cpp
                                ${HGL_GRAPHICS_MAIN_SOURCE})

target_link_libraries(CameraControl ${HGL_GRAPHICS_LIB})

SET_TARGET_PROPERTIES(CameraControl PROPERTIES WIN32_EXECUTABLE "true")
