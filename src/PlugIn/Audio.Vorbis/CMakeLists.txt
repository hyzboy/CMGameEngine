IF(WIN32)
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/libvorbis/include)
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/libvorbis/lib)
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/libvorbis/lib/modes)
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/libvorbis/lib/books/coupled)
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/libvorbis/lib/books/floor)
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/libvorbis/lib/books/uncoupled)

    SET(VORBIS_SOURCE    libvorbis/lib/analysis.c
                        libvorbis/lib/bitrate.c
                        libvorbis/lib/block.c
                        libvorbis/lib/codebook.c
                        libvorbis/lib/envelope.c
                        libvorbis/lib/floor0.c
                        libvorbis/lib/floor1.c
                        libvorbis/lib/info.c
                        libvorbis/lib/lookup.c
                        libvorbis/lib/lpc.c
                        libvorbis/lib/lsp.c
                        libvorbis/lib/mapping0.c
                        libvorbis/lib/mdct.c
                        libvorbis/lib/psy.c
                        libvorbis/lib/registry.c
                        libvorbis/lib/res0.c
                        libvorbis/lib/sharedbook.c
                        libvorbis/lib/smallft.c
                        libvorbis/lib/synthesis.c
                        libvorbis/lib/vorbisenc.c
                        libvorbis/lib/vorbisfile.c
                        libvorbis/lib/window.c)

    add_library(CMP.Audio.Vorbis SHARED OggRead.cpp ${OGG_SOURCE} ${VORBIS_SOURCE})

    SOURCE_GROUP("LibOGG"        FILES ${OGG_SOURCE})
    SOURCE_GROUP("LibVorbis"    FILES ${VORBIS_SOURCE})
ELSE()
    FIND_PATH(VORBIS_INCLUDE_DIR
        NAMES vorbis/vorbisfile.h
        HINTS
        /usr/include
        /usr/local/include)
    INCLUDE_DIRECTORIES(${VORBIS_INCLUDE_DIR})

    FIND_PATH(VORBIS_LIBRARY_DIR
        NAMES libvorbis.so
        HINTS
        ${LIB_3RD_FIND_HINT})
    LINK_DIRECTORIES(${VORBIS_LIBRARY_DIR})

    add_library(CMP.Audio.Vorbis SHARED OggRead.cpp)
    target_link_libraries(CMP.Audio.Vorbis vorbisfile ogg)
ENDIF()
