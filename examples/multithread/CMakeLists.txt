add_executable(Example.MultiThread.ASyncDemo ASyncDemo.cpp)
add_executable(Example.MultiThread.WorkLink WorkLink.cpp)

target_link_libraries(Example.MultiThread.ASyncDemo ${HGL_CONSOLE_LIB})
target_link_libraries(Example.MultiThread.WorkLink ${HGL_CONSOLE_LIB})
