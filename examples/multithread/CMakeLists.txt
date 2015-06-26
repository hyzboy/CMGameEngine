add_executable(ASyncDemo ASyncDemo.cpp)
add_executable(WorkLink WorkLink.cpp)

target_link_libraries(ASyncDemo ${HGL_CONSOLE_LIB})
target_link_libraries(WorkLink ${HGL_CONSOLE_LIB})
