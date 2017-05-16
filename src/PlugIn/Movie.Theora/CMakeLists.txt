IF(WIN32)

    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/libtheora/include)

    add_definitions(-DOC_X86_64_ASM)

    SET(THEORA_SOURCE   libtheora/lib/analyze.c
                        libtheora/lib/apiwrapper.c
                        libtheora/lib/bitpack.c
                        libtheora/lib/cpu.c
                        libtheora/lib/decapiwrapper.c
                        libtheora/lib/decinfo.c
                        libtheora/lib/decode.c
                        libtheora/lib/dequant.c
                        libtheora/lib/encapiwrapper.c
                        libtheora/lib/encfrag.c
                        libtheora/lib/encinfo.c
                        libtheora/lib/encode.c
                        libtheora/lib/enquant.c
                        libtheora/lib/fdct.c
                        libtheora/lib/fragment.c
                        libtheora/lib/huffdec.c
                        libtheora/lib/huffenc.c
                        libtheora/lib/idct.c
                        libtheora/lib/info.c
                        libtheora/lib/internal.c
                        libtheora/lib/mathops.c
                        libtheora/lib/mcenc.c
                        libtheora/lib/quant.c
                        libtheora/lib/rate.c
                        libtheora/lib/state.c
                        libtheora/lib/tokenize.c)

    SET(THEORA_X86_SOURCE   libtheora/lib/x86_vc/mmxencfrag.c
                            libtheora/lib/x86_vc/mmxfdct.c
                            libtheora/lib/x86_vc/mmxfrag.c
                            libtheora/lib/x86_vc/mmxidct.c
                            libtheora/lib/x86_vc/mmxstate.c
                            libtheora/lib/x86_vc/x86enc.c
                            libtheora/lib/x86_vc/x86state.c)

    SOURCE_GROUP("LibOGG"                    FILES ${OGG_SOURCE})
    SOURCE_GROUP("LibTheora"                 FILES ${THEORA_SOURCE})
    SOURCE_GROUP("LibTheora/x86"             FILES ${THEORA_X86_SOURCE})

    SET(THEORA_TIME_SOURCE  WinTime.cpp)

    add_library(CMP.Movie.Theora SHARED MainUnit.cpp TheoraPlayer.cpp ${THEORA_TIME_SOURCE} ${OGG_SOURCE} ${THEORA_SOURCE} ${THEORA_X86_SOURCE})

ELSE(WIN32)

    FIND_PATH(THEORA_INCLUDE_DIR
        NAMES theora/theora.h
        HINTS
        /usr/include
        /usr/local/include)
    INCLUDE_DIRECTORIES(${THEORA_INCLUDE_DIR})

    FIND_PATH(THEORA_LIBRARY_DIR
        NAMES libtheoradec.so
        HINTS
        ${LIB_3RD_FIND_HINT})
    LINK_DIRECTORIES(${THEORA_LIBRARY_DIR})

    SET(THEORA_TIME_SOURCE  UnixTime.cpp)

    add_library(CMP.Movie.Theora SHARED MainUnit.cpp TheoraPlayer.cpp ${THEORA_TIME_SOURCE})

    target_link_libraries(CMP.Movie.Theora ogg theoradec)

ENDIF(WIN32)

