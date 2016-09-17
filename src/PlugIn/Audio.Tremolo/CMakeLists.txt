INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/Tremolo)

SET(TREMOLO_SOURCE	Tremolo/codebook.c
                    Tremolo/dsp.c
                    Tremolo/floor0.c
                    Tremolo/floor1.c
                    Tremolo/floor_lookup.c
                    Tremolo/mapping0.c
                    Tremolo/mdct.c
                    Tremolo/misc.c
                    Tremolo/res012.c
                    Tremolo/treminfo.c
                    Tremolo/vorbisfile.c
                    Tremolo/bitwise.c
                    Tremolo/framing.c)

add_library(CMP.Audio.Vorbis SHARED OggRead.cpp ${TREMOLO_SOURCE})

SOURCE_GROUP("Tremolo"	FILES ${TREMOLO_SOURCE})
