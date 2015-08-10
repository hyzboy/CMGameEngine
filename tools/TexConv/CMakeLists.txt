add_executable(TexConv MainUnit.cpp ${HGL_GRAPHICS_MAIN_SOURCE})

target_link_libraries(TexConv ${HGL_GRAPHICS_LIB} IL ILU)
