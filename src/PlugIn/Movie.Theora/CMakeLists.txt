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
                        
IF(WIN32)

    SET(THEORA_TIME_SOURCE  WinTime.cpp)

    SET(THEORA_X86_SOURCE   libtheora/lib/x86_vc/mmxencfrag.c
                            libtheora/lib/x86_vc/mmxfdct.c
                            libtheora/lib/x86_vc/mmxfrag.c
                            libtheora/lib/x86_vc/mmxidct.c
                            libtheora/lib/x86_vc/mmxstate.c
                            libtheora/lib/x86_vc/x86enc.c
                            libtheora/lib/x86_vc/x86state.c)
ELSE(WIN32)

    SET(THEORA_TIME_SOURCE  UnixTime.cpp)
    
    SET(THEORA_X86_SOURCE   libtheora/lib/x86/mmxencfrag.c
                            libtheora/lib/x86/mmxfdct.c
                            libtheora/lib/x86/mmxfrag.c
                            libtheora/lib/x86/mmxidct.c
                            libtheora/lib/x86/mmxstate.c
                            libtheora/lib/X86/sse2fdct.c
                            libtheora/lib/x86/x86enc.c
                            libtheora/lib/x86/x86state.c)
ENDIF(WIN32)

SOURCE_GROUP("LibOGG"                    FILES ${OGG_SOURCE})
SOURCE_GROUP("LibTheora"                 FILES ${THEORA_SOURCE})
SOURCE_GROUP("LibTheora/x86"             FILES ${THEORA_X86_SOURCE})

add_library(CMP.Movie.Theora SHARED MainUnit.cpp TheoraPlayer.cpp ${THEORA_TIME_SOURCE} ${OGG_SOURCE} ${THEORA_SOURCE} ${THEORA_X86_SOURCE})
