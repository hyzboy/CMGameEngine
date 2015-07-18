cmake_minimum_required(VERSION 2.8)
project(MathGeoLib)

include(CommonOptions.cmake)

# N.B. These exist for inter-opping to other libraries written by the author.
# Feel free to remove. 
include_directories(../CodeLib/src)
include_directories(../GraphicsEngine/src)

file(GLOB_RECURSE sourceFiles ./src/*.cpp ./src/*.c)
file(GLOB_RECURSE headerFiles ./src/*.h)

if (BUILD_FOR_GCOV)
	file(REMOVE "MathGeoLib_code_files.txt")
	foreach(filename ${sourceFiles})
		get_filename_component(filebasename ${filename} NAME)
		file(APPEND "MathGeoLib_code_files.txt" "${filebasename} ")
	endforeach()
endif()

if (BUILD_FOR_GCOV OR MATH_TESTS_EXECUTABLE OR FAIL_USING_EXCEPTIONS) # TODO: Use a separate MATH_INCLUDE_TESTS?
	file(GLOB_RECURSE testSourceFiles ./tests/*.cpp)
	file(GLOB_RECURSE testHeaderFiles ./tests/*.h)
	set(sourceFiles ${sourceFiles} ${testSourceFiles})
	set(headerFiles ${headerFiles} ${testHeaderFiles})
	if (IOS)
		file(GLOB_RECURSE testObjCFiles ./tests/*.m)
		set(sourceFiles ${sourceFiles} ${testObjCFiles})
	endif()
endif()

if (MATH_TESTS_EXECUTABLE)
	add_executable(MathGeoLib ${sourceFiles} ${headerFiles})
	if (EMSCRIPTEN)
		set_target_properties(MathGeoLib PROPERTIES LINK_FLAGS "--js-library ${CMAKE_CURRENT_LIST_DIR}/src/emscripten/library_system.js -g")
	endif()
else()
	add_library(MathGeoLib STATIC ${sourceFiles} ${headerFiles})
endif()

if (LINUX)
	# clock_gettime() is found from the library librt on linux. 
	target_link_libraries(MathGeoLib rt)
endif()

if (WIN8RT)
	set_target_properties(MathGeoLib PROPERTIES VS_WINRT_EXTENSIONS TRUE)
	# Ignore warning LNK4264: archiving object file compiled with /ZW into a static library; note that when authoring Windows Runtime types it is not recommended to link with a static library that contains Windows Runtime metadata
	#    since we are not authoring any WinRT components from this code. 
	set_target_properties(MathGeoLib PROPERTIES STATIC_LIBRARY_FLAGS "/ignore:4264")
endif()

# install lib and header files if testing is disabled
if (NOT MATH_TESTS_EXECUTABLE)
	install (TARGETS MathGeoLib DESTINATION "lib")
	install (DIRECTORY "src/" DESTINATION "include/MathGeoLib" FILES_MATCHING PATTERN "*.h")
	install (DIRECTORY "src/" DESTINATION "include/MathGeoLib" FILES_MATCHING PATTERN "*.inl")
endif (NOT MATH_TESTS_EXECUTABLE)

    OPTION(X86_USE_SSE              "Use SSE"               TRUE    )
	OPTION(X86_USE_SSE2				"Use SSE2"				TRUE	)
	OPTION(X86_USE_SSE3             "Use SSE3"              TRUE    )
	OPTION(X86_USE_SSE4             "Use SSE4"              TRUE    )
	OPTION(X86_USE_SSE41            "Use SSE41"             TRUE    )
	OPTION(X86_USE_SSE42            "Use SSE42"             TRUE    )
	OPTION(X86_USE_AVX              "Use AVX"               FALSE   )
	
    if(X86_USE_SSE41)
        add_definitions("-DMATH_SSE41")
        add_definitions(-msse4.1)
    ELSE()
        if(X86_USE_SSE3)
            add_definitions("-DMATH_SSE3")
            add_definitions(-msse3)
        else()
            if(X86_USE_SSE2)
                add_definitions("-DMATH_SSE2")
                add_definitions(-msse2)
                else()
                if(X86_USE_SSE)
                    add_definitions("-DMATH_SSE")
                    add_definitions(-msse)
                endif()
            endif()
        endif()
    endif()