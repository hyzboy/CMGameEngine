INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/tremor)

SET(TREMOR_SOURCE	tremor/block.c
                    tremor/floor0.c
                    tremor/info.c
                    tremor/mdct.c
                    tremor/res012.c
                    tremor/synthesis.c
                    tremor/window.c
                    tremor/codebook.c
                    tremor/floor1.c
                    tremor/mapping0.c
                    tremor/registry.c
                    tremor/sharedbook.c
                    tremor/vorbisfile.c)

add_library(CMP.Audio.Vorbis SHARED OggRead.cpp ${TREMOR_SOURCE})
target_link_libraries(CMP.Audio.Vorbis ogg)

SOURCE_GROUP("Tremor"	FILES ${TREMOR_SOURCE})
