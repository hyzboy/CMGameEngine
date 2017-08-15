include_directories(../CameraControl)

add_executable(Example.3D.StaticModel                 
                ../CameraControl/CameraControl.cpp
                MainUnit.cpp 
                ${HGL_GRAPHICS_MAIN_SOURCE})

target_link_libraries(Example.3D.StaticModel ${HGL_GRAPHICS_LIB})

SET_TARGET_PROPERTIES(Example.3D.StaticModel PROPERTIES WIN32_EXECUTABLE "true")
