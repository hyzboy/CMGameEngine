option(CMGDK_DEBUG "Use Debug CMGDK" ON)

if(CMGDK_DEBUG)
	set(CMGDK_BUILD_TYPE "Debug")
	add_definitions(-D_DEBUG)
else()
	set(CMGDK_BUILD_TYPE "Release")
endif()

IF(MSVC)

    SET(X86_USE_CPU_COMMAND SSE2 CACHE STRING "Use Max CPU Ext command")

    SET_PROPERTY(CACHE X86_USE_CPU_COMMAND PROPERTY STRINGS SSE2 AVX AVX2)

	#visual c++ 2015没有sse3/4/41/42这几个选项
ENDIF(MSVC)

IF(CMAKE_COMPILER_IS_GNUCC OR MINGW)
	OPTION(USE_CPP14				"Use C++ 14"							FALSE	)
	OPTION(USE_ICE_CREAM			"Use IceCream(only openSUSE/SUSE)"		FALSE	)

	IF(APPLE)
		OPTION(USE_LLVM_CLANG			"Use LLVM Clang"						TRUE	)
	ELSE()
		OPTION(USE_LLVM_CLANG			"Use LLVM Clang"						FALSE	)
	ENDIF()

	OPTION(USE_GPERF_TOOLS			"Use Google Performance Tools"			FALSE	)
	OPTION(NO_RTTI                  "Don't Use RTTI"                       TRUE)

	OPTION(USE_ALL_STATIC			"Use All Static"						FALSE	)
	OPTION(USE_STATIC_GLIBC			"Use Static libgcc"						FALSE	)
	OPTION(USE_STATIC_STDCXX		"Use Static libstdc++"					FALSE	)

    SET(X86_USE_CPU_COMMAND SSE2 CACHE STRING "Use Max CPU Ext command")

    SET_PROPERTY(CACHE X86_USE_CPU_COMMAND PROPERTY STRINGS SSE3 SSE4 SSE41 SSE42 AVX)
ENDIF(CMAKE_COMPILER_IS_GNUCC OR MINGW)

IF(USE_LLVM_CLANG)
    OPTION(USE_LLVM_CLANG_STATIC_ANALYZER	"the static analyzer"				OFF		)
    OPTION(USE_LLVM_SAFECode                "use LLVM SAFECode"                 OFF     )

    IF(USE_LLVM_CLANG_STATIC_ANALYZER)
        add_compile_options("--analyze")
    ENDIF(USE_LLVM_CLANG_STATIC_ANALYZER)

    IF(USE_LLVM_SAFECode)
        add_compile_options("-fmemsafety")
    ENDIF(USE_LLVM_SAFECode)
ENDIF(USE_LLVM_CLANG)

IF(APPLE)
    SET(HGL_BITS 64)
ELSE()
    IF(ANDROID)
        if(ANDROID_ABI MATCHES ".*64.*")
            SET(HGL_BITS 64)
        else()
            SET(HGL_BITS 32)
        endif()
    ELSE()
        IF(CMAKE_SIZEOF_VOID_P EQUAL 4)
            SET(HGL_BITS 32)
        ELSE()
            SET(HGL_BITS 64)
        ENDIF()
    endif()
endif()

IF(HGL_BITS EQUAL 32)
    add_definitions(-DHGL_32_BITS)
    add_definitions(-DHGL_POINTER_UINT=uint32)
ELSE()
    add_definitions(-DHGL_64_BITS)
    add_definitions(-DHGL_POINTER_UINT=uint64)
ENDIF()

ADD_DEFINITIONS("-DUNICODE -D_UNICODE")

IF(WIN32)
    IF(HGL_BITS EQUAL 32)
        SET(HGL_PLATFORM_STRING	${CMAKE_SYSTEM_NAME}_x86)
    ELSE()
        SET(HGL_PLATFORM_STRING	${CMAKE_SYSTEM_NAME}_x64)           #VC 2017这里为x64,但CMAKE生成为AMD64
    ENDIF()

    ADD_DEFINITIONS(-DHGL_PLATFORM_STRING=L"${HGL_PLATFORM_STRING}_${CMGDK_BUILD_TYPE}")
    ADD_DEFINITIONS(-DHGL_COMPILE_PLATFORM=L"${CMAKE_HOST_SYSTEM}-${CMAKE_HOST_SYSTEM_PROCESSOR}")
    ADD_DEFINITIONS(-DHGL_COMPILE_TOOLSET=L"${CMAKE_GENERATOR}")

    SET(HGL_OUT_DIRECTORY   ${HGL_PLATFORM_STRING})                 #Win下，输出目录CMAKE会自动添加"\${CMGDK_BUILD_TYPE}"
ELSE()
    SET(HGL_PLATFORM_STRING	${CMAKE_SYSTEM_NAME}_${CMAKE_SYSTEM_PROCESSOR})

    ADD_DEFINITIONS(-DHGL_PLATFORM_STRING="${HGL_PLATFORM_STRING}_${CMGDK_BUILD_TYPE}")
    ADD_DEFINITIONS(-DHGL_COMPILE_PLATFORM="${CMAKE_HOST_SYSTEM}-${CMAKE_HOST_SYSTEM_PROCESSOR}")
    ADD_DEFINITIONS(-DHGL_COMPILE_TOOLSET="${CMAKE_GENERATOR}")

    SET(HGL_OUT_DIRECTORY   ${HGL_PLATFORM_STRING}_${CMGDK_BUILD_TYPE})
ENDIF(WIN32)

message("HGL_PLATFORM_STRING:" ${HGL_PLATFORM_STRING})
message("HGL_COMPILE_PLATFORM:" ${CMAKE_HOST_SYSTEM}-${CMAKE_HOST_SYSTEM_PROCESSOR})
message("HGL_COMPILE_TOOLSET:"  ${HGL_COMPILE_TOOLSET})

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMGDK_PATH}/bin/${HGL_OUT_DIRECTORY})
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMGDK_PATH}/lib/${HGL_OUT_DIRECTORY})
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMGDK_PATH}/plug-in/${HGL_OUT_DIRECTORY})

message("Runtime Output Directory: " ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
message("Archive Output Directory: " ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
message("Library Output Directory: " ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

IF(UNIX)
SET(LIB_3RD_FIND_HINT	/usr/lib${HGL_BITS}
						/usr/local/lib${HGL_BITS}
						/usr/lib
						/usr/local/lib
						/usr/lib/${CMAKE_SYSTEM_PROCESSOR}-linux-gnu)
ENDIF(UNIX)

IF(WIN32)
    SET(INC_3RD_FIND_HINT ${CMGDK_PATH}/3rdpty)
ENDIF(WIN32)

MESSAGE("3rd Library Find Hint:" ${LIB_3RD_FIND_HINT})

link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
link_directories(${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})

IF(UNIX)
	IF(USE_ICE_CREAM)
		IF(USE_LLVM_CLANG)
			SET(CMAKE_C_COMPILER /usr/lib/icecc/bin/clang)
			SET(CMAKE_CXX_COMPILER /usr/lib/icecc/bin/clang++)
		ELSE(USE_LLVM_CLANG)
			SET(CMAKE_C_COMPILER /usr/lib/icecc/bin/gcc)
			SET(CMAKE_CXX_COMPILER /usr/lib/icecc/bin/g++)
		ENDIF(USE_LLVM_CLANG)
	ELSE(USE_ICE_CREAM)
		IF(USE_LLVM_CLANG)
			SET(CMAKE_C_COMPILER clang)
			SET(CMAKE_CXX_COMPILER clang++)
		ENDIF(USE_LLVM_CLANG)
	ENDIF(USE_ICE_CREAM)
ENDIF(UNIX)

IF(CMAKE_COMPILER_IS_GNUCC OR MINGW)

    IF(USE_LLVM_CLANG)
    ELSE()
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fext-numeric-literals")
    ENDIF()

	if(USE_ALL_STATIC)
		add_compile_options("-static")
	endif(USE_ALL_STATIC)

	if(USE_STATIC_GLIBC)
		add_compile_options("-static-libgcc")
	endif(USE_STATIC_GLIBC)

	if(USE_STATIC_STDCXX)
		add_compile_options("-static-libstdc++")
	endif(USE_STATIC_STDCXX)

	IF(USE_CPP14)
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
		add_compile_options("-DHGL_CPP14")
	ELSE(USE_CPP14)
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	ENDIF(USE_CPP14)

	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")

	IF(CMGDK_DEBUG)
		add_compile_options(-ggdb3)
	ELSE()
		add_compile_options(-Ofast)
	ENDIF()

	IF(NO_RTTI)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
    ENDIF(NO_RTTI)

    if(X86_USE_CPU_COMMAND STREQUAL AVX)
        add_definitions("-DMATH_AVX")
        add_compile_options(-mavx)
    elseif(X86_USE_CPU_COMMAND STREQUAL SSE41)
        add_definitions("-DMATH_SSE41")
        add_compile_options(-msse4.1)
    ELSE()
        add_definitions("-DMATH_SSE3")
        add_compile_options(-msse3)
    endif()
ENDIF()

IF(MSVC)

    add_compile_options(/GR-)

	if(X86_USE_CPU_COMMAND STREQUAL AVX2)
		add_definitions("-DMATH_AVX2")
        add_compile_options(/arch:AVX2)
    elseif(X86_USE_CPU_COMMAND STREQUAL AVX)
        add_definitions("-DMATH_AVX")
        add_compile_options(/arch:AVX)
    else()
        add_definitions("-DMATH_SSE2")
       	add_compile_options(/arch:SSE2)
    endif()
ENDIF(MSVC)
