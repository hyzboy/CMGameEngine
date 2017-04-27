add_executable(TexConv MainUnit.cpp ${HGL_GRAPHICS_MAIN_SOURCE})

target_compile_definitions(TexConv PRIVATE "-DIL_STATIC_LIB")
target_include_directories(TexConv PRIVATE ${CMGDK_PATH}/src/PlugIn/Image.Monolayer/DevIL/include)
target_link_libraries(TexConv ${HGL_GRAPHICS_LIB} DevIL)

SET_TARGET_PROPERTIES(TexConv PROPERTIES WIN32_EXECUTABLE "true")
