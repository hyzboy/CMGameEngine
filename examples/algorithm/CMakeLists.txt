add_subdirectory(CountHash)

add_executable(Example.Algorithm.BASE64 base64test.cpp)
target_link_libraries(Example.Algorithm.BASE64 ${HGL_CONSOLE_LIB})
