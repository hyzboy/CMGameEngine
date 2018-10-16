add_executable(Example.FileSystem.EnumFile EnumFileTest.cpp)

TARGET_LINK_LIBRARIES(Example.FileSystem.EnumFile ${HGL_CONSOLE_LIB})

add_executable(Example.FileSystem.EnumVolume EnumVolumeTest.cpp)

target_link_libraries(Example.FileSystem.EnumVolume ${HGL_CONSOLE_LIB})
