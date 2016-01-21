include_directories(../PlaneGrid)

add_executable(SpiralCubeScene	MainUnit.cpp
								../PlaneGrid/PlaneGrid.cpp
								${HGL_GRAPHICS_MAIN_SOURCE})

target_link_libraries(SpiralCubeScene ${HGL_GRAPHICS_LIB})

SET_TARGET_PROPERTIES(SpiralCubeScene PROPERTIES WIN32_EXECUTABLE "true")
