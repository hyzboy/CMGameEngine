# add_executable(Example.MultiThread.ASyncDemo ASyncDemo.cpp)
#
# target_link_libraries(Example.MultiThread.ASyncDemo ${HGL_CONSOLE_LIB})

add_executable(Example.MultiThread.Workflow WorkflowTest.cpp ${HGL_CONSOLE_MAIN_SOURCE})
target_link_libraries(Example.MultiThread.Workflow ${HGL_CONSOLE_LIB})

add_executable(Example.MultiThread.WorkflowUnsort WorkflowTestUnsort.cpp ${HGL_CONSOLE_MAIN_SOURCE})
target_link_libraries(Example.MultiThread.WorkflowUnsort ${HGL_CONSOLE_LIB})
